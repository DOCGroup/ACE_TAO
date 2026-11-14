#include "Test_i.h"
#include "ORB_DLL.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

Client_Worker::Client_Worker ()
  : Abstract_Worker (ACE_TEXT ("file://test.ior"))
{
}

ACE_TCHAR const *
Client_Worker::kind () const
{
  return ACE_TEXT ("Client");
}

Client_Worker::~Client_Worker ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %@ Client::<dtor>\n", this));
}

int
Client_Worker::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"), 0);

  int option {};
  while ((option = get_opts ()) != -1)
    if (option == 'k')
      this->ior_file_ = get_opts.opt_arg ();

  // Indicates successful parsing of the command line
  return 0;
}

int
Client_Worker::test_main (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (this->parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%P|%t) Could not parse the arguments\n")),
                          1);

      // Retry while the server is starting or has not published its IOR.
      bool success {};
      for (int attempts_left = 5; attempts_left > 0 && !success; --attempts_left)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Client is ready to proceed - awaiting the server ...\n")));
          ACE_OS::sleep (1);

          try
            {
              CORBA::Object_var const object = orb->string_to_object (this->ior_file_.c_str ());

              if (CORBA::is_nil (object.in ()))
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) Unable to obtain object reference yet. Retrying.\n")));
                  continue;
                }
              Test::Hello_var const hello =
                Test::Hello::_narrow (object.in ());

              if (CORBA::is_nil (hello.in ()))
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) Nil Test::Hello reference <%s>. Retrying.\n"),
                              this->ior_file_.c_str ()));
                  continue;
                }

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Successfully narrowed the Hello interface\n")));

              CORBA::String_var const the_string =
                hello->get_string ();

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) String returned from the server <%C>\n"),
                          the_string.in ()));

              hello->shutdown ();
              success = true;
            }
          catch (CORBA::TRANSIENT const& ex)
            {
              if (attempts_left == 1)
                throw;

              ex._tao_print_exception ("Temporary problem encountered");

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Client was too quick. Pausing ")
                          ACE_TEXT ("while the server gets ready.\n")));
              ACE_OS::sleep (5);
            }
          catch (CORBA::Exception const& ex)
            {
              ex._tao_print_exception ("Unexpected CORBA exception caught");

              if (attempts_left == 1)
                throw;

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Pausing for a while.\n")));
              ACE_OS::sleep (5);
            }
        }

      if (!success)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Client failed to contact the server\n")),
                          1);

      orb->shutdown (false);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Client_Worker::test_main");
      return 1;
    }

  return 0;
}
