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
}

void
Policy_Tester::run (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POAManager_var poa_manager =
    this->child_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Policy_Tester::init (int argc,
                     char *argv[]
                     ACE_ENV_ARG_DECL)
{
  // ORB Initialization.
  this->orb_ =
    CORBA::ORB_init (argc, argv, ""
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get a reference to the RT-ORB.
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->rt_orb_ = RTCORBA::RTORB::_narrow (object.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Here we parse the command line paramether passed
  // to the application.

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = 0;
      // IOR File Name Option.
      if ((arg = arg_shifter.get_the_parameter ("-POAConfigFile")))
        {
          this->rt_poa_properties_ =
            RT_Properties::read_from (arg ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }
      else if ((arg = arg_shifter.get_the_parameter ("-ObjectConfigFile")))
        {
          this->rt_object_properties_ =
            RT_Properties::read_from (arg ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }
      else if ((arg = arg_shifter.get_the_parameter ("-BaseObjectIOR")))
        {
          if (this->rt_poa_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_poa_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                                  CORBA::COMPLETED_NO));
              ACE_CHECK_RETURN (-1);
            }
          this->rt_poa_properties_->ior_source (arg);
        }
      else if ((arg = arg_shifter.get_the_parameter ("-OverriddenIOR")))
        {
          if (this->rt_object_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_object_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                                  CORBA::COMPLETED_NO));
              ACE_CHECK_RETURN (-1);
            }
          this->rt_object_properties_->ior_source (arg);
        }
      else
        arg_shifter.consume_arg ();
    }

  if ((this->rt_poa_properties_ == 0) || (this->rt_object_properties_ == 0))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Configuration file missing!")));
      return -1;
    }

  int result =
    this->create_objects (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result != 0)
    return result;

  return 0;
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
Policy_Tester::create_objects (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::PolicyList poa_policy_list;
  poa_policy_list.length (3);

  // Create the priority policy using the RT-ORB.
  RTCORBA::Priority priority = this->rt_poa_properties_->priority ();
  poa_policy_list[0] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                 priority
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

      // Create priority Banded Connection Policy.
  RTCORBA::PriorityBands poa_priority_bands =
    this->rt_poa_properties_->priority_bands ();

  poa_policy_list[1] =
    this->rt_orb_->create_priority_banded_connection_policy (poa_priority_bands
                                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
    this->orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->poa_ =
    PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_mgr =
    PortableServer::POAManager::_nil ();

  object =
    this->poa_->create_POA ("Child_POA",
                            poa_mgr.in (),
                            poa_policy_list
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->child_poa_ =
    RTPortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create a Corba Object reference, using the policies
  // set at the POA level.
  object =
    this->child_poa_->create_reference ("IDL:Counter:1.0"
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference Created!\n")));

  if (!check_reference (object.in (),
                        "Unable to create Object!\n"))
    return -1;

  Counter_var base_object = Counter::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (!check_reference (base_object.in(),
                        "Unable to create a Object!\n"))
    return -1;

  CORBA::String_var ior =
    this->orb_->object_to_string (base_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%s>\n"), ior.in ()));

  FILE *output_file = ACE_OS::fopen (this->rt_poa_properties_->ior_source (), "w");
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       this->rt_poa_properties_->ior_source ()),
                      -1);
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);

  // Now we create an object that overrides some of the policies
  // set at the POA level.


  // Create a Corba Object reference, using the policies
  // set at the POA level.

  object =
    this->child_poa_->create_reference_with_priority
    ("IDL:Counter:1.0",
     this->rt_object_properties_->priority ()
     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference Created!\n")));

  if (!check_reference (object.in (),
                        "Unable to create a Counter Object!\n"))
    return -1;

  Counter_var over_object = Counter::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (!check_reference (over_object.in(),
                        "Unable to create Object!\n"))
    return -1;


  CORBA::String_var o_ior =
    this->orb_->object_to_string (over_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%s>\n"), o_ior.in ()));

  output_file = ACE_OS::fopen (this->rt_object_properties_->ior_source (), "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       this->rt_object_properties_->ior_source ()),
                      -1);
  ACE_OS::fprintf (output_file, "%s", o_ior.in ());
  ACE_OS::fclose (output_file);
  return 0;
}

void
Policy_Tester::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
