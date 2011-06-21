// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/UpdateableHandler.h"
#include "orbsvcs/FtRtEvent/EventChannel/Update_Manager.h"
#include "orbsvcs/FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h"
#include "../Utils/resolve_init.h"

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
  PortableServer::ObjectId& object_id)
{
  object_id.length(sizeof(mgr) + sizeof(id));
  ACE_OS::memcpy(object_id.get_buffer(), &mgr, sizeof(mgr));
  ACE_OS::memcpy(object_id.get_buffer() + sizeof(mgr), &id, sizeof(id));
  strategy_->poa()->activate_object_with_id(object_id,
                                            this);
  CORBA::Object_var object = strategy_->poa()->id_to_reference(
    object_id);

  return FTRT::AMI_UpdateableHandler::_narrow(object.in());
}



void UpdateableHandler::dispatch(UpdateableHandler::Handler handler)
{
  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(strategy_->orb(),
    "POACurrent");

  PortableServer::ObjectId_var object_id =
    current->get_object_id();

  Update_Manager* mgr;
  int id;

  ACE_OS::memcpy(&mgr, object_id->get_buffer(), sizeof(mgr));
  ACE_OS::memcpy(&id, object_id->get_buffer()+sizeof(mgr), sizeof(id));

  ACE_DEBUG((LM_DEBUG, "%d\n", id));

  (mgr->*handler)(id);

  strategy_->poa()->deactivate_object(object_id.in());
}

void UpdateableHandler::set_update (
                                    )
{
  ACE_DEBUG((LM_DEBUG,"Received reply from "));
  dispatch(&Update_Manager::handle_reply);
}
void UpdateableHandler::set_update_excep (
  ::Messaging::ExceptionHolder * excep_holder
  )
{
  ACE_DEBUG((LM_DEBUG, "Received Exception from"));
  try{
    excep_holder->raise_exception();
  }
  catch (const CORBA::Exception& ex){
    ex._tao_print_exception ("A corba exception\n");
  }

  dispatch(&Update_Manager::handle_exception);
}

TAO_END_VERSIONED_NAMESPACE_DECL
