// $Id$

// -- App. Specific Include --
#include "Policy_Tester.h"
#include "RT_Properties.h"
#include "CounterC.h"

// -- ACE Include --
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_stdio.h"

// -- RTCORBA Include --
#include "tao/RTCORBA/RT_Policy_i.h"

// -- RTCORBA Include --
#include "tao/RTCORBA/RT_ORB.h"

#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"

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
Policy_Tester::run (void)
{
  PortableServer::POAManager_var poa_manager =
    this->child_poa_->the_POAManager ();

  poa_manager->activate ();

  this->orb_->run ();
}

int
Policy_Tester::init (int argc,
                     ACE_TCHAR *argv[])
{
  // ORB Initialization.
  this->orb_ =
    CORBA::ORB_init (argc, argv);

  // Get a reference to the RT-ORB.
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB");

  this->rt_orb_ = RTCORBA::RTORB::_narrow (object.in ());

  // Here we parse the command line paramether passed
  // to the application.

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = 0;
      // IOR File Name Option.
      if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-POAConfigFile"))))
        {
          this->rt_poa_properties_ =
            RT_Properties::read_from (arg);
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-ObjectConfigFile"))))
        {
          this->rt_object_properties_ =
            RT_Properties::read_from (arg);
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-BaseObjectIOR"))))
        {
          if (this->rt_poa_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_poa_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO::VMCID,
                                                  CORBA::COMPLETED_NO));
            }
          this->rt_poa_properties_->ior_source (arg);
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-OverriddenIOR"))))
        {
          if (this->rt_object_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_object_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO::VMCID,
                                                  CORBA::COMPLETED_NO));
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
    this->create_objects ();

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
      ACE_DEBUG ((LM_DEBUG, msg));
      return 0;
    }
  return 1;
}


int
Policy_Tester::create_objects (void)
{
  CORBA::PolicyList poa_policy_list;
  poa_policy_list.length (3);

  // Create the priority policy using the RT-ORB.
  RTCORBA::Priority priority = this->rt_poa_properties_->priority ();
  poa_policy_list[0] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                 priority);

      // Create priority Banded Connection Policy.
  RTCORBA::PriorityBands poa_priority_bands =
    this->rt_poa_properties_->priority_bands ();

  poa_policy_list[1] =
    this->rt_orb_->create_priority_banded_connection_policy (poa_priority_bands);

      // Client Protocol Policy.
  RTCORBA::ProtocolList protocol_list;
  protocol_list.length (1);

  protocol_list[0].protocol_type = IOP::TAG_INTERNET_IOP;
  protocol_list[0].orb_protocol_properties =
    TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::TAG_INTERNET_IOP);

  protocol_list[0].transport_protocol_properties =
    TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::TAG_INTERNET_IOP,
                                                                         this->orb_->orb_core ());

  poa_policy_list[2] =
    this->rt_orb_->create_client_protocol_policy (protocol_list);

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RootPOA");

  this->poa_ =
    PortableServer::POA::_narrow (object.in ());

  PortableServer::POAManager_var poa_mgr =
    PortableServer::POAManager::_nil ();

  object =
    this->poa_->create_POA ("Child_POA",
                            poa_mgr.in (),
                            poa_policy_list);

  this->child_poa_ =
    RTPortableServer::POA::_narrow (object.in ());

  // Create a Corba Object reference, using the policies
  // set at the POA level.
  object =
    this->child_poa_->create_reference ("IDL:Counter:1.0");

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference Created!\n")));

  if (!check_reference (object.in (),
                        "Unable to create Object!\n"))
    return -1;

  Counter_var base_object = Counter::_narrow (object.in ());

  if (!check_reference (base_object.in(),
                        "Unable to create a Object!\n"))
    return -1;

  CORBA::String_var ior =
    this->orb_->object_to_string (base_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

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
     this->rt_object_properties_->priority ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference Created!\n")));

  if (!check_reference (object.in (),
                        "Unable to create a Counter Object!\n"))
    return -1;

  Counter_var over_object = Counter::_narrow (object.in ());

  if (!check_reference (over_object.in(),
                        "Unable to create Object!\n"))
    return -1;


  CORBA::String_var o_ior =
    this->orb_->object_to_string (over_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%C>\n"), o_ior.in ()));

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
Policy_Tester::shutdown (void)
{
  this->orb_->shutdown (0);
}
