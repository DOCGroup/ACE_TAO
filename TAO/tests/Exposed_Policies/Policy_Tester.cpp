// $Id$

// -- App. Specific Include --
#include "Policy_Tester.h"
#include "RT_Properties.h"
#include "CounterC.h"

// -- ACE Include --
#include "ace/Arg_Shifter.h"

// -- RTCORBA Include --
#include "tao/RTCORBA/RT_Policy_i.h"

// -- RTCORBA Include --
#include "tao/RTCORBA/RT_ORB.h"

ACE_RCSID (tao, Policy_Tester, "$Id$")

Policy_Tester::Policy_Tester (void)
  : rt_object_properties_ (0),
    rt_poa_properties_ (0)
{
  // No_Op
}

Policy_Tester::~Policy_Tester (void)
{
  this->shutdown ();
}

void
Policy_Tester::run (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->create_objects (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->child_poa_->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->orb_->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("CORBA Exception Raised."));
    }
  ACE_ENDTRY;
}

int
Policy_Tester::init (int argc,
                     char *argv[],
                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // ORB Initialization.
      this->orb_  = CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get a reference to the RT-ORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB", ACE_TRY_ENV);

      ACE_TRY_CHECK;

      this->rt_orb_ = RTCORBA::RTORB::_narrow (object.in (),
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // Here we parse the command line paramether passed
      // to the application.

      ACE_Arg_Shifter arg_shifter (argc, argv);

      while (arg_shifter.is_anything_left ())
        {
          char *arg;
          // IOR File Name Option.
          if ((arg = arg_shifter.get_the_parameter ("-POAConfigFile")))
            {
              this->rt_poa_properties_ =
                RT_Properties::read_from (arg, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else if ((arg = arg_shifter.get_the_parameter ("-ObjectConfigFile")))
            {
              this->rt_object_properties_ =
                RT_Properties::read_from (arg, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            arg_shifter.consume_arg ();

        }

      if ((this->rt_poa_properties_ == 0) || (this->rt_object_properties_ == 0))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Configuration file missing!")));
          return 0;
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("CORBA Exception Raised."));
      return 0;
    }
  ACE_ENDTRY;
  return 1;
}

CORBA::Boolean
Policy_Tester::check_reference (CORBA::Object_ptr object,
                                const char *msg)
{
  if (CORBA::is_nil (object))
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (msg)));
      return 0;
    }
  return 1;
}


int
Policy_Tester::create_objects (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (3);

      // Create the priority policy using the RT-ORB.
      RTCORBA::Priority priority = this->rt_poa_properties_->priority ();
      poa_policy_list[0] =
        this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                     priority,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create priority Banded Connection Policy.
      RTCORBA::PriorityBands poa_priority_bands =
        this->rt_poa_properties_->priority_bands ();

      poa_policy_list[1] =
        this->rt_orb_->create_priority_banded_connection_policy (poa_priority_bands,
                                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Client Protocol Policy.
      RTCORBA::ProtocolList protocol_list;
      protocol_list.length (1);

      protocol_list[0].protocol_type = IOP::TAG_INTERNET_IOP;
      protocol_list[0].orb_protocol_properties =
        TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::TAG_INTERNET_IOP);

      protocol_list[0].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::TAG_INTERNET_IOP);

      poa_policy_list[2] =
        this->rt_orb_->create_client_protocol_policy (protocol_list);

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->poa_ =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_mgr =
        PortableServer::POAManager::_nil ();

      object =
        this->poa_->create_POA ("Child_POA",
                                poa_mgr.in (),
                                poa_policy_list);

      this->child_poa_ =
        RTPortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create a Corba Object reference, using the policies
      // set at the POA level.
      object =
        this->child_poa_->create_reference ("IDL:Counter:1.0",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reference Created!\n")));

      if (!check_reference (object.in (),
                            "Unable to create Object!\n"))
        return 0;

      Counter_var base_object = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!check_reference (base_object.in(),
                            "Unable to create a Object!\n"))
        return 0;

      CORBA::String_var ior =
        this->orb_->object_to_string (base_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Activated as <%s>\n"), ior.in ()));

      FILE *output_file = ACE_OS::fopen (this->rt_poa_properties_->ior_source (), "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           this->rt_poa_properties_->ior_source ()),
                          0);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Now we create an object that overrides some of the policies
      // set at the POA level.


      // Create a Corba Object reference, using the policies
      // set at the POA level.

      object =
        this->child_poa_->create_reference_with_priority
        ("IDL:Counter:1.0",
         this->rt_object_properties_->priority (),
         ACE_TRY_ENV);

      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reference Created!\n")));

      if (!check_reference (object.in (),
                            "Unable to create a Counter Object!\n"))
        return 0;

      Counter_var over_object = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!check_reference (over_object.in(),
                            "Unable to create Object!\n"))
        return 0;


      CORBA::String_var o_ior = this->orb_->object_to_string (over_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Activated as <%s>\n"), o_ior.in ()));

      output_file = ACE_OS::fopen (this->rt_object_properties_->ior_source (), "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           this->rt_object_properties_->ior_source ()),
                          0);
      ACE_OS::fprintf (output_file, "%s", o_ior.in ());
      ACE_OS::fclose (output_file);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("CORBA Exception Raised."));
      return 0;
    }
  ACE_ENDTRY;
  return 1;
}

void
Policy_Tester::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
  ACE_CHECK;

  this->child_poa_->destroy (1, 1, ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_->destroy (1, 1, ACE_TRY_ENV);
  ACE_CHECK;
}


