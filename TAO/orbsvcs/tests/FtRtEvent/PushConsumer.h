// -*- C++ -*-
//=============================================================================
/**
 *  @file   PushConsumer.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef PUSHCONSUMERIMPL_H
#define PUSHCONSUMERIMPL_H

#include "orbsvcs/RtecEventCommS.h"

class PushConsumer_impl :
public virtual POA_RtecEventComm::PushConsumer
{
public:
  explicit PushConsumer_impl (CORBA::ORB_ptr orb);

  PushConsumer_impl (PushConsumer_impl const &) = delete;
  PushConsumer_impl (PushConsumer_impl &&) = delete;
  PushConsumer_impl &operator= (PushConsumer_impl const &) = delete;
  PushConsumer_impl &operator= (PushConsumer_impl &&) = delete;

  void push (RtecEventComm::EventSet const &data) override;
  void disconnect_push_consumer () override;
private:
  CORBA::ORB_var orb_ {};
};
#endif
