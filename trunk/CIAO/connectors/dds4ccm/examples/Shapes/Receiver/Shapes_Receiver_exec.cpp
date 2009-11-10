// -*- C++ -*-
//
// $Id$

#include "Shapes_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Shapes_Receiver_Impl
{
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &,
                                   const void *)
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
  // Facet Executor Implementation Class: ShapeType_RawListener_exec_i
  //============================================================

  ShapeType_RawListener_exec_i::ShapeType_RawListener_exec_i (void)
  {
  }

  ShapeType_RawListener_exec_i::~ShapeType_RawListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ShapeType_RawListener

  void
  ShapeType_RawListener_exec_i::on_data (
    const ShapeType & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("ShapeType_RawListener: ")
            ACE_TEXT ("received shape_info for <%s> at %u:%u:%u\n"),
            an_instance.color.in (),
            an_instance.x,
            an_instance.y,
            an_instance.shapesize));
  }
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
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
    /* Your code here. */
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {

  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iShapeType_RawListener_exec_i ();
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
        this->reader_->read_one (shape_info, readinfo );
        time_t tim = readinfo.timestamp.sec;
        tm* time = localtime(&tim);
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ONE Read_Info ")
                  ACE_TEXT (" -> date = %02d:%02d:%02d.%d\n"),
                            time->tm_hour,
                            time->tm_min,
                            time->tm_sec,
                            readinfo.timestamp.nanosec));
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ON shape info : ")
            ACE_TEXT ("received shape_info for <%s> at %u:%u:%u\n"),
            shape_info.color.in (),
            shape_info.x,
            shape_info.y,
            shape_info.shapesize));
      }
    catch(CCM_DDS::NonExistent& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ShapeType_Read_One: ")
                  ACE_TEXT ("no shape_info receieved\n")));
      }
  }

  void
  Receiver_exec_i::read_all (void)
  {
    ShapeType_Seq_var shape_infos;
    ::CCM_DDS::ReadInfoSeq_var readinfoseq;
    this->reader_->read_all(shape_infos.out(), readinfoseq.out());
    for(unsigned int i = 0; i < readinfoseq->length(); ++i)
      {
        time_t tim = readinfoseq[i].timestamp.sec;
        tm* time = localtime(&tim);
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
            ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                            time->tm_hour,
                            time->tm_min,
                            time->tm_sec,
                            readinfoseq[i].timestamp.nanosec));
      }
    for(CORBA::ULong i = 0; i < shape_infos->length(); ++i)
      {
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL Shape Info : ")
              ACE_TEXT ("Number <%d> : received shape_info for <%s> at %u:%u:%u\n"),
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
    ShapeType  shape_info;
    shape_info.color = "yellow";
    ::CCM_DDS::ReadInfo readinfo;

    try
      {
        if (this->getter_->get_one (shape_info, readinfo ))
          {
            time_t tim = readinfo.timestamp.sec;
            tm* time = localtime(&tim);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE ReadInfo -> ")
                                   ACE_TEXT ("date = %02d:%02d:%02d.%d\n"),
                                time->tm_hour,
                                time->tm_min,
                                time->tm_sec,
                                readinfo.timestamp.nanosec));
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE ShapeType : ")
                                   ACE_TEXT ("received shape_info for <%s> at %u:%u:%u\n"),
                shape_info.color.in (),
                shape_info.x,
                shape_info.y,
                shape_info.shapesize));
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE No data available for <%C>\n"), 
                    shape_info.color.in ()));
          }
      }
    catch(CCM_DDS::NonExistent& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ShapeType_Read_One: no shape_info receieved\n")));
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
  Receiver_exec_i::rate (
    ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }
  ::CORBA::Boolean
  Receiver_exec_i::get_data (void)
  {
    return this->get_data_;
  }
  
  void
  Receiver_exec_i::get_data (
    ::CORBA::Boolean get_data)
  {
    this->get_data_ = get_data;
  }
  
  ::CORBA::Boolean
  Receiver_exec_i::read_data (void)
  {
    return this->read_data_;
  }
  
  void
  Receiver_exec_i::read_data (
    ::CORBA::Boolean read_data)
  {
    this->read_data_ = read_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::raw_listen (void)
  {
    return this->raw_listen_;
  }

  void
  Receiver_exec_i::raw_listen (
    ::CORBA::Boolean raw_listen)
  {
    this->raw_listen_ = raw_listen;
  }

  // Port operations.
  ::CCM_DDS::CCM_ShapeType_RawListener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new ShapeType RAW listener\n")));
    return new ShapeType_RawListener_exec_i ();
  }
  
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
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
        this->reader_ = this->context_->get_connection_info_out_data();
      }
    if (this->get_data ())
      {
        this->getter_ = this->context_->get_connection_info_out_get_data();
      }
  }
  
  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::ListenerControl_var lc = 
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->enabled (this->raw_listen_);

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(0, usec),
                                          ACE_Time_Value(0, usec)) == -1)
      {
        CIAO_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
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

