// file      : RolyPoly/client.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

// IOR manipulation.
#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

#include "RolyPolyC.h"

#include <new>
#include <string>
#include <vector>

std::vector<std::string> ior_strs;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
    case 'k':
      {
      try
        {
          std::string const ior (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()));
          ior_strs.push_back (ior);
          ACE_DEBUG ((LM_DEBUG, "Enqueued IOR: %s\n", ior.c_str ()));
        }
      catch (std::bad_alloc const&)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to store IOR\n"),
                            -1);
        }
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB");

      if (::parse_args (argc, argv) != 0) return -1;

      if (ior_strs.empty ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "No IOR provided\n"),
                          -1);

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.

      CORBA::Object_var object;

      if (ior_strs.size () > 1)
      {
        // merge case

        ACE_DEBUG ((LM_DEBUG, "We got %B iors\n", ior_strs.size ()));
        for (std::string const& ior : ior_strs)
          ACE_DEBUG ((LM_DEBUG, "IOR: %s\n", ior.c_str ()));

        CORBA::Object_var object_primary;

        object_primary =
          orb->string_to_object (ior_strs.front ().c_str ());


        // Get an object reference for the ORBs IORManipultion object!
        CORBA::Object_ptr IORM =
          orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                           0);

        TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
          TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM);


        // Create the list
        CORBA::ULong const ior_count = static_cast<CORBA::ULong> (ior_strs.size ());
        TAO_IOP::TAO_IOR_Manipulation::IORList iors (ior_count);
        iors.length (ior_count);
        iors [0] = CORBA::Object::_duplicate (object_primary.in ());

        for (size_t cntr = 1; cntr < ior_strs.size (); ++cntr)
          {
            ACE_DEBUG ((LM_DEBUG, "IOR%B: %s\n", cntr, ior_strs[cntr].c_str ()));
            iors [static_cast<CORBA::ULong> (cntr)] =
              orb->string_to_object (ior_strs[cntr].c_str ());
          }

        ACE_DEBUG ((LM_DEBUG, "Prepare to merge IORs.\n"));
        // Create a merged set 1;
        object = iorm->merge_iors (iors);


        FT::TagFTGroupTaggedComponent ft_tag_component;

        // Property values

        // Major and Minor revision numbers
        ft_tag_component.object_group_ref_version = 0;

        // Domain id
        char const *id = "iogr_testing";
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
                                                    object.in ());

        retval = iorm->set_primary (&iogr_prop,
                                    object_primary.in (),
                                    object.in ());

        ACE_UNUSED_ARG (retval);
      }
      else
      {
        object = orb->string_to_object (ior_strs.front ().c_str ());
      }

      RolyPoly_var server =
        RolyPoly::_narrow (object.in ());

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
          number = server->number (str.inout ());
        }
        catch (RolyPoly::E const& e)
        {
          ACE_DEBUG ((LM_INFO,
                      "client: received exception %s .\n",
                      e.s.in ()));
          continue;
        }


        ACE_DEBUG ((LM_INFO,
                    "client: received %d\n",
                    number));
        ACE_OS::sleep (1);
      }

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return status;
}
