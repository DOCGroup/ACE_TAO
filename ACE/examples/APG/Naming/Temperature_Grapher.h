#ifndef TEMPERATURE_GRAPHER_H
#define TEMPERATURE_GRAPHER_H

#include "Naming_Context.h"
#include "Temperature_Grapher_Options.h"

class Temperature_Grapher
{
public:
  Temperature_Grapher (Temperature_Grapher_Options const &opt,
                       Naming_Context &naming_context)
    : opt_ (opt), naming_context_ (naming_context)
  { }

  Temperature_Grapher (Temperature_Grapher const &) = delete;
  Temperature_Grapher &operator= (Temperature_Grapher const &) = delete;
  Temperature_Grapher (Temperature_Grapher &&) = delete;
  Temperature_Grapher &operator= (Temperature_Grapher &&) = delete;

  void monitor ();

protected:
  void update_graph ();

private:
  Temperature_Grapher_Options const &opt_;
  Naming_Context &naming_context_;
};

#endif /* TEMPERATURE_GRAPHER_H */
