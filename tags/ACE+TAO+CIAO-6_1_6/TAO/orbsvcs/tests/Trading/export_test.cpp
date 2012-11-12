// $Id$

#include "tao/Utils/ORB_Manager.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *done_file = ACE_TEXT("export_test_ready");

int
parse_args_spec (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
          done_file = get_opts.opt_arg ();
                  break;

      case '?':
      default:
             ACE_ERROR_RETURN ((LM_ERROR,
                                     "usage:  %s "
                                     "-d <done_file> "
                                     "\n",
                                     argv [0]),
                                    -1);

      }
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv);

      // Command line argument interpretation.
      TT_Parse_Args parse_args (argc, argv);

      //Command line argument parser for
      if (parse_args_spec (argc, argv) != 0)
          return 1;

      // Init the orb and bootstrap to the trading service.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_ERROR, "*** Bootstrap to the Lookup interface.\n"));

      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior);

      if (CORBA::is_nil (trading_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to bootstrap to the Trading Service.\n"),
                           -1);

      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if =
        CosTrading::Lookup::_narrow (trading_obj.in ());

      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter (lookup_if.in (),
                                               ! parse_args.quiet ());

      type_exporter.remove_all_types ();

      type_exporter.add_all_types ();

      type_exporter.remove_all_types ();

      type_exporter.add_all_types ();

      if (parse_args.federated ())
        {
          type_exporter.add_all_types_to_all ();
        }

      type_exporter.list_all_types ();

      type_exporter.describe_all_types ();

      type_exporter.fully_describe_all_types ();

      ACE_DEBUG ((LM_DEBUG, "*** Service Type Exporter tests complete.\n"));

      // Run the Offer Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Exporter tests.\n"));
      TAO_Offer_Exporter offer_exporter (lookup_if.in (), ! parse_args.quiet ());

      // = Test series.

      offer_exporter.withdraw_offers ();

      offer_exporter.export_offers ();

      offer_exporter.describe_offers ();

      offer_exporter.modify_offers ();

      offer_exporter.describe_offers ();

      offer_exporter.withdraw_offers_using_constraints ();

      offer_exporter.describe_offers ();

      offer_exporter.withdraw_offers ();

      offer_exporter.export_offers ();

      if (parse_args.federated ())
        {
          offer_exporter.export_offers_to_all ();
        }

      offer_exporter.describe_offers ();

      ACE_DEBUG ((LM_DEBUG, "*** Offer Exporter tests complete.\n"));
      ACE_DEBUG ((LM_DEBUG, "*** Now serving dynamic properties.\n"));

      size_t offset = 0;
      ACE_TCHAR file[1024];
      ACE_OS::strcpy(file, argv[0]);
      if ((offset = (size_t)ACE_OS::strrchr(file, ACE_TEXT('/'))) != 0) {
        offset -= ((size_t)file - 1);
      }
      ACE_OS::strcpy(file + offset, done_file);

      FILE *ready_file =
        ACE_OS::fopen (file, "w");
      if (ready_file != 0) {
        ACE_OS::fprintf (ready_file, "The export test is ready\n");
        ACE_OS::fclose (ready_file);
      }
      else {
        ACE_DEBUG ((LM_WARNING, "Unable to open %s for output.\n", file));
      }

      orb_manager.run ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Trader Export Tests Failed\n"), -1);
    }

  return 0;
}
