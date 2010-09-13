// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"

const ACE_TCHAR *group_file = ACE_TEXT("group.ior");
const ACE_TCHAR *group_ior = ACE_TEXT("file://group.ior");
const ACE_TCHAR *direct_obj_file = ACE_TEXT("direct_obj2.ior");
const ACE_TCHAR *direct_obj_ior = ACE_TEXT("file://direct_obj2.ior");
const ACE_TCHAR *strategy = ACE_TEXT("roundrobin");

bool ping_timedout2 = false;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:j:ls:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        group_ior = get_opts.opt_arg ();
        break;
      case 'j':
        direct_obj_ior = get_opts.opt_arg ();
        break;
      case 'l':
        ping_timedout2 = true;
        break;
      case 's':
        strategy = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> -j <direct ior> -l -s <strategy>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}



int dead_member_rr_test (CORBA::ORB_ptr orb,
                         const ACE_TCHAR *group_ior,
                         Test::Basic_ptr direct_basic)
{
  int status = 0;

  for (int iter = 0; iter < 4; iter++)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request %d \n", iter));
      if (iter != 1)
      {
        CORBA::Object_var tmp =
          orb->string_to_object (group_ior);

        Test::Basic_var basic =
          Test::Basic::_narrow (tmp.in ());

        if (CORBA::is_nil (basic.in ()))
          {
            ACE_ERROR ((LM_DEBUG,
                              "Nil Test::Basic reference <%s>\n",
                              group_ior));
            status = 1;
          }

        CORBA::String_var the_string =
          basic->get_string ();

        ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request handled by object at <%s>\n",
          the_string.in ()));

        if ((iter == 0 && ACE_OS::strcmp (the_string.in(), "MyLocation 1"))
          || (iter == 2 && ACE_OS::strcmp (the_string.in(), "MyLocation 3")))
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
    catch (...)
    {
      throw;
    }
  }

  return status;
}


int hang_member_rr_test (CORBA::ORB_ptr orb,
                         const ACE_TCHAR *group_ior,
                         Test::Basic_ptr /*direct_basic*/)
{
  int status = 0;

  for (int iter = 0; iter < 4; iter++)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request %d \n", iter));

      CORBA::Object_var tmp =
        orb->string_to_object (group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             group_ior));
          status = 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request handled by object at <%s>\n",
        the_string.in ()));

      if ((iter == 0 && ACE_OS::strcmp (the_string.in(), "MyLocation 1"))
        || (iter == 1 && ACE_OS::strcmp (the_string.in(), "MyLocation 2"))
        || (iter == 2 && ACE_OS::strcmp (the_string.in(), "MyLocation 3")))
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
    catch (...)
    {
      throw;
    }
  }

  return status;
}


int dead_member_rand_test (CORBA::ORB_ptr orb,
                           const ACE_TCHAR *group_ior,
                           Test::Basic_ptr /*direct_basic*/)
{
  int status = 0;

  ACE_Vector <ACE_CString> locations;

  for (int iter = 0; iter < 4; iter ++)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request %d \n", iter));

      CORBA::Object_var tmp =
        orb->string_to_object (group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             group_ior));
          status = 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request handled by object at <%s>\n",
        the_string.in ()));

      // Any picked member will not be picked again since they exit right after
      // this request.
      for (size_t j = 0; j < locations.size (); ++j)
      {
        if (ACE_OS::strcmp (the_string.in (), locations[j].c_str ()) == 0)
        {
          status = 1;
        }
      }

      if (status == 0)
        locations.push_back (the_string.in ());

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
    catch (...)
    {
      throw;
    }
  }

  return status;
}


int hang_member_rand_test (CORBA::ORB_ptr orb,
                           const ACE_TCHAR *group_ior,
                           Test::Basic_ptr /*direct_basic*/)
{
  int status = 0;

  ACE_Vector <ACE_CString> locations;

  for (int iter = 0; iter < 4; iter++)
  {
    try {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request %d \n", iter));

      CORBA::Object_var tmp =
        orb->string_to_object (group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             group_ior));
          status = 1;
        }

      CORBA::String_var the_string =
        basic->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Client request handled by object at <%s>\n",
        the_string.in ()));

      for (size_t j = 0; j < locations.size (); ++j)
      {
        if (ACE_OS::strcmp (the_string.in (), locations[j].c_str ()) == 0)
        {
          status = 1;
        }
      }

      if (status == 0)
        locations.push_back (the_string.in ());

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
    catch (...)
    {
      throw;
    }
  }

  return status;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (group_ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             group_ior),
                            1);
        }

      tmp = orb->string_to_object (direct_obj_ior);

      Test::Basic_var direct_basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (direct_basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Basic direct reference <%s>\n",
                             direct_obj_ior),
                            1);
        }

      if (ACE_OS::strcasecmp (strategy, ACE_TEXT("roundrobin")) == 0)
      {
        if (ping_timedout2)
          status = hang_member_rr_test (orb.in (), group_ior, direct_basic.in ());
        else
          status = dead_member_rr_test (orb.in (), group_ior, direct_basic.in ());
      }
      else if (ACE_OS::strcasecmp (strategy, ACE_TEXT("random")) == 0)
        if (ping_timedout2)
          status = hang_member_rand_test (orb.in (), group_ior, direct_basic.in ());
        else
          status = dead_member_rand_test (orb.in (), group_ior, direct_basic.in ());
      else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Invalid strategy \n")));
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
