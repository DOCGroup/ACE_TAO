// $Id$

#include "benchmark/driver.h"
#include "visi_marshal_server.h"

ACE_RCSID(VisiBroker, server, "$Id$")

int main (int argc, char *argv [])
{
  // instantiate a DRIVER with the Visi_Client_Proxy as the template
  CORBA_Benchmark_Driver<Visi_Marshal_Server_Proxy>  driver (new
							     Visi_Marshal_Server_Proxy ());

  // let the driver do everything for us so that we remain very simple
  return driver.run (argc, argv);
}
