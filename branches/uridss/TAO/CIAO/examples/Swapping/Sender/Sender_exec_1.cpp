//$Id$

#include "Sender_exec_1.h"
#include "Sender_exec.h"
#include "ciao/Servant_Activator.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl_1::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender 1 sending out message. \n"));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_1_i::~Sender_exec_1_i ()
  {
  }

  void
  Sender_exec_1_i::local_message (const char * local_message
                                  ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_1_i::local_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup(message_.in ());
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_1_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::get_push_message called\n"));
    return ( new Message_Impl_1 (*this) );
  }

  void
  Sender_exec_1_i::start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    ACE_DEBUG ((LM_DEBUG, "Sender 1 initiates the process.\n"));
    this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Sender_exec_1_i::set_session_context (Components::SessionContext_ptr ctx
                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_1_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx
                                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Sender_exec_1_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_1_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::ccm_activate\n"));

    CORBA::Object_var o =
      this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Sender_exec_1_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_1_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_1_i::ccm_passivate\n"));

    Components::ConsumerDescriptions_var retval =
      this->context_->get_registered_consumers ("click_out"
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->base_exec_->consumers (retval._retn ());

    CORBA::Object_var o =
      this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    Hello::Sender_var sender =
      Hello::Sender::_narrow (o.in ()
                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Components::FacetDescriptions_var facets =
        sender->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

    const CORBA::ULong facet_len = facets->length ();
      CORBA::ULong i = 0;

      for (i = 0; i < facet_len; ++i)
      {
        this->context_->remove_facet (facets[i]->facet_ref ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        /*
        this->context_->update_port_activator (oid ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        this->context_->deactivate_facet (oid ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        */
      }

    /*
    this->context_->deactivate_facet ("Hello_Sender_push_message");
    this->context_->update_port_activator ("Hello_Sender_push_message");
    */
  }

  void
  Sender_exec_1_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_1_i::ccm_remove\n"));
  }

  extern "C" SENDER_EXEC_1_Export ::Components::EnterpriseComponent_ptr
  createSenderExec_Impl (SenderSwap_exec_i *p)
  {
    Sender_exec_1_i *tmp = new Sender_exec_1_i ();

    tmp->swap_exec (p);

    return tmp;
  }
}

