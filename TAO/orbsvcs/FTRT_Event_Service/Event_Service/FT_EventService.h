// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_EventService.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================#ifndef FT_EVENTSERVICE_H
#define FT_EVENTSERVICE_H

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"

namespace POA_RtecScheduler
{ 
    class Scheduler;
};

class FT_EventService 
{
public:
  FT_EventService();
  ~FT_EventService();

  int run(int argc, ACE_TCHAR* argv[]);

  int parse_args (int argc, ACE_TCHAR* argv []);
  void setup_scheduler(CosNaming::NamingContext_ptr naming_context
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  int report_factory(CORBA::ORB_ptr orb,
                   FtRtecEventChannelAdmin::EventChannel_ptr ec);
private:
  int global_scheduler_;
	// Should we use a global scheduler or a local one? 
	POA_RtecScheduler::Scheduler *sched_impl_;
	// The Scheduler implementation.
  TAO_FTEC_Event_Channel::MEMBERSHIP membership_;
  int num_threads_;
};

#endif
