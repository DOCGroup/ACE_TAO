/* -*- C++ -*- */
// $Id$

// FIFO_Send_Msg.i

inline ssize_t
ACE_FIFO_Send_Msg::send (const void *buf, size_t len)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");
  ACE_Str_Buf send_msg ((char *) buf, len);

  return this->send (send_msg);
}

#if defined (ACE_HAS_STREAM_PIPES)
inline ssize_t
ACE_FIFO_Send_Msg::send (const ACE_Str_Buf *data, 
			 const ACE_Str_Buf *cntl, 
			 int flags)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");
  return ACE_OS::putmsg (this->get_handle (), 
			 (strbuf *) cntl, 
			 (strbuf *) data, 
			 flags);
}

inline ssize_t
ACE_FIFO_Send_Msg::send (int band, 
			 const ACE_Str_Buf *data, 
			 const ACE_Str_Buf *cntl, 
			 int flags)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");
  return ACE_OS::putpmsg (this->get_handle (), 
			  (strbuf *) cntl, 
			  (strbuf *) data, 
			  band, 
			  flags);
}
#endif /* ACE_HAS_STREAM_PIPES */
