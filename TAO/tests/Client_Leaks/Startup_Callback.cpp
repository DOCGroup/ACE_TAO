//
// $Id$
//
#include "Startup_Callback.h"

ACE_RCSID(Client_Leaks, Startup_Callback, "$Id$")

Startup_Callback::Startup_Callback (void)
{
}

int
Startup_Callback::process_has_started (Test::Process_out the_process)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  if (!CORBA::is_nil (this->process_.in ()))
    {
      the_process = Test::Process::_duplicate (this->process_.in ());
      return 1;
    }
  return 0;
}

void
Startup_Callback::started (Test::Process_ptr process
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->process_ = Test::Process::_duplicate (process);
}
