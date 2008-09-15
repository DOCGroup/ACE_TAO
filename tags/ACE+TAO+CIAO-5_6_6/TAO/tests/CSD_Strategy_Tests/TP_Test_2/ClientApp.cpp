// $Id$
#include "ClientApp.h"
#include "AppHelper.h"
#include "TestAppExceptionC.h"
#include "Foo_A_ClientEngine.h"
#include "ace/Get_Opt.h"


ClientApp::ClientApp()
  : TestAppBase("TP_Test_2_Client"),
    ior_("Not Set"),
    client_kind_(0),
    client_id_(0)
{
}


ClientApp::~ClientApp()
{
}


int
ClientApp::run_i(int argc, char* argv[])
{
  int result = this->init(argc, argv);
  if (result != 0)
    {
      return result;
    }

  this->client_setup();

  result = this->run_engine();
  this->cleanup();
  return result;
}


int
ClientApp::init(int argc, char* argv[])
{
  this->orb_ = CORBA::ORB_init(argc, argv);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  return this->parse_args(argc, argv);
}


void
ClientApp::client_setup(void)
{
  // Turn the ior_ into a Foo_A obj ref.
  Foo_A_var foo = RefHelper<Foo_A>::string_to_ref(this->orb_.in(),
                                                  this->ior_.c_str());

  // Create the ClientEngine object, and give it the Foo_A obj ref.
  this->engine_ = new Foo_A_ClientEngine(foo.in(), this->client_id_);
}


int
ClientApp::run_engine(void)
{
  bool result = this->engine_->execute();
  return result ? 0 : -1;
}


void
ClientApp::cleanup()
{
}


int
ClientApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "i:k:n:");

  int c;

  while ((c = get_opts()) != -1)
    {
      int result = 0;
      switch (c)
      {
        case 'i':
          this->ior_ = get_opts.opt_arg();
          break;

        case 'k':
          result = set_arg(this->client_kind_,
                  get_opts.opt_arg(),
                  c,
                  "client_kind");
          break;

        case 'n':
          result = set_arg(this->client_id_,
                  get_opts.opt_arg(),
                  c,
                  "client_id");
          break;

        case '?':
          this->usage_statement();
          return 1;

        default:
          this->usage_statement();
          return -1;
      }

      if (result != 0)
        {
          return result;
        }
    }

  return this->arg_dependency_checks();
}

void
ClientApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-i <ior>]\n"
             "\t[-k <client_kind>]\n"
             "\t[-n <client_id>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}


int
ClientApp::arg_dependency_checks()
{
  if (this->ior_ == "Not Set")
    {
      ACE_ERROR((LM_ERROR,
                 "Error: Missing required command-line option (-i <ior>).\n"));
      this->usage_statement();
      return -1;
    }

  if (this->client_id_ <= 0)
    {
      ACE_ERROR((LM_ERROR,
                 "Error: Invalid command-line option (-n <client id>). \n"
                 "       The client id should be positive integer. \n"));
      this->usage_statement();
      return -1;
    }

  return 0;
}


int
ClientApp::set_arg(unsigned&   value,
                   const char* arg,
                   char        opt,
                   const char* name,
                   int         min)
{
  int tmp = ACE_OS::atoi(arg);

  if (tmp < min)
    {
      ACE_ERROR((LM_ERROR,
                 "Error: -%c <%s> must be integer type with a value of, "
                 "at least, %d.\n", opt, name, min));
      this->usage_statement();
      return -1;
    }

  value = tmp;
  return 0;
}


