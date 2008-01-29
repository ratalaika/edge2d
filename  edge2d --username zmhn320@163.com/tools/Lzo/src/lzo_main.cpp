/**
 * lzo tool , to compress files into a lzo archive and decompress files from a lzo archive.
 *
 * Lzo archive format author : Kevin Lynx
 * Lzo tool author : Kevin Lynx
 * Date : 10.20.2007
 *
 * bugs:
 * 1.当遍历指定后缀的文件名时，无法进行递归。因为这个时候FindNextFile查找不到文件夹了。
 *   解决办法：始终采用*.*查找文件，当查找到文件时，自己比较其后缀，如果是文件夹，则不处理。
 *
 * 1.29.2008 支持查找多个后缀的文件，即指定多个后缀查找。lzo -rp *.cpp;*.h src.lzo
 * 最多支持16个后缀。
 */
#ifdef _DEBUG
//#include <vld.h>
#endif

#include "lzoarchive.h"
#include <stdio.h>
#include <windows.h>

#if _MSC_VER >= 1400
#pragma warning( disable:4996 )
#endif

/// lzo archive tool version
#define LZO_ARCHIVE_TOOL_VERSION 0x000100
#define LZO_ARCHIVE_TOOL_VERSION_STR "Lzo Archive Tool Version 0.1.0"

/// max postfics
#define MAX_POST 16

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * file name list.
 *
 *
 */
typedef struct _file_name
{
	char name[MAX_PATH];  /// name in the archive
	char file[MAX_PATH];  /// file path name in the file system
	_file_name *next;
}
file_name;

/**
 * get file names in a directory, it can recure sub directory.
 * this function will return a file name list, and you must free the 
 * memory use free_file_name_list. if bpath param is true, the file name 
 * will include the path.
 * if recure is true, it will recure sub directory.
 */
file_name *get_file_name_list( const char *path, const char *file, bool recure, bool bpath );

/**
 * free a file name list, it's a helper function to free memory created
 * by get_file_name_list function.
 *
 */
void free_file_name_list( file_name *list );

/**
 * add a file name list into a lzo archive, and later , you can save the archive.
 *
 */
void add_file_list( lzo_archive *archive, file_name *file_list );

/**
 * construc a file name to the buffer
 *
 *
 */
void construct_file_name( char *buffer, const char *path, const char *filename, bool bpath );

/**
 * get a name list's tail node
 *
 */
file_name *get_tail( file_name *header );

/**
 * create a directory
 * basePath : the base directory, must be exist.
 * _file_name : like img/bk/bk1.jpg, if the img and bk directories both are not
 * exist, it will create them automatically.
 *
 */
void create_directory( const char *basePath, const char *_file_name );

/**
 * pause the command line and call sub functions.
 *
 */
void pause_cmdline( int argc, char **argv );

/**
 * compress files, create a lzo archive.
 *
 * lzo_file : output lzo archive file name
 * file : the files which will be compressed in the archive, it can be wildcard characters.
 * bpath : whether record the path information in the file
 * brecurse : whether recurse the sub directory when file param specifies a directory
 *
 */
void compress( const char *archive_name, const char *file, bool bpath, bool brecurse );

/**
 * decompress files from a lzo archive.
 *
 *
 */
void decompress( const char *archive_name, const char *out_dir );

/**
 * get a lzo archive's file info.
 * it will display the file list in the archive.
 *
 */
void get_archive_info( const char *archive_name );

/**
 * show help info
 *
 */
void show_help();

/**
 * get self name, get the tool 's name to avoid to compress itself into the archive
 *
 */
const char *get_self_name();
const char *get_self_log_name();

/**
 * compare the file's postfix.
 * it the file 's postfix is the param 'postfix', return true
 *
 */
bool compare_postfix( const char *filename, const char *postfix );

/**
 * construct a postfix array
 * return how many postfix it constructed
 */
int construct_postfix( const char *postfixs, char array[16][MAX_PATH] );

/**
 * these functions below mainly to deal with the program informations.
 *
 *
 */
#define LOG_FILE "./lzoarchive.log"

void start_log()
{
	FILE *fp = fopen( LOG_FILE, "w" );
	fclose( fp );
}

void log_info( const char *format, ... )
{
	va_list argList;
	FILE *pFile;
	va_start( argList, format );
	pFile = fopen( LOG_FILE, "a+" );
	vfprintf( pFile, format, argList );
	fclose( pFile );
	va_end( argList );
}

void lzo_callback( const char *msg )
{
	printf( msg );
	log_info( "lzo info : %s", msg );
}

/**
 * program entry
 *
 */
int main( int argc, char **argv )
{
	printf( "%s\n", LZO_ARCHIVE_TOOL_VERSION_STR );
	printf( "Lzo Archive Format and Compress/Decompress tool created by Kevin Lynx\n" );

	start_log();
	lzoex_init( lzo_callback );

	pause_cmdline( argc, argv );

	return 0;
}

file_name *get_file_name_list( const char *path, const char *_file, bool recurse, bool bpath )
{
	const char *file = "*.*";
	char full_path[MAX_PATH];
	file_name *header = 0;

	/// it works on Windows
	WIN32_FIND_DATA file_data;
	HANDLE file_handle;

	construct_file_name( full_path, path, file, true );

	file_handle = FindFirstFile( full_path, &file_data );
	if( file_handle == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	/// igone '.' and '..' files
	while( strcmp( file_data.cFileName, "." ) == 0 || strcmp( file_data.cFileName, ".." ) == 0 )
	{
		if( !FindNextFile( file_handle, &file_data ) )
		{
			return 0;
		}
	}

	/// deal with sub directory
	if( recurse && file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		char subpath[MAX_PATH];
		construct_file_name( subpath, path, file_data.cFileName, true );
		header = get_file_name_list( subpath, _file, true, bpath );
	}
	else if( !( file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
		compare_postfix( file_data.cFileName, _file ) )
	{
		file_name *filename = (file_name*) malloc( sizeof( file_name ) );
		construct_file_name( filename->name, path, file_data.cFileName, bpath );
		construct_file_name( filename->file, path, file_data.cFileName, true );

		if( strcmp( get_self_name(), filename->file ) != 0 &&
			strcmp( get_self_log_name(), filename->file ) != 0 )
		{
			filename->next = header;
			header = filename;
		}
		else
		{
			free( filename );
		}
	}

	/// deal with others
	while( FindNextFile( file_handle, &file_data ) )
	{
		if( recurse && file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			char subpath[MAX_PATH];
			construct_file_name( subpath, path, file_data.cFileName, true );
			file_name *sub_list = get_file_name_list( subpath, _file, true, bpath );
			if( sub_list != 0 )
			{
				file_name *tail = get_tail( sub_list );
				tail->next = header;
				header = sub_list;
			}
		}
		else if( !( file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			compare_postfix( file_data.cFileName, _file ) )
		{
			file_name *new_file = (file_name*) malloc( sizeof( file_name ) );
			construct_file_name( new_file->name, path, file_data.cFileName, bpath );
			construct_file_name( new_file->file, path, file_data.cFileName, true );

			if( strcmp( get_self_name(), new_file->file ) != 0 &&
				strcmp( get_self_log_name(), new_file->file ) != 0 )
			{
				new_file->next = header;
				header = new_file;
			}
			else
			{
				free( new_file );
			}
		}
	}

	return header;
}

void construct_file_name( char *buffer, const char *path, const char *filename, bool bpath )
{
	if( !bpath )
	{
		strcpy( buffer, filename );
		return;
	}

	strcpy( buffer, path );
	size_t len = strlen( buffer ) - 1;
	if( buffer[len] != '\\' && buffer[len] != '/' )
	{
		buffer[len+1] = '/';
		buffer[len+2] = '\0';
	}
	strcat( buffer, filename );
}

file_name *get_tail( file_name *header )
{
	file_name *tail = header;
	while( tail->next != 0 )
	{
		tail = tail->next;
	}

	return tail;
}

void add_file_list( lzo_archive *archive, file_name *file_list )
{
	file_name *name = file_list;
	while( name != 0 )
	{
		lzo_add_item( archive, name->file, name->name );
		name = name->next;
	}
}

void free_file_name_list( file_name *list )
{
	file_name *name = list;
	while( name != 0 )
	{
		file_name *tmp = name;
		name = name->next;
		free( tmp );
	}
}

void create_directory( const char *basePath, const char *_file_name )
{
	size_t size = strlen( _file_name );
	size_t i;
	char path[MAX_PATH];

	for( i = 0; i <= size; ++ i )
	{
		strcpy( path, basePath );
		if( _file_name[i] == '\\' || _file_name[i] == '/' )
		{
			char tmp_p[MAX_PATH];
			strncpy( tmp_p, _file_name, i );
			tmp_p[i] = '\0';
			strcat( path, tmp_p );

			::CreateDirectory( path, 0 );
		}
	}
}

void pause_cmdline( int argc, char **argv )
{
	char lzo_file[MAX_PATH];
	char files[MAX_PATH];
	bool brecurse;
	bool bpath;

	if( argc == 1 )
	{
		show_help();
	}
	else if( argc == 2 )
	{
		if( strcmp( argv[1], "-h" ) == 0 || strcmp( argv[1], "-help" ) == 0 )
		{
			show_help();
		}
	}
	else if( argc == 3 )
	{
		if( strcmp( argv[1], "-l" ) == 0 )
		{
			/// display the file info in the archive
			get_archive_info( argv[2] );
		}
		else if( strcmp( argv[1], "-d" ) == 0 )
		{
			/// decompress files in the current directory
			decompress( argv[2], "./" );
		}
		else
		{
			/// normal compress files
			strncpy( files, argv[1], MAX_PATH );
			strncpy( lzo_file, argv[2], MAX_PATH );
			compress( lzo_file, files, false, false );
		}
	}
	else if( argc == 4 )
	{
		if( strcmp( argv[1], "-d" ) == 0 )
		{
			/// decompress files in the specified directory
			decompress( argv[2], argv[3] );
		}
		else
		{
			/// compress files
			char cmd[64];
			strncpy( cmd, argv[1], 63 );
		
			bpath = strcmp( cmd, "-p" ) == 0 ? true : false;
			brecurse = strcmp( cmd, "-r" ) == 0 ? true : false;
			if( strcmp( cmd, "-rp" ) == 0 || strcmp( cmd, "-pr" ) == 0 )
			{
				bpath = brecurse = true;
			}

			strncpy( files, argv[2], MAX_PATH );
			strncpy( lzo_file, argv[3], MAX_PATH );

			compress( lzo_file, files, bpath, brecurse );
		}
	}
}

void compress( const char *archive_name, const char *_file, bool bpath, bool brecurse )
{
	char path[MAX_PATH];
	char file[MAX_PATH];

	/// split the _file into path and file name( wildcard characters (* and ?). 
	size_t len = strlen( _file );
	bool found_path = false;
	for( int i = (int)len; i >= 0; -- i )
	{
		if( _file[i] == '/' || _file[i] == '\\' )
		{
			strncpy( path, _file, i );
			path[i] = '\0';
			strncpy( file, _file + i + 1, len - i );
			found_path = true;
			break;
		}
	}
	if( !found_path )
	{
		strcpy( path, "./" );
		strcpy( file, _file );
	}

	/// create a lzo archive
	lzo_archive *archive = lzo_create_archive( archive_name );
	
	/// get a file list in the specidied directory
	file_name *file_list = get_file_name_list( path, file, brecurse, bpath );

	/// add file list into the archive
	add_file_list( archive, file_list );

	/// save the archive
	lzo_save_archive( archive );

	/// ok, just free resources
	lzo_destroy_archive( archive );
	free_file_name_list( file_list );

}

void decompress( const char *archive_name, const char *out_dir )
{
	char path[MAX_PATH];
	char file[MAX_PATH];
	construct_file_name( path, out_dir, "", true );

	/// create the base directory recusely
	create_directory( "./", path );

	/// load the lzo archive
	lzo_archive *archive = lzo_load_archive( archive_name );

	/// get the file count in the archive
	unsigned long count = lzo_get_file_count( archive );
	
	/// decompress all files in the archive
	for( unsigned long i = 0; i < count; ++ i )
	{
		lzo_file_info *info = lzo_get_file_info_byid( archive, i );
		create_directory( path, info->name );
			
		unsigned char *buf = (unsigned char*) malloc( info->decom_size );
		lzo_read_file( archive, info, buf );

		construct_file_name( file, path, info->name, true );
		FILE *fp = fopen( file, "wb" );
		if( fp == 0 )
		{
			printf( "open file %s to write failed\n", file );
		}
		fwrite( buf, info->decom_size, 1, fp );
		fclose( fp );

		free( buf );
	}

	/// free resources
	lzo_free_archive( archive );
}

void get_archive_info( const char *archive_name )
{
	/// load a lzo archive
	lzo_archive *archive = lzo_load_archive( archive_name );

	/// get the file count in the archive
	unsigned long count = lzo_get_file_count( archive );
	
	printf( "%lu files\n", count );

	/// display the file info
	for( unsigned long i = 0; i < count; ++ i )
	{
		lzo_file_info *info = lzo_get_file_info_byid( archive, i );
		printf( "name : %s\t\t\tsize : %lu\n", info->name, info->decom_size );
	}

	/// unload the archive
	lzo_free_archive( archive );
}

void show_help()
{
	printf( "lzo archive compress/decompress tool help\n \
Format : [cmd] file file\n \
lzo files data.lzo : compress files into data.lzo\n \
-h / -help show help information\n \
-p the same as lzo files data.lzo besides recording files' path info\n \
-r the same as lzo files data.lzo besides it will recuse sub directory\n \
-rp / -pr recuse the files and record releative path info\n \
-l list file info in a lzo archive\n \
-d decompres a lzo file, i.e lzo -d data.lzo. it will create directories for you\n" );			 

}

const char *get_self_name()
{
	static char s_name[MAX_PATH];
	
	if( s_name[0] == 0 )
	{
		char tmp[MAX_PATH];
		::GetModuleFileName( GetModuleHandle( 0 ), tmp, MAX_PATH ) ;
		size_t size = strlen( tmp );
		size_t pos;
		for( pos = size; pos >= 0; -- pos )
		{
			if( tmp[pos] == '/' || tmp[pos] == '\\' )
			{
				break;
			}
		}

		sprintf( s_name, "./%s", &tmp[pos+1] );
	}

	return s_name;
}

const char *get_self_log_name()
{
	return LOG_FILE;
}

bool compare_postfix( const char *filename, const char *postfix )
{
	char postfixs[MAX_POST][MAX_PATH];
	int post_count = construct_postfix( postfix, postfixs );

	char fp[MAX_PATH];
	int len = (int)strlen( filename );
	int i ;
	
	for( i = len; i >= 0; -- i )
	{
		if( filename[i] == '.' )
		{
			strcpy( fp, &filename[i+1] );
			for( int index = 0; index < post_count; ++ index )
			{
				if( strcmp( postfixs[index], "*.*" ) == 0 )
				{
					return true;
				}
				else if( strcmp( fp, &postfixs[index][2] ) == 0 )
				{
					return true;
				}
			}

			return false;
		}
	}

	return false;
}

int construct_postfix( const char *_postfixs, char array[16][MAX_PATH] )
{
	char postfixs[MAX_PATH];
	strcpy( postfixs, _postfixs );
	strcat( postfixs, ";" );

	int len = (int)strlen( postfixs );
	int index = 0;
	int i = 0;

	for( int pos = 0; pos < len; ++ pos )
	{
		if( postfixs[pos] == ';' )
		{
			array[index][i] = '\0';
			++ index;
			i = 0;
		}
		else
		{
			array[index][i] = postfixs[pos];
			++ i;
		}
	}

	return index ;
}

#ifdef __cplusplus
}
#endif
