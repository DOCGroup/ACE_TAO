/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    HTBP_Stream.cpp
 *
 *  $Id$
 *
 *  @author Phil Mesnier, Priyanka Gontla
 */
//=============================================================================
#include "HTBP_Stream.h"

#include "HTBP_Session.h"
#include "HTBP_Filter_Factory.h"

#include "ace/Message_Block.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Initialization and termination methods.
/// Constructor.
ACE::HTBP::Stream::Stream (ACE::HTBP::Session *s)
  : session_ (s)
{
  if (s == 0)
    // create a temporary session to be replaced on first recv.
    ACE_NEW (session_, ACE::HTBP::Session);
  session_->stream (this);
}

/// Destructor.
ACE::HTBP::Stream::~Stream (void)
{
}

  /// Dump the state of an object.
void
ACE::HTBP::Stream::dump (void) const
{
}



//---------------------------------------------------------------------------
// = I/O functions.

/// Recv an <n> byte buffer from the connected socket.
ssize_t
ACE::HTBP::Stream::recv (void *buf,
                         size_t n,
                         int flags,
                         const ACE_Time_Value *timeout) const
{
  if (this->session_->inbound() == 0)
    {
      errno = EWOULDBLOCK;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Stream::")
                         ACE_TEXT("recv(buf,n,flags) called, but no ")
                         ACE_TEXT("inbound channel connected to stream\n")),
                        -1);
    }
  return this->session_->inbound()->recv(buf,n,flags,timeout);
}

  /// Recv an <n> byte buffer from the connected socket.
ssize_t
ACE::HTBP::Stream::recv (void *buf,
                         size_t n,
                         const ACE_Time_Value *timeout) const
{
  if (this->session_->inbound() == 0)
    {
      errno = EWOULDBLOCK;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Stream::")
                         ACE_TEXT("recv(buf,n) called, but no ")
                         ACE_TEXT("inbound channel connected to stream\n")),
                        -1);
    }
  return this->session_->inbound()->recv(buf,n,timeout);
}

  /// Recv an <iovec> of size <n> from the connected socket.
ssize_t
ACE::HTBP::Stream::recvv (iovec iov[],
                  int iovcnt,
                  const ACE_Time_Value *timeout) const
{
  if (this->session_->inbound() == 0)
    {
      errno = EWOULDBLOCK;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Stream::")
                         ACE_TEXT("recv(iov,iovcnt) called, but no ")
                         ACE_TEXT("inbound channel connected to stream\n")),
                        -1);
    }
  return this->session_->inbound()->recvv(iov,iovcnt,timeout);
}

ssize_t
ACE::HTBP::Stream::recvv (iovec *io_vec,
                  const ACE_Time_Value *timeout) const
{
  if (this->session_->inbound() == 0)
    {
      errno = EWOULDBLOCK;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Stream::")
                         ACE_TEXT("recv(io_vec) called, but no ")
                         ACE_TEXT("inbound channel connected to stream\n")),
                        -1);
    }
  return this->session_->inbound()->recvv(io_vec,timeout);
}

ssize_t
ACE::HTBP::Stream::recv (void *,
                 size_t,
                 ACE_OVERLAPPED *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: Asynch ")
                     ACE_TEXT("recv not supported\n")),-1);
}

ssize_t
ACE::HTBP::Stream::send (const void *buf,
                 size_t n,
                 int flags,
                 const ACE_Time_Value *timeout) const
{
  if (this->session_->outbound() == 0)
    {
      ACE_Message_Block *msg = 0;
      ACE_NEW_RETURN (msg,ACE_Message_Block(n),-1);
      msg->copy ((const char *)buf,n);
      // probably ought to separately enqueue the flags and put the data buf
      // in a continuation message
      // Also, the timeout poses another interesting problem.
      return this->session_->enqueue(msg);
    }
  return this->session_->outbound()->send(buf,n,flags,timeout);
}

ssize_t
ACE::HTBP::Stream::send (const void *buf,
                 size_t n,
                 const ACE_Time_Value *timeout) const
{
  if (this->session_->outbound() == 0)
    {
      ACE_Message_Block *msg = 0;
      ACE_NEW_RETURN (msg,ACE_Message_Block(n),-1);
      msg->copy ((const char *)buf,n);
      return this->session_->enqueue(msg);
    }
  return this->session_->outbound()->send(buf,n,timeout);
}

ssize_t
ACE::HTBP::Stream::sendv (const iovec iov[],
                  int iovcnt,
                  const ACE_Time_Value *timeout) const
{
  if (this->session_->outbound() == 0)
    {
      ACE_Message_Block *msg = 0;
      size_t total = 0;
      int i = 0;
      for (; i < iovcnt; i++)
        total += iov[i].iov_len;
      ACE_NEW_RETURN (msg,ACE_Message_Block(total),-1);
      for (i = 0; i < iovcnt; i++)
        msg->copy ((const char *)iov[i].iov_base,iov[i].iov_len);
      return this->session_->enqueue(msg);
    }
  return this->session_->outbound()->sendv(iov,iovcnt,timeout);
}

ssize_t
ACE::HTBP::Stream::send (const void *,
                 size_t ,
                 ACE_OVERLAPPED *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: Asynch ")
                     ACE_TEXT("send not supported\n")),
                    -1);
}

ssize_t
ACE::HTBP::Stream::recv_n (void *,
                   size_t ,
                   int ,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: recv_n not supported\n")),
                    -1);
}

  /// Try to recv exactly <len> bytes into <buf> from the connected socket.
ssize_t
ACE::HTBP::Stream::recv_n (void *,
                   size_t ,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: recv_n not supported\n")),
                    -1);
}

  /// Receive an <iovec> of size <iovcnt> from the connected socket.
ssize_t
ACE::HTBP::Stream::recvv_n (iovec [],
                   int ,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: recvv_n not supported\n")),
                    -1);
}

  /// Try to send exactly <len> bytes from <buf> to the connection socket.
ssize_t
ACE::HTBP::Stream::send_n (const void *,
                   size_t ,
                   int ,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: send_n not supported\n")),
                    -1);
}

  /// Try to send exactly <len> bytes from <buf> to the connected socket.
ssize_t
ACE::HTBP::Stream::send_n (const void *,
                   size_t ,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: send_n not supported\n")),
                    -1);
}

  /// Send all the <message_block>s chained through their <next> and
  /// <cont> pointers.  This call uses the underlying OS gather-write
  /// operation to reduce the domain-crossing penalty.
ssize_t
ACE::HTBP::Stream::send_n (const ACE_Message_Block *,
                   const ACE_Time_Value *,
                   size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: send_n not supported\n")),
                    -1);
}

  /// Send an <iovec> of size <iovcnt> to the connected socket.
ssize_t
ACE::HTBP::Stream::sendv_n (const iovec [],
                    int,
                    const ACE_Time_Value *,
                    size_t *) const
{
  errno = ENOTSUP;
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT("ACE::HTBP::Stream: sendv_n not supported\n")),
                    -1);
}

int
ACE::HTBP::Stream::close_reader (void)
{
  return this->session_->close_inbound();
}

int
ACE::HTBP::Stream::close_writer (void)
{
  return this->session_->close_outbound();
}

int
ACE::HTBP::Stream::close (void)
{
  return this->session_->close();
}

int
ACE::HTBP::Stream::enable (int value) const
{
  return this->session_->enable(value);
}

int
ACE::HTBP::Stream::disable (int value) const
{
  return this->session_->disable(value);
}

int
ACE::HTBP::Stream::get_local_addr (ACE::HTBP::Addr &local_addr) const
{
  local_addr = this->session_->local_addr ();
  return 0;
}

int
ACE::HTBP::Stream::get_remote_addr (ACE::HTBP::Addr &peer_addr) const
{
  peer_addr = this->session_->peer_addr();
  return 0;
}

ACE::HTBP::Session *
ACE::HTBP::Stream::session (void) const
{
  return this->session_;
}

void
ACE::HTBP::Stream::session (ACE::HTBP::Session *s)
{
  delete this->session_;
  this->session_ = s;
  s->stream (this);
}


ACE_HANDLE
ACE::HTBP::Stream::get_handle (void) const
{
  return ACE_INVALID_HANDLE;
}

void
ACE::HTBP::Stream::set_handle (ACE_HANDLE )
{
}

ACE_END_VERSIONED_NAMESPACE_DECL
