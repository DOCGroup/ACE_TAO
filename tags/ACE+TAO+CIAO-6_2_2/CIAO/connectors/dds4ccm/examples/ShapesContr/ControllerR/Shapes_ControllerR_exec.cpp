// -*- C++ -*-
// $Id$

/**
 * @author Marijke Hengstmengel (mhengstmengel@remedy.nl)
 */


#include "Shapes_ControllerR_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_ShapesControllerR_Impl
{
  ShapeAggrSeq *
  shapes_aggregates_i::get_aggr_shapes()
  {
    ShapeAggrSeq_var info_seq = component_.get_shapes_list();
    return info_seq._retn ();
  }

  /**
    * Write action generator
    */
  pulse_Generator::pulse_Generator (ControllerR_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->pulse_callback_.get_received_shapes ();
    return 0;
    }

  /**
   * Component Executor Implementation Class: ControllerR_exec_i
   */

  ControllerR_exec_i::ControllerR_exec_i (void)
      : rate_ (10)
  {
    shape_aggr_seq_.length(0);
    ACE_NEW_THROW_EX (this->ticker_,
                       pulse_Generator (*this),
                       ::CORBA::NO_MEMORY ());

  }

  ControllerR_exec_i::~ControllerR_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.
  ACE_Reactor*
  ControllerR_exec_i::reactor (void)
   {
     ACE_Reactor* reactor = 0;
     ::CORBA::Object_var ccm_object =
       this->ciao_context_->get_CCM_object();
     if (! ::CORBA::is_nil (ccm_object.in ()))
       {
         ::CORBA::ORB_var orb = ccm_object->_get_orb ();
         if (! ::CORBA::is_nil (orb.in ()))
           {
             reactor = orb->orb_core ()->reactor ();
           }
       }
     if (reactor == 0)
       {
         throw ::CORBA::INTERNAL ();
       }
     return reactor;
   }


  // Component attributes and port operations.
  ::CORBA::UShort
  ControllerR_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  ControllerR_exec_i::rate (
    const ::CORBA::UShort rate)
  {
    this->rate_ = rate;
  }

  void
  ControllerR_exec_i::get_received_shapes (void)
  {
    shape_received_var invoker =
      this->ciao_context_->get_connection_shapeReceivedSub ();

    if (!::CORBA::is_nil(invoker))
    {
      ShapeInfoSeq_var info_seq = invoker->get_info_shapes();
      if (!::CORBA::is_nil(info_seq))
      {
        for (unsigned int i = 0; i < info_seq->length(); i ++)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ControllerR_exec_i::get_received_shapes: ")
                  ACE_TEXT ("received shapes_info <%C> for <%C> at X <%u> Y <%u> size <%u>\n"),
                  info_seq[i].shape.in(),
                  info_seq[i].color.in(),
                  info_seq[i].posY,
                  info_seq[i].posX,
                  info_seq[i].size));

        }
        //aggregate received samples, sort on color
        aggregate_samples(info_seq.in());
      }
    }
  }

  void
  ControllerR_exec_i::aggregate_samples(const ShapeInfoSeq &info_seq)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                       this->mutex_, CORBA::INTERNAL ());

    //handle all records
    for (unsigned int i = 0; i < info_seq.length(); i++)
    {
      CORBA::Boolean color_found = false;
      //color exists?
      for (unsigned int y = 0; y < shape_aggr_seq_.length(); y++)
      {
        if (ACE_OS::strcmp (info_seq[i].color,this->shape_aggr_seq_[y].color )== 0)
        {
          color_found = true;
          this->shape_aggr_seq_[y].number_of_shapes++;
          this->shape_aggr_seq_[y].last_size = info_seq[i].size;
          this->shape_aggr_seq_[y].last_posX = info_seq[i].posX;
          this->shape_aggr_seq_[y].last_posY = info_seq[i].posY;
        }
      }
      if (!color_found)
      {
        CORBA::ULong seq_length = this->shape_aggr_seq_.length();
        this->shape_aggr_seq_.length(seq_length + 1);
        this->shape_aggr_seq_[seq_length].shape = info_seq[i].shape;
        this->shape_aggr_seq_[seq_length].color = info_seq[i].color;
        this->shape_aggr_seq_[seq_length].number_of_shapes++;
        this->shape_aggr_seq_[seq_length].last_size = info_seq[i].size;
        this->shape_aggr_seq_[seq_length].last_posX = info_seq[i].posX;
        this->shape_aggr_seq_[seq_length].last_posY = info_seq[i].posY;
      }

    }
  }

  ShapeAggrSeq*
  ControllerR_exec_i::get_shapes_list(void)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                       this->mutex_, CORBA::INTERNAL ());
    ShapeAggrSeq_var info_seq = new ShapeAggrSeq(this->shape_aggr_seq_);
    this->shape_aggr_seq_.length(0);
    return info_seq._retn ();
  }

  void
  ControllerR_exec_i::clear_shape_list ()
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                       this->mutex_, CORBA::INTERNAL ());
    this->shape_aggr_seq_.length(0);
  }

  CCM_shapes_aggregates*
  ControllerR_exec_i::get_shapeAggrPub ()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ControllerR_exec_i::get_shapeAggrPub\n")));
    return ( new shapes_aggregates_i (*this) );
  }

  // Operations from Components::SessionComponent.

  void
  ControllerR_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ControllerR_exec_i::set_session_context\n")));

    this->ciao_context_ =
      ::Shapes::CCM_ShapesControllerR_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  ControllerR_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ControllerR_exec_i::configuration_complete\n")));
  }

  void
  ControllerR_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ControllerR_exec_i::ccm_activate\n")));
    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  ControllerR_exec_i::ccm_passivate (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
  }

  void
  ControllerR_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" CONTROLLERR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_ControllerR_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      ControllerR_exec_i);

    return retval;
  }
}
