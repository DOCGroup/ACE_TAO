/* -*- C++ -*- */
// $Id$

// FIFO_Recv.i

ASYS_INLINE ssize_t
ACE_FIFO_Recv::recv (void *buf, size_t len)
{
  ACE_TRACE ("ACE_FIFO_Recv::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, len);
}

ASYS_INLINE ssize_t
ACE_FIFO_Recv::recv_n (void *buf, size_t n)
{
  ACE_TRACE ("ACE_FIFO_Recv::recv_n");
  return ACE::recv_n (this->get_handle (), buf, n);
}
