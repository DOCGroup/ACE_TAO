// -*- C++ -*-

//=============================================================================
/**
 *  @file   ObjectGroupManagerHandler.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================#ifndef OBJECTGROUPMANAGERHANDLER_H
#define OBJECTGROUPMANAGERHANDLER_H

#include "orbsvcs/FTRT_GroupManagerS.h"

class ObjectGroupManagerHandler : public POA_FTRT::AMI_ObjectGroupManagerHandler
{
public:
  ObjectGroupManagerHandler(ACE_Auto_Event& evt, int num_backups);
  virtual void start (CORBA::Boolean ami_return_val,
                      const FTRT::Location & the_location)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void start_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void create_group ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void create_group_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_member ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_member_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_state ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_state_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder)
    ACE_THROW_SPEC ((CORBA::SystemException));
private:
  ACE_Auto_Event& evt_;
  ACE_Atomic_Op< ACE_Thread_Mutex, int > num_backups_;
};

#endif
