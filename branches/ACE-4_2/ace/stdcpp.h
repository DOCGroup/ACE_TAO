/* -*- C++ -*- */
// $Id: stdcpp.h

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    stdcpp.h
//
// = AUTHOR
//    Irfan and Tim
//
// = DESCRIPTION
//    This file contains the portability ugliness for the Standard C++
//    Library.  As implementations of the "standard" emerge, this file
//    will need to be updated.
// 
// ============================================================================

#include "ace/config.h"

#if defined (ACE_WIN32)
#if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
// For some reason, The Standard C++ Library has decided to save space
// and ommit the file extensions.
#include /**/ <iomanip>
#include /**/ <ios>
#include /**/ <iostream>
#include /**/ <istream>
#include /**/ <ostream>
#include /**/ <streambuf>
#else /* ACE_HAS_STANDARD_CPP_LIBRARY */
#include /**/ <fstream.h>
#include /**/ <iomanip.h>
#include /**/ <ios.h>
#include /**/ <iostream.h>
#include /**/ <istream.h>
#include /**/ <ostream.h>
#include /**/ <streamb.h>
#include /**/ <strstrea.h>
#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
#else /* ACE_WIN32 */
// All platforms except for NT include iostream.h.
#include /**/ <iostream.h>
#endif /* ACE_WIN32 */

