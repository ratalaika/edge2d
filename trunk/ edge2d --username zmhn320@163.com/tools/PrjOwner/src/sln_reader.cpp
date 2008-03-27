/**
 * sln_reader.cpp
 *
 * read vcproj file to compress vc source code file
 * Kevin Lynx
 * 3.27.2008
 *
 */
#include "../tinyxml/tinyxml.h"
#include <windows.h>
#include <stdio.h>
#include "lzo_main.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * construct a valid file
 *
 */
const char *construc_valid_file( const char *rel_file_name, const char *path, char *real_file_name )
{
	if( strncmp( rel_file_name, ".\\", 2 ) == 0 ||
		strncmp( rel_file_name, "./", 2 ) == 0 )
	{
		strcpy( real_file_name, path );
		strcat( real_file_name, &rel_file_name[1] );
	}
	else
	{
		strcpy( real_file_name, path );
		strcat( real_file_name, "/" );
		strcat( real_file_name, rel_file_name );
	}

	return real_file_name;
}

/**
 * get file name list in a vcproj file.
 *
 */
file_name *get_file_name_list_vcproj( const char *path, const char *prj_file )
{
	TiXmlDocument xmldoc;
	TiXmlElement *root_elem;
	file_name *header = 0;
	char full_name[MAX_PATH];

	if( !xmldoc.LoadFile( prj_file ) )
	{
		return NULL;
	}

	header = (file_name*) malloc( sizeof( file_name ) );
	header->next = 0;
	strcpy( header->name, prj_file );
	strcpy( header->file, prj_file );

	root_elem = xmldoc.RootElement();
	TiXmlElement *file_root_elem = root_elem->FirstChildElement( "Files" );
	if( file_root_elem != NULL )
	{
		TiXmlElement *filter_root_elem = file_root_elem->FirstChildElement( "Filter" );
		while( filter_root_elem != NULL )
		{
			TiXmlElement *file_elem = filter_root_elem->FirstChildElement( "File" );
			while( file_elem != NULL )
			{
				const char *file_name_str = file_elem->Attribute( "RelativePath" );

				file_name *new_file = (file_name*) malloc( sizeof( file_name ) );
				new_file->next = 0;
				construc_valid_file( file_name_str, path, full_name );
				strcpy( new_file->name, full_name );
				strcpy( new_file->file, full_name );

				if( header == 0 )
				{
					header = new_file;
				}
				else
				{
					new_file->next = header;
					header = new_file;
				}

				/// retrieve next element
				file_elem = file_elem->NextSiblingElement( "File" );
			}

			/// retrieve next element
			filter_root_elem = filter_root_elem->NextSiblingElement( "Filter" );
		}
	}

	xmldoc.Clear();
	return header;
}

/**
 * compress the vcproj file and the source code file specified in the vcproj file.
 *
 */
void compress_vcproj( const char *archive_name, const char *prj_file )
{
	char path[MAX_PATH];
	char file[MAX_PATH];
	
	split_path( prj_file, path, file );

	/// create a lzo archive
	lzo_archive *archive = lzo_create_archive( archive_name );
	
	/// get a file list in the specidied directory
	file_name *file_list = get_file_name_list_vcproj( path, prj_file );

	/// add file list into the archive
	add_file_list( archive, file_list );

	/// save the archive
	lzo_save_archive( archive );

	/// ok, just free resources
	lzo_destroy_archive( archive );
	free_file_name_list( file_list );

}

/**
 * compress a group of vcproj file
 *
 */
void compress_sln( const char *archive_name, const char *cfg_file )
{
	char path[MAX_PATH];
	char file[MAX_PATH];
	char sln_file_name[MAX_PATH];	
	char prj_file_name[MAX_PATH];
	file_name *file_list = 0;

	/// create a lzo archive
	lzo_archive *archive = lzo_create_archive( archive_name );

	/// get the solution file
	FILE *fp = fopen( cfg_file, "r" );
	if( fp == NULL )
	{
		return ;
	}

	/// solution file
	fscanf( fp, "%s", sln_file_name );
	file_name *sln_file = (file_name*) malloc( sizeof( file_name ) );
	sln_file->next = 0;
	strcpy( sln_file->name, sln_file_name );
	strcpy( sln_file->file, sln_file_name );
	file_list = sln_file;

	file_name *prj_file;
	while( feof( fp ) == 0 )
	{
		fscanf( fp, "%s", prj_file_name );
		split_path( prj_file_name, path, file );

		/// get the file list specified in the vcprj file
		prj_file = get_file_name_list_vcproj( path, prj_file_name );
		get_tail( file_list )->next = prj_file;
	}

	/// add file list into the archive
	add_file_list( archive, file_list );

	/// save the archive
	lzo_save_archive( archive );

	/// ok, just free resources
	lzo_destroy_archive( archive );
	free_file_name_list( file_list );

}

#ifdef __cplusplus
}
#endif
