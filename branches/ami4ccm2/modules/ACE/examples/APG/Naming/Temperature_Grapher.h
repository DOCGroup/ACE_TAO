// $Id$

#ifndef TEMPERATURE_GRAPHER_H
#define TEMPERATURE_GRAPHER_H

#include "Thermometer.h"
#include "Temperature_Grapher_Options.h"
#include "Naming_Context.h"

class Temperature_Grapher
  {
  public:
    Temperature_Grapher( Temperature_Grapher_Options & opt,
                         Naming_Context & naming_context )
        : opt_(opt), naming_context_(naming_context)
    {
    }

    void monitor();

  protected:
    void update_graph();

  private:
    Thermometer * thermometer_;
    Temperature_Grapher_Options & opt_;
    Naming_Context & naming_context_;
  };

#endif /* TEMPERATURE_GRAPHER_H */
