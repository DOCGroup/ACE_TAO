//$Id$

#include "Sender_exec.h"

const char* first_exe_dll_name = "Sender_DnC_exec_1";
const char* first_exe_entrypt = "createSenderExec_Impl";
const char* second_exe_dll_name = "Sender_DnC_exec_2";
const char* second_exe_entrypt = "createSenderExec_Impl";

Sender_Impl::SenderHome_exec_i::SenderHome_exec_i ()
{
}

Sender_Impl::SenderHome_exec_i::~SenderHome_exec_i ()
{
}

Sender_Impl::SenderSwap_exec_i::SenderSwap_exec_i ()
  : count_ (0)
{
}

Sender_Impl::SenderSwap_exec_i::~SenderSwap_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
Sender_Impl::SenderSwap_exec_i::incarnate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // return new Sender_Impl::Sender_exec_i;

  ExecFactory first_exec_creator = 0;
  ExecFactory second_exec_creator = 0;
  ACE_DLL first_dll, second_dll;

  if (first_dll.open (first_exe_dll_name,
                      ACE_DEFAULT_SHLIB_MODE, 0) != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Could not open the DLL %s\n", first_exe_dll_name));
      return 0;
    }

  if (second_dll.open (second_exe_dll_name,
                       ACE_DEFAULT_SHLIB_MODE, 0) != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Could not open the DLL %s\n",
                 second_exe_dll_name));
      return 0;
    }

  if (first_exe_entrypt == 0 || second_exe_entrypt == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "NIL entry points\n"));
      return 0;
    }

  if (this->count_ == 0)
    {
      first_exec_creator = (ExecFactory) first_dll.symbol (first_exe_entrypt);
      Components::EnterpriseComponent_var first_executor =
        first_exec_creator (this);
      count_++;
      return first_executor._retn ();
    }
  else if (count_ == 1)
    {
      second_exec_creator = (ExecFactory)
        second_dll.symbol (second_exe_entrypt);
      Components::EnterpriseComponent_var second_executor =
        second_exec_creator (this);

      --count;
      return second_executor._retn ();
    }
}

::Components::EnterpriseComponent_ptr
Sender_Impl::SenderSwap_exec_i::etherealize (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // return new Sender_Impl::Sender_exec_i;
}

::Components::EnterpriseComponent_ptr
Sender_Impl::SenderHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "%P|%t) creating SenderHome \n"));
  return new Sender_Impl::SenderSwap_exec_i;
  // return new Sender_Impl::Sender_exec_i;
}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void)
{
  return new Sender_Impl::SenderHome_exec_i ();
}
