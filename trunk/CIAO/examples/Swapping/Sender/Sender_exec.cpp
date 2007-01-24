//$Id$

#include "Sender_exec.h"

const char* first_exe_dll_name = "Sender_DnC_exec_1";
const char* first_exe_entrypt = "createSenderExec_Impl";
const char* second_exe_dll_name = "Sender_DnC_exec_2";
const char* second_exe_entrypt = "createSenderExec_Impl";

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl::get_message (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender sending out message. \n"));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_i::~Sender_exec_i ()
  {
  }

  void
  Sender_exec_i::local_message (const char * local_message
                                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_i::local_message (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup(message_. in());
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message (void)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec.i::get_push_message called\n "));
    return ( new Message_Impl (*this) );
  }

  void
  Sender_exec_i::start (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
    this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
  }

  void
  Sender_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx
                                        ACE_ENV_ARG_PARAMETER);

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Sender_exec_i::ciao_preactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_i::ccm_activate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ccm_activate\n"));
  }

  void
  Sender_exec_i::ciao_postactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_i::ccm_passivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_passivate\n"));
  }

  void
  Sender_exec_i::ccm_remove (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_remove\n"));
  }


  SenderHome_exec_i::SenderHome_exec_i ()
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i ()
  {
  }

  SenderSwap_exec_i::SenderSwap_exec_i ()
    : count_ (0)
  {
  }

  SenderSwap_exec_i::~SenderSwap_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  SenderSwap_exec_i::incarnate (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
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

    void *void_ptr = 0;
    ptrdiff_t tmp = 0;

    if (0 == this->count_)
      {
        // Cast the void* to non-pointer type first - it's not legal to
        // cast a pointer-to-object directly to a pointer-to-function.
        void_ptr = first_dll.symbol (first_exe_entrypt);
        tmp = reinterpret_cast<ptrdiff_t> (void_ptr);
        first_exec_creator = reinterpret_cast<ExecFactory> (tmp);

        Components::EnterpriseComponent_var first_executor =
          first_exec_creator (this);
        count_++;
        return first_executor._retn ();
      }
    else if (-1 == count_)
      {
        // Cast the void* to non-pointer type first - it's not legal to
        // cast a pointer-to-object directly to a pointer-to-function.
        void_ptr = second_dll.symbol (second_exe_entrypt);
        tmp = reinterpret_cast<ptrdiff_t> (void_ptr);
        second_exec_creator = reinterpret_cast<ExecFactory> (tmp);

        Components::EnterpriseComponent_var second_executor =
          second_exec_creator (this);
        --count_;
        return second_executor._retn ();
      }

    return 0;
  }

  ::Components::EnterpriseComponent_ptr
  SenderSwap_exec_i::etherealize (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return 0;
  }

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "%P|%t) creating SenderHome \n"));
    return new SenderSwap_exec_i;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void)
  {
    return new SenderHome_exec_i ();
  }
}

