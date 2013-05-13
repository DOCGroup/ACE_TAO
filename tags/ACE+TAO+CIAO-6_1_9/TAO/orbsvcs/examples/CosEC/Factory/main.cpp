// -*- C++ -*-
// $Id$

#include "FactoryDriver.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();
  FactoryDriver driver;
  driver.start (argc, argv);
  return 0;
}
