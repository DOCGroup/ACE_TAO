#include "ior_mcast_client_i.h"

int
ior_mcast_Client_i::init (int &argc,
                          ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const mcast_server_object =
        orb->resolve_initial_references ("MCASTServer");

      if (CORBA::is_nil (mcast_server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve MCast Server\n : client"),
                          -1);

      MCast::Server_var const mcast_srvr =
        MCast::Server::_narrow (mcast_server_object.in ());

      if (CORBA::is_nil (mcast_srvr.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot narrow MCast Service\n :client"),
                            -1);

      // Invoke a request on the server
      CORBA::Boolean const ret_value =
        mcast_srvr->connect_server ();

      if (ret_value)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "The server has not been contacted. Error!!\n"),
                             -1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been successfully contacted!\n"));
        }
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
