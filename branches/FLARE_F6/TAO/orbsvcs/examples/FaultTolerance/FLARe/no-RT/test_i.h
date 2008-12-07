// -*- C++ -*-

//=============================================================================
/**
 *  @file    test_i.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _FLARE_TEST_H_
#define _FLARE_TEST_H_

#include "ace/Sample_History.h"
#include "ace/High_Res_Timer.h"

#include "testS.h"

#include "orbsvcs/orbsvcs/LWFT/StateSynchronizationAgentC.h"

#ifdef FLARE_USES_DDS
#  include "ccpp_State.h"
#endif

class test_i :  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
	  const std::string& object_id,
	  StateSynchronizationAgent_ptr agent,
	  int & stop);

  virtual void set_state (const CORBA::Any & state_value);

  virtual CORBA::Any *get_state ();

  virtual void method (CORBA::ULong start, CORBA::ULong end, CORBA::ULong work,
               CORBA::ULong prime_number, CORBA::ULong kill);

  virtual void shutdown (void);

  PortableServer::POA_ptr _default_POA (void);

  virtual void dump (void);

  virtual StateSynchronizationAgent_ptr agent (void);
  
  virtual void agent (StateSynchronizationAgent_ptr agent);
  
  virtual char * object_id (void);
  
  virtual void object_id (const char * object_id);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ACE_Sample_History history_;
  ACE_hrtime_t start_;
  ACE_hrtime_t end_;
  std::string object_id_;
  StateSynchronizationAgent_var agent_;
  State state_;
  int & stop_;
};

#endif /* FLARE_TEST_H_ */
