/**
 *
 *
 */
#ifndef __LZO_MAIN_H_
#define __LZO_MAIN_H_

#include "lzoarchive.h"

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
 * split a full name 
 *
 */
void split_path( const char *full_name, char *path, char *file );

/**
 * these functions below mainly to deal with the program informations.
 *
 *
 */
#define LOG_FILE "./lzoarchive.log"

#ifdef __cplusplus
}
#endif

#endif
