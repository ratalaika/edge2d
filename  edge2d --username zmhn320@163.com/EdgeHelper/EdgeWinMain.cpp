/**
 *
 *
 */
#include "EdgeCompile.h"
#include <vector>
#include <string>
#include <windows.h>

using std::vector;
using std::string;

int main( int argc, char **argv );

#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32

vector<string> spitCmdLine()
{
	vector<string> Argvs;

	const char *cmdLine = ::GetCommandLineA();
	size_t size = strlen( cmdLine );
	string cmdLineS( cmdLine );
	size_t pos ;

	/// get the exe file name
	if( ( pos = cmdLineS.find( '"', 1 ) ) < cmdLineS.size() )
	{
		/// the file name is quoted in '"', and it's the full path, i only need the file name.
		string file( cmdLineS, 1, pos - 1 );
		size_t npos = file.rfind( '\\' );
		file.assign( file, npos + 1, file.size() - npos );
		Argvs.push_back( file );

		pos = cmdLineS.find( ' ' );
		if( pos >= cmdLineS.size() - 1 )
		{
			return Argvs;
		}
		pos ++;
	}
	else
	{
		/// the exe file is runned in the CMD program, so the file name is not quoted in '"'
		pos = cmdLineS.find( ' ' );
		if( pos >= cmdLineS.size() - 1 )
		{
			/// the argument is only the file name
			Argvs.push_back( cmdLineS );
			return Argvs;
		}
		else
		{
			string file( cmdLineS, 0, pos );
			Argvs.push_back( file );
		}

		pos ++;
	}

	size_t lastPos = pos;
	pos = cmdLineS.find( ' ', pos );
	while( pos < cmdLineS.size() )
	{
		string argv( cmdLineS, lastPos, pos - lastPos );
		Argvs.push_back( argv );

		lastPos = pos + 1;
		pos = cmdLineS.find( ' ', pos + 1 );
	}

	string argv( cmdLineS, lastPos, pos - lastPos );
	Argvs.push_back( argv );

	return Argvs;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	/// spit the command line
	vector<string> argStr = spitCmdLine();	
	vector<char*> argv( argStr.size() );
	for( size_t i = 0; i < argv.size(); ++i )
	{
		argv[i] = const_cast<char*>( argStr[i].c_str() );
	}

	return main( (int)argv.size(), &argv[0] );
}
#endif
