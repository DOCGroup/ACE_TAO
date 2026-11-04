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
#include <memory>

void
Policy_Tester::run ()
{
  PortableServer::POAManager_var const poa_manager =
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
      ACE_TCHAR const *arg {};
      // IOR File Name Option.
      if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-POAConfigFile"))))
        {
          this->rt_poa_properties_ =
            RT_Properties::read_from (ACE_TString (arg));
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-ObjectConfigFile"))))
        {
          this->rt_object_properties_ =
            RT_Properties::read_from (ACE_TString (arg));
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-BaseObjectIOR"))))
        {
          if (!this->rt_poa_properties_)
            {
              this->rt_poa_properties_ = RT_Properties::create ();
            }
          this->rt_poa_properties_->ior_source (ACE_TString (arg));
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-OverriddenIOR"))))
        {
          if (!this->rt_object_properties_)
            {
              this->rt_object_properties_ = RT_Properties::create ();
            }
          this->rt_object_properties_->ior_source (ACE_TString (arg));
        }
      else
        arg_shifter.consume_arg ();
    }

  if (!this->rt_poa_properties_ || !this->rt_object_properties_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Configuration file missing!")));
      return -1;
    }

  int const result = this->create_objects ();

  if (result != 0)
    return result;

  return 0;
}

CORBA::Boolean
Policy_Tester::check_reference (CORBA::Object_ptr object,
                                char const *msg)
{
  if (CORBA::is_nil (object))
    {
      ACE_DEBUG ((LM_DEBUG, msg));
      return false;
    }
  return true;
}


int
Policy_Tester::create_objects ()
{
  CORBA::PolicyList poa_policy_list {};
  poa_policy_list.length (3);

  // Create the priority policy using the RT-ORB.
  RTCORBA::Priority const priority = this->rt_poa_properties_->priority ();
  poa_policy_list[0] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                 priority);

  // Create priority Banded Connection Policy.
  RTCORBA::PriorityBands const poa_priority_bands =
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

  PortableServer::POAManager_var const poa_mgr {};

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

  Counter_var const base_object = Counter::_narrow (object.in ());

  if (!check_reference (base_object.in(),
                        "Unable to create a Object!\n"))
    return -1;

  CORBA::String_var const ior =
    this->orb_->object_to_string (base_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

  {
    std::unique_ptr<FILE, int (*) (FILE *)> output_file (
      ACE_OS::fopen (this->rt_poa_properties_->ior_source ().c_str (), ACE_TEXT ("w")),
      ACE_OS::fclose);
    if (!output_file)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                         this->rt_poa_properties_->ior_source ().c_str ()),
                        -1);
    ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  }

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

  Counter_var const over_object = Counter::_narrow (object.in ());

  if (!check_reference (over_object.in(),
                        "Unable to create Object!\n"))
    return -1;


  CORBA::String_var const o_ior =
    this->orb_->object_to_string (over_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activated as <%C>\n"), o_ior.in ()));

  {
    std::unique_ptr<FILE, int (*) (FILE *)> output_file (
      ACE_OS::fopen (this->rt_object_properties_->ior_source ().c_str (), ACE_TEXT ("w")),
      ACE_OS::fclose);
    if (!output_file)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                         this->rt_object_properties_->ior_source ().c_str ()),
                        -1);
    ACE_OS::fprintf (output_file.get (), "%s", o_ior.in ());
  }
  return 0;
}

void
Policy_Tester::shutdown ()
{
  this->orb_->shutdown (false);
}
