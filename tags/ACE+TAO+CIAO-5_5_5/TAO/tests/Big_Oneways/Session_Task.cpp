//
// $Id$
//

#include "Session_Task.h"
#include "Session.h"

ACE_RCSID(Big_Oneways, Session_Task, "$Id$")

Session_Task::Session_Task (Session *session)
  : session_ (session)
{
}

int
Session_Task::svc (void)
{
  return this->session_->svc ();
}
