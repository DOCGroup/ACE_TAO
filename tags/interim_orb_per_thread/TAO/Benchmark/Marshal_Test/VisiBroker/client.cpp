#include "benchmark/driver.h"
#include "visi_marshal_client.h"

int main (int argc, char *argv [])
{
  // instantiate a DRIVER with the Visi_Client_Proxy as the template
  CORBA_Benchmark_Driver<Visi_Marshal_Client_Proxy>  driver (new
							     Visi_Marshal_Client_Proxy ());

  // let the driver do everything for us so that we remain very simple
  return driver.run (argc, argv);
}
