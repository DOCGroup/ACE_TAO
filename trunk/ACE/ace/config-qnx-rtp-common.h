// -*- C++ -*-
// $Id$
// several macros common to various qnx neutrino version.

#ifndef ACE_CONFIG_QNX_RTP_COMMON_H
#define ACE_CONFIG_QNX_RTP_COMMON_H
#include /**/ "ace/pre.h"

#define _POSIX_C_SOURCE 199506
#define _QNX_SOURCE

// The following defines the Neutrino compiler.
// gcc should know to call g++ as necessary
#ifdef __GNUC__
# define ACE_CC_NAME ACE_TEXT ("gcc")
#else
# define ACE_CC_NAME ACE_TEXT ("QNX-RTP compiler ??")
#endif

#include "ace/config-g++-common.h"

// /usr/nto/include/float.h defines
//  FLT_MAX_EXP 127
//  DBL_MAX_EXP 1023
//  ace expects 128 & 1024 respectively
//  to set the following macros in ace/Basic_Types.h
//  These macros are:
#define ACE_SIZEOF_DOUBLE   8
#define ACE_SIZEOF_FLOAT    4

// At least qnx 6.3.2 uses a void return for unsetenv
// This assumes that older versions do too.
#define ACE_HAS_VOID_UNSETENV

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_QNX_RTP_COMMON_H */
