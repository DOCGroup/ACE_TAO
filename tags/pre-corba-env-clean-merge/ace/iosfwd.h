/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    iosfwd.h
 *
 *  $Id$
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
 *
 *
 */
//=============================================================================


#ifndef ACE_IOSFWD_H
#define ACE_IOSFWD_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY)  && \
    (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

# if !defined (ACE_USES_OLD_IOSTREAMS)  || \
    defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
#   include /**/ <iosfwd>
# endif /* ! ACE_USES_OLD_IOSTREAMS  ||  ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

# if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
             (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)

#   if !defined (ACE_USES_OLD_IOSTREAMS)
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
#   endif /* ! ACE_USES_OLD_IOSTREAMS */

# endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

#else /* ! ACE_HAS_STANDARD_CPP_LIBRARY */

  class ios;
  class streambuf;
  class istream;
  class ostream;
  class iostream;
  class filebuf;
  class ifstream;
  class ofstream;
  class fstream;

# endif /* ! ACE_HAS_STANDARD_CPP_LIBRARY */

#include "ace/post.h"
#endif /* ACE_IOSFWD_H */
