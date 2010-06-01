// -*- C++ -*-
//
// $Id$

#include "Shapes_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Shapes_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    if (pulse_callback_.read_data ())
      {
        this->pulse_callback_.read_one();
        this->pulse_callback_.read_all();
      }
    if (pulse_callback_.get_data ())
      {
        this->pulse_callback_.get_one ();
        this->pulse_callback_.get_all ();
      }
    return 0;
  }

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
    const ShapeTypeSeq & /* an_instance */,
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
  // PortStatusListener_exec_i
  //============================================================
  PortStatusListener_exec_i::PortStatusListener_exec_i (void)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener
  void
  PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : rate_ (0),
      get_data_ (true),
      read_data_ (true),
      raw_listen_ (false)

  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::read_one (void)
  {
    ShapeType  shape_info;
    shape_info.color = "GREEN";
    ::CCM_DDS::ReadInfo readinfo;

    try
      {
        this->reader_->read_one_last (shape_info, readinfo, ::DDS::HANDLE_NIL);
        time_t tim = readinfo.source_timestamp.sec;
        tm* time = localtime(&tim);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ONE Read_Info ")
                  ACE_TEXT (" -> date = %02d:%02d:%02d.%d\n"),
                            time->tm_hour,
                            time->tm_min,
                            time->tm_sec,
                            readinfo.source_timestamp.nanosec));
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ON shape info : ")
            ACE_TEXT ("received shape_info for <%C> at %u:%u:%u\n"),
            shape_info.color.in (),
            shape_info.x,
            shape_info.y,
            shape_info.shapesize));
      }
    catch(const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ShapeType_Read_One: ")
                  ACE_TEXT ("no shape_info receieved\n")));
      }
  }

  void
  Receiver_exec_i::read_all (void)
  {
    ShapeTypeSeq shape_infos;
    ::CCM_DDS::ReadInfoSeq readinfoseq;
    this->reader_->read_all(shape_infos, readinfoseq);
    for(CORBA::ULong i = 0; i < readinfoseq.length(); ++i)
      {
        time_t tim = readinfoseq[i].source_timestamp.sec;
        tm* time = ACE_OS::localtime(&tim);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
            ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                            time->tm_hour,
                            time->tm_min,
                            time->tm_sec,
                            readinfoseq[i].source_timestamp.nanosec));
      }
    for(CORBA::ULong i = 0; i < shape_infos.length(); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL Shape Info : ")
              ACE_TEXT ("Number <%d> : received shape_info for <%C> at %u:%u:%u\n"),
            i,
            shape_infos[i].color.in (),
            shape_infos[i].x,
            shape_infos[i].y,
            shape_infos[i].shapesize));
      }
  }

  void
  Receiver_exec_i::get_one (void)
  {
    ShapeType_var shape_info;
    ::CCM_DDS::ReadInfo_var readinfo;

    try
      {
        if (this->getter_->get_one (shape_info.out (), readinfo.out ()))
          {
            time_t tim = readinfo->source_timestamp.sec;
            tm* time = ACE_OS::localtime(&tim);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE ReadInfo -> ")
                                   ACE_TEXT ("date = %02d:%02d:%02d.%d\n"),
                                time->tm_hour,
                                time->tm_min,
                                time->tm_sec,
                                readinfo->source_timestamp.nanosec));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE ShapeType : ")
                                   ACE_TEXT ("received shape_info for <%C> at %u:%u:%u\n"),
                shape_info->color.in (),
                shape_info->x,
                shape_info->y,
                shape_info->shapesize));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE No data available for <%C>\n"),
                    shape_info->color.in ()));
          }
      }
    catch(const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ShapeType_Read_One: no shape_info receieved\n")));
      }
  }

  void
  Receiver_exec_i::get_all (void)
  {
  }

  // Component attributes.
  ::CORBA::ULong
  Receiver_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Receiver_exec_i::rate (::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::Boolean
  Receiver_exec_i::get_data (void)
  {
    return this->get_data_;
  }

  void
  Receiver_exec_i::get_data (::CORBA::Boolean get_data)
  {
    this->get_data_ = get_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::read_data (void)
  {
    return this->read_data_;
  }

  void
  Receiver_exec_i::read_data (::CORBA::Boolean read_data)
  {
    this->read_data_ = read_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::raw_listen (void)
  {
    return this->raw_listen_;
  }

  void
  Receiver_exec_i::raw_listen (::CORBA::Boolean raw_listen)
  {
    this->raw_listen_ = raw_listen;
  }

  // Port operations.
  ::Shapes::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new ShapeType RAW listener\n")));
    return new ShapeType_Listener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Shapes::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    if (this->read_data ())
      {
        this->reader_ = this->context_->get_connection_info_out_data ();
      }
    if (this->get_data ())
      {
        this->getter_ = this->context_->get_connection_info_get_fresh_data ();
      }
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

    lc->mode (this->raw_listen_ ? ::CCM_DDS::ONE_BY_ONE : ::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(0, usec),
                                          ACE_Time_Value(0, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

