// -*- C++ -*-
//=============================================================================
/**
 *  @file   PushConsumer.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef PUSHCONSUMERIMPL_H
#define PUSHCONSUMERIMPL_H
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"

class PushConsumer_impl :
public virtual POA_RtecEventComm::PushConsumer
{
public:
  PushConsumer_impl(CORBA::ORB_ptr orb);

    virtual void push (
        const RtecEventComm::EventSet & data
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    virtual void disconnect_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_;
private:
  CORBA::ORB_var orb_;
  PushConsumer_impl(const PushConsumer_impl&);
  void operator==(const PushConsumer_impl&);
};
#endif
