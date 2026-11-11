#include "TLS_Client.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TLS_Client client {}; // Telecom Log Service client

  try
    {
      client.init (argc, argv);
      client.run_tests ();
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception ("TLS_Client user error: ");
      return 1;
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception ("TLS_Client system error: ");
      return 1;
    }

  return 0;
}
