// $Id$

#include "Notify_Update_Dispatch_Command.h"
#include "Notify_Collection.h"
#include "Notify_Listeners.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, Notify_Update_Dispatch_Command, "$Id$")

TAO_Notify_Update_Dispatch_Command::TAO_Notify_Update_Dispatch_Command (TAO_Notify_UpdateListener* update_listener, TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed)
  :TAO_Notify_Command (0,0),
   update_listener_ (update_listener),
   added_ (added),
   removed_ (removed)
{
  update_listener_->_incr_refcnt ();
}

TAO_Notify_Update_Dispatch_Command::~TAO_Notify_Update_Dispatch_Command ()
{
  update_listener_->_decr_refcnt ();
}

int
TAO_Notify_Update_Dispatch_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  this->update_listener_->dispatch_update (this->added_, this->removed_
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}
