/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    CC_command.cpp
//
// = DESCRIPTION
//      This is the command classes for the test of the concurrency service.
//
// = AUTHORS
//	Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "CC_command.h"
#include "CC_test_utils.h"
#include "CC_naming_service.h"
#include "ace/ACE.h"
#include "ace/Process.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include <stdio.h>

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
CC_Command::GetLockSet(char *lock_set_name, CORBA::Environment &_env)
{
  TAO_TRY
    {
      if(ACE_OS::strcmp(lock_set_name, "")!=0)
        {
          CORBA::Object_var ccls_obj =
            CC_naming_service::Instance()->get_obj_from_name ("", lock_set_name,
                                                              TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CosConcurrencyControl::LockSet_var ccls =
            CosConcurrencyControl::LockSet::_narrow (ccls_obj.in (),
                                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;

          return ccls;
        }
      else
        {
          // No lock set name was given. Use the global lock set.
          if(cc_lockset_.in()==0)
            {
              TAO_TRY_ENV.clear();
              TAO_TRY_ENV.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
            }
          else
            return cc_lockset_;
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN(0);
    }
  TAO_ENDTRY;
  // We should never get here
  return 0;
}

CORBA::Exception *CC_Command::excep_ = 0;

CosConcurrencyControl::LockSet_var CC_Command::cc_lockset_(0);

CC_Start_Cmd::CC_Start_Cmd(char *config_file_name)
  : cfg_name_ (config_file_name)
{
  //  printf("CC_Start_Cmd::CC_Start_Cmd: config: %s\n", config_file_name);
}

CC_Start_Cmd::~CC_Start_Cmd()
{
  // cfg_name_ is allocated in the lexer with strdup
  if(cfg_name_)
    {
      ACE_OS::free(cfg_name_);
      cfg_name_ = 0;
    }
}

int CC_Start_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing start command (script file: %s)\n", cfg_name_);

  char cmd_line[1024];
  int success = ACE_OS::sprintf(&cmd_line[0], "%s -c %s",
                                "./CC_client", cfg_name_);
  if(success>=1024 || success==-1)
    ACE_ERROR_RETURN((LM_ERROR, "Creation of process failed: %s\n",
                      cmd_line), 0);

  ACE_Process new_process;
  ACE_Process_Options options;
  options.command_line(cmd_line);

  if(new_process.spawn(options) == -1)
    {
      ACE_ERROR_RETURN((LM_ERROR, "Creation of process failed: %s\n",
                        cmd_line), 0);
    }
  return 1; // CC_SUCCESS
}

CC_CreateLockSet_Cmd::CC_CreateLockSet_Cmd(char *lock_set_name)
  : name_ (lock_set_name)
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
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing create command (lock set: %s)\n", name_);

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_ptr lock_set =
        CC_TestUtils::create_lock_set();
      if(ACE_OS::strcmp(name_,"")!=0)// Do not bind an empty name
        {
          CC_naming_service::Instance()->bind_name(name_,
                                                   lock_set, TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      else
        {
          // We did not specify a name => the global lock set variable is set
          cc_lockset_ = lock_set;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception("CC_CreateLockSet_Cmd::execute(void)");
      excep_ = TAO_TRY_ENV.exception();
    }
  TAO_ENDTRY;

  return 1; // CC_SUCCESS
}

CC_Lock_Cmd::CC_Lock_Cmd(char *lock_set_name,
                         CosConcurrencyControl::lock_mode mode)
  : name_(lock_set_name), mode_(mode)
{
  //  printf("CC_Lock_Cmd::CC_Lock_Cmd: lock set: %s, mode: %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_Lock_Cmd::~CC_Lock_Cmd()
{
}

int CC_Lock_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing lock command (lock set: %s, mode: %s)\n",
         name_, CC_TestUtils::get_lock_mode_name(mode_));

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ccls->lock (mode_,
                  TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Lock_Cmd::execute(void)");
      excep_ = TAO_TRY_ENV.exception();
      //      return 0; // should be CC_FAIL
    }
  TAO_ENDTRY;
  return 1; // CC_SUCCESS
}

CC_UnLock_Cmd::CC_UnLock_Cmd(char *lock_set_name,
                             CosConcurrencyControl::lock_mode mode)
  : name_(lock_set_name), mode_ (mode)
{
  //  printf("CC_UnLock_Cmd::CC_UnLock_Cmd: lock set: %s, mode: %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_UnLock_Cmd::~CC_UnLock_Cmd()
{
}

int CC_UnLock_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing unlock command (lock set: %s, mode: %s)\n",
         name_, CC_TestUtils::get_lock_mode_name(mode_));

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ccls->unlock (mode_,
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_UnLock_Cmd::execute(void)");
      excep_ = TAO_TRY_ENV.exception();
      //      return 0; // should be CC_FAIL
    }
  TAO_ENDTRY;
  return 1; // CC_SUCCESS
}

CC_TryLock_Cmd::CC_TryLock_Cmd(char *lock_set_name,
                               CosConcurrencyControl::lock_mode mode)
  : name_ (lock_set_name), mode_ (mode)
{
  //  printf("CC_TryLock_Cmd::CC_TryLock_Cmd: lock set: %s, mode %s\n",
  //         lock_set_name, CC_TestUtils::get_lock_mode_name(mode));
}

CC_TryLock_Cmd::~CC_TryLock_Cmd()
{
}

int CC_TryLock_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing try_lock command (lock set: %s, mode: %s)\n",
         name_, CC_TestUtils::get_lock_mode_name(mode_));

  CORBA::Boolean lock_not_held;

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      lock_not_held = ccls->try_lock (mode_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock not held\n",
                      CC_TestUtils::get_lock_mode_name (mode_)));
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock held\n",
                    CC_TestUtils::get_lock_mode_name (mode_)));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_TryLock_Cmd::execute(void)");
      excep_ = TAO_TRY_ENV.exception();
      //      return 0; // Should be CC_FAIL
    }
  TAO_ENDTRY;

  return 1; // CC_SUCCESS
}

CC_ChangeMode_Cmd::CC_ChangeMode_Cmd(char *lock_set_name,
                                     CosConcurrencyControl::lock_mode held_mode,
                                     CosConcurrencyControl::lock_mode new_mode)
  : name_(lock_set_name), held_mode_ (held_mode), new_mode_ (new_mode)
{
  //  printf("CC_ChangeMode_Cmd::CC_ChangeMode_Cmd: lock set: %s, held mode: %s, new mode: %s\n",
  //         lock_set_name,
  //         CC_TestUtils::get_lock_mode_name(held_mode),
  //         CC_TestUtils::get_lock_mode_name(new_mode));
}

CC_ChangeMode_Cmd::~CC_ChangeMode_Cmd()
{
}

int CC_ChangeMode_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing change_mode command (lock set: %s, held_mode: %s, new_mode: %s)\n",
         name_, CC_TestUtils::get_lock_mode_name(held_mode_),
         CC_TestUtils::get_lock_mode_name(new_mode_));

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_var ccls = GetLockSet(name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ccls->change_mode (held_mode_, new_mode_,
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_ChangeMode_Cmd::execute(void)");
      excep_ = TAO_TRY_ENV.exception();
      //      return 0; // CC_FAIL
    }
  TAO_ENDTRY;
  return 1; // CC_SUCCESS
}

CC_Sleep_Cmd::CC_Sleep_Cmd(int seconds)
  : time_ (seconds)
{
  //  printf("CC_Sleep_Cmd::CC_Sleep_Cmd: time: %i\n", seconds);
}

CC_Sleep_Cmd::~CC_Sleep_Cmd()
{
}

int CC_Sleep_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing sleep command (time: %i)\n", time_);

  ACE_OS::sleep(time_);
  return 1; // CC_SUCCESS
}

CC_Repeat_Cmd::CC_Repeat_Cmd(int times)
  : times_ (times)
{
  //  printf("CC_Repeat_Cmd::CC_Repeat_Cmd: times: %i\n", times);
}

CC_Repeat_Cmd::~CC_Repeat_Cmd()
{
}

int CC_Repeat_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing repeat command (times: %i)\n", times_);

  return 1; // CC_SUCCESS
}

CC_Wait_Cmd::CC_Wait_Cmd(char *prompt)
  : prompt_ (prompt)
{
  //  printf("CC_Wait_Cmd::CC_Wait_Cmd: prompt: %s\n", prompt);
}

CC_Wait_Cmd::~CC_Wait_Cmd()
{
}

int CC_Wait_Cmd::execute(void)
{
  if(excep_)
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing wait command\n");

  char s[1];
  printf("%s", prompt_);
  ACE_OS::gets(&s[0]);
  return 1; // CC_SUCCESS
}

CC_Excep_Cmd::CC_Excep_Cmd(char *excep)
  : ex_(excep)
{
  //  printf("CC_Excep_Cmd::CC_Excep_Cmd: excep: %s\n", excep);
}

CC_Excep_Cmd::~CC_Excep_Cmd(void)
{
  if(ex_)
    delete ex_;
}

int
CC_Excep_Cmd::execute(void)
{
  printf("Executing excep command (expected: %s)\n", ex_);
  // First we check to see if an exception has occured. If not we fail
  // because we expected to see one
  if(excep_==0)
    return 0; // CC_FAIL

  // If there is an exception check that it's the expected one
  if(ACE_OS::strcmp(excep_->_id(), ex_)==0)
    {
      delete excep_;
      excep_ = 0;
      return 1; // CC_SUCCESS
    }
  else
    {
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }
  return 0; // CC_FAIL - we shouldn't end here
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

CC_Print_Cmd::CC_Print_Cmd(char * message)
  : msg_ (message)
{
}

CC_Print_Cmd::~CC_Print_Cmd(void)
{
  if(msg_)
    {
      ACE_OS::free(msg_);
      msg_ = 0;
    }
}

int
CC_Print_Cmd::execute(void)
{
  printf("%s\n", msg_);
  return 1; // CC_SUCCESS
}

CC_Lookup_Cmd::CC_Lookup_Cmd(char *lock_set_name)
  : name_ (lock_set_name)
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
      printf("Exception: %s\n", excep_->_id());
      delete excep_;
      excep_ = 0;
      return 0; // CC_FAIL
    }

  printf("Executing lookup command (lock set: %s)\n", name_);

  // Do the lookup if we haven't done it before
  if(cc_lockset_.in() == 0)
    {
      TAO_TRY
        {
          CORBA::Object_var ccls_obj =
            CC_naming_service::Instance()->get_obj_from_name ("", name_,
                                                              TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CosConcurrencyControl::LockSet_var ccls =
            CosConcurrencyControl::LockSet::_narrow (ccls_obj.in (),
                                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;

          cc_lockset_ = ccls;
          TAO_CHECK_ENV;

        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("CC_UnLock_Cmd::execute(void)");
          excep_ = TAO_TRY_ENV.exception();
          //      return 0; // CC_FAIL
        }
      TAO_ENDTRY;
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
  printf("CC_CommandList::CC_CommandList\n");
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
