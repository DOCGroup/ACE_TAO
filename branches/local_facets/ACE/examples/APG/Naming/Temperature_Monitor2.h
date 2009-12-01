// $Id$

#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include "Thermometer.h"
#include "Temperature_Monitor_Options.h"
#include "Naming_Context.h"

class Temperature_Monitor2
{
public:
  Temperature_Monitor2 (Temperature_Monitor_Options & opt,
                        Naming_Context & naming_context,
                        Naming_Context & shared_context)
        : opt_(opt),
          naming_context_(naming_context),
          shared_context_(shared_context)
    { }

  void monitor (void);

protected:
  void record_temperature (float temp);
  void record_history (float temp);
  void record_failure (void);
  void reset_device (Name_Binding_Ptr & resetCount);

private:
  Thermometer *thermometer_;
  Temperature_Monitor_Options &opt_;
  Naming_Context &naming_context_;
  Naming_Context &shared_context_;
};

#endif /* TEMPERATURE_MONITOR_H */
