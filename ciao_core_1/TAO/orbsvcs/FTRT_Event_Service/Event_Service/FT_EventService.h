// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_EventService.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FT_EVENTSERVICE_H
#define FT_EVENTSERVICE_H

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.h"
#include "TP_Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace POA_RtecScheduler
{
    class Scheduler;
}
TAO_END_VERSIONED_NAMESPACE_DECL

class FT_EventService : private TAO_FTEC_Become_Primary_Listener
{
public:
  FT_EventService();
  ~FT_EventService();

  int run(int argc, ACE_TCHAR* argv[]);

private:
  int parse_args (int argc, ACE_TCHAR* argv []);
  void setup_scheduler(CosNaming::NamingContext_ptr naming_context);
  int report_factory(CORBA::ORB_ptr orb,
                   FtRtecEventChannelAdmin::EventChannel_ptr ec);
  virtual void become_primary();

  int global_scheduler_;
  // Should we use a global scheduler or a local one?
  POA_RtecScheduler::Scheduler *sched_impl_;
  // The Scheduler implementation.
  TAO_FTEC_Event_Channel::MEMBERSHIP membership_;
  int num_threads_;
  CORBA::ORB_var orb_;
  TP_Task task_;
};

#endif
