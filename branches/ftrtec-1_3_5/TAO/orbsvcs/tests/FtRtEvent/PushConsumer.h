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
#include <vector>

struct Options;

class PushConsumer_impl :
  public virtual POA_RtecEventComm::PushConsumer
{
public:
  PushConsumer_impl();

  int init(CORBA::ORB_ptr orb,
    RtecEventChannelAdmin::EventChannel_ptr,
    const Options& options ACE_ENV_ARG_DECL);

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

private:
  void output_result();
  CORBA::ORB_var orb_;
  int num_iterations_;
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_;
  std::vector<int> run_times_;

  PushConsumer_impl(const PushConsumer_impl&);
  void operator==(const PushConsumer_impl&);
};
#endif
