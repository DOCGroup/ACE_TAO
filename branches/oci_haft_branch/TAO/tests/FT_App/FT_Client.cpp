// $Id$

#include "FT_TestReplicaC.h"
// the following include is relative to $TAO_ROOT
#include "examples/Simple/Simple_util.h"
#include <fstream>

class FTClientMain
{
 public:
  ///////////////////////////
  // construction/destruction
  FTClientMain ();

  ~FTClientMain ();

  /////////////////
  // initialization
  int parse_args (int argc, char *argv[]);

  ////////////
  // execution
  int run();


  /////////////////
  // implementation
private:
  void usage(ostream & out)const;
  void commandUsage(ostream & out);

  ////////////////////
  // forbidden methods
 private:
    FTClientMain (const FTClientMain & rhs);
    FTClientMain & operator = (const FTClientMain & rhs);

  ////////////////
  // Data members
 private:
  int argc_;
  char ** argv_;
  const char * inFileName_;
  std::ifstream inFile_;
  std::istream *commandIn_;
};


FTClientMain::FTClientMain ()
  : commandIn_(&std::cin)
{
}

FTClientMain::~FTClientMain ()
{
  if (inFile_.is_open())
  {
    inFile_.close();
  }
}

void FTClientMain::commandUsage(ostream & out)
{
  out
    << "Valid commands:" << std::endl
    << "  Access via method call:" << std::endl
    << "    =N set(N)" << std::endl
    << "    cN get and compare to N"<< std::endl
    << "    +N increment(N)" << std::endl
    << "    -N increment(-N)" << std::endl
    << "    (note \"get\" is used to display result." << std::endl
    << "  Access as attribute:" << std::endl
    << "    >N set attribute to N" << std::endl
    << "    < get attribite" << std::endl
    << "  Try methods to be used by fault tolerant infrastructure: " << std::endl
    << "    ! is_alive" << std::endl
    << "    s get_state" << std::endl
    << "    S set_state" << std::endl
    << "    u get_update" << std::endl
    << "    U set_update" << std::endl
    << "  Simulate failure:" << std::endl
    << "    dN die on condition:" << std::endl
    << "      " << FT_TEST::TestReplica::NOT_YET << " don't die" << std::endl
    << "      " << FT_TEST::TestReplica::RIGHT_NOW << " immediately" << std::endl
    << "      " << FT_TEST::TestReplica::DURING_GET << " during next get" << std::endl
    << "      " << FT_TEST::TestReplica::BEFORE_SET << " before next set" << std::endl
    << "      " << FT_TEST::TestReplica::AFTER_SET << " after next set" << std::endl
    << "      " << FT_TEST::TestReplica::BEFORE_INCREMENT << " before next increment "<< std::endl
    << "      " << FT_TEST::TestReplica::AFTER_INCREMENT << " after next increment "<< std::endl
    << "      " << FT_TEST::TestReplica::DURING_ATTRIBUTE_GET << " during next attribute get" << std::endl
    << "      " << FT_TEST::TestReplica::BEFORE_ATTRIBUTE_SET << " before next attribute set" << std::endl
    << "      " << FT_TEST::TestReplica::AFTER_ATTRIBUTE_SET << " after next attribute set" << std::endl
    << "      " << FT_TEST::TestReplica::DURING_GET_UPDATE << " during get update" << std::endl
    << "      " << FT_TEST::TestReplica::BEFORE_SET_UPDATE << " before set update" << std::endl
    << "      " << FT_TEST::TestReplica::AFTER_SET_UPDATE << " after set update" << std::endl
    << "      " << FT_TEST::TestReplica::DURING_GET_STATE << " during get state" << std::endl
    << "      " << FT_TEST::TestReplica::BEFORE_SET_STATE << " before set state" << std::endl
    << "      " << FT_TEST::TestReplica::AFTER_SET_STATE << " after set state" << std::endl
    << "      " << FT_TEST::TestReplica::DURING_IS_ALIVE << " during is alive" << std::endl
    << "      " << FT_TEST::TestReplica::DENY_IS_ALIVE << " is_alive returns false" << std::endl
    << " Logistics commands:" << std::endl
    << "   v set verbosity:" << std::endl
    << "     0 don't check counter value." << std::endl
    << "     1 only display counter value mismatch." << std::endl
    << "     2 display counter value after every command (default)." << std::endl
    << "     3 display commands." << std::endl
    << "     4 display method calls." << std::endl
    << "   q quit (end the client, not the replica(s).)" << std::endl
    << "   ? help (this message)" << std::endl;
}

int
FTClientMain::parse_args (int argc, char *argv[])
{
  argc_ = argc;
  argv_ = argv;
  int result = 0;
  // note: dfnkx are simple_util options
  // include them here so we can detect bad args
  // todo: we also need to ignore TAO args
  ACE_Get_Opt get_opts (argc, argv, "c:df:g:nk:x");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
      case 'f':
      case 'n':
      case 'k':
      case 'x':
        // ignore args used by other command line parsers
        // poor design, but its ubiquitous
        break;

      case 'c':
        inFileName_ = get_opts.opt_arg ();
        inFile_.open(inFileName_);
        if(inFile_.is_open() && inFile_.good())
        {
          std::cerr << "reading commands from " << inFileName_ << std::endl;
          commandIn_ = & inFile_;
        }
        else
        {
          std::cout << "Can't open input file: " << inFileName_ << std::endl;
          result = -1;
        }
        break;

      default:
        std::cerr << "Unknown argument -" << (char) c << std::endl;
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


void FTClientMain::usage(ostream & out)const
{
  out << "usage"
      << " -c <command file>"
      << std::endl;
}

int FTClientMain::run ()
{
  int result = 0;
  enum Verbosity{
    SILENT,
    QUIET,
    NORMAL,
    NOISY,
    LOUD}
    verbose = NORMAL;
  Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var> ft_server;
  // Initialize the ft_server.
  result = ft_server.init ("FT_TEST",argc_, argv_);
  if (result == 0)
  {
    ::FT::State_var state;
    unsigned long stateValue = 0;
    ::FT::State_var update;
    unsigned long updateValue = 0;

    long counter = ft_server->get(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (verbose >= NORMAL)
    {
      std::cout << "Initial counter " << counter << std::endl;
    }
    std::cout << "Commands(=N,cN,>N,+N,-N,<,dN,!,s,S,u,U,vN,q,?)" << std::endl;

    int more = 1;
    while(more && ! (*commandIn_).eof())
    {
      std::string command;
      (*commandIn_) >> command;
      if (command.length() >0)
      {
        char op = command[0];
        std::string cdr = command.substr(1);
        char * junque;
        long operand = strtol(cdr.c_str(),&junque, 10);

        if (verbose >= NOISY)
        {
          cout << op << operand << std::endl;
        }

        switch(op)
        {
          case '=':
          {
            counter = operand;
            if (verbose >= LOUD)
            {
              cout << "->set(" << operand << ");" << std::endl;
            }
            ft_server->set(operand ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            break;
          }
          case 'c':
          {
            if (verbose >= LOUD)
            {
              cout << "->get();" << std::endl;
            }
            long value = ft_server->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            if (value == operand)
            {
              std::cout << "Read: " << value << std::endl;
              counter = operand;
            }
            else
            {
              std::cout << "Error: read " << value << " expecting " << operand << endl;
            }
            break;

          }
          case '>':
          {
            counter = operand;
            if (verbose >= LOUD)
            {
              cout << "->counter(" << operand << ");" << std::endl;
            }
            ft_server->counter(operand ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            break;
          }
          case '+':
          {
            counter += operand;
            if (verbose >= LOUD)
            {
              cout << "->increment(" << operand << ");" << std::endl;
            }
            ft_server->increment(operand ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            break;
          }
          case '-':
          {
            counter -= operand;
            if (verbose >= LOUD)
            {
              cout << "->increment(" << -operand << ");" << std::endl;
            }
            ft_server->increment(-operand ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            break;
          }
          case '<':
          {
            if (verbose >= LOUD)
            {
              cout << "->counter();" << std::endl;
            }
            long attribute = ft_server->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            cout << "Attribute: " << attribute << endl;
            break;
          }
          case '!':
          {
            if (verbose >= LOUD)
            {
              cout << "->is_alive();" << std::endl;
            }
            int alive = ft_server->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            cout << "Is alive?  " << alive << endl;
            break;
          }
          case 'd':
          {
            if (verbose >= LOUD)
            {
              cout << "->die(" << operand << ");" << std::endl;
            }
            ft_server->die(static_cast<FT_TEST::TestReplica::Bane>(operand ACE_ENV_ARG_PARAMETER));
            ACE_TRY_CHECK;
            break;
          }
          case 's':
          {
            if (verbose >= LOUD)
            {
              cout << "->get_state();" << std::endl;
            }
            state = ft_server->get_state(ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            stateValue = counter;
            break;
          }
          case 'S':
          {
            if (state.in() != 0)
            {
              if (verbose >= LOUD)
              {
                cout << "->set_state(saved_state);" << std::endl;
              }
              ft_server->set_state(state ACE_ENV_ARG_PARAMETER);
              counter = stateValue;
              ACE_TRY_CHECK;
            }
            else
            {
              std::cout << "Error: no saved state." << std::endl;
            }
            break;
          }
          case 'u':
          {
            if (verbose >= LOUD)
            {
              cout << "->get_update();" << std::endl;
            }
            update = ft_server->get_update(ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            updateValue = counter;
            break;
          }
          case 'U':
          {
            if (update.in() != 0)
            {
              if (verbose >= LOUD)
              {
                cout << "->set_update(saved_update);" << std::endl;
              }
              ft_server->set_update(update ACE_ENV_ARG_PARAMETER);
              counter = updateValue;
            }
            else
            {
              std::cout << "ERROR: No saved update information." << endl;
            }
            ACE_TRY_CHECK;
            break;
          }
          case 'v':
          {
            verbose = static_cast<Verbosity> (operand);
            break;
          }
          case 'z':
          {
            if (operand == 0)
            {
              operand = 1;
            }
            ACE_Time_Value tv (operand,0);
            ACE_OS::sleep(tv);
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
              std::cout << "Unknown: " << command << std::endl;
            }
            commandUsage(std::cerr);
            break;
          }
        }
        if (verbose >= QUIET)
        {
          if (verbose >= LOUD)
          {
            cout << "->get();" << std::endl;
          }

          long value = ft_server->get(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (value == counter)
          {
            if (verbose >= NORMAL)
            {
              std::cout << counter << endl;;
            }
          }
          else
          {
            std::cout << "Error: read " << value << " expecting " << counter << endl;
            result = -1;
          }
        }
      }
    }
  }
  else
  {
    std::cout << "Error: initialization failed." << std::endl;
  }
  return result;
}


int
main (int argc, char *argv[])
{
  FTClientMain app;
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
                           "FT_Client::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}
