/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
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