#include "ClientApp.h"
#include "FooC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

int
ClientApp::run (int argc, ACE_TCHAR* argv[])
{
  CORBA::ORB_var const orb
    = CORBA::ORB_init (argc, argv);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int const result = this->parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  CORBA::Object_var const obj
    = orb->string_to_object (this->ior_.c_str ());

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to convert IOR string to obj ref.\n")));
      throw TestException ();
    }

  Foo_var const foo = Foo::_narrow (obj.in ());

  if (CORBA::is_nil (foo.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to narrow obj ref to Foo interface.\n")));
      throw TestException ();
    }

  for (CORBA::Long i = 1; i <= 100; ++i)
    {
      foo->op1 ();
      foo->op2 (i);
      CORBA::Long const value = foo->op3 ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) ===> Value retrieved from op3() == %d\n"),
                 value));

      for (CORBA::Long j = 1; j <= 5; ++j)
        {
          foo->op4 (495 + (i * 5) + j);
        }

      try
        {
          foo->op5 ();
        }
      catch (FooException const &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) ===> Caught FooException - as expected.\n")));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ===> Tell server that we are done().\n")));

  foo->done ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ===> Back from done().\n")));

  return 0;
}

int
ClientApp::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));

  int c {};

  while ((c = get_opts()) != -1)
    {
      switch (c)
        {
        case 'i':
          this->ior_ = get_opts.opt_arg ();
          break;

        case '?':
          this->usage_statement ();
          return 1;
        default:
          this->usage_statement ();
          return -1;
        }
    }

  return 0;
}

void
ClientApp::usage_statement () const
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage:  %s [options]\n\n")
              ACE_TEXT ("OPTIONS:\n\n")
              ACE_TEXT ("\t[-i <ior>]\n")
              ACE_TEXT ("\t[-?]\n\n"),
              this->exe_name_.c_str ()));
}

