// $Id$

#include "Test_i.h"
#include "ORB_DLL.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_unistd.h"

Client_Worker::Client_Worker ()
  : Abstract_Worker (ACE_TEXT ("file://test.ior"))
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %@ Client::<ctor>\n", this));
}

const ACE_TCHAR *
Client_Worker::kind (void) const
{
  return ACE_TEXT ("Client");
}

Client_Worker::~Client_Worker (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %@ Client::<dtor>\n", this));
}

int
Client_Worker::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"),0);

  for( int c = 0; ((c = get_opts ()) != -1); )
    switch (c)
    {
    case 'k':
      this->ior_file_ = get_opts.opt_arg ();
      break;
    }

  // Indicates successful parsing of the command line
  return 0;
}

int
Client_Worker::test_main (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  if (parse_args (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("(%P|%t) Could not parse the arguments\n")),
                      1);

  // Doing this dance to allow the server some time to come up.
  CORBA::Object_ptr co = 0;
  for (int attempts_left=5; attempts_left > 0; --attempts_left)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Client is ready to proceed - awaiting the server ...\n")));
    ACE_OS::sleep (1);

    try
    {
      co = orb->string_to_object (ior_file_.c_str ());

      if (co == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Unable to obtain object reference yet. Retrying.\n")));
          continue;
        }
      CORBA::Object_var tmp (co);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Nil Test::Hello reference <%s>. Retrying.\n"),
                      ior_file_.c_str ()));
          continue;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Successfuly narrowed the Hello interface\n")));

      CORBA::String_var the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) String returned from the server <%C>\n"),
                  the_string.in ()));

      hello->shutdown ();

      attempts_left = 0; // We're done here!

    }
    catch (const CORBA::TRANSIENT& ex)
    {
      if (!attempts_left)
        throw;

      ex._tao_print_exception ("Temporary problem encountered");

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client was too quick. Pausing ")
                  ACE_TEXT ("while the server gets ready.\n")));
      ACE_OS::sleep (5);
    }
  }

  orb->shutdown (0);

  orb->destroy ();

  return 0;
}
