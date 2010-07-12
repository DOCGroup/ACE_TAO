// -*- C++ -*-
//
// $Id$

#include "Shapes_Receiver_comp_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Shapes_Receiver_comp_Impl
{
  //============================================================
  // info_out_data_listener_exec_i
  //============================================================
  info_out_data_listener_exec_i::info_out_data_listener_exec_i (void)
  {
  }

  info_out_data_listener_exec_i::~info_out_data_listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ShapeType_Listener
  void
  info_out_data_listener_exec_i::on_many_data (
    const ::Shapes::ShapeType_msgSeq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  info_out_data_listener_exec_i::on_one_data (
    const ShapeType_msg & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ShapeType_Listener: ")
            ACE_TEXT ("received shape_info for <%C> at %u:%u:%u\n"),
            an_instance.color.in (),
            an_instance.x,
            an_instance.y,
            an_instance.shapesize));
  }

  //============================================================
  // Receiver_comp_exec_i
  //============================================================
  Receiver_comp_exec_i::Receiver_comp_exec_i (void)
  {
  }

  Receiver_comp_exec_i::~Receiver_comp_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.
  ::Shapes::ShapeType_conn::CCM_Listener_ptr
  Receiver_comp_exec_i::get_info_out_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_info_out_data_listener_.in ()))
      {
        info_out_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_out_data_listener_exec_i (),
          ::Shapes::ShapeType_conn::CCM_Listener::_nil ());

        this->ciao_info_out_data_listener_ = tmp;
      }

    return
      ::Shapes::ShapeType_conn::CCM_Listener::_duplicate (
        this->ciao_info_out_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_comp_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Shapes::CCM_Receiver_comp_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_comp_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_comp_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw ::CORBA::INTERNAL ();
      }

    lc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_comp_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_comp_exec_i::ccm_remove (void)
  {
  }

  extern "C" SHAPES_RECEIVER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Receiver_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_comp_exec_i);

    return retval;
  }
}

