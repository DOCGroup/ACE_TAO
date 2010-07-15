// $Id$

#ifndef ACE_IOS_SOCK_IOSTREAM_CPP
#define ACE_IOS_SOCK_IOSTREAM_CPP

#include "ace/INet/Sock_IOStream.h"
#include "ace/INet/IOS_util.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <ACE_SYNCH_DECL>
    Sock_StreamBufferBase<ACE_SYNCH_USE>::Sock_StreamBufferBase (stream_type* stream)
      : BidirStreamBuffer<StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> > (
            stream,
            BUFFER_SIZE,
            std::ios::in | std::ios::out)
      {
      }

    template <ACE_SYNCH_DECL>
    Sock_StreamBufferBase<ACE_SYNCH_USE>::~Sock_StreamBufferBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    Sock_IOSBase<ACE_SYNCH_USE>::Sock_IOSBase (stream_type* stream)
      : streambuf_ (stream)
      {
        ace_ios_init (&this->streambuf_);
      }

    template <ACE_SYNCH_DECL>
    Sock_IOSBase<ACE_SYNCH_USE>::~Sock_IOSBase ()
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
    typename Sock_IOSBase<ACE_SYNCH_USE>::buffer_type*
    Sock_IOSBase<ACE_SYNCH_USE>::rdbuf ()
      {
        return &this->streambuf_;
      }

    template <ACE_SYNCH_DECL>
    void Sock_IOSBase<ACE_SYNCH_USE>::close ()
      {
        this->streambuf_.sync ();
        this->streambuf_.close_stream ();
      }

    template <ACE_SYNCH_DECL>
    const typename Sock_IOSBase<ACE_SYNCH_USE>::stream_type&
    Sock_IOSBase<ACE_SYNCH_USE>::stream () const
      {
        return this->streambuf_.stream ();
      }


    template <ACE_SYNCH_DECL>
    Sock_OStreamBase<ACE_SYNCH_USE>::Sock_OStreamBase(stream_type* stream)
      : Sock_IOSBase<ACE_SYNCH_USE> (stream), std::ostream (Sock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    Sock_OStreamBase<ACE_SYNCH_USE>::~Sock_OStreamBase()
      {
      }

    template <ACE_SYNCH_DECL>
    void Sock_OStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

    template <ACE_SYNCH_DECL>
    Sock_IStreamBase<ACE_SYNCH_USE>::Sock_IStreamBase(stream_type* stream)
      : Sock_IOSBase<ACE_SYNCH_USE> (stream), std::istream (Sock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    Sock_IStreamBase<ACE_SYNCH_USE>::~Sock_IStreamBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    void Sock_IStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

    template <ACE_SYNCH_DECL>
    Sock_IOStreamBase<ACE_SYNCH_USE>::Sock_IOStreamBase(stream_type* stream)
      : Sock_IOSBase<ACE_SYNCH_USE> (stream), std::iostream (Sock_IOSBase<ACE_SYNCH_USE>::rdbuf ())
      {
      }

    template <ACE_SYNCH_DECL>
    Sock_IOStreamBase<ACE_SYNCH_USE>::~Sock_IOStreamBase ()
      {
      }

    template <ACE_SYNCH_DECL>
    void Sock_IOStreamBase<ACE_SYNCH_USE>::set_interceptor (
          typename buffer_type::interceptor_type& interceptor)
      {
        this->rdbuf ()->set_interceptor (interceptor);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_SOCK_IOSTREAM_CPP */
