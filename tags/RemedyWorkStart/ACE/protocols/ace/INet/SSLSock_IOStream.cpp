// $Id$

#ifndef ACE_IOS_SSLSOCK_IOSTREAM_CPP
#define ACE_IOS_SSLSOCK_IOSTREAM_CPP

#include "ace/INet/SSLSock_IOStream.h"
#include "ace/INet/IOS_util.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <ACE_SYNCH_DECL>
    SSLSock_StreamBufferBase<ACE_SYNCH_USE>::SSLSock_StreamBufferBase (stream_type* stream)
      : BidirStreamBuffer<StreamHandler<ACE_SSL_SOCK_Stream, ACE_SYNCH_USE> > (
            stream,
            BUFFER_SIZE,
            std::ios::in | std::ios::out)
      {
      }

    template <ACE_SYNCH_DECL>
    SSLSock_StreamBufferBase<ACE_SYNCH_USE>::~SSLSock_StreamBufferBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IOSBase<ACE_SYNCH_USE>::SSLSock_IOSBase (stream_type* stream)
      : streambuf_ (stream)
      {
        ace_ios_init (&this->streambuf_);
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IOSBase<ACE_SYNCH_USE>::~SSLSock_IOSBase ()
      {
        try
          {
            this->streambuf_.sync();
          }
        catch (...)
          {
          }
      }

    template <ACE_SYNCH_DECL>
    typename SSLSock_IOSBase<ACE_SYNCH_USE>::buffer_type*
    SSLSock_IOSBase<ACE_SYNCH_USE>::rdbuf ()
      {
        return &this->streambuf_;
      }

    template <ACE_SYNCH_DECL>
    void SSLSock_IOSBase<ACE_SYNCH_USE>::close ()
      {
        this->streambuf_.sync ();
        this->streambuf_.close_stream ();
      }

    template <ACE_SYNCH_DECL>
    const typename SSLSock_IOSBase<ACE_SYNCH_USE>::stream_type&
    SSLSock_IOSBase<ACE_SYNCH_USE>::stream () const
      {
        return this->streambuf_.stream ();
      }


    template <ACE_SYNCH_DECL>
    SSLSock_OStreamBase<ACE_SYNCH_USE>::SSLSock_OStreamBase(stream_type* stream)
      : SSLSock_IOSBase<ACE_SYNCH_USE> (stream), std::ostream (SSLSock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    SSLSock_OStreamBase<ACE_SYNCH_USE>::~SSLSock_OStreamBase()
      {
      }

    template <ACE_SYNCH_DECL>
    void SSLSock_OStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IStreamBase<ACE_SYNCH_USE>::SSLSock_IStreamBase(stream_type* stream)
      : SSLSock_IOSBase<ACE_SYNCH_USE> (stream), std::istream (SSLSock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IStreamBase<ACE_SYNCH_USE>::~SSLSock_IStreamBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    void SSLSock_IStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IOStreamBase<ACE_SYNCH_USE>::SSLSock_IOStreamBase(stream_type* stream)
      : SSLSock_IOSBase<ACE_SYNCH_USE> (stream), std::iostream (SSLSock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    SSLSock_IOStreamBase<ACE_SYNCH_USE>::~SSLSock_IOStreamBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    void SSLSock_IOStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_SSLSOCK_IOSTREAM_CPP */
