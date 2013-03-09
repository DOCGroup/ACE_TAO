// $Id$

#ifndef ACE_IOS_STREAM_INTERCEPTOR_CPP
#define ACE_IOS_STREAM_INTERCEPTOR_CPP

#include "ace/INet/StreamInterceptor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <class ACE_CHAR_T, class TR>
    StreamInterceptorBase<ACE_CHAR_T, TR>::StreamInterceptorBase ()
      {
      }

    template <class ACE_CHAR_T, class TR>
    StreamInterceptorBase<ACE_CHAR_T, TR>::~StreamInterceptorBase ()
      {
      }

    template <class ACE_CHAR_T, class TR>
    void StreamInterceptorBase<ACE_CHAR_T, TR>::before_write (const char_type* /*buffer*/,
                                              std::streamsize /*length_to_write*/)
      {
      }

    template <class ACE_CHAR_T, class TR>
    void StreamInterceptorBase<ACE_CHAR_T, TR>::after_write (int /*length_written*/)
      {
      }

    template <class ACE_CHAR_T, class TR>
    void StreamInterceptorBase<ACE_CHAR_T, TR>::before_read (std::streamsize /*length_to_read*/)
      {
      }

    template <class ACE_CHAR_T, class TR>
    void StreamInterceptorBase<ACE_CHAR_T, TR>::after_read (const char_type* /*buffer*/,
                                            int /*length_read*/)
      {
      }

    template <class ACE_CHAR_T, class TR>
    void StreamInterceptorBase<ACE_CHAR_T, TR>::on_eof ()
      {
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_STREAM_INTERCEPTOR_CPP */
