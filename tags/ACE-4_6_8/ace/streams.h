/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    streams.h
//
// = AUTHOR
//    Irfan Pyarali
//
// = DESCRIPTION
//    This file contains the portability ugliness for the Standard C++
//    Library.  As implementations of the "standard" emerge, this file
//    will need to be updated.
//
//    This files deals with the streams includes.
//
// ============================================================================

#ifndef ACE_STREAMS_H
#define ACE_STREAMS_H

#include "ace/inc_user_config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

#if defined (_MSC_VER)
#pragma warning(disable: 4018 4114 4146 4245)
#pragma warning(disable: 4663 4664 4665 4511 4512)
#endif /* _MSC_VER */

#if defined (ACE_USES_OLD_IOSTREAMS)
#include /**/ <iostream.h>
#include /**/ <fstream.h>
#else
#include /**/ <iomanip>
#include /**/ <ios>
#include /**/ <iostream>
#include /**/ <istream>
#include /**/ <ostream>
#include /**/ <fstream>
#include /**/ <streambuf>
#endif /* ACE_USES_OLD_IOSTREAMS */

#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)

#if !defined (ACE_USES_OLD_IOSTREAMS)
// Make these available in the global name space
using std::ios;
using std::streambuf;
using std::istream;
using std::ostream;
using std::iostream;
using std::filebuf;
using std::ifstream;
using std::ofstream;
using std::fstream;

using std::cin;
using std::cout;
using std::cerr;
using std::clog;

using std::endl;
using std::ends;
using std::flush;

using std::ws;

using std::resetiosflags;
using std::setfill;
using std::setiosflags;
using std::setprecision;
using std::setw;

using std::dec;
using std::hex;
using std::oct;
#endif /* ! ACE_USES_OLD_IOSTREAMS */

#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

#if defined (_MSC_VER)
#pragma warning(4: 4018 4114 4146 4245)
#pragma warning(4: 4663 4664 4665 4512 4511)
#endif /* _MSC_VER */

#else /* ACE_HAS_STANDARD_CPP_LIBRARY */

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
#include /**/ <fstream.h>
#include /**/ <iostream.h>

#if defined (ACE_WIN32)
#if defined(_MSC_VER) // VSB
#include /**/ <ios.h>
#include /**/ <streamb.h>
#include /**/ <istream.h>
#include /**/ <ostream.h>
#include /**/ <iomanip.h>
#endif /* _MSC_VER */
#include /**/ <strstrea.h> // VSB
#endif /* ACE_WIN32 */
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

#endif /* ACE_STREAMS_H */
