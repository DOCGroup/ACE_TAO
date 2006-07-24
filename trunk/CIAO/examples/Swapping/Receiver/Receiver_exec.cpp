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
  ReceiverSwap_exec_i::incarnate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return new Receiver_exec_i;
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverSwap_exec_i::etherealize (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
  Receiver_exec_i::push_click_in (Hello::TimeOut *
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    //Get the message from the Sender first.
    ACE_DEBUG ((LM_DEBUG,
                "Receiver - Informed by the Sender \n"));

    Hello::ReadMessage_var rev
      = this->context_->get_connection_read_message 
        (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (rev.in ()))
      {
        ACE_THROW (CORBA::BAD_INV_ORDER ());
      }

    CORBA::String_var str =
      rev->get_message (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG,
                "Receiver - Got message from the server [%s] \n",
                str.in () ));
  }

  // Operations from Components::SessionComponen
  void
  Receiver_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::set_session_context\n"));

    this->context_ =
      Receiver_Exec_Context::_narrow (ctx
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Receiver_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Receiver_exec_i::ciao_preactivate\n"));
  }

  void
  Receiver_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate\n"));
  }

  void
  Receiver_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Receiver_exec_i::ciao_postactivate\n"));
  }

  void
  Receiver_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_passivate\n"));
    /*
    CORBA::Object_var comp_object =
      this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Hello::Receiver_var receiver = Hello::Receiver::_narrow (comp_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
                                                      ACE_CHECK;

    ::Components::ConsumerDescriptions_var cons_desc =
      receiver->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

    ::Components::FacetDescriptions_var facet_desc =
      receiver->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

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
  Receiver_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
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
  ReceiverHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
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

