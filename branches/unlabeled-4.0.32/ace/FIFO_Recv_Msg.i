/* -*- C++ -*- */
// $Id$

// FIFO_Recv_Msg.i

// Note that the return values mean different things if
// ACE_HAS_STREAM_PIPES vs. if it doesn't...  See the manual page on
// getmsg(2) and read(2) for more details.

inline ssize_t
ACE_FIFO_Recv_Msg::recv (ACE_Str_Buf &recv_msg)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::recv");
#if defined (ACE_HAS_STREAM_PIPES)
  int i = 0;
  return ACE_OS::getmsg (this->get_handle (), (strbuf *) 0, (strbuf *) &recv_msg, &i);
#else /* Do the ol' 2-read trick... */
  if (ACE_OS::read (this->get_handle (), 
		    (char *) &recv_msg.len, 
		    sizeof recv_msg.len) != sizeof recv_msg.len)
    return -1;
  else
    return ACE_OS::read (this->get_handle (), 
			 (char *) recv_msg.buf, 
			 (int) recv_msg.len);	
#endif /* ACE_HAS_STREAM_PIPES */
}

inline ssize_t
ACE_FIFO_Recv_Msg::recv (void *buf, size_t max_len)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::recv");
  ACE_Str_Buf recv_msg ((char *) buf, 0, max_len);

  return this->recv (recv_msg);
}

#if defined (ACE_HAS_STREAM_PIPES)
inline ssize_t
ACE_FIFO_Recv_Msg::recv (ACE_Str_Buf *data, ACE_Str_Buf *cntl, int *flags)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::recv");
  return ACE_OS::getmsg (this->get_handle (), 
			 (strbuf *) cntl, (strbuf *) data, flags);
}

inline ssize_t
ACE_FIFO_Recv_Msg::recv (int *band, ACE_Str_Buf *data, ACE_Str_Buf *cntl, int *flags)
{
  ACE_TRACE ("ACE_FIFO_Recv_Msg::recv");
  return ACE_OS::getpmsg (this->get_handle (), 
			  (strbuf *) cntl, (strbuf *) data, band, flags);
}

#endif /* ACE_HAS_STREAM_PIPES */
