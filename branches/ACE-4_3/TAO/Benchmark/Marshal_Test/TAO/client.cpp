#include "benchmark/driver.h"
#include "tao_marshal_client.h"

int main (int argc, char *argv [])
{
  // instantiate a DRIVER with the Tao_Client_Proxy as the template
  CORBA_Benchmark_Driver<Tao_Marshal_Client_Proxy>  driver (new
							     Tao_Marshal_Client_Proxy ());

  // let the driver do everything for us so that we remain very simple
  return driver.run (argc, argv);
}
