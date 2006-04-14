// -*- C++ -*-
// $Id$

#include "FactoryDriver.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  TAO_CEC_Default_Factory::init_svcs ();
  FactoryDriver driver;
  driver.start (convert.get_argc(), convert.get_ASCII_argv());
  return 0;
}
