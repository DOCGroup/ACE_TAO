//$Id$

#include "Sender_exec_1.h"
#include "Sender_exec.h"
#include "ciao/Servant_Activator.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl_1::get_message ()
  {
    ACE_DEBUG ((LM_DEBUG, "Sender 1 sending out message. \n"));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_1_i::~Sender_exec_1_i ()
  {
  }

  void
  Sender_exec_1_i::local_message (const char * local_message)
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_1_i::local_message ()
  {
    return CORBA::string_dup(message_.in ());
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_1_i::get_push_message ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::get_push_message called\n"));
    return ( new Message_Impl_1 (*this) );
  }

  void
  Sender_exec_1_i::start ()
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    ACE_DEBUG ((LM_DEBUG, "Sender 1 initiates the process.\n"));
    this->context_->push_click_out (event);
  }

  void
  Sender_exec_1_i::set_session_context (Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_1_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
      {
        throw CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_1_i::configuration_complete ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::configuration_complete\n"));
  }

  void
  Sender_exec_1_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_1_i::ccm_activate\n"));

    CORBA::Object_var o =
      this->context_->get_CCM_object ();
  }

  void
  Sender_exec_1_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_1_i::ccm_passivate\n"));

    Components::ConsumerDescriptions_var retval =
      this->context_->get_registered_consumers ("click_out");

    this->base_exec_->consumers (retval._retn ());

    CORBA::Object_var o =
      this->context_->get_CCM_object ();

    Hello::Sender_var sender =
      Hello::Sender::_narrow (o.in ());

    Components::FacetDescriptions_var facets =
        sender->get_all_facets ();

    const CORBA::ULong facet_len = facets->length ();
      CORBA::ULong i = 0;

      for (i = 0; i < facet_len; ++i)
      {
        this->context_->remove_facet (facets[i]->facet_ref ());
        /*
        this->context_->update_port_activator (oid);

        this->context_->deactivate_facet (oid);
        */
      }

    /*
    this->context_->deactivate_facet ("Hello_Sender_push_message");
    this->context_->update_port_activator ("Hello_Sender_push_message");
    */
  }

  void
  Sender_exec_1_i::ccm_remove ()
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

