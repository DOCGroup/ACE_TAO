/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_ACK_WORKER_H
#define ACE_RMCAST_ACK_WORKER_H
#include /**/ "ace/pre.h"

#include "RMCast_Retransmission.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Process an Ack message in the ACE_RMCast_Retransmission module
/**
 * The retransmission module uses internal iterators, thus it needs to
 * create helper Worker classes to process some messages.
 */
class ACE_RMCast_Ack_Worker
  : public ACE_RMCast_Worker<ACE_UINT32,ACE_RMCast::Data>
{
public:
  ACE_RMCast_Ack_Worker (ACE_RMCast::Ack &ack,
                         ACE_RMCast_Retransmission::Messages::Write_Guard &g,
                         ACE_RMCast_Retransmission::Messages *messages);

  virtual int work (ACE_UINT32 const & key,
                    ACE_RMCast::Data const &item);

private:
  ACE_RMCast_Ack_Worker (const ACE_RMCast_Ack_Worker&);
  ACE_RMCast_Ack_Worker& operator= (const ACE_RMCast_Ack_Worker&);

private:
  ACE_RMCast::Ack &ack_;

  ACE_RMCast_Retransmission::Messages::Write_Guard &ace_mon_;

  ACE_RMCast_Retransmission::Messages *messages_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Ack_Worker.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_ACK_WORKER_H */
