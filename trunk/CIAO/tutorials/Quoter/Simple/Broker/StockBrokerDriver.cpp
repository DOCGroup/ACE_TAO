//$Id$
/*
 * @file StockBrokerDriver.cpp
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */

#include "StockBrokerC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *broker_ior = ACE_TEXT ("file://StockBroker.ior");
const ACE_TCHAR *subscribe_name = 0;
const ACE_TCHAR *unsubscribe_name = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:s:u:"));
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
                              "-k <Broker IOR> (default is file://StockBroker.ior)\n"
                              "-s <stock name you wish to subscribe>\n"
                              "-u <stock name you wish to unsubscribe>\n"
                              "\n",
                              argv [0]),
                              -1);
        }
    }

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return -1;

      // create the factory object reference of StockBrokerHome
      CORBA::Object_var broker_obj =
        orb->string_to_object (broker_ior);

      // downcast the object reference to the appropriate type
      Stock::StockBroker_var broker =
      Stock::StockBroker::_narrow (broker_obj.in ());

      if (CORBA::is_nil (broker.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to acquire 'Broker' objref\n"),
                            -1);
        }

      if (subscribe_name != 0)
        {
          broker->stock_subscribe (ACE_TEXT_ALWAYS_CHAR (subscribe_name));

          ACE_DEBUG ((LM_DEBUG, "Subscribe successful!\n"));
        }

      if (unsubscribe_name != 0)
        {
          broker->stock_unsubscribe (ACE_TEXT_ALWAYS_CHAR (unsubscribe_name));

          ACE_DEBUG ((LM_DEBUG, "Unsubscribe successful!\n"));
        }

      // Finally destroy the ORB
      orb->destroy ();

    }
  catch (const Stock::Invalid_Stock& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid stock exception.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Uncaught CORBA exception\n"),
                        1);
    }

  return 0;
}
