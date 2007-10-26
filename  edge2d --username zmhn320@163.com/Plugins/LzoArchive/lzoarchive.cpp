/**
 * LZO extend library. It can read/write a LZO archive.
 *
 * Kevin Lynx
 * 10.19.2007
 */
#include "lzoarchive.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// working memory for compress
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

/**
 * callback functions to output error
 *
 */
void (*lzo_error_func)( const char* );

/**
 * lzo error default callback function
 *
 */
static void _lzo_default_error( const char *info )
{
	printf( info );
}

/**
 * format the path name, make '\\' to '/'
 *
 */
static void _lzo_format_path( char *path )
{
	size_t i = 0;
	for( i = 0; i <= strlen( path ); ++ i )
	{
		if( path[i] == '\\' )
		{
			path[i] = '/';
		}
	}
}

/**
 * get a c file 's size
 *
 */
static long _lzo_get_file_size( FILE *fp )
{
	fseek( fp, 0, SEEK_END );
	long FileSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	return FileSize;
}

static const char *_lzo_error( const char *format, ... )
{
	static char s_info[256];
	va_list argList;
	va_start( argList, format );
	vsprintf( s_info, format, argList );
	va_end( argList );
	
	return s_info;
}

void lzoex_init( void(*lzo_error_fun)(const char* ) )
{
	if( lzo_error_fun == 0 )
	{
		lzo_error_func = _lzo_default_error;
	}
	else
	{
		lzo_error_func = lzo_error_fun;
	}

	if( lzo_init() != LZO_E_OK )
	{
		lzo_error_func( "init lzo failed\n" );
	}
}

lzo_archive *lzo_create_archive( const char *file )
{
	lzo_archive *archive = (lzo_archive*)malloc( sizeof( lzo_archive ) );

	strncpy( archive->name, file, LZO_MAX_PATH );
	archive->name[LZO_MAX_PATH] = 0;
	archive->header.item_count = 0;
	archive->header.items = 0;

	return archive;
}

int lzo_save_archive( lzo_archive *archive )
{
	/// open the archive's file to write
	FILE *fp_ar = fopen( archive->name, "wb" );
	if( fp_ar == 0 )
	{
		lzo_error_func( _lzo_error( "Open archive file %s failed\n", archive->name ) );
		return -1;
	}

	/// create file items information
	lzo_uint file_count = archive->header.item_count ;
	lzo_file_info *file_infos = (lzo_file_info*)malloc( sizeof( lzo_file_info ) * file_count );
	lzo_item *item = archive->header.items;
	int index = 0;
	while( item != 0 )
	{
		file_infos[index].com_size = item->com_size ;
		file_infos[index].decom_size = 0;
		file_infos[index].offset = 0;
		strcpy( file_infos[index].name, item->name );

		item = item->next;
		++ index;
	}

	/// ready to write file info into the archive
	lzo_uint info_com_size;
	lzo_uint info_decom_size = sizeof( lzo_file_info ) * file_count ;
	lzo_uint info_t_size = info_decom_size + info_decom_size / 16 + 64 + 3;
	unsigned char *info_buf = (unsigned char*)malloc( info_t_size );

	/// jump to the right offset , ready to write compressed file
	long offset = sizeof( lzo_uint ) * 3 + 2 + info_decom_size + 1; // "%lu %lu %lu"
	fseek( fp_ar, offset, SEEK_SET );
	item = archive->header.items;
	for( lzo_uint i = 0; i < file_count; ++ i )
	{
		lzo_error_func( _lzo_error( "ready to open file : %s\n", item->file ) );
		offset = ftell( fp_ar );
		FILE *fp = fopen( item->file, "rb" );
		if( fp == 0 )
		{
			lzo_error_func( _lzo_error( "open file %s to read failed\n", item->file ) );
			return -1;
		}
		long size = _lzo_get_file_size( fp );
		long tsize = size + size / 16 + 64 + 3;
		lzo_uint com_size;
		unsigned char *file_buf = (unsigned char*)malloc( size );
		unsigned char *com_buf = (unsigned char*)malloc( tsize );
		fread( file_buf, size,1, fp );
		fclose( fp );

		/// compress it
		int r = lzo1x_1_compress( file_buf, size, com_buf, &com_size, wrkmem );
		if( r != LZO_E_OK )
		{
	        lzo_error_func( _lzo_error( "compress file %s failed: %d\n", item->file, r ) );
			return -1;
		}
		else
		{
			lzo_error_func( _lzo_error( "%s compressed %lu bytes into %lu bytes\n",
            file_infos[i].name, (lzo_uint)size, (lzo_uint) com_size ) );
		}

		/// write the compressed data to archive
		fwrite( com_buf, com_size, 1, fp_ar );

		/// save information
		file_infos[i].com_size = com_size;
		file_infos[i].decom_size = size;
		file_infos[i].offset = offset;

		/// free resources
		free( file_buf );
		free( com_buf );

		item = item->next;
	}

	/// compress the header
	int r = lzo1x_1_compress( (lzo_bytep) file_infos, info_decom_size, info_buf, &info_com_size, wrkmem );
    if( r != LZO_E_OK )
    {
        lzo_error_func( _lzo_error( "compress archive header failed: %d\n", r ) );
        return -1;
    }

	/// write the header 
	fseek( fp_ar, 0, SEEK_SET );
	fprintf( fp_ar, "%lu %lu %lu", info_com_size, info_decom_size, file_count );
	fwrite( info_buf, info_com_size, 1, fp_ar );

	fclose( fp_ar );

	free( info_buf );
	free( file_infos );

	return 0;
}

void lzo_destroy_archive( lzo_archive *archive )
{
	lzo_item *item = archive->header.items;
	while( item != 0 )
	{
		lzo_item *next = item->next;
		free( item );
		item = next;
	}

	free( archive );
}

void lzo_add_item( lzo_archive *archive, const char *file, const char *name )
{
	lzo_item *lzoitem = (lzo_item*) malloc( sizeof( lzo_item ) );
	strncpy( lzoitem->name, name, LZO_MAX_PATH );
	strncpy( lzoitem->file, file, LZO_MAX_PATH );
	lzoitem->name[LZO_MAX_PATH] = 0;
	lzoitem->file[LZO_MAX_PATH] = 0;
	
	_lzo_format_path( lzoitem->name );

	lzoitem->decom_size = 0;
	lzoitem->next = archive->header.items;
	archive->header.item_count ++;
	archive->header.items = lzoitem;
}

////////////////////////////////////////////////////////////////////////////////////////////
lzo_archive *lzo_load_archive( const char *archiveFile )
{
	lzo_archive *archive = (lzo_archive*) malloc( sizeof( lzo_archive ) );
	strncpy( archive->name, archiveFile, LZO_MAX_PATH );
	archive->fp = fopen( archiveFile, "rb" );
	if( archive->fp == 0 )
	{
		lzo_error_func( _lzo_error( "load lzo archive file %s failed", archiveFile ) );
	}
	
	/// read header compressed and decompressed size
	lzo_uint header_com_size, header_decom_size, file_count;
	fscanf( archive->fp, "%lu %lu %lu", &header_com_size, &header_decom_size, &file_count );
	
	/// ready to read file info
	lzo_file_info *file_infos = (lzo_file_info*) malloc( sizeof( lzo_file_info) * file_count );
	unsigned char *header_buf = (unsigned char*) malloc( header_com_size );
	fread( header_buf, header_com_size, 1, archive->fp );

	/// decompress the header
	lzo_uint tsize;
	int r = lzo1x_decompress( header_buf, header_com_size, (lzo_bytep) file_infos, &tsize, NULL );
    if( r != LZO_E_OK )
	{
		lzo_error_func( _lzo_error( "decompression header failed: %d\n", r ) );
		return archive;
    }

	/// save the file info
	archive->file_count = file_count;
	archive->files = file_infos;

	/// free resources
	free( header_buf );

	return archive;
}

lzo_file_info *lzo_get_file_info( lzo_archive *archive, const char *_file )
{
	char file[LZO_MAX_PATH];
	strcpy( file, _file );
	_lzo_format_path( file );
	
	/// append './' if there isnot
	if( strlen( file ) >= 2 && ( !( file[0] == '.' && ( file[1] == '\\' || file[1] == '/' ) ) ) )
	{
		char cfile[LZO_MAX_PATH];
		strcpy( cfile, file );
		strcpy( file, "./" );
		strcat( file, cfile );
	}

	for( lzo_uint i = 0; i < archive->file_count; ++ i )
	{
		if( _stricmp( file, archive->files[i].name ) == 0 )
		{
			return &archive->files[i];
		}
	}

	lzo_error_func( _lzo_error( "cannot find the file %s in the archive %s\n",
		_file, archive->name ) );

	return 0;
}

long lzo_read_file( lzo_archive *archive, lzo_file_info *info, void *pOutData )
{
	/// read the compress data
	unsigned char *com_buf = (unsigned char *) malloc( info->com_size );
	fseek( archive->fp, info->offset, SEEK_SET );
	fread( com_buf, info->com_size, 1, archive->fp );

	/// decompress the data
	lzo_uint tsize;
	int r = lzo1x_decompress( com_buf, info->com_size, (lzo_bytep) pOutData, &tsize, NULL );
    if( r != LZO_E_OK )
	{
		lzo_error_func( _lzo_error( "decompression file %s failed: %d\n", info->name, r ) );
		return 0;
    }
	else
	{
		lzo_error_func( _lzo_error( "%s decompressed file %s %lu bytes back into %lu bytes\n",
            archive->name, info->name, (lzo_uint)info->com_size, tsize ) );
	}

	/// free resources
	free( com_buf );

	return tsize;
}

unsigned long lzo_get_file_count( lzo_archive *archive )
{
	return archive->file_count ;
}

lzo_file_info *lzo_get_file_info_byid( lzo_archive *archive, unsigned long id )
{
	if( id >= archive->file_count )
	{
		lzo_error_func( _lzo_error( "invalid id %d to get file info\n", id ) );
		return 0;
	}

	return &archive->files[id];
}

void lzo_free_archive( lzo_archive *archive )
{
	free( archive->files );
	free( archive );
}

#ifdef __cplusplus
}
#endif
