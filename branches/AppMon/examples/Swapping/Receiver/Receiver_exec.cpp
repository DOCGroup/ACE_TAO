//$Id$
/*
 * @file Receiver_exec.cpp
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */

#include "Receiver_exec.h"
#include "CIAO_common.h"

namespace CIDL_Receiver_Impl
{
  ReceiverSwap_exec_i::ReceiverSwap_exec_i ()
  {
  }

  ReceiverSwap_exec_i::~ReceiverSwap_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverSwap_exec_i::incarnate ()
  {
    return new Receiver_exec_i;
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverSwap_exec_i::etherealize ()
  {
    return new Receiver_exec_i;
  }

  Receiver_exec_i::Receiver_exec_i ()
  {
  }

  Receiver_exec_i::~Receiver_exec_i ()
  {
  }

  void
  Receiver_exec_i::push_click_in (Hello::TimeOut *)
  {
    //Get the message from the Sender first.
    ACE_DEBUG ((LM_DEBUG,
                "Receiver - Informed by the Sender \n"));

    Hello::ReadMessage_var rev
      = this->context_->get_connection_read_message
        ();

    if (CORBA::is_nil (rev.in ()))
      {
        throw CORBA::BAD_INV_ORDER ();
      }

    CORBA::String_var str =
      rev->get_message ();

    ACE_DEBUG ((LM_DEBUG,
                "Receiver - Got message from the server [%s] \n",
                str.in () ));
  }

  // Operations from Components::SessionComponen
  void
  Receiver_exec_i::set_session_context (Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::set_session_context\n"));

    this->context_ =
      Receiver_Exec_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
      {
        throw CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::ciao_preactivate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Receiver_exec_i::ciao_preactivate\n"));
  }

  void
  Receiver_exec_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate\n"));
  }

  void
  Receiver_exec_i::ciao_postactivate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Receiver_exec_i::ciao_postactivate\n"));
  }

  void
  Receiver_exec_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_passivate\n"));
    /*
    CORBA::Object_var comp_object =
      this->context_->get_CCM_object ();

    Hello::Receiver_var receiver = Hello::Receiver::_narrow (comp_object.in ());

    ::Components::ConsumerDescriptions_var cons_desc =
      receiver->get_all_consumers ();

    ::Components::FacetDescriptions_var facet_desc =
      receiver->get_all_facets ();

    CORBA::ULong cons_len = cons_desc->length ();
    CORBA::ULong facet_len = facet_desc->length ();
    for (CORBA::ULong i = 0; i < cons_len; ++i)
      {
        // ACE_DEBUG ((LM_DEBUG, "consumer name is %s\n", cons_desc[i]->name ()));
      }
    for (CORBA::ULong i = 0; i < facet_len; ++i)
      {
        // ACE_DEBUG ((LM_DEBUG, "facet name is %s\n", facet_desc[i]->name ()));
      }
    */
  }

  void
  Receiver_exec_i::ccm_remove ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_remove\n"));
  }


  ReceiverHome_exec_i::ReceiverHome_exec_i ()
  {
  }

  ReceiverHome_exec_i::~ReceiverHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverHome_exec_i::create ()
  {
    Components::EnterpriseComponent_ptr tmp;

    ACE_NEW_THROW_EX (tmp,
                      ReceiverSwap_exec_i,
                      CORBA::NO_MEMORY ());

    /*
    ACE_NEW_THROW_EX (tmp,
                      Receiver_exec_i,
                      CORBA::NO_MEMORY ());
    */

    return tmp;
  }

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createReceiverHome_Impl (void)
  {
    return new ReceiverHome_exec_i ();
  }
}

