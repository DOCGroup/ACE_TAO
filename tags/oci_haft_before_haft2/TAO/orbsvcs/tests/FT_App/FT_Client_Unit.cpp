// $Id$

//#include "FT_TestReplicaC.h"
#include "FT_UnitTests.h"
// the following include is relative to $TAO_ROOT
#include "examples/Simple/Simple_util.h"
#include <iostream>
#include <fstream>

class FTClientUnitMain
{
 public:
  ///////////////////////////
  // construction/destruction
  FTClientUnitMain ();

  ~FTClientUnitMain ();

  /////////////////
  // initialization
  int parse_args (int argc, char *argv[]);

  ////////////
  // execution
  int run ();

  /////////////////
  // implementation
private:
  void usage (ostream & out)const;
  void commandUsage (ostream & out);
  int do_commands (
    FT_UnitTests & unitTests, // in
    int & more,               // out
    ACE_CString & command     // inout
    );

  ////////////////////
  // forbidden methods
 private:
    FTClientUnitMain (const FTClientUnitMain & rhs);
    FTClientUnitMain & operator = (const FTClientUnitMain & rhs);

  ////////////////
  // Data members
 private:
  int argc_;
  char ** argv_;
  const char * inFileName_;
  std::ifstream inFile_;
  std::istream *commandIn_;
  CORBA::ORB_var orb_;

  enum Verbosity{
    SILENT,
    QUIET,
    NORMAL,
    NOISY,
    LOUD}
    verbose_;

  char * fargValue_;
  char * fargPos_;
  char * fargEnd_;

};


FTClientUnitMain::FTClientUnitMain ()
  : commandIn_(&std::cin)
  , verbose_(NORMAL)
  , fargValue_(0)
  , fargPos_(0)
  , fargEnd_(0)
{
}

FTClientUnitMain::~FTClientUnitMain ()
{
  if (inFile_.is_open())
  {
    inFile_.close();
  }
  ACE_OS::free (fargValue_);
  fargValue_ = 0;
}

void FTClientUnitMain::commandUsage(ostream & out)
{
  out
    << "Each command must be at the beginning of a separate line." << std::endl
    << "Everything after the command (and operand if any) is ignored." << std::endl
    << "Valid commands are:" << std::endl
    << "  Access via method call:" << std::endl
    << "   q    quit (end the client, not the replica(s).)" << std::endl
    << "   tN   run unit test N" << std::endl
    << "   ?    help (this message)" << std::endl;
}

int
FTClientUnitMain::parse_args (int argc, char *argv[])
{
  argc_ = argc;
  argv_ = argv;
  int result = 0;

  // find the -f filename argument
  // and treat it specially
  fargValue_ = 0;
  int nArg;
  for(nArg = 1; fargValue_ == 0 && nArg < argc - 1; ++nArg)
  {
    if (argv[nArg][0] == '-'
      &&  argv[nArg][1] == 'f'
      &&  argv[nArg][2] == '\0')
    {
      // remember the starting, current, and ending position of farg
      fargValue_ = ACE_OS::strdup(argv[nArg+1]);
      fargPos_ = fargValue_;
      fargEnd_ = fargValue_ + ACE_OS::strlen(fargPos_);

      // find a comma delimiter, and
      // chop the string there.
      char * delim = ACE_OS::strchr (fargValue_, ',');
      while(delim != 0)
      {
        *delim = '\0';
        delim = ACE_OS::strchr (delim + 1, ',');
      }
      argv[nArg+1] = fargValue_;

//      std::cout << "FT Client UnitTest: Initial primary replica: " << fargPos_ << std::endl;

      // point fargPos at the next filename
      fargPos_ = fargPos_ + ACE_OS::strlen(fargPos_);
      if (fargPos_ != fargEnd_)
      {
        fargPos_ += 1;
      }
    }
  }

  // note: dfnkx are simple_util options
  // include them here so we can detect bad args
  ACE_Get_Opt get_opts (argc, argv, "df:nk:r:x");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        inFileName_ = get_opts.opt_arg ();
        inFile_.open(inFileName_);
        if(inFile_.is_open() && inFile_.good())
        {
          std::cout << "FT Client UnitTest: Reading commands from " << inFileName_ << std::endl;
          commandIn_ = & inFile_;
        }
        else
        {
          std::cout << "FT Client UnitTest: Can't open input file: " << inFileName_ << std::endl;
          result = -1;
        }
        break;

      case 'd':
      case 'f':
      case 'k':
      case 'n':
      case 'x':
        // ignore args used by other command line parsers
        // poor design, but its ubiquitous
        break;

     case 'r':
        // ignore: process by FT_UnitTests
        break;

      default:
        std::cerr << "FT Client UnitTest: Unknown argument -" << (char) c << std::endl;
        usage(std::cerr);
        result = 1;
        break;

      case '?':
        usage(std::cerr);
        result = 1;
      }
  // Indicates sucessful parsing of the command line
  return result;
}

void FTClientUnitMain::usage(ostream & out)const
{
  out << "Usage:"
      << " -c <command file>"
      << " -f <rep-factory-ior>,<rep-factory-ior>, ..."
      << " -r <ior of ReplicationManager>"
      << " -d (debug)"
      << std::endl;
}

int FTClientUnitMain::do_commands (
  FT_UnitTests & unitTests,
  int & more,
  ACE_CString & command)
{
  int result = 0;

  while(more && result == 0 &&  ! commandIn_->eof())
  {
    char buffer[1000];
    commandIn_->getline(buffer, sizeof(buffer)-1);
    command = buffer;

    if (command.length() >0)
    {
      char op = command[0];
      ACE_CString cdr = command.substr(1);
      char * junque;
      long operand = strtol(cdr.c_str(),&junque, 10);

      std::cout << "FT Client UnitTest: " << command << std::endl;

      switch(op)
      {
        case 't':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client UnitTest: Unit Test (" << operand << ");" << std::endl;
          }
          if (unitTests.run_test(operand))
          {
            std::cout << "FT Client UnitTest: ERROR: Test failed." << std::endl;
          }
          break;
        }
        case 'q':
        {
          more = 0;
          break;
        }
        default:
        {
          if (op != '?')
          {
            std::cout << "FT Client UnitTest: Unknown: " << command << std::endl;
          }
          commandUsage(std::cerr);
          break;
        }
      }
    }
  }
  return result;
}


int FTClientUnitMain::run ()
{
  int result = 0;

  orb_ = CORBA::ORB_init (argc_,
                          argv_,
                          0
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  FT_UnitTests unitTests(orb_);
  unitTests.parse_args(argc_, argv_);
  unitTests.init();

  ACE_CString command;

  std::cout << "FT Client UnitTest: Commands(? for help):" << std::endl;
  if (ACE_OS::isatty(stdin))
  {
    std::cout << "FT Client UnitTest: Commands(? for help):" << std::endl;
  }

  int more = 1;
  while (more && result == 0 &&  ! commandIn_->eof())
  {
    ACE_TRY_NEW_ENV
    {
      result = do_commands(unitTests, more, command);
      ACE_TRY_CHECK;
    }
    ACE_CATCH (CORBA::SystemException, sysex)
    {
      std::cout << "FT Client UnitTest: Caught system exception: " << std::endl;
      ACE_PRINT_EXCEPTION (sysex, "FT Client UnitTest");
    }
    ACE_ENDTRY;
  }

  return result;
}


int
main (int argc, char *argv[])
{
  FTClientUnitMain app;
  int result = app.parse_args(argc, argv);
  if (result == 0)
  {
    ACE_TRY_NEW_ENV
    {
      result = app.run ();
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "FT_Client_Unit::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
