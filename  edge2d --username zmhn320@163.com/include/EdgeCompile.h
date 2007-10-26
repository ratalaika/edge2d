/**
 *
 *
 *
 *
 */
#ifndef EDGE_COMPILE_H
#define EDGE_COMPILE_H

namespace Edge
{
/// platform
#define EDGE_PLATFORM_WIN32 1
#define EDGE_PLATFORM_LINUE 2

/// compiler
#define EDGE_COMPILER_MSVC 1
#define EDGE_COMPILER_GCC 2

/// determine the Compiler
#ifdef _MSC_VER
#define EDGE_COMPILER EDGE_COMPILER_MSVC
#endif

/// determine the Platform (OS )
#if defined (__WIN32__) || defined(_WIN32)
#define EDGE_PLATFORM EDGE_PLATFORM_WIN32
#endif

/// determine the compile type
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32

#if EDGE_COMPILER == EDGE_COMPILER_MSVC
#ifdef _DEBUG
#define EDGE_DEBUG
#endif
#endif

/// dynamic library, specify this Macro in compile settings
#ifdef EDGE_DYNAMIC
#define EDGE_EXPORT __declspec( dllexport )
#else
#define EDGE_EXPORT __declspec( dllimport )
#endif

/// disable some warnings
#if EDGE_COMPILER == EDGE_COMPILER_MSVC
/// sprintf <-> sprintf_s etc
#pragma warning( disable:4996 )
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#pragma warning( disable:4251 )
#endif

#endif

/// define Edge Version
#define EDGE_VERSION_MAJOR 0
#define EDGE_VERSION_MINOR 1
#define EDGE_VERSION_PATCH 0

#define EDGE_VERSION (( EDGE_VERSION_MAJOR << 16 ) | (EDGE_VERSION_MINOR << 8 ) | EDGE_VERSION_PATCH )


}

#endif