/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    iosfwd.h
//
// = AUTHOR
//    Irfan Pyarali
//
// = DESCRIPTION
//    This file contains the portability ugliness for the Standard C++
//    Library.  As implementations of the "standard" emerge, this file
//    will need to be updated.
//
//    This files deals with forward declaration for the stream
//    classes.  Remember that since the new Standard C++ Library code
//    for streams uses templates, simple forward declaration will not
//    work.
//
// ============================================================================

#if !defined (ACE_IOSFWD_H)
#define ACE_IOSFWD_H

#include "ace/inc_user_config.h"

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

#include /**/ <iosfwd>

#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)

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

#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

# else /* ACE_HAS_STANDARD_CPP_LIBRARY */

class ios;
class streambuf;
class istream;
class ostream;
class iostream;
class filebuf;
class ifstream;
class ofstream;
class fstream;

# endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

#endif /* ACE_IOSFWD_H */
