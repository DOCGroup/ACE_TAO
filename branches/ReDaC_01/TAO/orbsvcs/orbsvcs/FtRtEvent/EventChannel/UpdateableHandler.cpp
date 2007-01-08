// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/UpdateableHandler.h"
#include "orbsvcs/FtRtEvent/EventChannel/Update_Manager.h"
#include "orbsvcs/FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h"
#include "../Utils/resolve_init.h"

ACE_RCSID (EventChannel,
           UpdateableHandler,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

UpdateableHandler::UpdateableHandler(AMI_Primary_Replication_Strategy* strategy)
  : strategy_(strategy)
{
}

UpdateableHandler::~UpdateableHandler()
{
}

FTRT::AMI_UpdateableHandler_ptr UpdateableHandler::activate(
  Update_Manager* mgr, int id,
  PortableServer::ObjectId& object_id
  ACE_ENV_ARG_DECL)
{
  object_id.length(sizeof(mgr) + sizeof(id));
  memcpy(object_id.get_buffer(), &mgr, sizeof(mgr));
  memcpy(object_id.get_buffer() + sizeof(mgr), &id, sizeof(id));
  strategy_->poa()->activate_object_with_id(object_id,
                                            this
                                            ACE_ENV_ARG_PARAMETER);
  CORBA::Object_var object = strategy_->poa()->id_to_reference(
    object_id
    ACE_ENV_ARG_PARAMETER);

  return FTRT::AMI_UpdateableHandler::_narrow(object.in() ACE_ENV_ARG_PARAMETER);
}



void UpdateableHandler::dispatch(UpdateableHandler::Handler handler ACE_ENV_ARG_DECL)
{
  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(strategy_->orb(),
    "POACurrent"
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var object_id =
    current->get_object_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Update_Manager* mgr;
  int id;

  memcpy(&mgr, object_id->get_buffer(), sizeof(mgr));
  memcpy(&id, object_id->get_buffer()+sizeof(mgr), sizeof(id));

  ACE_DEBUG((LM_DEBUG, "%d\n", id));

  (mgr->*handler)(id);

  strategy_->poa()->deactivate_object(object_id.in());
}

void UpdateableHandler::set_update (
                                    ACE_ENV_SINGLE_ARG_DECL
                                    )
                                    ACE_THROW_SPEC ((
                                    CORBA::SystemException
                                    ))
{
  ACE_DEBUG((LM_DEBUG,"Received reply from "));
  dispatch(&Update_Manager::handle_reply ACE_ENV_ARG_PARAMETER);
}
void UpdateableHandler::set_update_excep (
  ::Messaging::ExceptionHolder * excep_holder
  ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
  CORBA::SystemException
  ))
{
  ACE_DEBUG((LM_DEBUG, "Received Exception from"));
  ACE_TRY {
    excep_holder->raise_exception();
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A corba exception\n");
  }
  ACE_ENDTRY;

  dispatch(&Update_Manager::handle_exception ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
