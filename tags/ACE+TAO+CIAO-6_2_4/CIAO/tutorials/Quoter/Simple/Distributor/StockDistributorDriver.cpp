//$Id$
/*
 * @file StockDistributorDriver.cpp
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */


#include "StockDistributorC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *distributor_ior = ACE_TEXT ("file://StockDistributor.ior");
int rate = 2;
int turn_on = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:r:of"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'k':
            distributor_ior = get_opts.opt_arg ();
            break;

          case 'o':
            turn_on = 1;
            break;

          case 'f':
            turn_on = 0;
            break;

          case 'r':
            rate = ACE_OS::atoi (get_opts.opt_arg ());
            break;

          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s\n"
                              "-o (Start the distribution service)\n"
                              "-f (Stop the distribution service)\n"
                              "-k <Distributor IOR> (default is file://StockDistributor.ior)\n"
                              "-r <broadcast frequency in Hertz> (default is 2)\n"
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      // create the factory object reference,
      CORBA::Object_var distributor_obj =
        orb->string_to_object (distributor_ior);

      // downcast the object reference to the appropriate type
      Stock::StockDistributor_var distributor =
        Stock::StockDistributor::_narrow (distributor_obj.in ());

      if (CORBA::is_nil (distributor.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to acquire 'Distributor' objref\n"),
                            -1);
        }

      if (turn_on)
        {
          distributor->rate (rate);

          ACE_DEBUG ((LM_DEBUG, "Start up the Distribution service\n"));

          distributor->start ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Stop the Distribution service\n"));

          distributor->stop ();
        }

      orb->destroy ();
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
