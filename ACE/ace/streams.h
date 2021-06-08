// -*- C++ -*-

//=============================================================================
/**
 *  @file    streams.h
 *
 *  @author Irfan Pyarali
 *
 *  This file contains the portability ugliness for the Standard C++
 *  Library.  As implementations of the "standard" emerge, this file
 *  will need to be updated.
 *
 *  This files deals with the streams includes.
 */
//=============================================================================


#ifndef ACE_STREAMS_H
#define ACE_STREAMS_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Do this so the #pragma warning in the MSVC headers do not
// affect our #pragma warning settings
#if defined (_MSC_VER)
#pragma warning(push)
#endif /* _MSC_VER*/

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
#  if defined (_MSC_VER)
#    pragma warning(disable: 4018 4114 4146 4245)
#    pragma warning(disable: 4663 4664 4665 4511 4512)
#  endif /* _MSC_VER */

#  if defined (ACE_USES_OLD_IOSTREAMS)
#    include /**/ <iostream.h>
#    include /**/ <fstream.h>
  // This has been commented as it is not needed and causes problems with Qt.
  // (brunsch) But has been uncommented since it should be included.  Qt
  // probably should have some sort of macro that will prevent including this
  // when it is used.
#    include /**/ <iomanip.h>
#  else
#    include /**/ <iostream>
#    include /**/ <fstream>
#    include /**/ <istream>
#    include /**/ <ostream>
#    include /**/ <streambuf>
#    include /**/ <iomanip>
#    include /**/ <ios>
#  endif /* ACE_USES_OLD_IOSTREAMS */

#  if !defined (ACE_USES_OLD_IOSTREAMS)
      // Make these available in the global name space
      using std::ios;
      using std::ios_base;
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
#   endif /* ! ACE_USES_OLD_IOSTREAMS */

#   if defined (_MSC_VER)
#     pragma warning(4: 4018 4114 4146 4245)
#     pragma warning(4: 4663 4664 4665 4512 4511)
#   endif /* _MSC_VER */
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

// Do this so the #pragma warning in the MSVC headers do not
// affect our #pragma warning settings
#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ACE_STREAMS_H */
