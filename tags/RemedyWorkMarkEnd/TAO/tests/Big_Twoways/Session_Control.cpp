//
// $Id$
//
#include "Session_Control.h"

Session_Control::Session_Control (CORBA::ULong session_count)
  : session_count_ (session_count)
  , success_ (1)
{
}

int
Session_Control::all_sessions_finished (void) const
{
  return this->session_count_ == 0;
}

Session_Control::~Session_Control (void)
{
  if (this->session_count_ == 0 && this->success_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Session_Control::~Session_control, "
                  " good, all sessions did finish\n"));
    }
  else if (session_count_ != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Session_Control::~Session_control, "
                  " %d sessions did not finish\n",
                  this->session_count_));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Session_Control::~Session_control, "
                  " some sessions failed\n"));
    }
}

void
Session_Control::session_finished (CORBA::Boolean success)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->session_count_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Session_Control::session_finished, "
                  "unexpected callback\n"));
    }
  if (success == 0)
    this->success_ = 0;

  this->session_count_--;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Session_Control::session_finished, "
              "%d sessions to go\n",
              this->session_count_));
  if (session_count_ == 0)
    {
      PortableServer::POA_var poa =
        this->_default_POA ();
      PortableServer::ObjectId_var oid =
        poa->servant_to_id (this);
      poa->deactivate_object (oid.in ());
    }

}
