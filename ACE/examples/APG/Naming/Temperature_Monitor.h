#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include <memory>

#include "Naming_Context.h"
#include "Temperature_Monitor_Options.h"
#include "Thermometer.h"

class Temperature_Monitor
{
public:
  Temperature_Monitor (Temperature_Monitor const &) = delete;
  Temperature_Monitor &operator= (Temperature_Monitor const &) = delete;
  Temperature_Monitor (Temperature_Monitor &&) = delete;
  Temperature_Monitor &operator= (Temperature_Monitor &&) = delete;

  Temperature_Monitor (Temperature_Monitor_Options const &opt,
                       Naming_Context &naming_context);

  void monitor ();

protected:
  void record_temperature (float temp);
  void record_failure ();
  void reset_device (Name_Binding_Ptr const &resetCount);

private:
  std::unique_ptr<Thermometer> thermometer_ {};
  Temperature_Monitor_Options const &opt_;
  Naming_Context &naming_context_;
};

#endif /* TEMPERATURE_MONITOR_H */
