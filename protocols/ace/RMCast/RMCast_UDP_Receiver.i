// $Id$

ACE_INLINE
ACE_RMCast_UDP_Receiver::
    ACE_RMCast_UDP_Receiver (ACE_RMCast_Sender_Proxy_Factory *factory)
  :  factory_ (factory)
  ,  eh_ (this)
{
}
