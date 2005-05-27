//$Id$
/*
 * @file Broker.cpp
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */

#include "BrokerC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const char *broker_ior = "file://Broker.ior";
const char *subscribe_name = 0;
const char *unsubscribe_name = 0;


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:s:u:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'k':
        broker_ior = get_opts.opt_arg ();
        break;

      case 's':
        subscribe_name = get_opts.opt_arg ();
        break;

      case 'u':
        unsubscribe_name = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage:  %s\n"
                          "-k <Broker IOR> (default is file:\\broker.ior) \n"
                          "-s <stock name you wish to subscribe>\n"
                          "-u <stock name you wish to unsubscribe>\n"
                          "\n",
                          argv [0]),
                          -1);
    }
  }

  return 0;
}

int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
  {
    // initialize the ORB
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,""
                       ACE_ENV_ARG_PARAMETER);

    ACE_TRY_CHECK;

    if (parse_args (argc, argv) != 0)
      return -1;

    // create the factory object reference of StockBrokerHome
    CORBA::Object_var broker_obj =
      orb->string_to_object (broker_ior
                             ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // downcast the object reference to the appropriate type
    Stock::StockBroker_var broker =
    Stock::StockBroker::_narrow (broker_obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil (broker.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Unable to acquire 'Broker' objref\n"),
                        -1);
    }


    if (subscribe_name != 0)
    {
      broker->stock_subscribe (subscribe_name
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Subscribe successful!\n"));
    }

    if (unsubscribe_name != 0)
    {
      broker->stock_unsubscribe (unsubscribe_name
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Unsubscribe successful!\n"));
    }

    // Finally destroy the ORB
    orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);

    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "Who is the culprit \n");
    ACE_ERROR_RETURN ((LM_ERROR,
                        "Uncaught CORBA exception\n"),
                      1);
  }
  ACE_ENDTRY;

  return 0;
}
