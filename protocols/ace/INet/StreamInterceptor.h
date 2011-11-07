// $Id$

/**
 * @file StreamInterceptor.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_STREAM_INTERCEPTOR_H
#define ACE_IOS_STREAM_INTERCEPTOR_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include <iosfwd>
#include <ios>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        /**
        * @class ACE_IOS_StreamInterceptorBase
        *
        * @brief Abstract base for stream interceptors.
        *
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class StreamInterceptorBase
          {
            public:
              typedef std::basic_ios<ACE_CHAR_T, TR> ios_type;
              typedef ACE_CHAR_T char_type;
              typedef typename ios_type::openmode openmode;

              virtual ~StreamInterceptorBase ();
            protected:
              StreamInterceptorBase ();

            public:
              virtual void before_write (const char_type* buffer,
                                         std::streamsize length_to_write);
              virtual void after_write (int length_written);
              virtual void before_read (std::streamsize length_to_read);
              virtual void after_read (const char_type* buffer,
                                       int length_read);
              virtual void on_eof (void);
          };

        typedef StreamInterceptorBase<char> StreamInterceptor;

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/StreamInterceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("StreamInterceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_STREAM_INTERCEPTOR_H */
