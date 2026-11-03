/**
 * @file Echo_Caller.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Echo_Caller.h"
#include "Server_Timer.h"
#include "tao/ORB_Core.h"

Echo_Caller::Echo_Caller (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Echo_Caller::start_task (Test::Echo_ptr client)
{
  // The reactor holds references after activation; release the creator reference.
  Server_Timer * const task =
    new Server_Timer (client, this->orb_->orb_core ()->reactor ());
  task->activate ();
  task->remove_reference ();

  Server_Timer * const second_task =
    new Server_Timer (client, this->orb_->orb_core ()->reactor ());
  second_task->activate ();
  second_task->remove_reference ();
}

void
Echo_Caller::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG, "Server received shutdown message\n"));
  this->orb_->shutdown (false);
}
