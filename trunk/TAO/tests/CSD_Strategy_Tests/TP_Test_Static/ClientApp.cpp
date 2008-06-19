// $Id$
#include "ClientApp.h"
#include "Foo_A_ClientEngine.h"
#include "AppHelper.h"
#include "TestAppExceptionC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"

ClientApp::ClientApp()
  : TestAppBase("TP_Test_1_Client"),
    ior_ ("file://test.ior")
{
}


ClientApp::~ClientApp()
{
}


int
ClientApp::run_i(int argc, char* argv[])
{
  // Initialize the ORB before parsing our own args.
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

  // Parse the command-line args for this application.
  // * Returns -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);
  if (result != 0)
    {
      return result;
    }
  // Convert the IOR string to a Foo_A object reference.
  Foo_A_var foo = RefHelper<Foo_A>::string_to_ref(orb.in(),
                                                  this->ior_.c_str());

  // Create the appropriate client "engine" object.
  Foo_A_ClientEngine engine(foo.in());

  // Execute the client algorithm
  result = engine.execute() ? 0 : -1;

  return result;
}


int
ClientApp::parse_args(int argc, char* argv[])
{
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
          ACE_DEBUG((LM_DEBUG,
                     "(%P|%t) usage:  %s -i <ior_string>\n",
                     argv[0]));
          return 1;

        default:
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) usage:  %s -i <ior_string>\n",
                     argv[0]));
          return -1;
      }
    }

  return 0;
}
