// file      : RolyPoly/client.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"
#include "ace/Unbounded_Queue.h"

// IOR manipulation.
#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

#include "RolyPolyC.h"

typedef ACE_Unbounded_Queue<ACE_SString> IOR_QUEUE;

IOR_QUEUE ior_strs;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
    case 'k':
      {
      ACE_SString ior(get_opts.opt_arg ());
      if (ior_strs.enqueue_tail (ior) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to enqueue IOR: %s\n",
                           ior.c_str ()),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG, "Enqueued IOR: %s\n", ior.c_str ()));
      }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Usage:  %s "
                         "-k IOR ...\n",
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

      if (ior_strs.is_empty ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "No IOR provided\n"),
                          -1);

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.

      CORBA::Object_var object;

      if (ior_strs.size() > 1)
      {
        // merge case

        ACE_DEBUG ((LM_DEBUG, "We got %d iors\n", ior_strs.size ()));
        IOR_QUEUE::ITERATOR ior_go = ior_strs.begin ();
        ACE_SString *pior = 0;
         while (ior_go.next (pior) != 0)
           {
             ACE_DEBUG ((LM_DEBUG, "IOR: %s\n", pior->c_str ()));
             ior_go.advance ();
           }


        IOR_QUEUE::ITERATOR ior_iter = ior_strs.begin ();

        ACE_SString *ior = 0;
        ior_iter.next (ior);

        CORBA::Object_var object_primary;

        object_primary =
          orb->string_to_object (ior->c_str() ACE_ENV_ARG_PARAMETER);

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
        TAO_IOP::TAO_IOR_Manipulation::IORList iors (ior_strs.size ());
        iors.length(ior_strs.size ());
        size_t cntr = 0;
        iors [cntr] = CORBA::Object::_duplicate (object_primary.in ());
        
        while (ior_iter.advance ())
          {
            ++cntr;
            ior_iter.next (ior);
            ACE_DEBUG ((LM_DEBUG, "IOR%d: %s\n",cntr, ior->c_str ()));
            iors [cntr] =
              orb->string_to_object (ior->c_str() ACE_ENV_ARG_PARAMETER);

            ACE_CHECK_RETURN (-1);
          }

        ACE_DEBUG ((LM_DEBUG, "Prepare to merge IORs.\n"));
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
        ACE_SString *ior = 0;
        if (ior_strs.get (ior) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to extract the only IOR string\n"),
                            -1);
        object = orb->string_to_object (ior->c_str() ACE_ENV_ARG_PARAMETER);
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
        ACE_OS::sleep (1);
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
