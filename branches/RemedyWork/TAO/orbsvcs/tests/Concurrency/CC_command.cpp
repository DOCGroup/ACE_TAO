//=============================================================================
/**
 *  @file    CC_command.cpp
 *
 *  $Id$
 *
 *    This is the command classes for the test of the concurrency service.
 *
 *
 *  @author   Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "CC_command.h"
#include "CC_test_utils.h"
#include "CC_naming_service.h"
#include "ace/ACE.h"
#include "ace/Process.h"
#include "ace/Log_Msg.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"

CC_Command::~CC_Command(void)
{
}

int CC_Command::execute()
{
  return 0;
}

CC_Command::CC_Command(void)
{
}

CosConcurrencyControl::LockSet_var
CC_Command::GetLockSet (const char *lock_set_name)
{
  CosConcurrencyControl::LockSet_var ccls_ret;

  try
    {
      if(ACE_OS::strcmp(lock_set_name, "")!=0)
        {
          CORBA::Object_var ccls_obj =
            CC_naming_service::Instance()->get_obj_from_name ("",
                                                              lock_set_name);

          ccls_ret =
            CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());
        }
      else
        {
          // No lock set name was given. Use the global lock set.
          if(cc_lockset_.in()==0)
            {
              throw CORBA::UNKNOWN ();
            }
          else
            ccls_ret = cc_lockset_.in ();
        }
    }
  catch (const CORBA::Exception&)
    {
      throw;
    }

  return ccls_ret;
}

CORBA::Exception *CC_Command::excep_ = 0;

CosConcurrencyControl::LockSet_var CC_Command::cc_lockset_(0);

CC_Start_Cmd::CC_Start_Cmd (const char *config_file_name)
  : cfg_name_ (ACE_OS::strdup (config_file_name))
{
  //  printf("CC_Start_Cmd::CC_Start_Cmd: config: %s\n", config_file_name);
}

CC_Start_Cmd::~CC_Start_Cmd()
{
  // cfg_name_ is allocated in the lexer with strdup
  if (cfg_name_)
    {
      ACE_OS::free(cfg_name_);
      cfg_name_ = 0;
    }
}

int CC_Start_Cmd::execute(void)
{
  if (excep_)
    {
      ACE_OS::printf ("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf ("Executing start command (script file: %s)\n", cfg_name_);

  char cmd_line[1024];
  int success = ACE_OS::sprintf(&cmd_line[0], "%s -c %s",
                                "./CC_client", cfg_name_);
  if(success>=1024 || success==-1)
    ACE_ERROR_RETURN((LM_ERROR, "Creation of process failed: %s\n",
                      cmd_line), 0);

  ACE_Process new_process;
  ACE_Process_Options options;
  options.command_line(ACE_TEXT_CHAR_TO_TCHAR(cmd_line));

  if(new_process.spawn(options) == -1)
    {
      ACE_ERROR_RETURN((LM_ERROR, "Creation of process failed: %C\n",
                        cmd_line), 0);
    }
  return 1; // CC_SUCCESS
}

CC_CreateLockSet_Cmd::CC_CreateLockSet_Cmd (const char *lock_set_name)
  : name_ (ACE_OS::strdup (lock_set_name))
{
  //  printf("CC_CreateLockSet_Cmd::CC_CreateLockSet_Cmd: lock set: %s\n",
  //         lock_set_name);
}

CC_CreateLockSet_Cmd::~CC_CreateLockSet_Cmd()
{
  // name_ is allocated in the lexer with strdup
  if(name_)
    {
      ACE_OS::free(name_);
      name_ = 0;
    }
}

int CC_CreateLockSet_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing create command (lock set: %s)\n", name_);

  try
    {
      CosConcurrencyControl::LockSet_ptr lock_set =
        CC_TestUtils::create_lock_set();
      if(ACE_OS::strcmp(name_,"")!=0)// Do not bind an empty name
        {
          CC_naming_service::Instance()->bind_name(name_,
                                                   lock_set);
        }
      else
        {
          // We did not specify a name => the global lock set variable is set
          cc_lockset_ = lock_set;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_CreateLockSet_Cmd::execute(void)");
    }

  return 1; // CC_SUCCESS
}

CC_Lock_Cmd::CC_Lock_Cmd (const char *lock_set_name,
                          CosConcurrencyControl::lock_mode mode)
  : name_ (ACE_OS::strdup (lock_set_name)),
    mode_(mode)
{
  //  printf("CC_Lock_Cmd::CC_Lock_Cmd: lock set: %s, mode: %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_Lock_Cmd::~CC_Lock_Cmd()
{
  ACE_OS::free (this->name_);
}

int CC_Lock_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing lock command (lock set: %s, mode: %s)\n",
                 name_, CC_TestUtils::get_lock_mode_name(mode_));

  try
    {
      CosConcurrencyControl::LockSet_var ccls =
        GetLockSet(name_);
      ccls->lock (mode_);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Lock_Cmd::execute(void)");
      return 0;
    }
  return 1; // CC_SUCCESS
}

CC_UnLock_Cmd::CC_UnLock_Cmd (const char *lock_set_name,
                              CosConcurrencyControl::lock_mode mode)
  : name_ (ACE_OS::strdup (lock_set_name)),
    mode_ (mode)
{
  //  printf("CC_UnLock_Cmd::CC_UnLock_Cmd: lock set: %s, mode: %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_UnLock_Cmd::~CC_UnLock_Cmd()
{
  ACE_OS::free (this->name_);
}

int CC_UnLock_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing unlock command (lock set: %s, mode: %s)\n",
                 name_, CC_TestUtils::get_lock_mode_name(mode_));

  try
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_);

      ccls->unlock (mode_);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_UnLock_Cmd::execute(void)");
      return 0;
    }
  return 1; // CC_SUCCESS
}

CC_TryLock_Cmd::CC_TryLock_Cmd (const char *lock_set_name,
                                CosConcurrencyControl::lock_mode mode)
  : name_ (ACE_OS::strdup (lock_set_name)),
    mode_ (mode)
{
  //  printf("CC_TryLock_Cmd::CC_TryLock_Cmd: lock set: %s, mode %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_TryLock_Cmd::~CC_TryLock_Cmd()
{
  ACE_OS::free (this->name_);
}

int CC_TryLock_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing try_lock command (lock set: %s, mode: %s)\n",
                 name_, CC_TestUtils::get_lock_mode_name(mode_));

  CORBA::Boolean lock_not_held;

  try
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_);

      lock_not_held = ccls->try_lock (mode_);

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%C lock not held\n",
                      CC_TestUtils::get_lock_mode_name (mode_)));
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%C lock held\n",
                    CC_TestUtils::get_lock_mode_name (mode_)));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_TryLock_Cmd::execute(void)");
      return 0;
    }

  return 1; // CC_SUCCESS
}

CC_ChangeMode_Cmd::
    CC_ChangeMode_Cmd (const char *lock_set_name,
                       CosConcurrencyControl::lock_mode held_mode,
                       CosConcurrencyControl::lock_mode new_mode)
  : name_(ACE_OS::strdup (lock_set_name)),
    held_mode_ (held_mode),
    new_mode_ (new_mode)
{
  //  printf("CC_ChangeMode_Cmd::CC_ChangeMode_Cmd: lock set: %s, held mode: %s, new mode: %s\n",
  //         lock_set_name,
  //         CC_TestUtils::get_lock_mode_name(held_mode),
  //         CC_TestUtils::get_lock_mode_name(new_mode));
}

CC_ChangeMode_Cmd::~CC_ChangeMode_Cmd()
{
  ACE_OS::strdup (this->name_);
}

int CC_ChangeMode_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing change_mode command (lock set: %s, held_mode: %s, new_mode: %s)\n",
                 name_, CC_TestUtils::get_lock_mode_name(held_mode_),
                 CC_TestUtils::get_lock_mode_name(new_mode_));

  try
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_);

      ccls->change_mode (held_mode_, new_mode_);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_ChangeMode_Cmd::execute(void)");
    }
  return 1; // CC_SUCCESS
}

CC_Sleep_Cmd::CC_Sleep_Cmd(int seconds)
  : time_ (seconds)
{
}

CC_Sleep_Cmd::~CC_Sleep_Cmd()
{
}

int CC_Sleep_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing sleep command (time: %i)\n", time_);

  ACE_OS::sleep(time_);
  return 1; // CC_SUCCESS
}

CC_Repeat_Cmd::CC_Repeat_Cmd(int times)
  : times_ (times)
{
}

CC_Repeat_Cmd::~CC_Repeat_Cmd()
{
}

int CC_Repeat_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf("Executing repeat command (times: %i)\n", times_);

  return 1; // CC_SUCCESS
}

CC_Wait_Cmd::CC_Wait_Cmd (const char *prompt)
  : prompt_ (ACE_OS::strdup (prompt))
{
}

CC_Wait_Cmd::~CC_Wait_Cmd()
{
  ACE_OS::free (this->prompt_);
}

int CC_Wait_Cmd::execute(void)
{
  if (excep_)
    {
      ACE_OS::printf ("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf ("Executing wait command\n");

  ACE_OS::printf ("%s", prompt_);
  (void) ACE_OS::fgetc (stdin);

  return 1; // CC_SUCCESS
}

CC_Excep_Cmd::CC_Excep_Cmd (const char *excep)
  : ex_ (ACE_OS::strdup (excep))
{
  //  printf("CC_Excep_Cmd::CC_Excep_Cmd: excep: %s\n", excep);
}

CC_Excep_Cmd::~CC_Excep_Cmd(void)
{
  ACE_OS::free (this->ex_);
}

int
CC_Excep_Cmd::execute(void)
{
  ACE_OS::printf ("Executing excep command (expected: %s)\n", ex_);
  // First we check to see if an exception has occured. If not we fail
  // because we expected to see one
  if(excep_==0)
    return 0; // CC_FAIL

  // If there is an exception check that it's the expected one
  if(ACE_OS::strcmp(excep_->_rep_id (), ex_)==0)
    {
      delete excep_;
      excep_ = 0;
      return 1; // CC_SUCCESS
    }
  else
    {
      ACE_OS::printf ("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }
}

CC_Dummy_Cmd::CC_Dummy_Cmd(void)
{
}

CC_Dummy_Cmd::~CC_Dummy_Cmd(void)
{
}

int
CC_Dummy_Cmd::execute(void)
{
  return 1; // CC_SUCCESS
}

CC_Print_Cmd::CC_Print_Cmd (const char * message)
  : msg_ (ACE_OS::strdup (message))
{
}

CC_Print_Cmd::~CC_Print_Cmd(void)
{
  ACE_OS::free(msg_);
}

int
CC_Print_Cmd::execute(void)
{
  ACE_OS::printf ("%s\n", msg_);
  return 1; // CC_SUCCESS
}

CC_Lookup_Cmd::CC_Lookup_Cmd (const char *lock_set_name)
  : name_ (ACE_OS::strdup (lock_set_name))
{
}

CC_Lookup_Cmd::~CC_Lookup_Cmd()
{
  if(name_)
    {
      ACE_OS::free(name_);
      name_ = 0;
    }
}

int
CC_Lookup_Cmd::execute(void)
{
  if(excep_)
    {
      ACE_OS::printf ("Exception: %s\n", excep_->_rep_id ());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  ACE_OS::printf ("Executing lookup command (lock set: %s)\n", name_);

  // Do the lookup if we haven't done it before
  if(cc_lockset_.in() == 0)
    {
      try
        {
          CORBA::Object_var ccls_obj =
            CC_naming_service::Instance()->get_obj_from_name ("",
                                                              name_);

          CosConcurrencyControl::LockSet_var ccls =
            CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());

          cc_lockset_ = ccls;

        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("CC_UnLock_Cmd::execute(void)");
        }
    }
  return 1; // CC_SUCCESS
}

CC_CommandElem::CC_CommandElem(CC_Command *cmd, CC_CommandElem *next)
  : next_ (next), cmd_ (cmd)
{
}

CC_CommandElem::~CC_CommandElem(void)
{
}

CC_Command *CC_CommandElem::GetCommand(void)
{
  return cmd_;
}

CC_CommandElem *
CC_CommandElem::GetNext(void)
{
  return next_;
}

void
CC_CommandElem::SetNext(CC_CommandElem *next)
{
  next_ = next;
}

CC_CommandList::CC_CommandList(void)
  : head_ (0), last_ (0), times_ (1)
{
  ACE_OS::printf("CC_CommandList::CC_CommandList\n");
}

CC_CommandList::~CC_CommandList(void)
{
}

int
CC_CommandList::add(CC_Command *cmd)
{
  if(head_==0)
    {
      head_ = new CC_CommandElem(cmd, 0);
      last_ = head_;
    }
  else
    {
      CC_CommandElem *tmp = new CC_CommandElem(cmd, 0);
      last_->SetNext(tmp);
      last_ = tmp;
    }
  return 0;
}

int
CC_CommandList::execute(void)
{
  CC_CommandElem *current = head_;

  for(int i=0; i<times_; i++)
    {
      current = head_;
      while(current!=0)
        {
          if(current->GetCommand()->execute()==0) // == CC_FAIL
            {
              return 0; // CC_FAIL
            }
          current = current->GetNext();
        }
    }
  return 1; // CC_SUCCESS
}

void
CC_CommandList::setrepeat(int times)
{
  times_ = times;
}
