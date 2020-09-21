#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_VNCPCORELIB_20001102_INCLUDED
#define _INC_VNCPCORELIB_20001102_INCLUDED

//#ifdef _NDEBUG
//#undef _STLP_DEBUG
//#else
//#define _STLP_DEBUG 1
//#endif

//#define _WIN32_WINNT 0x0500
//#define NO_VINCADB 1


// Для использования VincaDB
#ifdef VINCADB_EXPORTS
#define BOOST_DECL_EXPORTS
#define VINCALIB_API __declspec(dllexport)
#define      EXPIMP
#elif STATIC_VINCADB
#define VINCALIB_API 
#define      EXPIMP  extern
#elif VINCADB_IMPORTS
#define BOOST_DECL_IMPORTS
#define VINCALIB_API __declspec(dllimport)
#define      EXPIMP  extern
#endif

#ifdef NO_VINCADB
#undef VINCALIB_API 
#define      EXPIMP  extern
#endif


// Для использования VincaAE
#ifdef VINCAAE_EXPORTS
#define BOOST_DECL_EXPORTS
#define VINCAAE_API __declspec(dllexport)
//#define      EXPIMP
#else
#define BOOST_DECL_IMPORTS
#define VINCAAE_API __declspec(dllimport)
//#define      EXPIMP  extern
#endif
#ifndef __VINCAHIS_H__
#define __VINCAHIS_H__

// Для использования CCycleArchive
#ifdef VINCAHIS_EXPORTS
#define BOOST_DECL_EXPORTS
#define VINCAHIS_API __declspec(dllexport)
#define      EXPIMP extern
#else
#define BOOST_DECL_IMPORTS
#define VINCAHIS_API __declspec(dllimport)
//#define      EXPIMP  extern
#endif

#endif //__VINCAHIS_H__
#endif