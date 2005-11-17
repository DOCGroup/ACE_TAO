// $Id$
#include "ClientApp.h"
#include "FooC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"


ClientApp::ClientApp()
{
}


ClientApp::~ClientApp()
{
}


int
ClientApp::run(int argc, char* argv[] ACE_ENV_ARG_DECL)
{
  CORBA::ORB_var orb 
    = CORBA::ORB_init(argc, argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);
  if (result != 0)
    {
      return result;
    }

  CORBA::Object_var obj 
    = orb->string_to_object(this->ior_.c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to convert IOR string to obj ref.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  Foo_var foo = Foo::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(foo.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to Foo interface.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  for (CORBA::Long i = 1; i <= 100; i++)
    {
      foo->op1(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      foo->op2(i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      CORBA::Long value = foo->op3(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_DEBUG((LM_DEBUG,
                 "(%P|%t) ===> Value retrieved from op3() == %d\n",
                 value));

      for (CORBA::Long j = 1; j <= 5; j++)
        {
          foo->op4(495 + (i * 5) + j ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }

      ACE_TRY_NEW_ENV
      {
        foo->op5(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCH (FooException, ex)
      {
        ACE_DEBUG((LM_DEBUG,
                  "(%P|%t) ===> Caught FooException - as expected.\n"));
        
      }
      ACE_ENDTRY;
    }

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ===> Tell server that we are done().\n"));

  foo->done(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ===> Back from done().\n"));

  return 0;
}


int
ClientApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "i:");

  int c;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'i':
          this->ior_ = get_opts.opt_arg();
          break;

        case '?':
          this->usage_statement();
          return 1;
        default:
          this->usage_statement();
          return -1;
      }
    }

  return 0;
}


void
ClientApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-i <ior>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}

