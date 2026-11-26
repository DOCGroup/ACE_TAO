#include "ace/OS_NS_stropts.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ssize_t
ACE_FIFO_Send_Msg::send (void const *buf, size_t len)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");
  ACE_Str_Buf send_msg (const_cast<void *> (buf),
                        static_cast<int> (len));

  return this->send (send_msg);
}

#if defined (ACE_HAS_STREAM_PIPES)
ACE_INLINE ssize_t
ACE_FIFO_Send_Msg::send (ACE_Str_Buf const *data,
                         ACE_Str_Buf const *cntl,
                         int flags)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");
  if (ACE_OS::putmsg (this->get_handle (),
                      cntl,
                      data,
                      flags) == -1)
    {
      return-1;
    }
  else
    {
      return (cntl == 0 ? 0 : cntl->len) + (data == 0 ? 0 : data->len);
    }
}

ACE_INLINE ssize_t
ACE_FIFO_Send_Msg::send (int band,
                         ACE_Str_Buf const *data,
                         ACE_Str_Buf const *cntl,
                         int flags)
{
  ACE_TRACE ("ACE_FIFO_Send_Msg::send");

  if (ACE_OS::putpmsg (this->get_handle (),
                       cntl,
                       data,
                       band,
                       flags) == -1)
    {
      return -1;
    }
  else
    {
      return (cntl == 0 ? 0 : cntl->len) + (data == 0 ? 0 : data->len);
    }
}
#endif /* ACE_HAS_STREAM_PIPES */

ACE_END_VERSIONED_NAMESPACE_DECL
