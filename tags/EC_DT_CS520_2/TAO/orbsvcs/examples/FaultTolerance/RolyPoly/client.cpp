// file      : RolyPoly/client.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ace/Get_Opt.h"

// IOR manipulation.
#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

#include "RolyPolyC.h"

const char *ior1 = 0;
const char *ior2 = 0;


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
    case 'k':
      if (ior1 == 0) ior1 = get_opts.opt_arg ();
      else ior2 = get_opts.opt_arg ();
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Usage:  %s "
                         "-k IOR_1 -k IOR_2\n",
                         argv[0]),
                        -1);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  int status = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0) return -1;

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.

      CORBA::Object_var object;

      if (ior2 != 0)
      {
        // merge case

        CORBA::Object_var object_primary;
        CORBA::Object_var object_secondary;

        object_primary = orb->string_to_object (
          ior1 ACE_ENV_ARG_PARAMETER);

        ACE_CHECK_RETURN (-1);

        object_secondary = orb->string_to_object (
          ior2 ACE_ENV_ARG_PARAMETER);

        ACE_CHECK_RETURN (-1);

        // Get an object reference for the ORBs IORManipultion object!
        CORBA::Object_ptr IORM =
          orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                           0
                                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);

        TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
          TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);


        // Create the list
        TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
        iors.length(2);
        iors [0] = CORBA::Object::_duplicate (object_primary.in ());
        iors [1] = CORBA::Object::_duplicate (object_secondary.in ());

        // Create a merged set 1;
        object = iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);

        ACE_CHECK_RETURN (-1);


        FT::TagFTGroupTaggedComponent ft_tag_component;

        // Property values

        // Major and Minor revision numbers
        ft_tag_component.object_group_ref_version = 0;

        // Domain id
        const char *id = "iogr_testing";
        ft_tag_component.group_domain_id = id;

        // Object group id
        ft_tag_component.object_group_id =
          (CORBA::ULongLong) 10;

        // Version
        ft_tag_component.object_group_ref_version =
          (CORBA::ULong) 5;

        // Construct the IOGR Property class
        TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

        // Set the property
        CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                                    object.in ()
                                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);

        retval = iorm->set_primary (&iogr_prop,
                                    object_primary.in (),
                                    object.in ()
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);

      }
      else
      {
        object = orb->string_to_object (ior1 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      RolyPoly_var server =
        RolyPoly::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference is nil\n"),
                          1);
      }

      CORBA::Short number = 0;


      for (int i = 1; i < 500; ++i)
      {
        CORBA::String_var str;

        try
        {
          number = server->number (str.inout ()
                                   ACE_ENV_ARG_PARAMETER);
        }
        catch (RolyPoly::E const& e)
        {
          ACE_DEBUG ((LM_INFO,
                      "client: received exception %s .\n",
                      e.s.in ()));
          continue;
        }

        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_INFO,
                    "client: received %d\n",
                    number));
        sleep (1);
      }

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return status;
}
