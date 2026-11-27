#include "ior_mcast_client_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ior_mcast_Client_i client;

      return client.init (argc, argv);
    }
  catch (CORBA::SystemException const &)
    {
      // Ignore
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
