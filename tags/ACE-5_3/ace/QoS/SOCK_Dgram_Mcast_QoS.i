/* -*- C++ -*- */
// $Id$

ASYS_INLINE
ACE_SOCK_Dgram_Mcast_QoS::~ACE_SOCK_Dgram_Mcast_QoS (void)
{
}

ASYS_INLINE ssize_t
ACE_SOCK_Dgram_Mcast_QoS::send (const iovec buffers[],
                                int buffer_count,
                                size_t &number_of_bytes_sent,
                                int flags,
                                const ACE_Addr &addr,
                                ACE_OVERLAPPED *overlapped,
                                ACE_OVERLAPPED_COMPLETION_FUNC func) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_QoS::send");

  return ACE_SOCK_Dgram::send (buffers,
                               buffer_count,
                               number_of_bytes_sent,
                               flags,
                               addr,
                               overlapped,
                               func);

}

ASYS_INLINE ssize_t
ACE_SOCK_Dgram_Mcast_QoS::send (const void *buf,
                                size_t n,
                                const ACE_Addr &addr,
                                int flags,
                                ACE_OVERLAPPED *overlapped,
                                ACE_OVERLAPPED_COMPLETION_FUNC func) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_QoS::send");

  return ACE_SOCK_Dgram::send (buf,
                               n,
                               addr,
                               flags,
                               overlapped,
                               func);
}

ASYS_INLINE ACE_QoS_Manager
ACE_SOCK_Dgram_Mcast_QoS::qos_manager (void)
{
  return this->qos_manager_;
}
