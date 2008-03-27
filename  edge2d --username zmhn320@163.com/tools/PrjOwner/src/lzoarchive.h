/**
 * LZO extend library. It can read/write a LZO archive.
 *
 * Kevin Lynx
 * 10.19.2007
 */
#ifndef LZO_ARCHIVE_H
#define LZO_ARCHIVE_H

#include <stdio.h>
#include "minilzo/minilzo.h"

#define LZO_MAX_PATH 260

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable:4996 )
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * LZO Archive item info.
 *
 */
typedef struct _lzo_item
{
	char name[1+LZO_MAX_PATH];
	char file[1+LZO_MAX_PATH];
	lzo_uint offset;
	lzo_uint com_size;
	lzo_uint decom_size;
	_lzo_item *next;
}lzo_item;

/**
 * LZO Archive header
 *
 */
typedef struct _lzo_header
{
	lzo_uint item_count;
	lzo_item *items;
}lzo_header;

/**
 * LZO Archive file info.It's the main structure you will pay attention to.
 *
 *
 */
typedef struct _lzo_file_info
{
	char name[1+LZO_MAX_PATH];
	lzo_uint offset;
	lzo_uint com_size;
	lzo_uint decom_size;

}lzo_file_info;


/**
 * LZO Archive
 *
 */
typedef struct _lzo_archive
{
	char name[LZO_MAX_PATH+1];
	lzo_header header;
	// only used to decompress
	FILE *fp; 
	lzo_file_info *files;
	lzo_uint file_count;

}lzo_archive;


/**
 * These functions below is used to compress files into a LZO archive.
 * These functions includes : 
 * -- lzo_create_archive
 * -- lzo_save_archive
 * -- lzo_destroy_archive
 * -- lzo_add_item
 */

/**
 * init the lzo extend library, you can specify a callback function to get 
 * error information
 *
 */
void lzoex_init( void(*lzo_error_fun)( const char *) );

/**
 * create a lzo archive, the archive 's name is specified by file param.
 *
 */
lzo_archive *lzo_create_archive( const char *file );

/**
 * save archive, it will compress all the file item specified previosly and
 * write data to the archive.
 *
 * The function returns 0 if successfully.
 *
 * -------------------------------------------------------------------------
 * The archive 's format :
 * -file info compressed size : the library read these data into memory and decompress it.
 *  And then the library will read these data to get file information in archive.
 * -file info decompressed size
 * -file info item count : used to alloc memroy to maitain file info.
 * -file 1 compressed data
 * -file 2 compressed data
 * -...
 * -file n compressed data
 * -------------------------------------------------------------------------
 *
 */
int lzo_save_archive( lzo_archive *archive );

/**
 * destroy a lzo archive.it will close the file.
 *
 */
void lzo_destroy_archive( lzo_archive *archive );

/**
 * add an item into the archive, the file param must be avaiable 
 * file on the file system. 
 *
 */
void lzo_add_item( lzo_archive *archive, const char *file, const char *name );

/**
 * These functions below is used to decompress files from a LZO archive. 
 * These functions includes :
 * -- lzo_load_archive
 * -- lzo_get_file_info
 * -- lzo_read_file
 * -- lzo_free_archive
 * -- lzo_get_file_count
 * -- lzo_get_file_info_byid
 */
/**
 * load a LZO archive from file system (on the disk)
 * this function will get the file info in the archive
 *
 */
lzo_archive *lzo_load_archive( const char *archiveFile );

/**
 * get a file in the archive 's info, if the file is not exist in the archive 
 * return NULL
 *
 */
lzo_file_info *lzo_get_file_info( lzo_archive *archive, const char *file );

/**
 * read a file in the archive, it will return the byte count that really read. 
 * you should allocate memory and give the memory's address to this function.
 *
 */
long lzo_read_file( lzo_archive *archive, lzo_file_info *info, void *pOutData );

/**
 * get file count in the archive
 *
 */
unsigned long lzo_get_file_count( lzo_archive *archive );

/**
 * get a file info in the archive by the file 's id.
 *
 */
lzo_file_info *lzo_get_file_info_byid( lzo_archive *archive, unsigned long id );

/**
 * free the archive
 *
 */
void lzo_free_archive( lzo_archive *archive );

#ifdef __cplusplus
}
#endif

#endif