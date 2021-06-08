// -*- C++ -*-

//=============================================================================
/**
 *  @file    iosfwd.h
 *
 *  @author Irfan Pyarali
 *
 *  This file contains the portability ugliness for the Standard C++
 *  Library.  As implementations of the "standard" emerge, this file
 *  will need to be updated.
 *
 *  This files deals with forward declaration for the stream
 *  classes.  Remember that since the new Standard C++ Library code
 *  for streams uses templates, simple forward declaration will not
 *  work.
 */
//=============================================================================


#ifndef ACE_IOSFWD_H
#define ACE_IOSFWD_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// We except that each platform does support std streams
#define ACE_HAS_CPP98_IOSTREAMS 1

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

#if !defined (ACE_USES_OLD_IOSTREAMS)
#   include /**/ <iosfwd>
#else
  // @note If these forward declarations don't work (e.g. aren't
  //       portable), we may have to include "ace/streams.h" as a last
  //       resort.  Doing so would defeat the purpose of this header,
  //       unfortunately.
  class ios;
  class streambuf;
  class istream;
  class ostream;
  class iostream;
  class filebuf;
  class ifstream;
  class ofstream;
  class fstream;
#endif /* ! ACE_USES_OLD_IOSTREAMS */

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
#endif /* ! ACE_USES_OLD_IOSTREAMS */

#include /**/ "ace/post.h"

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

#endif /* ACE_IOSFWD_H */
