// $Id$

#include "FT_TestReplicaC.h"
// the following include is relative to $TAO_ROOT
#include "examples/Simple/Simple_util.h"
#include <fstream>

class FTClientMain
{
  typedef Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var> ServerVar;
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
  int run ();

  /////////////////
  // implementation
private:
  void usage (ostream & out)const;
  void commandUsage (ostream & out);
  int pass (
    ServerVar & ft_server,  // in
    long & counter,         // inout
    int & more,             // out
    ACE_CString & command,  // inout
    int retry               // in
    );

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


FTClientMain::FTClientMain ()
  : commandIn_(&std::cin)
  , verbose_(NORMAL)
  , fargValue_(0)
  , fargPos_(0)
  , fargEnd_(0)
{
}

FTClientMain::~FTClientMain ()
{
  if (inFile_.is_open())
  {
    inFile_.close();
  }
  ACE_OS::free (fargValue_);
  fargValue_ = 0;
}

void FTClientMain::commandUsage(ostream & out)
{
  out
    << "Each command must be at the beginning of a separate line." << std::endl
    << "Everything after the command (and operand if any) is ignored." << std::endl
    << "Valid commands are:" << std::endl
    << "  Access via method call:" << std::endl
    << "    =N    set counter to N" << std::endl
    << "    cN    get counter and compare to N (c stands for \"check\""<< std::endl
    << "    +N    increment counter by N" << std::endl
    << "    -N    decrement counter by N" << std::endl
    << "  Access as attribute:" << std::endl
    << "    >N    set attribute to N" << std::endl
    << "    <     get attribite" << std::endl
    << "  Try methods to be used by fault tolerant infrastructure: " << std::endl
    << "    !     is_alive" << std::endl
    << "    s     get_state" << std::endl
    << "    S     set_state" << std::endl
    << "    u     get_update" << std::endl
    << "    U     set_update" << std::endl
    << "  Simulate failure:" << std::endl
    << "    dN    die on condition:" << std::endl
    << "      " << FT_TEST::TestReplica::NOT_YET << " don't die" << std::endl
    << "      " << FT_TEST::TestReplica::RIGHT_NOW << " immediately" << std::endl
    << "      " << FT_TEST::TestReplica::WHILE_IDLE << " while idle" << std::endl
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
    << "   #    ignore this line (comment)." << std::endl
    << "   v    set verbosity:" << std::endl
    << "     0 don't check counter value." << std::endl
    << "     1 only display counter value mismatch." << std::endl
    << "     2 display counter value after every command (default)." << std::endl
    << "     3 display commands." << std::endl
    << "     4 display method calls." << std::endl
    << "   zN   sleep N seconds." << std::endl
    << "   q    quit (end the client, not the replica(s).)" << std::endl
    << "   q1   quit (end the client, and shutdown the currently active replica.)" << std::endl
    << "   ?    help (this message)" << std::endl;
}

int
FTClientMain::parse_args (int argc, char *argv[])
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

      std::cout << "FT Client: Initial primary replica: " << fargPos_ << endl;

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
  // todo: we also need to ignore TAO args
  ACE_Get_Opt get_opts (argc, argv, "c:df:g:nk:x");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        inFileName_ = get_opts.opt_arg ();
        inFile_.open(inFileName_);
        if(inFile_.is_open() && inFile_.good())
        {
          std::cout << "FT Client: Reading commands from " << inFileName_ << std::endl;
          commandIn_ = & inFile_;
        }
        else
        {
          std::cout << "FT Client: Can't open input file: " << inFileName_ << std::endl;
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

      default:
        std::cerr << "FT Client: Unknown argument -" << (char) c << std::endl;
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
      << " -f <ior file list>"
      << " -d (debug)"
      << " -k (ior)"
      << std::endl;
}

int FTClientMain::pass (
  ServerVar & ft_server,
  long & counter,
  int & more,
  ACE_CString & command,
  int retry)
{
  int result = 0;

  ::FT::State_var state;
  unsigned long stateValue = 0;
  ::FT::State_var update;
  unsigned long updateValue = 0;

  while(more && result == 0 &&  ! commandIn_->eof())
  {
    if (! retry || command.length () == 0 )
    {
      char buffer[1000];
      commandIn_->getline(buffer, sizeof(buffer)-1);
      command = buffer;
    }
    retry = 0;

    if (command.length() >0)
    {
      char op = command[0];
      ACE_CString cdr = command.substr(1);
      char * junque;
      long operand = strtol(cdr.c_str(),&junque, 10);

      if (verbose_ >= NOISY)
      {
        std::cout << "FT Client: " << command << std::endl;
      }

      // turn echo on (based on verbose)
      // individual commands can turn it off
      int echo = verbose_ >= QUIET;

      switch(op)
      {
        case '#':
        {
          echo = 0;
          break;
        }
        case '=':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->set(" << operand << ");" << std::endl;
          }
          ft_server->set(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter = operand;
          break;
        }
        case 'c':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get();" << std::endl;
          }
          long value = ft_server->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (value == operand)
          {
            std::cout << "FT Client: Good: Read " << value << " expecting " << operand << std::endl;
            counter = operand;
          }
          else
          {
            std::cout << "FT Client: Error: Read " << value << " expecting " << operand << endl;
          }
          echo = 0;
          break;

        }
        case '>':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->counter(" << operand << ");" << std::endl;
          }
          ft_server->counter(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter = operand;
          break;
        }
        case '+':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->increment(" << operand << ");" << std::endl;
          }
          ft_server->increment(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter += operand;
          break;
        }
        case '-':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->increment(" << -operand << ");" << std::endl;
          }
          ft_server->increment(-operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter -= operand;
          break;
        }
        case '<':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->counter();" << std::endl;
          }
          long attribute = ft_server->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          std::cout << "FT Client: Attribute: " << attribute << endl;
          echo = 0;
          break;
        }
        case '!':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->is_alive();" << std::endl;
          }
          int alive = ft_server->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          std::cout << "FT Client: Is alive?  " << alive << endl;
          break;
        }
        case 'd':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->die(" << operand << ");" << std::endl;
          }
          ft_server->die(static_cast<FT_TEST::TestReplica::Bane>(operand ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;
          echo = 0;
          break;
        }
        case 's':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get_state();" << std::endl;
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
            if (verbose_ >= LOUD)
            {
              std::cout << "FT Client: ->set_state(saved_state);" << std::endl;
            }
            ft_server->set_state(state ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            counter = stateValue;
          }
          else
          {
            std::cout << "FT Client: Error: no saved state." << std::endl;
          }
          break;
        }
        case 'u':
        {
          if (verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get_update();" << std::endl;
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
            if (verbose_ >= LOUD)
            {
              std::cout << "FT Client: ->set_update(saved_update);" << std::endl;
            }
            ft_server->set_update(update ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            counter = updateValue;
          }
          else
          {
            std::cout << "FT Client: ERROR: No saved update information." << endl;
          }
          break;
        }
        case 'v':
        {
          verbose_ = static_cast<Verbosity> (operand);
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
          if (operand != 0)
          {
            ACE_TRY_NEW_ENV
            {
              if (verbose_ >= LOUD)
              {
                std::cout << "FT Client: ->shutdown();" << std::endl;
              }
              ft_server->shutdown( ACE_ENV_SINGLE_ARG_PARAMETER);
            }
            ACE_CATCHANY
            {
              std::cout << "FT Client: Ignoring expected exception during shutdown." << std::endl;
              ; // ignore exception during shutdown
            }
            ACE_ENDTRY;
          }
          echo = 0;
          more = 0;
          break;
        }
        default:
        {
          if (op != '?')
          {
            std::cout << "FT Client: Unknown: " << command << std::endl;
          }
          commandUsage(std::cerr);
          break;
        }
      }
      if (echo && verbose_ >= QUIET)
      {
        if (verbose_ >= LOUD)
        {
          std::cout << "FT Client: ->get();" << std::endl;
        }

        long value = ft_server->get(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (value == counter)
        {
          if (verbose_ >= NORMAL)
          {
            std::cout << "FT Client: " << counter << endl;;
          }
        }
        else
        {
          std::cout << "FT Client: Error: read " << value << " expecting " << counter << endl;
          result = -1;
        }
      }
    }
  }
  return result;
}

int FTClientMain::run ()
{
  int result = 0;
  ServerVar ft_server;
  // Initialize the ft_server.
  result = ft_server.init ("FT_TEST",argc_, argv_);

  long counter = ft_server->get(ACE_ENV_SINGLE_ARG_PARAMETER);

  // retry information
  ACE_CString command;
  int retry = 0;

  ACE_TRY_CHECK;
  if (verbose_ >= NORMAL)
  {
    std::cout << "FT Client: Initial counter " << counter << std::endl;
  }
  std::cout << "FT Client: Commands(=N,cN,>N,+N,-N,<,dN,!,s,S,u,U,vN,q,?)" << std::endl;

  int more = 1;
  while (more && result == 0 &&  ! commandIn_->eof())
  {
    ACE_TRY_NEW_ENV
    {
      result = pass(ft_server, counter, more, command, retry);
      ACE_TRY_CHECK;
    }
    ACE_CATCH (CORBA::SystemException, sysex)
    {
      std::cout << "FT Client: Caught system exception: " << std::endl;
      ACE_PRINT_EXCEPTION (sysex, "FT Client");

      retry = 0;
      int handled = 0;
      if(fargPos_ != 0 && fargPos_ != fargEnd_)
      {
        handled = ! ft_server.reconnect_file(fargPos_);
        if (handled)
        {
          std::cout << "FT Client: Recovering from fault." << std::endl;
          std::cout << "FT Client:   Activate " << fargPos_ << std::endl;
          if (command.length () == 0)
          {
            std::cout << "FT Client:   No command to retry." << std::endl;
          }
          else if (command[0] == 'd')
          {
            std::cout << "FT Client:   Not retrying \"die\" command." << std::endl;
          }
          else if (sysex.completed () == CORBA::COMPLETED_YES)
          {
            std::cout << "FT Client:   Last command completed.  No retry needed." << std::endl;
          }
          else
          {
            if (sysex.completed () == CORBA::COMPLETED_MAYBE)
            {
              std::cout << "FT Client:   Last command may have completed.  Retrying anyway." << std::endl;
            }
            retry = 1;
            std::cout << "FT Client:   Retrying command: " << command << std::endl;
          }

          // advance fargPos to next filename
          fargPos_ += ACE_OS::strlen(fargPos_);
          if (fargPos_ != fargEnd_)
          {
            fargPos_ += 1;
          }
        }
      }
      if (! handled)
      {
        ACE_RE_THROW;
      }
    }
    ACE_ENDTRY;
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
