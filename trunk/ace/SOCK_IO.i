/* -*- C++ -*- */
// $Id$

// SOCK_IO.i

// Send an n byte message to the connected socket.

ASYS_INLINE
ACE_SOCK_IO::ACE_SOCK_IO (void)
{
  ACE_TRACE ("ACE_SOCK_IO::ACE_SOCK_IO");
}

ASYS_INLINE
ACE_SOCK_IO::~ACE_SOCK_IO (void)
{
  ACE_TRACE ("ACE_SOCK_IO::~ACE_SOCK_IO");
}

ASYS_INLINE ssize_t  
ACE_SOCK_IO::send (const void *buf, size_t n, int flags) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE_OS::send (this->get_handle (), (const char *) buf, n, flags);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t  
ACE_SOCK_IO::recv (void *buf, size_t n, int flags) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE_OS::recv (this->get_handle (), (char *) buf, n, flags);
}

// Send an n byte message to the connected socket.

ASYS_INLINE ssize_t  
ACE_SOCK_IO::send (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
#if defined (ACE_WIN32)
  return ACE_OS::send (this->get_handle (), (const char *) buf, n, 0);
#else
  return ACE_OS::write (this->get_handle (), (const char *) buf, n);
#endif /* ACE_WIN32 */
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t  
ACE_SOCK_IO::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
#if defined (ACE_WIN32)
  return ACE_OS::recv (this->get_handle (), (char *) buf, n, 0);
#else
  return ACE_OS::read (this->get_handle (), (char *) buf, n);
#endif /* ACE_WIN32 */
}

// Send a vector of n byte messages to the connected socket.

ASYS_INLINE ssize_t  
ACE_SOCK_IO::send (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE_OS::writev (this->get_handle (), (iovec *) iov, n);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t  
ACE_SOCK_IO::recv (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE_OS::readv (this->get_handle (), (iovec *) iov, n);
}

ASYS_INLINE ssize_t
ACE_SOCK_IO::send (const void *buf, size_t n,
		   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE_OS::write (this->get_handle (), 
			(const char *) buf, n,
			overlapped);
}

ASYS_INLINE ssize_t
ACE_SOCK_IO::recv (void *buf, size_t n,
		   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n,
		       overlapped);
}

ASYS_INLINE ssize_t 
ACE_SOCK_IO::send (const void *buf, size_t len, int flags,
		   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE::send (this->get_handle (), buf, len, flags, timeout);
}

ASYS_INLINE ssize_t 
ACE_SOCK_IO::recv (void *buf, size_t len, int flags,
		   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE::recv (this->get_handle (), buf, len, flags, timeout);
}
