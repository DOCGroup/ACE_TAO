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
#include "ace/pre.h"
#include "Fault_Detector_i.h"
#include "FT_FaultDetectorFactory_i.h"

Fault_Detector_i::Fault_Detector_i (
      FT_FaultDetectorFactory_i & factory,
      CORBA::ULong id,
      FT::FaultNotifier_ptr & notifier,
      FT::PullMonitorable_ptr & monitorable,
      FT::FTDomainId domain_id,
      const FT::Location & object_location,
      FT::TypeId object_type,
      FT::ObjectGroupId group_id
      )
  : factory_(factory)
  , id_(id)
  , domain_id_(domain_id)
  , object_location_(object_location)
  , object_type_(object_type)
  , group_id_(group_id)
  , sleep_(0)           // initially not signaled
  , quitRequested_(0)
{
  notifier_ = FT::FaultNotifier::_duplicate(notifier);
  monitorable_ = FT::PullMonitorable::_duplicate(monitorable);
}

Fault_Detector_i::~Fault_Detector_i ()
{
}

void Fault_Detector_i::run()
{
  while ( ! quitRequested_ )
  {
    ACE_TRY_NEW_ENV
    {
      if (monitorable_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER))
      {
        ACE_TRY_CHECK;
        // use this rather than ACE_OS::sleep
        // to allow the nap to be interruped see requestQuit
        sleep_.wait (&sleepTime_, 0);
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "FaultDetector%d FAULT: not alive.\n",
          id_
          ));
        notify();
        quitRequested_ = 1;
      }
    }
    ACE_CATCHANY  // todo refine this
    {
      ACE_ERROR ((LM_ERROR,
        "FaultDetector FAULT: exception.\n"
        ));
      notify();
      quitRequested_ = 1;
    }
    ACE_ENDTRY;
  }
  // warning:  The following call will delete
  // this object.  Be careful not to reference
  // member data after making this call.
  // todo: use "scoped resource management" to make
  // this exception-safe and stupid-return-safe.
  factory_.removeDetector (id_, this);
}


void Fault_Detector_i::notify()
{
  CosNotification::StructuredEvent *  pEvent;
  ACE_NEW_NORETURN(pEvent, CosNotification::StructuredEvent );
  if (pEvent != 0)
  {
    CosNotification::StructuredEvent_var vEvent(pEvent);
    short length = 2;
    if( object_type_ != 0)
    {
      length = 3;
      if (group_id_!= 0)
      {
        length = 4;
      }
    }

    vEvent->header.fixed_header.event_type.domain_name = FT::FT_EVENT_TYPE_DOMAIN;
    vEvent->header.fixed_header.event_type.type_name = FT::FT_EVENT_TYPE_NAME;
    vEvent->filterable_data.length(length);
    vEvent->filterable_data[0].name = FT::FT_DOMAIN_ID;
    (vEvent->filterable_data[0].value) <<= domain_id_;
    vEvent->filterable_data[1].name = FT::FT_LOCATION;
    (vEvent->filterable_data[1].value) <<= object_location_;
    if (object_type_!= 0)
    {
      vEvent->filterable_data[2].name = FT::FT_TYPE_ID;
      (vEvent->filterable_data[2].value) <<= object_type_;
      if (group_id_!= 0)
      {
        vEvent->filterable_data[3].name = FT::FT_GROUP_ID;
        vEvent->filterable_data[3].value <<= group_id_;
      }
    }
    ACE_TRY_NEW_ENV
    {
      notifier_->push_structured_fault(vEvent.in()
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
    }
    ACE_ENDTRY;
  }
  else
  {
    // todo log error: cannot create event
  }
}

void Fault_Detector_i::requestQuit()
{
  quitRequested_ = 1;
  // wake up the thread
  sleep_.signal ();
}

void Fault_Detector_i::start(ACE_Thread_Manager & threadManager)
{
  threadManager.spawn(thr_func, this);
}

//static
ACE_THR_FUNC_RETURN Fault_Detector_i::thr_func (void * arg)
{
  Fault_Detector_i * detector = static_cast<Fault_Detector_i *>(arg);
  detector->run();
  return 0;
}

//static
ACE_Time_Value Fault_Detector_i::sleepTime_(1,0);

//static
void Fault_Detector_i::setTimeValue(ACE_Time_Value value)
{
}

