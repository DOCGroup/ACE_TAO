// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

ACE_RCSID(Hello, client, "$Id$")

//const char *ior = "file://test.ior";
TAO_IOP::TAO_IOR_Manipulation_var iorm = 0;
CORBA::ULong number_of_servers = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

CORBA::Object_ptr
make_iogr (const char* domain_id, CORBA::ULongLong group_id, CORBA::ULong group_version, Test::Hello_ptr* refs ACE_ENV_ARG_DECL)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (number_of_servers);
  iors.length(number_of_servers);
  for (CORBA::ULong i = 0; i < number_of_servers; ++i)
    {
      iors [i] = CORBA::Object::_duplicate (refs[i]);
    }

  CORBA::Object_var new_ref =
    iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

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
                                              new_ref.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  refs[0],
                                  new_ref.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return new_ref._retn ();
}

int
main (int argc, char *argv[])
{
  CORBA::Boolean result = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get a ref to the IORManipulation object
      CORBA::Object_var IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow
      iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_ptr *servers = new Test::Hello_ptr [number_of_servers];

      for (CORBA::ULong i = 0; i < number_of_servers; ++ i)
        {
          char buf[4]; // if you run more than 10000 servers then you need your head looking at
          const char *number = ACE_OS::itoa ((int) i, buf, 10);
          ACE_CString ior_file ("file://n");
          const char *ior = ((ior_file += number) += ".ior").c_str ();

          CORBA::Object_var tmp =
            orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          servers[i] =
            Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (servers[i]))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                  "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                                  ior),
                                  1);
            }
        }

      CORBA::Object_var iogr = make_iogr ("Domain_1", 1, 1, servers  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello_iogr = Test::Hello::_narrow(iogr.in () ACE_ENV_ARG_PARAMETER);

      CORBA::ULong last_server = 0;

      ACE_TRY
        {
          last_server = hello_iogr->drop_down_dead (ACE_ENV_SINGLE_ARG_PARAMETER);
          // If the call 'succeeds' the server has identified a regression.
          result = 1;
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified by server %u. Test Failed !!\n", last_server));
        }
      ACE_CATCH (CORBA::COMM_FAILURE, my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          // We can't use the word exception for fear of upsetting the build log parser
          ACE_DEBUG ((LM_DEBUG, "Client caught one of those things that is normally used to indicate a problem ("
                      "although it doesn't in this case) and which we cannot name because the autobuild "
                      "script will think we have a problem if we do mention the word. No problem !\n"));
        }
      ACE_ENDTRY;

      for (CORBA::ULong j = last_server; j < number_of_servers; ++j)
        {
          ACE_TRY
            {
              servers[j]->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCHALL
            {
              // Well we tried...
            }
          ACE_ENDTRY;

          CORBA::release (servers [j]);
        }

      for (CORBA::ULong k = 0; k < last_server; ++k)
        {
          CORBA::release (servers [k]);
        }

      delete [] servers;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }
  ACE_ENDTRY;

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
