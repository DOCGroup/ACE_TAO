// $Id$

#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

#include "Thermometer.h"
#include "Temperature_Monitor.h"
#include "EMail.h"

// Listing 1 code/ch21
Temperature_Monitor::Temperature_Monitor
  (Temperature_Monitor_Options &opt,
   Naming_Context &naming_context)
    : opt_(opt), naming_context_(naming_context)
{ }
// Listing 1

// Listing 31 code/ch21
void Temperature_Monitor::record_temperature (float temp)
{
  Name_Binding_Ptr current
    (this->naming_context_.fetch ("current"));
  if (current.get())
    {
      this->naming_context_.rebind ("previous",
                                    current->value ());
    }
// Listing 31

// Listing 32 code/ch21
  this->naming_context_.rebind ("current", temp);
// Listing 32

// Listing 33 code/ch21
  this->naming_context_.unbind ("lastReset");
  this->naming_context_.unbind ("resetCount");
// Listing 33
}

// Listing 41 code/ch21
void Temperature_Monitor::record_failure (void)
{
  Name_Binding_Ptr lastReset
    (this->naming_context_.fetch ("lastReset"));
  Name_Binding_Ptr resetCount
    (this->naming_context_.fetch ("resetCount"));
// Listing 41

// Listing 42 code/ch21
  int now = (int) ACE_OS::time ();
  int lastResetTime;
  if (lastReset.get ())
    {
      lastResetTime = lastReset->int_value ();
    }
  else
    {
      this->naming_context_.rebind ("lastReset", now);
      lastResetTime = now;
    }
  // Listing 42

  // Listing 43 code/ch21
  if (now - lastResetTime > this->opt_.reset_interval ())
    {
      this->reset_device (resetCount);
    }
  // Listing 43
}

// Listing 5 code/ch21
void
Temperature_Monitor::reset_device (Name_Binding_Ptr &resetCount)
{
  int number_of_resets = 1;
  if (resetCount.get ())
    {
      number_of_resets = resetCount->int_value () + 1;
      if (number_of_resets > this->opt_.excessive_resets ())
        {
          // Exclude 5
          EMail notification;

          char message[BUFSIZ];
          ACE_OS::sprintf (message,
                           "Thermometer: %s\n"
                           "Reset Count: %d\n",
                           this->thermometer_->address(),
                           number_of_resets);

          notification.send (this->opt_.admin_email (),
                             this->opt_.email_from (),
                             "Excessive number of thermometer resets",
                             message);
          // Exclude 5
        }
    }
  this->thermometer_->reset ();
  this->naming_context_.rebind ("lastReset",
                                (int) ACE_OS::time ());
  this->naming_context_.rebind ("resetCount",
                                number_of_resets);
}
// Listing 5

// Listing 2 code/ch21
void Temperature_Monitor::monitor (void)
{
  this->thermometer_ =
    new Thermometer (this->opt_.thermometer_address ());

  for(;;)
    {
      float temp = this->thermometer_->temperature ();
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Read temperature %.2f\n"),
                  temp));

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

  ACE_NOTREACHED (delete this->thermometer_;)
}
// Listing 2
