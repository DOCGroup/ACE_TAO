// $Id$

#include "FT_TestReplicaC.h"
#include <ace/Vector_T.h>
#include <ace/SString.h>
#include <ace/Get_Opt.h>
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

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
  int run (ACE_ENV_SINGLE_ARG_DECL);

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
    ACE_ENV_ARG_DECL);


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
  ifstream inFile_;
  istream *commandIn_;

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
  : commandIn_(&cin)
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
    << "Each command must be at the beginning of a separate line." << endl
    << "Everything after the command (and operand if any) is ignored." << endl
    << "Valid commands are:" << endl
    << "  Access via method call:" << endl
    << "    =N    set counter to N" << endl
    << "    cN    get counter and compare to N (c stands for \"check\""<< endl
    << "    +N    increment counter by N" << endl
    << "    -N    decrement counter by N" << endl
    << "  Access as attribute:" << endl
    << "    >N    set attribute to N" << endl
    << "    <     get attribite" << endl
    << "  Try methods to be used by fault tolerant infrastructure: " << endl
    << "    !     is_alive" << endl
    << "    s     get_state" << endl
    << "    S     set_state" << endl
    << "    u     get_update" << endl
    << "    U     set_update" << endl
    << "  Simulate failure:" << endl
    << "    dN    die on condition:" << endl
    << "        d" << FT_TEST::TestReplica::NOT_YET << " don't die" << endl
    << "        d" << FT_TEST::TestReplica::RIGHT_NOW << " immediately" << endl
    << "        d" << FT_TEST::TestReplica::WHILE_IDLE << " while idle" << endl
    << "      (FT_TestReplica interface)" << endl
    << "        d" << FT_TEST::TestReplica::BEFORE_STATE_CHANGE << " before state change" << endl
    << "        d" << FT_TEST::TestReplica::BEFORE_REPLICATION << " after state change, before replication" << endl
    << "        d" << FT_TEST::TestReplica::BEFORE_REPLY << " after replication, before reply "<< endl
    << "      (Monitorable interface)" << endl
    << "        d" << FT_TEST::TestReplica::DURING_IS_ALIVE << " during is alive" << endl
    << "        d" << FT_TEST::TestReplica::DENY_IS_ALIVE << " is_alive returns false" << endl
    << "      (Updatable interface)" << endl
    << "        d" << FT_TEST::TestReplica::DURING_GET_UPDATE << " during get update" << endl
    << "        d" << FT_TEST::TestReplica::BEFORE_SET_UPDATE << " before set update" << endl
    << "        d" << FT_TEST::TestReplica::AFTER_SET_UPDATE << " after set update" << endl
    << "      (Checkpointable interface)" << endl
    << "        d" << FT_TEST::TestReplica::DURING_GET_STATE << " during get state" << endl
    << "        d" << FT_TEST::TestReplica::BEFORE_SET_STATE << " before set state" << endl
    << "        d" << FT_TEST::TestReplica::AFTER_SET_STATE << " after set state" << endl
    << " Logistics commands:" << endl
    << "   #    ignore this line (comment)." << endl
    << "   v    set verbosity:" << endl
    << "     0 don't check counter value." << endl
    << "     1 only display counter value mismatch." << endl
    << "     2 display counter value after every command (default)." << endl
    << "     3 display commands." << endl
    << "     4 display method calls." << endl
    << "   zN   sleep N seconds." << endl
    << "   q    quit (end the client, not the replica(s).)" << endl
    << "   q1   quit (end the client, and shutdown the currently active replica.)" << endl
    << "   ?    help (this message)" << endl;
}

int
FTClientMain::parse_args (int argc, char *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;
  int result = 0;

  // note: dfnkx are simple_util options
  // include them here so we can detect bad args
  ACE_Get_Opt get_opts (argc, argv, "c:f:");
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
          cout << "FT Client: Reading commands from " << this->inFileName_ << endl;
          this->commandIn_ = & this->inFile_;
        }
        else
        {
          cout << "FT Client: Can't open input file: " << this->inFileName_ << endl;
          result = -1;
        }
        break;
      }
      case 'f':
      {
        replica_iors_.push_back(get_opts.opt_arg ());
        break;
      }

      default:
      case '?':
        usage(cerr);
        result = 1;
    }
  }
  return result;
}

void FTClientMain::usage(ostream & out)const
{
  out << "usage"
      << " -c <command file>"
      << " [-f <ior file>]..."
      << endl;
}

int FTClientMain::pass (
  long & counter,
  int & more,
  ACE_CString & command,
  int retry
  ACE_ENV_ARG_DECL)
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
        cout << "FT Client: " << command << endl;
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
            cout << "FT Client: ->set(" << operand << ");" << endl;
          }
          this->replica_->set(operand ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          counter = operand;
          break;
        }
        case 'c':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->get();" << endl;
          }
          long value = this->replica_->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          if (value == operand)
          {
            cout << "FT Client: Good: Read " << value << " expecting " << operand << endl;
            counter = operand;
          }
          else
          {
            cout << "FT Client: Error: Read " << value << " expecting " << operand << endl;
          }
          echo = 0;
          break;

        }
        case '>':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->counter(" << operand << ");" << endl;
          }
          this->replica_->counter(operand ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          counter = operand;
          break;
        }
        case '+':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->increment(" << operand << ");" << endl;
          }
          this->replica_->increment(operand ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          counter += operand;
          break;
        }
        case '-':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->increment(" << -operand << ");" << endl;
          }
          this->replica_->increment(-operand ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          counter -= operand;
          break;
        }
        case '<':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->counter();" << endl;
          }
          long attribute = this->replica_->counter(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          cout << "FT Client: Attribute: " << attribute << endl;
          echo = 0;
          break;
        }
        case '!':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->is_alive();" << endl;
          }
          int alive = this->replica_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          cout << "FT Client: Is alive?  " << alive << endl;
          break;
        }
        case 'd':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->die(" << operand << ");" << endl;
          }
          this->replica_->die(ACE_static_cast (FT_TEST::TestReplica::Bane, operand) ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          echo = 0;
          break;
        }
        case 's':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->get_state();" << endl;
          }
          state = this->replica_->get_state(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          stateValue = counter;
          break;
        }
        case 'S':
        {
          if (state.in() != 0)
          {
            if (this->verbose_ >= LOUD)
            {
              cout << "FT Client: ->set_state(saved_state);" << endl;
            }
            this->replica_->set_state(state ACE_ENV_ARG_PARAMETER);
            ACE_CHECK_RETURN (-1);
            counter = stateValue;
          }
          else
          {
            cout << "FT Client: Error: no saved state." << endl;
          }
          break;
        }
        case 'u':
        {
          if (this->verbose_ >= LOUD)
          {
            cout << "FT Client: ->get_update();" << endl;
          }
          update = this->replica_->get_update(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          updateValue = counter;
          break;
        }
        case 'U':
        {
          if (update.in() != 0)
          {
            if (this->verbose_ >= LOUD)
            {
              cout << "FT Client: ->set_update(saved_update);" << endl;
            }
            this->replica_->set_update(update ACE_ENV_ARG_PARAMETER);
            ACE_CHECK_RETURN (-1);
            counter = updateValue;
          }
          else
          {
            cout << "FT Client: ERROR: No saved update information." << endl;
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
                cout << "FT Client: ->shutdown();" << endl;
              }
              this->replica_->shutdown( ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
              // @@ Note: this is here because the corba event loop seems to go to sleep
              // if there's nothing for it to do.
              // not quite sure why, yet.  Dale
              this->replica_->is_alive(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
            }
            ACE_CATCHANY
            {
              cout << "FT Client: Ignoring expected exception during shutdown." << endl;
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
            cout << "FT Client: Unknown: " << command << endl;
          }
          commandUsage(cerr);
          break;
        }
      }
      if (echo && this->verbose_ >= QUIET)
      {
        if (this->verbose_ >= LOUD)
        {
          cout << "FT Client: ->get();" << endl;
        }

        long value = this->replica_->get(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
        if (value == counter)
        {
          if (this->verbose_ >= NORMAL)
          {
            cout << "FT Client: " << counter << endl;;
          }
        }
        else
        {
          cout << "FT Client: Error: read " << value << " expecting " << counter << endl;
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
    ACE_CHECK_RETURN (0);
    this->replica_ = FT_TEST::TestReplica::_narrow (rep_obj.in ());
    if (! CORBA::is_nil (replica_.in ()))
    {
      result = 1;
    }
    else
    {
      cerr << "FT Client: Can't resolve IOR: " << this->replica_name_ << endl;
    }
  }
  else
  {
    cerr << "***OUT_OF_REPLICAS*** " << this->replica_pos_ << endl;
  }
  return result;
}


int FTClientMain::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;

  this->orb_ = CORBA::ORB_init(this->argc_, this->argv_);

  int ok = next_replica (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (ok)
  {
    // retry information
    ACE_CString command;
    int retry = 0;
    long counter = this->replica_->get(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (this->verbose_ >= NORMAL)
    {
      cout << "FT Client: Initial counter " << counter << endl;
    }
    if (ACE_OS::isatty(fileno(stdin)))
    {
      cout << "FT Client: Commands(? for help):" << endl;
    }

    int more = 1;
    while (more && result == 0 &&  ! this->commandIn_->eof())
    {
      ACE_TRY_NEW_ENV
      {
        result = pass (counter, more, command, retry ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCH (CORBA::SystemException, sysex)
      {
        cout << "FT Client: Caught system exception: " << endl;
        ACE_PRINT_EXCEPTION (sysex, "FT Client");

        retry = 0;
        int handled = 0;

        handled = next_replica(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
        if (handled)
        {
          cout << "FT Client: Recovering from fault." << endl;
          cout << "FT Client:   Activate " << this->replica_name_ << endl;
          if (command.length () == 0)
          {
            cout << "FT Client:   No command to retry." << endl;
          }
          else if (command[0] == 'd')
          {
            cout << "FT Client:   Not retrying \"die\" command." << endl;
          }
          else if (sysex.completed () == CORBA::COMPLETED_YES)
          {
            cout << "FT Client:   Last command completed.  No retry needed." << endl;
          }
          else
          {
            if (sysex.completed () == CORBA::COMPLETED_MAYBE)
            {
              cout << "FT Client:   Last command may have completed.  Retrying anyway." << endl;
            }
            retry = 1;
            cout << "FT Client:   Retrying command: " << command << endl;
          }
        }
        if (! handled)
        {
          cout << "FT Client: Exception not handled.  Rethrow. " << endl;
          ACE_RE_THROW;
        }
      }
      ACE_ENDTRY;
    }
  }
  else
  {
    cerr << "FT Client: Can't connect to replica." << endl;
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
      result = app.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
  template class ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
