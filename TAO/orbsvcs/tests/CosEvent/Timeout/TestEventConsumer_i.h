/* -*- C++ -*- */
// $Id$

#ifndef TestEventConsumer_i_h_
#define TestEventConsumer_i_h_

#include "orbsvcs/CosEventCommS.h"

#include "ace/Task.h"
#include "ace/Condition_T.h"

class TestEventConsumer_i : public virtual POA_CosEventComm::PushConsumer
{
public:
  TestEventConsumer_i (CORBA::ORB_ptr orb, bool hang);

  // Override operations from PushConsumer interface.
  virtual void push (const CORBA::Any & data);

  virtual void disconnect_push_consumer ();

  void activate ();

  struct ORB_task : ACE_Task_Base
  {
    ORB_task (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb))
    {}

    CORBA::ORB_var orb_;

    int svc ();
  };

private:
  CORBA::ORB_var orb_;
  bool hang_;
  unsigned count_;
  ORB_task orbtask_;

  TAO_SYNCH_MUTEX mtx_;
  ACE_Condition<TAO_SYNCH_MUTEX> cond_;
  bool shutdown_;
};

#endif
