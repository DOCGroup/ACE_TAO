/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   config-win32.h
 *
 *  $Id$
 *
 *  @brief  Microsoft Windows configuration file. 
 *
 *  This file is the ACE configuration file for all of Microsoft Windows
 *  platforms that ACE runs on.  Based on preprocessor definitions, it 
 *  includes other more specific configuration files.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_H
#define ACE_CONFIG_WIN32_H
#include "ace/pre.h"



// NOTE: Please do not add anything besides #include's here.  Put other stuff
//       (definitions, etc.) in the included headers



// Include the platform specific config file (Windows CE is special)

#if defined (_WIN32_WCE)
#	include "ace/config-win32-wince.h"
#else /* _WIN32_WCE */
#   include "ace/config-win32-common.h"
#endif /* _WIN32_WCE */

// Include the config-win32-* file specific to the compiler

#if defined (_MSC_VER)
#    include "ace/config-win32-msvc.h"
#elif defined (__BORLANDC__)
#    include "ace/config-win32-borland.h"
#elif defined (__IBMCPP__)
#    include "ace/config-win32-visualage.h"
#elif defined (ghs)
#    include "ace/config-win32-ghs.h"
#else
#    error Compiler is not supported
#endif

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_H */
