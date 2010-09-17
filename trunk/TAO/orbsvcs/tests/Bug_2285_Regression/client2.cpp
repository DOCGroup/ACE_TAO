// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/IORManipulation/IORManip_Loader.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ace/Auto_Ptr.h"



//const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
TAO_IOP::TAO_IOR_Manipulation_var iorm = 0;
CORBA::ULong number_of_servers = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        break;
      case 'n':
        number_of_servers = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

CORBA::Object_ptr
make_iogr (const char* domain_id, CORBA::ULongLong group_id, CORBA::ULong group_version, Test::Hello_var *refs)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (number_of_servers);
  iors.length(number_of_servers);
  for (CORBA::ULong i = 0; i < number_of_servers; ++i)
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
  //const char *id = "iogr_testing";
  ft_tag_component.group_domain_id = domain_id;

  // Object group id
  ft_tag_component.object_group_id = group_id;

  // Version
  ft_tag_component.object_group_ref_version = group_version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                              new_ref.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  refs[0].in (),
                                  new_ref.in ());
    }

  return new_ref._retn ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get a ref to the IORManipulation object
      CORBA::Object_var IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0);

      // Narrow
      iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in());

      Test::Hello_var *servers = 0;
      ACE_NEW_RETURN (servers,
                      Test::Hello_var [number_of_servers],
                      -1);
      ACE_Auto_Array_Ptr<Test::Hello_var> owner (servers);

      for (CORBA::ULong i = 0; i < number_of_servers; ++ i)
        {
          char buf[4]; // if you run more than 10000 servers then you need your head looking at
          const char *number = ACE_OS::itoa ((int) i, buf, 10);
          ACE_CString ior_file ("file://n");
          const char *ior = ((ior_file += number) += ".ior").c_str ();

          CORBA::Object_var tmp =
            orb->string_to_object(ior);

          servers[i] =
            Test::Hello::_narrow(tmp.in ());

          if (CORBA::is_nil (servers[i].in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                  "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                                  ior),
                                  1);
            }
        }

      CORBA::Object_var iogr = make_iogr ("Domain_1", 1, 1, servers);

      Test::Hello_var hello_iogr = Test::Hello::_narrow(iogr.in ());

      CORBA::ULong last_server = 0;

      try
        {
          last_server = hello_iogr->drop_down_dead ();
          // If the call 'succeeds' the server has identified a regression.
          result = 1;
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified by server %u. Test Failed !!\n", last_server));
        }
      catch (const CORBA::COMM_FAILURE& my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          // We can't use the word exception for fear of upsetting the build log parser
          ACE_DEBUG ((LM_DEBUG, "Client caught one of those things that is normally used to indicate a problem ("
                      "although it doesn't in this case) and which we cannot name because the autobuild "
                      "script will think we have a problem if we do mention the word. No problem !\n"));
        }

      for (CORBA::ULong j = last_server; j < number_of_servers; ++j)
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified!!!\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Test passed !!!\n"));
    }
  return result;
}
