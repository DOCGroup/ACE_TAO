#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include <memory>

#include "Naming_Context.h"
#include "Temperature_Monitor_Options.h"
#include "Thermometer.h"

class Temperature_Monitor2
{
public:
  Temperature_Monitor2 (Temperature_Monitor2 const &) = delete;
  Temperature_Monitor2 &operator = (Temperature_Monitor2 const &) = delete;
  Temperature_Monitor2 (Temperature_Monitor2 &&) = delete;
  Temperature_Monitor2 &operator = (Temperature_Monitor2 &&) = delete;

  Temperature_Monitor2 (Temperature_Monitor_Options const &opt,
                        Naming_Context &naming_context,
                        Naming_Context &shared_context)
    : opt_ (opt),
      naming_context_ (naming_context),
      shared_context_ (shared_context)
  { }

  void monitor ();

protected:
  void record_temperature (float temp);
  void record_history (float temp);
  void record_failure ();
  void reset_device (Name_Binding_Ptr const &resetCount);

private:
  std::unique_ptr<Thermometer> thermometer_ {};
  Temperature_Monitor_Options const &opt_;
  Naming_Context &naming_context_;
  Naming_Context &shared_context_;
};

#endif /* TEMPERATURE_MONITOR_H */
