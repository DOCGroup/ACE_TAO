// -*- C++ -*-
// $Id$

#include "FactoryDriver.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

int
main (int argc, char *argv [])
{
  TAO_CEC_Default_Factory::init_svcs ();
  FactoryDriver driver;
  driver.start (argc, argv);
  return 0;
}
