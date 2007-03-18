// $Id$
#include "orbsvcs/FtRtEvent/EventChannel/ObjectGroupManagerHandler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ObjectGroupManagerHandler::ObjectGroupManagerHandler(
  ACE_Auto_Event& evt, int num_backups)
: evt_(evt), num_backups_(num_backups)
{
}

void
ObjectGroupManagerHandler::start (CORBA::Boolean ami_return_val,
                                  const FTRT::Location & the_location)
{
  ACE_UNUSED_ARG(ami_return_val);
  ACE_UNUSED_ARG(the_location);
}

void
ObjectGroupManagerHandler::start_excep (::Messaging::ExceptionHolder *)
{
}

void ObjectGroupManagerHandler::create_group (void)
{
}


void
ObjectGroupManagerHandler::create_group_excep (::Messaging::ExceptionHolder *)
{
}

void
ObjectGroupManagerHandler::add_member (void)
{
  if (--num_backups_ ==0)
    evt_.signal();
}

void
ObjectGroupManagerHandler::add_member_excep (::Messaging::ExceptionHolder *)
{
  this->add_member();
}

void
ObjectGroupManagerHandler::set_state (void)
{
}

void
ObjectGroupManagerHandler::set_state_excep (::Messaging::ExceptionHolder *)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
