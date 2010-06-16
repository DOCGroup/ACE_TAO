// -*- C++ -*-
//
// $Id$

#include "ShapesReceiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Shapes_Receiver_comp_Impl
{
  //============================================================
  // ShapeType_Listener_exec_i
  //============================================================
  ShapeType_Listener_exec_i::ShapeType_Listener_exec_i (void)
  {
  }

  ShapeType_Listener_exec_i::~ShapeType_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ShapeType_Listener
  void
  ShapeType_Listener_exec_i::on_many_data (
    const ::Shapes::ShapeTypeSeq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  ShapeType_Listener_exec_i::on_one_data (
    const ShapeType & an_instance ,
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
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.
  ::Shapes::ShapesConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new ShapeType_Listener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
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
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }

    lc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" SHAPES_RECEIVER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Receiver_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

