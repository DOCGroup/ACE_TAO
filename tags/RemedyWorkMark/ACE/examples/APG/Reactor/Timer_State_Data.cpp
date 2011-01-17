/**
 * $Id$
 *
 * Reactor examples
 *
 * Timers & state data
 */

#include "ace/OS_NS_time.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

// Listing 0 code/ch07
class TemperatureSensor
{
public:
  TemperatureSensor (const char *location)
    : location_(location),
      count_(0),
      temperature_(0.0)
    // ...
  { }

  const char *location () const
  {
    return this->location_;
  }

  int querySensor (void)
  {
    // ...
    return ++this->count_;
  }

  float temperature (void) const
  {
    return this->temperature_;
  }

private:
  const char *location_;
  int count_;
  float temperature_;
  // ...
};
// Listing 0

// Listing 1 code/ch07
class TemperatureQueryHandler : public ACE_Event_Handler
{
public:
  TemperatureQueryHandler ()
    : ACE_Event_Handler(),
      counter_(0),
      averageTemperature_(0.0)
    // ...
  {
  }

  int handle_timeout (const ACE_Time_Value &current_time,
                      const void *arg)
  {
    time_t epoch = ((timespec_t)current_time).tv_sec;

    const TemperatureSensor *const_sensor =
      reinterpret_cast<const TemperatureSensor *> (arg);
    TemperatureSensor *sensor =
      const_cast<TemperatureSensor *> (const_sensor);

    int queryCount = sensor->querySensor ();
    this->updateAverageTemperature (sensor);

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("%s\t")
                ACE_TEXT ("%d/%d\t")
                ACE_TEXT ("%.2f/%.2f\t")
                ACE_TEXT ("%s\n"),
                sensor->location (),
                ++this->counter_,
                queryCount,
                this->averageTemperature_,
                sensor->temperature (),
                ACE_OS::ctime(&epoch)));
    return 0;
  }

private:
  void updateAverageTemperature (TemperatureSensor *)
  {
    // ...
  }

  int counter_;
  float averageTemperature_;
};
// Listing 1

// Create a SIGINT handler so that we can exit
// the program politely
class SigintHandler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum, siginfo_t * = 0,
                     ucontext_t * = 0)
  {
    if (signum == SIGINT)
      {
        ACE_Reactor::instance ()->end_reactor_event_loop ();
      }
    return 0;
  }
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 2 code/ch07
  TemperatureQueryHandler *temperatureMonitor =
    new TemperatureQueryHandler ();
  // Listing 2

  ACE_Time_Value initialDelay (3);
  ACE_Time_Value intervalOne (5);
  // Listing 3 code/ch07
  TemperatureSensor *sensorOne =
    new TemperatureSensor ("Kitchen");

  ACE_Reactor::instance ()->schedule_timer (temperatureMonitor,
                                            sensorOne,
                                            initialDelay,
                                            intervalOne);
  // Listing 3

  ACE_Time_Value intervalTwo (4);
  // Listing 4 code/ch07
  TemperatureSensor *sensorTwo =
    new TemperatureSensor ("Foyer");

  ACE_Reactor::instance ()->schedule_timer (temperatureMonitor,
                                            sensorTwo,
                                            initialDelay,
                                            intervalTwo);
  // Listing 4

  SigintHandler *handleExit = new SigintHandler ();

  ACE_Reactor::instance ()->register_handler (SIGINT,
                                              handleExit);

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  return 0;
}
