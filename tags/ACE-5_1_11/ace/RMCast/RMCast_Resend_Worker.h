/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_RESEND_WORKER_H
#define ACE_RMCAST_RESEND_WORKER_H
#include "ace/pre.h"

#include "RMCast.h"
#include "RMCast_Copy_On_Write.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Module;

/// Process an Ack message in the ACE_RMCast_Retransmission module
/**
 * The retransmission module uses internal iterators, thus it needs to
 * create helper Worker classes to process some messages.
 */
class ACE_RMCast_Resend_Worker
  : public ACE_RMCast_Worker<ACE_UINT32,ACE_RMCast::Data>
{
public:
  ACE_RMCast_Resend_Worker (ACE_RMCast_Module *next,
                            ACE_UINT32 max_sequence_number);

  virtual int work (ACE_UINT32 const & key,
                    ACE_RMCast::Data const &item);

  int n;

private:
  ACE_RMCast_Module *next_;

  ACE_UINT32 max_sequence_number_;

private:
  ACE_RMCast_Resend_Worker (const ACE_RMCast_Resend_Worker&);
  ACE_RMCast_Resend_Worker& operator= (const ACE_RMCast_Resend_Worker&);
};

#if defined (__ACE_INLINE__)
#include "RMCast_Resend_Worker.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_RESEND_WORKER_H */
