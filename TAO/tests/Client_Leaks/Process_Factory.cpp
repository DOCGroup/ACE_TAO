//
// $Id$
//
#include "Process_Factory.h"
#include "ace/Process_Manager.h"

ACE_RCSID(Client_Leaks, Process_Factory, "$Id$")

Process_Factory::Process_Factory (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Process_ptr
Process_Factory::create_new_process (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,Test::Spawn_Failed))
{
  ACE_Process_Options options;

  static const char* argv[] = {
    "child",
    "-o",
    "child.ior",
    0};

  options.command_line (argv);

  (void) ACE_OS::unlink ("child.ior");

  pid_t pid =
    ACE_Process_Manager::instance ()->spawn (options);

  // Good chance to wait for older processes
  ACE_Time_Value interval (0, 10000);
  (void) ACE_Process_Manager::instance ()->wait (interval);

  if (pid == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Process_Factory::create_new_process, "
                  " spawn call failed (%d)\n",
                  errno));
      ACE_THROW_RETURN (Test::Spawn_Failed (), Test::Process::_nil ());
    }
  for (int i = 0; i != 500; ++i)
    {
      if (ACE_OS::access ("child.ior", R_OK) == 0)
        break;
      ACE_OS::sleep (interval);
    }

  ACE_OS::sleep (interval);
  if (ACE_OS::access ("child.ior", R_OK) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Process_Factory::create_new_process, "
                  " can't find child.ior file\n"));
      ACE_THROW_RETURN (Test::Spawn_Failed (), Test::Process::_nil
                        ());
    }

  CORBA::Object_var object =
    this->orb_->string_to_object ("file://child.ior", ACE_TRY_ENV);
  ACE_CHECK_RETURN (Test::Process::_nil ());

  Test::Process_var process =
    Test::Process::_narrow (object.in (), ACE_TRY_ENV);
  if (CORBA::is_nil (process.in ()))
    ACE_THROW_RETURN (Test::Spawn_Failed (), Test::Process::_nil ());

  (void) ACE_OS::unlink ("child.ior");

  return process._retn ();
}

void
Process_Factory::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
  ACE_CHECK;
}
