// $Id$

#include "ace/Reactor.h"
#include "orbsvcs/FtRtEvent/EventChannel/Fault_Detector.h"
#include "ace/Select_Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Fault_Detector::ReactorTask::ReactorTask()
  : reactor_(new ACE_Select_Reactor, 1)
{
}

int
Fault_Detector::ReactorTask::svc (void)
{
  this->reactor_.owner(ACE_OS::thr_self());
  this->reactor_.run_reactor_event_loop();
  return 0;
}


Fault_Detector::~Fault_Detector()
{
}

const FTRT::Location& Fault_Detector::my_location() const
{
  return location_;
}

namespace {
  Fault_Detector* detector;
}

Fault_Detector* Fault_Detector::instance()
{
  return detector;
}

int Fault_Detector::init(int argc, ACE_TCHAR** argv)
{
  detector = this;
  if (this->parse_conf(argc, argv)==0 &&
    this->init_acceptor() ==0)
  {
    if (!reactor_task_.thr_count() &&
      reactor_task_.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,"Cannot activate reactor thread\n"),
      -1);
    return 0;
  }
  return -1;
}

void Fault_Detector::stop()
{
  reactor_task_.reactor_.end_reactor_event_loop();
}

void Fault_Detector::set_listener(TAO_FTEC_Fault_Listener* listener)
{
  listener_ = listener;
}

int Fault_Detector::parse_conf(int , ACE_TCHAR** )
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
