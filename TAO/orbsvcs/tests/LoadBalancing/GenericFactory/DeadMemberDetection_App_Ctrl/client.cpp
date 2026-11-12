#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_unistd.h"
#include <array>
#include <cstddef>
#include <string>

struct Client_Options
{
  // Command-line values borrow argv storage; defaults are string literals.
  ACE_TCHAR const *group_ior { ACE_TEXT ("file://group.ior") };
  ACE_TCHAR const *direct_obj_ior { ACE_TEXT ("file://direct_obj2.ior") };
  ACE_TCHAR const *strategy { ACE_TEXT ("roundrobin") };
  bool ping_timedout {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:j:ls:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.group_ior = get_opts.opt_arg ();
        break;
      case 'j':
        options.direct_obj_ior = get_opts.opt_arg ();
        break;
      case 'l':
        options.ping_timedout = true;
        break;
      case 's':
        options.strategy = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -j <direct ior> "
                                     "-l -s <strategy>\n"),
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

static int
dead_member_rr_test (CORBA::ORB_ptr orb,
                     Client_Options const &options,
                     Test::Basic_ptr direct_basic)
{
  int status {};

  for (int iter {}; iter < 4; ++iter)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request %d\n"), iter));
      if (iter != 1)
      {
        CORBA::Object_var tmp =
          orb->string_to_object (options.group_ior);

        Test::Basic_var basic =
          Test::Basic::_narrow (tmp.in ());

        if (CORBA::is_nil (basic.in ()))
          {
            ACE_ERROR ((LM_DEBUG,
                        ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                        options.group_ior));
            return 1;
          }

        CORBA::String_var the_string =
          basic->get_string ();
        std::string const location (the_string.in ());

        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request handled by object at <%C>\n"),
                    location.c_str ()));

        if ((iter == 0 && location != "MyLocation 1")
            || (iter == 2 && location != "MyLocation 3"))
        {
          status = 1;
        }

        basic->exit ();
      }
      else
        direct_basic->exit ();

      if (iter == 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not complete.\n"), iter));
        status = 1;
      }

      // Delay more than LB validate member interval so
      // the server shutdown can be detected by LB.
      ACE_OS::sleep (10);
    }
    catch (const ::CORBA::OBJECT_NOT_EXIST& /*ex*/)
    {
      // When no member available, the LB service raise OBJECT_NOT_EXIST exception.
      if (iter < 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("OBJECT_NOT_EXIST exception\n"), iter));
        status = 1;
      }
    }
  }

  return status;
}

static int
hang_member_rr_test (CORBA::ORB_ptr orb,
                     Client_Options const &options,
                     Test::Basic_ptr /*direct_basic*/)
{
  int status {};

  for (int iter {}; iter < 4; ++iter)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request %d\n"), iter));

      CORBA::Object_var tmp =
        orb->string_to_object (options.group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                      options.group_ior));
          return 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();
      std::string const location (the_string.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request handled by object at <%C>\n"),
                  location.c_str ()));

      if ((iter == 0 && location != "MyLocation 1")
          || (iter == 1 && location != "MyLocation 2")
          || (iter == 2 && location != "MyLocation 3"))
      {
        status = 1;
      }

      basic->exit ();

      if (iter == 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not complete.\n"), iter));
        status = 1;
      }

      ACE_OS::sleep (10);
    }
    catch (const ::CORBA::OBJECT_NOT_EXIST& /*ex*/)
    {
      // When no member available, the LB service raise OBJECT_NOT_EXIST exception.
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("OBJECT_NOT_EXIST exception\n"), iter));
        status = 1;
      }
    }
    catch (const ::CORBA::COMM_FAILURE& /*ex*/)
    {
      // It is possible that the LB has not detected the server 2 exit as it hangs
      // and returns the reference to the hang server(server 2) for 4th iteration.
      // So the 4th operation could get the COMM_FAILURE or TRANSIENT exceptions.
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("COMM_FAILURE exception\n"), iter));
        status = 1;
      }
    }
    catch (const ::CORBA::TRANSIENT& /*ex*/)
    {
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("TRANSIENT exception\n"), iter));
        status = 1;
      }
    }
  }

  return status;
}

static int
dead_member_rand_test (CORBA::ORB_ptr orb,
                       Client_Options const &options,
                       Test::Basic_ptr /*direct_basic*/)
{
  int status {};

  std::array<std::string, 4> locations {};
  size_t location_count {};

  for (int iter {}; iter < 4; ++iter)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request %d\n"), iter));

      CORBA::Object_var tmp =
        orb->string_to_object (options.group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                      options.group_ior));
          return 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();
      std::string const location (the_string.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request handled by object at <%C>\n"),
                  location.c_str ()));

      // Any picked member will not be picked again since they exit right after
      // this request.
      for (size_t j {}; j < location_count; ++j)
        if (location == locations[j])
          status = 1;

      if (status == 0)
        locations[location_count++] = location;

      if (iter == 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not complete\n"), iter));
        status = 1;
      }

      basic->exit ();

      ACE_OS::sleep (10);
    }
    catch (const ::CORBA::OBJECT_NOT_EXIST& /*ex*/)
    {
      // Only upon 4th iteration, there is no member available,
      // the LB service raise OBJECT_NOT_EXIST exception.
      if (iter < 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("OBJECT_NOT_EXIST exception \n"), iter));
        status = 1;
      }
    }
  }

  return status;
}

static int
hang_member_rand_test (CORBA::ORB_ptr orb,
                       Client_Options const &options,
                       Test::Basic_ptr /*direct_basic*/)
{
  int status {};

  std::array<std::string, 4> locations {};
  size_t location_count {};

  for (int iter {}; iter < 4; ++iter)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request %d\n"), iter));

      CORBA::Object_var tmp =
        orb->string_to_object (options.group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                      options.group_ior));
          return 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();
      std::string const location (the_string.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)%T - Client request handled by object at <%C>\n"),
                  location.c_str ()));

      for (size_t j {}; j < location_count; ++j)
        if (location == locations[j])
          status = 1;

      if (status == 0)
        locations[location_count++] = location;

      if (iter == 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not complete\n"), iter));
        status = 1;
      }

      basic->exit ();

      ACE_OS::sleep (10);
    }
    catch (const ::CORBA::OBJECT_NOT_EXIST& /*ex*/)
    {
      // When no member available, the LB service raise OBJECT_NOT_EXIST exception.
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("OBJECT_NOT_EXIST exception"), iter));
        // no member available.
        status = 1;
      }
    }
    catch (const ::CORBA::COMM_FAILURE& /*ex*/)
    {
      // It is possible that the LB has not detected the server 2 exit as it hangs
      // and returns the reference to the hang server(server 2) for 4th iteration.
      // So the 4th operation could get the COMM_FAILURE or TRANSIENT exceptions.
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("COMM_FAILURE exception\n"), iter));
        status = 1;
      }
    }
    catch (const ::CORBA::TRANSIENT& /*ex*/)
    {
      if (iter != 3)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Request %d should not catch ")
          ACE_TEXT ("TRANSIENT exception\n"), iter));
        status = 1;
      }
    }
  }

  return status;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  try
    {
      Client_Options options {};
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (options.group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                             options.group_ior),
                            1);
        }

      tmp = orb->string_to_object (options.direct_obj_ior);

      Test::Basic_var direct_basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (direct_basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Basic direct reference <%s>\n"),
                             options.direct_obj_ior),
                            1);
        }

      if (ACE_OS::strcasecmp (options.strategy, ACE_TEXT ("roundrobin")) == 0)
        {
          if (options.ping_timedout)
            status = hang_member_rr_test (orb.in (), options, direct_basic.in ());
          else
            status = dead_member_rr_test (orb.in (), options, direct_basic.in ());
        }
      else if (ACE_OS::strcasecmp (options.strategy, ACE_TEXT ("random")) == 0)
        {
          if (options.ping_timedout)
            status = hang_member_rand_test (orb.in (), options, direct_basic.in ());
          else
            status = dead_member_rand_test (orb.in (), options, direct_basic.in ());
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Invalid strategy\n")));
          status = 1;
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client.cpp:");
      status = 1;
    }

  return status;
}
