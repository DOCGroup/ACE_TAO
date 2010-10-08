// -*- C++ -*-
// $Id$

#include "Shapes_Sender_comp_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"

namespace CIAO_Shapes_Sender_comp_Impl
{
  //============================================================
  // Controller_exec_i
  //============================================================
  Controller_exec_i::Controller_exec_i (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  Controller_exec_i::~Controller_exec_i (void)
  {
  }

  ::Shapes::ReturnStatus
  Controller_exec_i::setSize (::CORBA::UShort size)
  {
    return this->callback_.setSize (size);
  }

  ::Shapes::ReturnStatus
  Controller_exec_i::setLocation (::CORBA::UShort x,
                                  ::CORBA::UShort y)
  {
    return this->callback_.setLocation (x, y);
  }


  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : instance_handle_ (::DDS::HANDLE_NIL)
  {
    this->square_.x = 10;
    this->square_.y = 10;
    this->square_.shapesize = 1;
    this->square_.color = CORBA::string_dup("GREEN");
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  ::Shapes::CCM_Control_obj_ptr
  Sender_exec_i::get_control (void)
  {
    return new Controller_exec_i (*this);
  }

  ::Shapes::ReturnStatus
  Sender_exec_i::setSize (::CORBA::UShort size)
  {
    this->square_.shapesize = size;
    return this->update_square ();
  }

  ::Shapes::ReturnStatus
  Sender_exec_i::setLocation (::CORBA::UShort x,
                              ::CORBA::UShort y)
  {
    this->square_.x = x;
    this->square_.y = y;
    return this->update_square ();
  }

  ::Shapes::ReturnStatus
  Sender_exec_i::update_square ()
  {
    ::Shapes::ShapeType_conn::Writer_var writer =
      this->context_->get_connection_info_write_data ();
    if (::CORBA::is_nil (writer.in ()))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::update_square - ")
                              ACE_TEXT ("Unable to write: write_data is nil.\n")));
        return ::Shapes::RETURN_ERROR;
      }

    try
      {
        writer->write_one (this->square_, this->instance_handle_);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("UPDATED Shape_info for <%C> %u:%u:%u\n"),
                  this->square_.color.in (),
                  this->square_.x,
                  this->square_.y,
                  this->square_.shapesize));
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Shape_info for <%C> not updated: <%C> didn't exist.\n"),
                    this->square_.color.in (), this->square_.color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Internal Error while updating Shape_info for <%C>.\n"),
                    this->square_.color.in ()));
      }
    return ::Shapes::RETURN_OK;
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Shapes::CCM_Sender_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ::Shapes::ShapeType_conn::Writer_var writer =
      this->context_->get_connection_info_write_data ();
    if (::CORBA::is_nil (writer.in ()))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::update_square - ")
                              ACE_TEXT ("Unable to write: write_data is nil.\n")));
        throw ::CORBA::INTERNAL ();
      }

    try
      {
        this->instance_handle_ = writer->register_instance (this->square_);
        //Register shape with dds.
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("REGISTER Shape_info for <%C> %u:%u:%u\n"),
                      this->square_.color.in (),
                      this->square_.x,
                      this->square_.y,
                      this->square_.shapesize));
      }
    catch (const CCM_DDS::AlreadyCreated& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> already created.\n"),
                    this->square_.color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating Shape_info for <%C>.\n"),
                      this->square_.color.in ()));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    ::Shapes::ShapeType_conn::Writer_var writer =
      this->context_->get_connection_info_write_data ();
    if (::CORBA::is_nil (writer.in ()))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::update_square - ")
                              ACE_TEXT ("Unable to write: write_data is nil.\n")));
        throw ::CORBA::INTERNAL ();
      }

    try
      {
        writer->unregister_instance (this->square_,
                                     this->instance_handle_);
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Shape_info for <%C> not deleted: <%C> didn't exist.\n"),
                    this->square_.color.in (),
                    this->square_.color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while deleting Shape_info for <%C>.\n"),
                    this->square_.color.in ()));
      }
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SHAPES_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Sender_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

