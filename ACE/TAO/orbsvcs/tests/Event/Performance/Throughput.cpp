// $Id$

#include "Throughput.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Get_Opt.h"

ACE_RCSID (EC_Tests_Performance,
           Throughput,
           "$Id$")

int
main (int argc, char *argv [])
{
  EC_Throughput driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_Throughput::EC_Throughput (void)
{
}

int
EC_Throughput::parse_args (int& argc, char* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;
  return 0;
}

void
EC_Throughput::print_args (void) const
{
  this->EC_Driver::print_args ();
}

void
EC_Throughput::print_usage (void)
{
  this->EC_Driver::print_usage ();
}

void
EC_Throughput::modify_attributes (TAO_EC_Event_Channel_Attributes&)
{
}

void
EC_Throughput::execute_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->EC_Driver::execute_test (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
EC_Throughput::dump_results (void)
{
  this->EC_Driver::dump_results ();
}
