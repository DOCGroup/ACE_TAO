
#include "App_Monitor_Impl.h"
#include "ace/Time_Value.h"


App_Monitor_i* app_monitor = 0;


extern "C" void start_timing ()
{
  app_monitor->start_timing ();
}

extern "C" void stop_timing ()
{
  app_monitor->stop_timing ();
}


// Implementation skeleton constructor
App_Monitor_i::App_Monitor_i (void)
  : elapsed_time_ (0)
{
}

// Implementation skeleton destructor
App_Monitor_i::~App_Monitor_i (void)
{
}

::Deployment::QoSSpecifications * App_Monitor_i::get_app_QoS (
    void)
{
  // Add your implementation here

  //  ACE_DEBUG ((LM_DEBUG, "Get App QoS returns "));

  Deployment::QoSSpecifications *qos = new Deployment::QoSSpecifications ();

  qos->length (1);

  (*qos)[0].value <<= (CORBA::ULong)this->elapsed_time_;

  return qos;
}

void App_Monitor_i::start_timing ()
{
  this->tmp_elapsed_time_ = 0;
  //  ACE_DEBUG ((LM_DEBUG , "\r\n Inside the START_TIMING %u\n\n", this->elapsed_time_));
  timer_.start ();
}

void App_Monitor_i::stop_timing ()
{
  timer_.stop ();

  ACE_Time_Value tv;

  ACE_hrtime_t usec;

  timer_.elapsed_microseconds (usec);

  this->elapsed_time_ = usec;

  //  ACE_DEBUG ((LM_DEBUG , "\r\n The time is %u\n\n", this->elapsed_time_));
}
