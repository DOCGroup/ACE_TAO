/**
 * Reactor examples
 *
 * Timers & state data
 */

#include <memory>
#include <string>
#include <utility>

#include "ace/OS_NS_time.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Signal.h"

// Listing 0 code/ch07
class TemperatureSensor
{
public:
  explicit TemperatureSensor (std::string location)
    : location_ (std::move (location))
    // ...
  { }

  std::string const &location () const
  {
    return this->location_;
  }

  int querySensor ()
  {
    // ...
    return ++this->count_;
  }

  float temperature () const
  {
    return this->temperature_;
  }

private:
  std::string location_;
  int count_ {};
  float temperature_ {};
  // ...
};
// Listing 0

// Listing 1 code/ch07
class TemperatureQueryHandler : public ACE_Event_Handler
{
public:
  TemperatureQueryHandler () = default;
  TemperatureQueryHandler (TemperatureQueryHandler const &) = delete;
  TemperatureQueryHandler (TemperatureQueryHandler &&) = delete;
  TemperatureQueryHandler &operator= (TemperatureQueryHandler const &) = delete;
  TemperatureQueryHandler &operator= (TemperatureQueryHandler &&) = delete;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *arg) override
  {
    time_t const epoch = static_cast<timespec_t> (current_time).tv_sec;

    TemperatureSensor const *const_sensor =
      static_cast<TemperatureSensor const *> (arg);
    TemperatureSensor *const sensor =
      const_cast<TemperatureSensor *> (const_sensor);

    int const query_count = sensor->querySensor ();
    this->updateAverageTemperature (sensor);

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("%C\t")
                ACE_TEXT ("%d/%d\t")
                ACE_TEXT ("%.2f/%.2f\t")
                ACE_TEXT ("%C\n"),
                sensor->location ().c_str (),
                ++this->counter_,
                query_count,
                this->averageTemperature_,
                sensor->temperature (),
                ACE_OS::ctime (&epoch)));
    return 0;
  }

private:
  void updateAverageTemperature (TemperatureSensor *)
  {
    // ...
  }

  int counter_ {};
  float averageTemperature_ {};
};
// Listing 1

// Create a SIGINT handler so that we can exit
// the program politely
class SigintHandler : public ACE_Event_Handler
{
public:
  SigintHandler () = default;
  SigintHandler (SigintHandler const &) = delete;
  SigintHandler (SigintHandler &&) = delete;
  SigintHandler &operator= (SigintHandler const &) = delete;
  SigintHandler &operator= (SigintHandler &&) = delete;

  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override
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
  std::unique_ptr<TemperatureQueryHandler> temperatureMonitor =
    std::make_unique<TemperatureQueryHandler> ();

  // Listing 2 code/ch07
  // Listing 2

  ACE_Time_Value const initialDelay {3};
  ACE_Time_Value const intervalOne {5};
  // Listing 3 code/ch07
  std::unique_ptr<TemperatureSensor> sensorOne =
    std::make_unique<TemperatureSensor> ("Kitchen");

  long const timer_id_one = ACE_Reactor::instance ()->schedule_timer (
    temperatureMonitor.get (), sensorOne.get (), initialDelay, intervalOne);
  // Listing 3

  ACE_Time_Value const intervalTwo {4};
  // Listing 4 code/ch07
  std::unique_ptr<TemperatureSensor> sensorTwo =
    std::make_unique<TemperatureSensor> ("Foyer");

  long const timer_id_two = ACE_Reactor::instance ()->schedule_timer (
    temperatureMonitor.get (), sensorTwo.get (), initialDelay, intervalTwo);
  // Listing 4

  std::unique_ptr<SigintHandler> handleExit =
    std::make_unique<SigintHandler> ();

  ACE_Reactor::instance ()->register_handler (SIGINT,
                                              handleExit.get ());

  ACE_Reactor::instance ()->run_reactor_event_loop ();
  ACE_Reactor::instance ()->cancel_timer (timer_id_one);
  ACE_Reactor::instance ()->cancel_timer (timer_id_two);
  ACE_Sig_Set signals;
  signals.sig_add (SIGINT);
  ACE_Reactor::instance ()->remove_handler (signals);

  return 0;
}
