// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ssize_t
ACE_SOCK_Dgram_Mcast_QoS::send (iovec const buffers[],
                                int buffer_count,
                                size_t &number_of_bytes_sent,
                                int flags,
                                ACE_Addr const &addr,
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

ACE_INLINE ssize_t
ACE_SOCK_Dgram_Mcast_QoS::send (void const *buf,
                                size_t n,
                                ACE_Addr const &addr,
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

ACE_INLINE ACE_QoS_Manager
ACE_SOCK_Dgram_Mcast_QoS::qos_manager ()
{
  return this->qos_manager_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
