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
                          char *argv[]
                          ACE_ENV_ARG_DECL)
{

  ACE_TRY
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "" /* the ORB name, it can be anything! */
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var mcast_server_object =
        orb->resolve_initial_references ("MCASTServer" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (mcast_server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve MCast Server\n : client"),
                          -1);

      MCast::Server_var mcast_srvr =
        MCast::Server::_narrow (mcast_server_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (mcast_srvr.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot narrow MCast Service\n :client"),
                            -1);

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        mcast_srvr->connect_server (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret_value != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has not been contacted. Error!!\n",
                      0));
        }
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "client");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
