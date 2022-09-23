// -*- C++ -*-

//=============================================================================
/**
 *  @file   ObjectGroupManagerHandler.h
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
  void start (CORBA::Boolean ami_return_val,
                      const FTRT::Location & the_location) override;

  void start_excep (::Messaging::ExceptionHolder * excep_holder) override;

  void create_group () override;

  void create_group_excep (::Messaging::ExceptionHolder * excep_holder) override;

  void add_member () override;

  void add_member_excep (::Messaging::ExceptionHolder * excep_holder) override;

  void set_state () override;

  void set_state_excep (::Messaging::ExceptionHolder * excep_holder) override;
private:
  ACE_Auto_Event& evt_;
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, int> num_backups_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
