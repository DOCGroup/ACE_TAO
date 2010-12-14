// $Id$

#include "ior_mcast_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ior_mcast_Client_i::ior_mcast_Client_i (void)
{
  // Constructor
}

ior_mcast_Client_i::~ior_mcast_Client_i (void)
{
}

int
ior_mcast_Client_i::init (int& argc,
                          ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var mcast_server_object =
        orb->resolve_initial_references ("MCASTServer");

      if (CORBA::is_nil (mcast_server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve MCast Server\n : client"),
                          -1);

      MCast::Server_var mcast_srvr =
        MCast::Server::_narrow (mcast_server_object.in ());

      if (CORBA::is_nil (mcast_srvr.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot narrow MCast Service\n :client"),
                            -1);

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        mcast_srvr->connect_server ();

      if (ret_value != 0)
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
  catch (const CORBA::SystemException& ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
