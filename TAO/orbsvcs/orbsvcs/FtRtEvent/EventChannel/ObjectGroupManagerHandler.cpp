// $Id$
#include "ObjectGroupManagerHandler.h"

ObjectGroupManagerHandler::ObjectGroupManagerHandler(
  ACE_Auto_Event& evt, int num_backups)
: evt_(evt), num_backups_(num_backups)
{
}

void 
ObjectGroupManagerHandler::start (CORBA::Boolean ami_return_val,
                                  const FTRT::Location & the_location
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(ami_return_val);
  ACE_UNUSED_ARG(the_location);
}

void 
ObjectGroupManagerHandler::start_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder
                                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(excep_holder);
}

void ObjectGroupManagerHandler::create_group (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


void 
ObjectGroupManagerHandler::create_group_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder
                                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(excep_holder);
}

void 
ObjectGroupManagerHandler::add_member (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (--num_backups_ ==0)
    evt_.signal();
}

void 
ObjectGroupManagerHandler::add_member_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(excep_holder);
  this->add_member(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void 
ObjectGroupManagerHandler::set_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
ObjectGroupManagerHandler::set_state_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * excep_holder
                                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG(excep_holder);
}
