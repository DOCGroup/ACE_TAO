// $Id$

ACE_INLINE
ACE_RMCast_Ack_Worker::
ACE_RMCast_Ack_Worker (ACE_RMCast::Ack &ack,
                       ACE_RMCast_Retransmission::Messages::Write_Guard &g,
                       ACE_RMCast_Retransmission::Messages *messages)
  : ack_ (ack)
  , ace_mon_ (g)
  , messages_ (messages)
{
}
