// $Id$

#include "server_i.h"

int main (int argc, char *argv[])
{
  try
    {
      Server_i svr_i;

      const int init_result = svr_i.init (argc, argv);

      if (init_result != 0)
        return 1;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CORBA exception raised in server!");
    }

  return 0;
}
