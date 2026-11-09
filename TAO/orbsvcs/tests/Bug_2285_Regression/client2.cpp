#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/IORManipulation/IORManip_Loader.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include <memory>
#include <string>

struct Client_Options
{
  CORBA::ULong number_of_servers {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'n':
          options.number_of_servers = static_cast<CORBA::ULong> (ACE_OS::atoi (get_opts.opt_arg ()));
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -n <number of servers>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

CORBA::Object_ptr
make_iogr (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
           char const *domain_id,
           CORBA::ULongLong group_id,
           CORBA::ULong group_version,
           Test::Hello_var const *refs,
           CORBA::ULong number_of_servers)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (number_of_servers);
  iors.length (number_of_servers);
  for (CORBA::ULong i {}; i < number_of_servers; ++i)
    {
      iors [i] = CORBA::Object::_duplicate (refs[i].in ());
    }

  CORBA::Object_var new_ref =
    iorm->merge_iors (iors);

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = (CORBA::Octet) 1;
  ft_tag_component.component_version.minor = (CORBA::Octet) 0;

  // Domain id
  ft_tag_component.group_domain_id = domain_id;

  // Object group id
  ft_tag_component.object_group_id = group_id;

  // Version
  ft_tag_component.object_group_ref_version = group_version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean const has_property = iorm->set_property (&iogr_prop, new_ref.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (has_property)
    {
      iorm->set_primary (&iogr_prop, refs[0].in (), new_ref.in ());
    }

  return new_ref._retn ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // Get a ref to the IORManipulation object
      CORBA::Object_var const iorm_object =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0);

      // Narrow
      TAO_IOP::TAO_IOR_Manipulation_var const iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

      Test::Hello_var *servers {};
      ACE_NEW_RETURN (servers,
                      Test::Hello_var [options.number_of_servers],
                      -1);
      // ACE_NEW_RETURN needs a raw pointer; unique_ptr owns the allocated array.
      std::unique_ptr<Test::Hello_var[]> owner (servers);

      for (CORBA::ULong i {}; i < options.number_of_servers; ++i)
        {
          std::string const ior_file =
            "file://n" + std::to_string (i) + ".ior";

          CORBA::Object_var const tmp =
            orb->string_to_object (ior_file.c_str ());

          servers[i] =
            Test::Hello::_narrow (tmp.in ());

          if (CORBA::is_nil (servers[i].in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                  ACE_TEXT ("Test failed - Not regression - Unexpected Nil "
                                            "Test::Hello reference <%s>\n"),
                                  ior_file.c_str ()),
                                1);
            }
        }

      CORBA::Object_var const iogr = make_iogr (
        iorm.in (), "Domain_1", 1, 1, servers, options.number_of_servers);

      Test::Hello_var const hello_iogr = Test::Hello::_narrow (iogr.in ());

      CORBA::ULong last_server {};

      try
        {
          last_server = hello_iogr->drop_down_dead ();
          // If the call 'succeeds' the server has identified a regression.
          result = true;
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Error: REGRESSION identified by server %u. Test Failed !!\n"),
                      static_cast<unsigned int> (last_server)));
        }
      catch (CORBA::COMM_FAILURE const &)
        {
          // We can't use the word exception for fear of upsetting the build log parser
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client caught one of those things that is normally used to indicate a problem ("
                                "although it doesn't in this case) and which we cannot name because the autobuild "
                                "script will think we have a problem if we do mention the word. No problem !\n")));
        }

      for (CORBA::ULong j = last_server; j < options.number_of_servers; ++j)
        {
          try
            {
              servers[j]->shutdown ();
            }
          catch (...)
            {
              // Well we tried...
            }
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Error: REGRESSION identified!!!\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test passed !!!\n")));
    }
  return result;
}
