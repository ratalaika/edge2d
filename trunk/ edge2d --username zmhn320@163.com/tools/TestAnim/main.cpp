/**
 *
 *
 */
#include <iostream>
#include <stdio.h>
#include <windows.h>

using namespace std;

struct AnimInfo
{
	char name[64];
	float speed;
	int play_count;
	int frame_count;
	int start_f, end_f;
};

struct FrameInfo
{
	char file[256];
	float x, y, w, h;
};

AnimInfo *g_anims;
int g_animCount;
FrameInfo *g_frames;
int g_frameCount;

void testAnim( const char *file );

int main( int argc, char **argv )
{

	if( argc < 3 )
	{
		cout << "Error parameters" << endl;
		cout << "i.e : AnimCreator input_file output_file" << endl;
		return 0;
	}

	FILE *fp = fopen( argv[1], "rb" );
	fscanf( fp, "%d%d", &g_animCount, &g_frameCount );
	g_anims = new AnimInfo [g_animCount];
	g_frames = new FrameInfo [g_frameCount];

	for( int i = 0; i < g_animCount; ++ i )
	{
		fscanf( fp, "%s%f%d%d%d%d", g_anims[i].name, &g_anims[i].speed,
			&g_anims[i].play_count, &g_anims[i].frame_count, &g_anims[i].start_f, &g_anims[i].end_f );
	}
	for( int i = 0; i < g_frameCount; ++ i )
	{
		fscanf( fp, "%s%f%f%f%f", g_frames[i].file, 
			&g_frames[i].x, &g_frames[i].y, &g_frames[i].w, &g_frames[i].h );
	}

	fclose( fp );

	//// wirte output file
	fp = fopen( argv[2], "wb" );

	char header[sizeof(int)*2+2];
	sprintf( header, "%d %d ", g_animCount, g_frameCount );

	fwrite( header, sizeof( header ), 1, fp );
	fwrite( g_anims, sizeof( AnimInfo ) * g_animCount, 1, fp );
	fwrite( g_frames, sizeof( FrameInfo ) * g_frameCount, 1, fp );
	fclose( fp );

	testAnim( argv[2] );

	return 0;
}

void testAnim( const char *file )
{
	AnimInfo *anims;
	FrameInfo *frames;
	int animCount;
	int frameCount;

	FILE *fp = fopen( file, "rb" );
	fseek(fp, 0, SEEK_END);
	size_t FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *buf = new char [FileSize];
	fread( buf, FileSize, 1, fp );
	fclose( fp );

	sscanf( buf, "%d%d", &animCount, &frameCount );
	anims = new AnimInfo [animCount];
	frames = new FrameInfo [frameCount];
	
	int offset = sizeof( int ) * 2 + 2;
	memcpy( anims, buf + offset, sizeof( AnimInfo ) * animCount );
	offset += sizeof( AnimInfo ) * animCount;
	
	memcpy( frames, buf + offset, sizeof( FrameInfo ) * frameCount );

	delete [] buf;

	/// output result
	for( int i = 0; i < animCount; ++ i )
	{
		cout << anims[i].name << '\t' << anims[i].speed << '\t' <<
			anims[i].play_count << '\t' << anims[i].frame_count << '\t' <<
			anims[i].start_f << '\t' << anims[i].end_f << endl;
	}
	for( int i = 0; i < frameCount; ++ i )
	{
		cout << frames[i].file << '\t' << frames[i].x << '\t' << 
			frames[i].y << '\t' << frames[i].w << '\t' <<
			frames[i].h << endl;
	}

	delete [] anims;
	delete [] frames;
}