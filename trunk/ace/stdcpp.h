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
//    Irfan and Tim and Darrell
//
// = DESCRIPTION
//    This file contains the portability ugliness for the Standard C++
//    Library.  As implementations of the "standard" emerge, this file
//    will need to be updated.
// 
// ============================================================================

#if !defined (ACE_STDCPP_H)
#define ACE_STDCPP_H

#include "ace/config.h"

#if !defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

#   if defined (_MSC_VER)
#     pragma warning(disable: 4018 4114 4146 4245)
#     pragma warning(disable: 4663 4664 4665 4511 4512)
#   endif

    // For some reason, The Standard C++ Library has decided to save space
    // and ommit the file extensions.
#   include /**/ <iomanip>
#   include /**/ <ios>
#   include /**/ <iostream>
#   include /**/ <istream>
#   include /**/ <ostream>
#   include /**/ <fstream>
#   include /**/ <streambuf>


/*  As more compilers conform to Draft 2, ACE should be modified to use the 
    namespace instead of promoting names to the global namespace.  For those
    compilers that don't support it yet, there could be  a global struct that
    looks like:

    struct std
    {
      typedef ostream ::ostream;
      
      // ...
    }
*/

// In case iostream.h is not #included before this header . . .
#   if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) &&		\
	    (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)
      using std::istream; 
      using std::ostream;
      using std::ofstream;
      using std::endl;
      using std::flush;
      using std::iostream;
      using std::cerr; 
      using std::cout; 
      using std::cin;
      using std::streambuf;
      using std::ios;
#   endif // ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 

#   if defined (_MSC_VER)
#     pragma warning(4: 4018 4114 4146 4245)
#     pragma warning(4: 4663 4664 4665 4512 4511)
#   endif

# else /* ACE_HAS_STANDARD_CPP_LIBRARY */
#   include /**/ <fstream.h>
#   include /**/ <iomanip.h>
#   include /**/ <iostream.h>
#   include /**/ <istream.h>
#   include /**/ <ostream.h>

#   if defined (ACE_WIN32)
#     include /**/ <ios.h>
#     include /**/ <streamb.h>
#     include /**/ <strstrea.h>
#   endif /* ACE_WIN32 */
# endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

// Now

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
# if defined (_MSC_VER)
#   pragma warning(disable: 4018 4114 4146 4245)
#   pragma warning(disable: 4663 4664 4665 4511 4512)
# endif /* _MSC_VER */

# include /**/ <cassert>
# include /**/ <climits>
# include /**/ <cstdio>
# include /**/ <new>
# include /**/ <cctype>
# include /**/ <csignal>
# include /**/ <cstring>
# include /**/ <cstdarg>
# include /**/ <cerrno>
# include /**/ <cstdlib>

# if defined (_MSC_VER)
#   pragma warning(4: 4018 4114 4146 4245)
#   pragma warning(4: 4663 4664 4665 4512 4511)
# endif

#else /* ACE_HAS_STANDARD_CPP_LIBRARY && ACE_HAS_STANDARD_CPP_LIBRARY */

# include /**/ <assert.h>
# include /**/ <limits.h>
# include /**/ <stdio.h>
# include /**/ <new.h>
# include /**/ <ctype.h>
# include /**/ <signal.h>
# include /**/ <string.h>
# include /**/ <stdarg.h>
# include /**/ <errno.h>
# include /**/ <stdlib.h>

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY && ACE_HAS_STANDARD_CPP_LIBRARY */

#endif // ACE_STDCPP_H