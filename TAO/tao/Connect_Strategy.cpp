// -*- C++ -*-
#include "Connect_Strategy.h"
#include "Transport.h"
#include "Connection_Handler.h"
#include "LF_Multi_Event.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Connect_Strategy::TAO_Connect_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}


TAO_Connect_Strategy::~TAO_Connect_Strategy ()
{
}

int
TAO_Connect_Strategy::wait (TAO_Connection_Handler *ch,
                            ACE_Time_Value *max_wait_time)
{
  if (ch == nullptr)
    return -1;

  return this->wait_i (ch, ch->transport (), max_wait_time);
}

int
TAO_Connect_Strategy::wait (TAO_Transport *t, ACE_Time_Value *max_wait_time)
{
  // Basically the connection was EINPROGRESS, but before we could
  // wait for it some other thread detected a failure and cleaned up
  // the connection handler.
  if (t == nullptr)
    return -1;

  return this->wait_i (t->connection_handler (), t, max_wait_time);
}

int
TAO_Connect_Strategy::wait (TAO_LF_Multi_Event *mev,
                            ACE_Time_Value *max_wait_time)
{
  return this->wait_i (mev, mev->base_transport (), max_wait_time);
}

int
TAO_Connect_Strategy::poll (TAO_LF_Multi_Event *mev)
{
  ACE_Time_Value zero (ACE_Time_Value::zero);
  return this->wait_i (mev, mev->base_transport (), &zero);
}

TAO_END_VERSIONED_NAMESPACE_DECL
