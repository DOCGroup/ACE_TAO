// $Id$

/**
 * @file IOS_util.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_UTIL_H
#define ACE_IOS_UTIL_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include <iostream>

// ace_ios_init
//
// There is reported to be a bug in the Dinkumware
// implementation of iostreams where calling
// basic_ios::init() multiple times for the same
// basic_ios instance creates a memory leak as a result
// of the ios' locale being allocated more than
// once, each time overwriting the old pointer.
//
// This usually happens in multiple inheritance
// schemes involving a derived IOS class and a
// derived IO stream class (i, o or io).
//
// The fix for Dinkumware is to not call init() in the
// derived IOS class but call init() only for each
// stream object - in the istream, ostream or
// iostream constructor.
//
// Other stream implementations, however, require that
// init() is called in the derived IOS constructor.
// So we replace each call to init() with the
// ace_ios_init macro defined below.


#if !defined(ACE_IOS_INIT_FIX)
  // Microsoft Visual Studio with Dinkumware STL (but not STLport)
# if defined(_MSC_VER) && (!defined(_STLP_MSVC) || defined(_STLP_NO_OWN_IOSTREAMS))
#   define ACE_IOS_INIT_FIX 1
    // QNX with Dinkumware but not GNU C++ Library
# elif defined(__QNX__) && !defined(__GLIBCPP__)
#   define ACE_IOS_INIT_FIX 1
# endif
#endif

#if defined(ACE_IOS_INIT_FIX)
# define ace_ios_init(buf)
#else
# define ace_ios_init(buf) this->init(buf)
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        class ACE_INET_Export Null
          {
            public:
              static std::ostream out_stream_;
              static std::istream in_stream_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_IOS_UTIL_H */
