// $Id$
#include "ObjectGroupManagerHandler.h"

ObjectGroupManagerHandler::ObjectGroupManagerHandler(
  ACE_Auto_Event& evt, int num_backups
  ACE_ENV_ARG_DECL)
: evt_(evt), num_backups_(num_backups)
{
}

void 
ObjectGroupManagerHandler::start (CORBA::Boolean ,
                                  const FTRT::Location & 
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
ObjectGroupManagerHandler::start_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * 
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void ObjectGroupManagerHandler::create_group (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


void 
ObjectGroupManagerHandler::create_group_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * 
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
ObjectGroupManagerHandler::add_member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (--num_backups_ ==0)
    evt_.signal();
}

void 
ObjectGroupManagerHandler::add_member_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * 
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->add_member();
}

void 
ObjectGroupManagerHandler::set_state (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
ObjectGroupManagerHandler::set_state_excep (FTRT::AMI_ObjectGroupManagerExceptionHolder * 
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
