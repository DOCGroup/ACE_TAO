/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   config-win32-msvc-15.h
 *
 *  @brief  Microsoft Visual C++ 15.0 configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++ version 2017.
 *
 *  @note Do not include this file directly, include config-win32.h instead.
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_15_H
#define ACE_CONFIG_WIN32_MSVC_15_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#ifndef ACE_WIN32_VC15
#  define ACE_WIN32_VC15
#endif

// Until we have specific msvc15 settings, include the msvc12 file
#include "ace/config-win32-msvc-14.h"

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_15_H */
