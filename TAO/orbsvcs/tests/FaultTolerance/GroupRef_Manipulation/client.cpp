// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "testC.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include <cstring>

struct Client_Options
{
  ACE_TString ior {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "Client ORB");
      Client_Options options {};

      if (parse_args (argc, argv, options) != 0 || options.ior.empty ())
        return -1;

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.
      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

      ForwardRequestTest::test_var const server =
        ForwardRequestTest::test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Object reference is nil\n"), 1);

      CORBA::String_var const marshaled_obj_snapshot1 =
        orb->object_to_string (server.in ());

      CORBA::ULong number {};
      for (CORBA::ULong i {1}; i <= 25; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request " ACE_UINT32_FORMAT_SPECIFIER ".\n",
                      i));

          number += server->number ();

          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Number " ACE_UINT32_FORMAT_SPECIFIER ".\n",
                      number));
        }

      if (number < 250)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Did not forward to new location\n"));
          ACE_OS::abort ();
        }

      // One of the request triggerd a LOCATION_FORWARD_PERM, in
      // consequence the marshaled representation of "server" should
      // look different now, compare to snapshot1.
      CORBA::String_var const marshaled_obj_snapshot2 =
        orb->object_to_string (server.in ());

      if (std::strcmp (marshaled_obj_snapshot1.in (),
                       marshaled_obj_snapshot2.in ()) == 0)
        {
          // Error, before and after the marhaled object references look equal
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Marshaled Object reference should differ after LOCATION_FORWARD_PERM\n"));
          ACE_OS::abort ();
        }

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  ACE_DEBUG ((LM_INFO,
              "Group_Ref_Manip Test passed.\n"));

  return 0;
}
