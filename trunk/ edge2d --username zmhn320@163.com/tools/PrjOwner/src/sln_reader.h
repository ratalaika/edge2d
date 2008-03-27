/**
 *
 *
 */
#ifndef __SLN_READER_H_
#define __SLN_READER_H_

#ifdef __cplusplus
extern "C"
{
#endif

void compress_vcproj( const char *archive_name, const char *prj_file );
void compress_sln( const char *archive_name, const char *cfg_file );

#ifdef __cplusplus
}
#endif
#endif 