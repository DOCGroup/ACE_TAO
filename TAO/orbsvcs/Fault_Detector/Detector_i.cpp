// $Id$
#include "Detector_i.h"
#include <ace/Thread_Manager.h>

Detector_i::Detector_i (
    FT::FaultNotifier_var & notifier,
    FT::PullMonitorable_var & monitorable,
    FT::FTDomainId domain_id,
    FT::ObjectGroupId group_id,
    FT::Location object_location,
    FT::TypeId object_type
    )
  : notifier_(notifier)
  , monitorable_(monitorable)
  , domain_id_(domain_id)
  , group_id_(group_id)
  , object_location_(object_location)
  , object_type_(object_type)
  , quitRequested_(0)
{
}

Detector_i::~Detector_i ()
{
}

void Detector_i::run()
{
  while ( ! quitRequested_ )
  {
    ACE_TRY_NEW_ENV
    {
      if (monitorable_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER))
      {
        ACE_TRY_CHECK;
        ACE_OS::sleep (sleepTime_);
      }
      else
      {
        notify();
        quitRequested_ = 1;
      }
    }
    ACE_CATCHANY  // todo refine this
    {
      notify();
      quitRequested_ = 1;
    }
    ACE_ENDTRY;
  }
}

void Detector_i::notify()
{
   int todo;
}


void
Detector_i::start(ACE_Thread_Manager & threadManager)
{
  threadManager.spawn(thr_func, this);
}

//static
ACE_THR_FUNC_RETURN Detector_i::thr_func (void * arg)
{
  Detector_i * detector = static_cast<Detector_i *>(arg);
  detector->run();
  return 0;
}

//static
ACE_Time_Value Detector_i::sleepTime_(1,0);

//static
void Detector_i::setTimeValue(ACE_Time_Value value)
{
}



