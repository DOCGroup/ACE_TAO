// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerTask.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _FLARE_SERVER_TASK_H_
#define _FLARE_SERVER_TASK_H_

#include "ace/Task.h"
#include "StateSynchronizationAgentC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/PortableServer/POAC.h"

struct ServerOptions
{
  ServerOptions () 
    : bands_file ("empty-file"),
      lanes_file ("empty-file"),
      rm_ior_file ("file://rm.ior"),
      pool_priority (ACE_INT16_MIN),      
      static_threads (1),
      number_of_lanes (0),
      stop (0),
      number_of_servants (1) {}

  const char *bands_file;
  const char *lanes_file;
  const char *rm_ior_file;
  RTCORBA::Priority pool_priority;
  CORBA::ULong static_threads;
  CORBA::ULong number_of_lanes;
  int stop;
  int number_of_servants;
};

class ServerTask : public ACE_Task_Base
{
public:

  ServerTask (ServerOptions & options,
	      CORBA::ORB_ptr orb,
	      StateSynchronizationAgent_ptr agent);

  int svc (void);

private:
  void read_object_info (std::string file_name,
			 int count);

  int write_ior_to_file (const char *ior_file,
			 CORBA::ORB_ptr orb,
			 CORBA::Object_ptr object);

  PortableServer::POA_ptr create_rt_poa ();

  ServerOptions & options_;

  CORBA::ORB_var orb_;

  StateSynchronizationAgent_var agent_;
};

#endif /* _FLARE_SERVER_TASK_H_ */
