// -*- C++ -*-

//=============================================================================
/**
 *  @file   ObjectGroupManagerHandler.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef OBJECTGROUPMANAGERHANDLER_H
#define OBJECTGROUPMANAGERHANDLER_H

#include "orbsvcs/FTRT_GroupManagerS.h"
#include "ace/Auto_Event.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ObjectGroupManagerHandler
  : public POA_FTRT::AMI_ObjectGroupManagerHandler
{
public:
  ObjectGroupManagerHandler(ACE_Auto_Event& evt, int num_backups);
  virtual void start (CORBA::Boolean ami_return_val,
                      const FTRT::Location & the_location);

  virtual void start_excep (::Messaging::ExceptionHolder * excep_holder);

  virtual void create_group (void);

  virtual void create_group_excep (::Messaging::ExceptionHolder * excep_holder);

  virtual void add_member (void);

  virtual void add_member_excep (::Messaging::ExceptionHolder * excep_holder);

  virtual void set_state (void);

  virtual void set_state_excep (::Messaging::ExceptionHolder * excep_holder);
private:
  ACE_Auto_Event& evt_;
  ACE_Atomic_Op< TAO_SYNCH_MUTEX, int > num_backups_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
