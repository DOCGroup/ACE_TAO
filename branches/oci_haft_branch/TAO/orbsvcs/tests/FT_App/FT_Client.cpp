// $Id$

#include "FT_TestReplicaC.h"
#include <ace/Vector_T.h>
#include <ace/SString.h>
#include <ace/Get_Opt.h>
#include <iostream>
#include <fstream>

class FTClientMain
{
  typedef ACE_Vector<ACE_CString> StringVec;
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
    long & counter,         // inout
    int & more,             // out
    ACE_CString & command,  // inout
    int retry               // in
    );


  int next_replica (ACE_ENV_SINGLE_ARG_DECL);

  ////////////////////
  // forbidden methods
 private:
    FTClientMain (const FTClientMain & rhs);
    FTClientMain & operator = (const FTClientMain & rhs);

  ////////////////
  // Data members
 private:

  CORBA::ORB_var orb_;

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


  StringVec replica_iors_;
  size_t replica_pos_;
  const char * replica_name_;
  FT_TEST::TestReplica_var replica_;
};


FTClientMain::FTClientMain ()
  : commandIn_(&std::cin)
  , verbose_(NORMAL)
  , replica_pos_(0)
  , replica_name_("none")
{
}

FTClientMain::~FTClientMain ()
{
  if (this->inFile_.is_open())
  {
    this->inFile_.close();
  }
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
    << "        d" << FT_TEST::TestReplica::NOT_YET << " don't die" << std::endl
    << "        d" << FT_TEST::TestReplica::RIGHT_NOW << " immediately" << std::endl
    << "        d" << FT_TEST::TestReplica::WHILE_IDLE << " while idle" << std::endl
    << "      (FT_TestReplica interface)" << std::endl
    << "        d" << FT_TEST::TestReplica::BEFORE_STATE_CHANGE << " before state change" << std::endl
    << "        d" << FT_TEST::TestReplica::BEFORE_REPLICATION << " after state change, before replication" << std::endl
    << "        d" << FT_TEST::TestReplica::BEFORE_REPLY << " after replication, before reply "<< std::endl
    << "      (Monitorable interface)" << std::endl
    << "        d" << FT_TEST::TestReplica::DURING_IS_ALIVE << " during is alive" << std::endl
    << "        d" << FT_TEST::TestReplica::DENY_IS_ALIVE << " is_alive returns false" << std::endl
    << "      (Updatable interface)" << std::endl
    << "        d" << FT_TEST::TestReplica::DURING_GET_UPDATE << " during get update" << std::endl
    << "        d" << FT_TEST::TestReplica::BEFORE_SET_UPDATE << " before set update" << std::endl
    << "        d" << FT_TEST::TestReplica::AFTER_SET_UPDATE << " after set update" << std::endl
    << "      (Checkpointable interface)" << std::endl
    << "        d" << FT_TEST::TestReplica::DURING_GET_STATE << " during get state" << std::endl
    << "        d" << FT_TEST::TestReplica::BEFORE_SET_STATE << " before set state" << std::endl
    << "        d" << FT_TEST::TestReplica::AFTER_SET_STATE << " after set state" << std::endl
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
  this->argc_ = argc;
  this->argv_ = argv;
  int result = 0;

  // note: dfnkx are simple_util options
  // include them here so we can detect bad args
  ACE_Get_Opt get_opts (argc, argv, "c:df:g:nk:x");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'c':
      {
        this->inFileName_ = get_opts.opt_arg ();
        this->inFile_.open(this->inFileName_);
        if(this->inFile_.is_open() && this->inFile_.good())
        {
          std::cout << "FT Client: Reading commands from " << this->inFileName_ << std::endl;
          this->commandIn_ = & this->inFile_;
        }
        else
        {
          std::cout << "FT Client: Can't open input file: " << this->inFileName_ << std::endl;
          result = -1;
        }
        break;
      }
      case 'f':
      {
        replica_iors_.push_back(get_opts.opt_arg ());
        break;
      }

      case 'd':
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
  }
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

  while(more && result == 0 &&  ! this->commandIn_->eof())
  {
    if (! retry || command.length () == 0 )
    {
      char buffer[1000];
      this->commandIn_->getline(buffer, sizeof(buffer)-1);
      command = buffer;
    }
    retry = 0;

    if (command.length() >0)
    {
      char op = command[0];
      ACE_CString cdr = command.substr(1);
      char * junque;
      long operand = strtol(cdr.c_str(),&junque, 10);

      if (this->verbose_ >= NOISY)
      {
        std::cout << "FT Client: " << command << std::endl;
      }

      // turn echo on (based on verbose)
      // individual commands can turn it off
      int echo = this->verbose_ >= QUIET;

      switch(op)
      {
        case '#':
        {
          echo = 0;
          break;
        }
        case '=':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->set(" << operand << ");" << std::endl;
          }
          this->replica_->set(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter = operand;
          break;
        }
        case 'c':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get();" << std::endl;
          }
          long value = this->replica_->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (value == operand)
          {
            std::cout << "FT Client: Good: Read " << value << " expecting " << operand << std::endl;
            counter = operand;
          }
          else
          {
            std::cout << "FT Client: Error: Read " << value << " expecting " << operand << std::endl;
          }
          echo = 0;
          break;

        }
        case '>':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->counter(" << operand << ");" << std::endl;
          }
          this->replica_->counter(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter = operand;
          break;
        }
        case '+':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->increment(" << operand << ");" << std::endl;
          }
          this->replica_->increment(operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter += operand;
          break;
        }
        case '-':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->increment(" << -operand << ");" << std::endl;
          }
          this->replica_->increment(-operand ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          counter -= operand;
          break;
        }
        case '<':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->counter();" << std::endl;
          }
          long attribute = this->replica_->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          std::cout << "FT Client: Attribute: " << attribute << std::endl;
          echo = 0;
          break;
        }
        case '!':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->is_alive();" << std::endl;
          }
          int alive = this->replica_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          std::cout << "FT Client: Is alive?  " << alive << std::endl;
          break;
        }
        case 'd':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->die(" << operand << ");" << std::endl;
          }
          this->replica_->die(ACE_static_cast (FT_TEST::TestReplica::Bane, operand) ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          echo = 0;
          break;
        }
        case 's':
        {
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get_state();" << std::endl;
          }
          state = this->replica_->get_state(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          stateValue = counter;
          break;
        }
        case 'S':
        {
          if (state.in() != 0)
          {
            if (this->verbose_ >= LOUD)
            {
              std::cout << "FT Client: ->set_state(saved_state);" << std::endl;
            }
            this->replica_->set_state(state ACE_ENV_ARG_PARAMETER);
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
          if (this->verbose_ >= LOUD)
          {
            std::cout << "FT Client: ->get_update();" << std::endl;
          }
          update = this->replica_->get_update(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          updateValue = counter;
          break;
        }
        case 'U':
        {
          if (update.in() != 0)
          {
            if (this->verbose_ >= LOUD)
            {
              std::cout << "FT Client: ->set_update(saved_update);" << std::endl;
            }
            this->replica_->set_update(update ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            counter = updateValue;
          }
          else
          {
            std::cout << "FT Client: ERROR: No saved update information." << std::endl;
          }
          break;
        }
        case 'v':
        {
          this->verbose_ = ACE_static_cast(Verbosity, operand);
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
              if (this->verbose_ >= LOUD)
              {
                std::cout << "FT Client: ->shutdown();" << std::endl;
              }
              this->replica_->shutdown( ACE_ENV_SINGLE_ARG_PARAMETER);
              // @@ Note: this is here because the corba event loop seems to go to sleep
              // if there's nothing for it to do.
              // not quite sure why, yet.  Dale
              this->replica_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
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
      if (echo && this->verbose_ >= QUIET)
      {
        if (this->verbose_ >= LOUD)
        {
          std::cout << "FT Client: ->get();" << std::endl;
        }

        long value = this->replica_->get(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (value == counter)
        {
          if (this->verbose_ >= NORMAL)
          {
            std::cout << "FT Client: " << counter << std::endl;;
          }
        }
        else
        {
          std::cout << "FT Client: Error: read " << value << " expecting " << counter << std::endl;
          result = -1;
        }
      }
    }
  }
  return result;
}

int FTClientMain::next_replica (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;
  if (this->replica_pos_ < this->replica_iors_.size())
  {
    this->replica_name_ = this->replica_iors_[this->replica_pos_].c_str();
    this->replica_pos_ += 1;
    CORBA::Object_var rep_obj = this->orb_->string_to_object (this->replica_name_ ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    replica_ = FT_TEST::TestReplica::_narrow (rep_obj);
    if (! CORBA::is_nil (replica_))
    {
      result = 1;
    }
    else
    {
      std::cerr << "FT Client: Can't resolve IOR: " << this->replica_name_ << std::endl;
    }
  }
  else
  {
    std::cerr << "***OUT_OF_REPLICAS*** " << this->replica_pos_ << std::endl;
  }
  return result;
}


int FTClientMain::run ()
{
  int result = 0;

  this->orb_ = CORBA::ORB_init(this->argc_, this->argv_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (next_replica ())
  {
    long counter = this->replica_->get(ACE_ENV_SINGLE_ARG_PARAMETER);

    // retry information
    ACE_CString command;
    int retry = 0;

    ACE_TRY_CHECK;
    if (this->verbose_ >= NORMAL)
    {
      std::cout << "FT Client: Initial counter " << counter << std::endl;
    }
    if (ACE_OS::isatty(stdin))
    {
      std::cout << "FT Client: Commands(? for help):" << std::endl;
    }

    int more = 1;
    while (more && result == 0 &&  ! this->commandIn_->eof())
    {
      ACE_TRY_NEW_ENV
      {
        result = pass (counter, more, command, retry);
        ACE_TRY_CHECK;
      }
      ACE_CATCH (CORBA::SystemException, sysex)
      {
        std::cout << "FT Client: Caught system exception: " << std::endl;
        ACE_PRINT_EXCEPTION (sysex, "FT Client");

        retry = 0;
        int handled = 0;

        handled = next_replica();
        if (handled)
        {
          std::cout << "FT Client: Recovering from fault." << std::endl;
          std::cout << "FT Client:   Activate " << this->replica_name_ << std::endl;
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
        }
        if (! handled)
        {
          std::cout << "FT Client: Exception not handled.  Rethrow. " << std::endl;
          ACE_RE_THROW;
        }
      }
      ACE_ENDTRY;
    }
  }
  else
  {
    std::cerr << "FT Client: Can't connect to replica." << std::endl;
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
  template ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate Client<FT_TEST::TestReplica, FT_TEST::TestReplica_var>
# pragma ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
