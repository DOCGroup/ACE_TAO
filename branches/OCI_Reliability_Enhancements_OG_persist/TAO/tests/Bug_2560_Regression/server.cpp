//
// $Id$
//

#include "Stock_Factory_i.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TestORB");

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
    poa_manager->activate ();

    // Make policies for child POA
    CORBA::PolicyList policies(2) ;
    policies.length(2) ;
    policies[0] = root_poa->create_lifespan_policy ( PortableServer::PERSISTENT ) ;
    policies[1] = root_poa->create_id_assignment_policy ( PortableServer::USER_ID ) ;

    PortableServer::POA_var poa = root_poa->create_POA ( "MyPOA", poa_manager.in(), policies );

    // Creation of the new POAs over, so destroy the Policy_ptr's.
    for ( CORBA::ULong i = 0 ; i < policies.length (); ++i ) {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy ();
    }

    // use this poa for making system objects
    Quoter_Stock_i::set_default_POA ( poa.in() ) ;

    // Create the servant
    Quoter_Stock_Factory_i *stock_factory_i = 0;
    ACE_NEW_RETURN (stock_factory_i,
                    Quoter_Stock_Factory_i,
                    -1);
    PortableServer::ServantBase_var safe (stock_factory_i);

    // Activate it to obtain the object reference
    PortableServer::ObjectId_var id =
      root_poa->activate_object (stock_factory_i);

    CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

    Quoter::Stock_Factory_var stock_factory =
      Quoter::Stock_Factory::_narrow (object_act.in ());

    // Put the object reference as an IOR string
    CORBA::String_var ior = orb->object_to_string (stock_factory.in ());

    // Output the IOR to the <ior_output_file>
    FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
    if (output_file == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file for writing IOR: %s\n",
                          ior_output_file),
                          1);
    ACE_OS::fprintf (output_file, "%s", ior.in ());
    ACE_OS::fclose (output_file);

    ACE_Time_Value timeout (15);
    orb->run (timeout);

    // Destroy the POA, waiting until the destruction terminates
    root_poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception & e) {
    cerr << "CORBA exception raised: " << e << endl;
  }
  return 0;
}
