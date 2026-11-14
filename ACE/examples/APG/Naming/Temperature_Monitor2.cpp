#include <cstdio>
#include <ctime>

#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

#include "Thermometer.h"
#include "Temperature_Monitor2.h"
#include "EMail.h"

// Listing 1 code/ch21
void Temperature_Monitor2::record_temperature (float temp)
{
  Name_Binding_Ptr current {this->naming_context_.fetch ("current")};
  if (current)
    {
      this->naming_context_.rebind ("previous",
                                    current->value ());
    }

  this->record_history (temp);

  this->naming_context_.unbind ("lastFailure");
  this->naming_context_.unbind ("lastReset");
  this->naming_context_.unbind ("resetCount");
}
// Listing 1

// Listing 2 code/ch21
void Temperature_Monitor2::record_history (float temp)
{
  int const now = static_cast<int> (std::time (nullptr));
  this->shared_context_.rebind ("lastUpdate", now);

  Name_Binding_Ptr counter {this->shared_context_.fetch ("counter")};
  int counterValue = counter ? counter->int_value () : 0;

  char name[BUFSIZ] {};
  std::snprintf (name, sizeof (name), "history[%d]", counterValue);

  char value[BUFSIZ] {};
  std::snprintf (value, sizeof (value), "%d|%.2f", now, temp);

  this->shared_context_.rebind (name, value);

  ++counterValue;
  counterValue %= this->opt_.history_size ();
  this->shared_context_.rebind ("counter", counterValue);
}
// Listing 2

void Temperature_Monitor2::reset_device (Name_Binding_Ptr const &resetCount)
{
  int number_of_resets = 1;

  if (resetCount)
    {
      number_of_resets = resetCount->int_value () + 1;

      if (number_of_resets > this->opt_.excessive_resets ())
        {
          EMail notification {};

          char message[BUFSIZ] {};
          std::snprintf (message,
                         sizeof (message),
                         "Thermometer: %s\n"
                         "Reset Count: %d\n",
                         this->thermometer_->address ().c_str (),
                         number_of_resets);

          notification.send (this->opt_.admin_email (),
                             this->opt_.email_from (),
                             "Excessive number of thermometer resets",
                             message);
        }
    }

  this->thermometer_->reset ();

  this->naming_context_.rebind (
    "lastReset", static_cast<int> (std::time (nullptr)));
  this->naming_context_.rebind ("resetCount", number_of_resets);
}

void Temperature_Monitor2::record_failure ()
{
  Name_Binding_Ptr lastFailure {this->naming_context_.fetch ("lastFailure")};
  Name_Binding_Ptr lastReset {this->naming_context_.fetch ("lastReset")};
  Name_Binding_Ptr resetCount {this->naming_context_.fetch ("resetCount")};

  int const now = static_cast<int> (std::time (nullptr));

  int lastFailureTime;
  int lastResetTime {};

  if (lastFailure)
    {
      lastFailureTime = lastFailure->int_value ();
    }
  else
    {
      this->naming_context_.rebind ("firstFailure", now);
      this->naming_context_.rebind ("lastReset", now);
      lastFailureTime = now;
      lastResetTime = now;
    }

  if (lastReset)
    {
      lastResetTime = lastReset->int_value ();
    }

  if (now - lastResetTime > this->opt_.reset_interval ())
    {
      this->reset_device (resetCount);
    }

  this->naming_context_.rebind ("lastFailure", lastFailureTime);
}

void Temperature_Monitor2::monitor ()
{
  this->thermometer_ = std::make_unique<Thermometer> (
    this->opt_.thermometer_address ());

  while (true)
    {
      float const temp = this->thermometer_->temperature ();

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Read temperature %.2f\n"), temp));

      if (temp >= 0)
        {
          this->record_temperature (temp);
        }
      else
        {
          this->record_failure ();
        }

      ACE_OS::sleep (this->opt_.poll_interval ());
    }
}
