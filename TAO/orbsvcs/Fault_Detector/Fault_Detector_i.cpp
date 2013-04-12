/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Fault_Detector_i.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file implements the Fault_Detector_i class as declared in Fault_Detector_i.h.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "orbsvcs/Log_Macros.h"
#include "Fault_Detector_i.h"
#include "FT_FaultDetectorFactory_i.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

///////////////////////////////
// Fault_Detector_i static data

ACE_Time_Value TAO::Fault_Detector_i::sleep_time_(1,0);


/////////////////////////////////////////
// Fault_Detector_i public static methods

void TAO::Fault_Detector_i::set_time_for_all_detectors(ACE_Time_Value value)
{
  sleep_time_ = value;
}

////////////////////////////////////////////
// Fault_Detector_i construction/destruction

TAO::Fault_Detector_i::Fault_Detector_i (
      FT_FaultDetectorFactory_i & factory,
      CORBA::ULong id,
      FT::FaultNotifier_ptr & notifier,
      FT::PullMonitorable_ptr & monitorable,
      FT::FTDomainId domain_id,
      const PortableGroup::Location & object_location,
      PortableGroup::TypeId object_type,
      PortableGroup::ObjectGroupId group_id
      )
  : factory_(factory)
  , id_(id)
  , domain_id_(domain_id)
  , object_location_(object_location)
  , object_type_(object_type)
  , group_id_(group_id)
  , sleep_(0)           // initially not signaled
  , quit_requested_(0)
{
  this->notifier_ = FT::FaultNotifier::_duplicate(notifier);
  this->monitorable_ = FT::PullMonitorable::_duplicate(monitorable);
  ORBSVCS_DEBUG ((LM_DEBUG,
    "Object type %s\n", object_type
    ));
}

TAO::Fault_Detector_i::~Fault_Detector_i ()
{
}

////////////////////////////////////
// Fault_Detector_i public interface


void TAO::Fault_Detector_i::request_quit()
{
  this->quit_requested_ = 1;
  // wake up the thread
  this->sleep_.signal ();
}

void TAO::Fault_Detector_i::start(ACE_Thread_Manager & threadManager)
{
  threadManager.spawn(thr_func, this);
}

///////////////////////////////////////////////////
// Fault_Detector_i private implementation methods

void TAO::Fault_Detector_i::run()
{
  while ( ! this->quit_requested_ )
  {
    try
    {
      if (this->monitorable_->is_alive())
      {
        // use this rather than ACE_OS::sleep
        // to allow the nap to be interruped see request_quit
        this->sleep_.wait (&sleep_time_, 0);
      }
      else
      {
        ORBSVCS_ERROR ((LM_INFO,
          "FaultDetector%d FAULT: not alive.\n",
          id_
          ));
        notify();
        this->quit_requested_ = 1;
      }
    }
    catch (const CORBA::Exception&)// todo refine this
    {
      ORBSVCS_ERROR ((LM_ERROR,
        "FaultDetector FAULT: exception.\n"
        ));
      notify();
      this->quit_requested_ = 1;
    }
  }
  // warning:  The following call will delete
  // this object.  Be careful not to reference
  // member data after making this call.
  // todo: use "scoped resource management" to make
  // this exception-safe and stupid-return-safe.
  this->factory_.remove_detector (this->id_, this);
}

void TAO::Fault_Detector_i::notify()
{
  CosNotification::StructuredEvent_var vEvent;
  ACE_NEW_NORETURN(vEvent, CosNotification::StructuredEvent );
  if (vEvent.ptr() != 0)
  {
    CORBA::ULong length = 2;
    if( this->object_type_ != 0)
    {
      length = 3;
      if (this->group_id_!= 0)
      {
        length = 4;
      }
    }

    vEvent->header.fixed_header.event_type.domain_name = FT::FT_EVENT_TYPE_DOMAIN;
    vEvent->header.fixed_header.event_type.type_name = FT::FT_EVENT_TYPE_NAME;
    vEvent->filterable_data.length(length);
    vEvent->filterable_data[0].name = FT::FT_DOMAIN_ID;
    (vEvent->filterable_data[0].value) <<= this->domain_id_;
    vEvent->filterable_data[1].name = FT::FT_LOCATION;
    (vEvent->filterable_data[1].value) <<= this->object_location_;
    if (this->object_type_!= 0)
    {
      vEvent->filterable_data[2].name = FT::FT_TYPE_ID;
      (vEvent->filterable_data[2].value) <<= this->object_type_;
      if (this->group_id_!= 0)
      {
        vEvent->filterable_data[3].name = FT::FT_GROUP_ID;
        vEvent->filterable_data[3].value <<= this->group_id_;
      }
    }
    try
    {
      if (TAO_debug_level > 5)
      {
        ORBSVCS_ERROR ((LM_ERROR,
        "call Fault Detector push Structured Event.\n"
        ));
      }
      this->notifier_->push_structured_fault(vEvent.in());
      if (TAO_debug_level > 5)
      {

        ORBSVCS_ERROR ((LM_ERROR,
        "return from Fault Detector push Structured Event.\n"
        ));
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Fault Detector cannot send notification.");
    }
  }
  else
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "Fault Detector cannot create Structured Event.\n"
      ));
  }
}


/////////////////////////////////////////////////////////
// Fault_Detector_i private static implementation methods

//static
ACE_THR_FUNC_RETURN TAO::Fault_Detector_i::thr_func (void * arg)
{
  TAO::Fault_Detector_i * detector = static_cast<TAO::Fault_Detector_i * > (arg);
  detector->run ();
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
