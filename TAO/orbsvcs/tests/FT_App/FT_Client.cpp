// $Id$

#include "FT_TestReplicaC.h"
#include <ace/Vector_T.h>
#include <ace/SString.h>
#include <ace/Get_Opt.h>
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

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
  int parse_args (int argc, ACE_TCHAR *argv[]);

  ////////////
  // execution
  int run (void);

  /////////////////
  // implementation
private:
  void usage (FILE* out)const;
  void commandUsage (FILE* out);
  int pass (
    long & counter,         // inout
    int & more,             // out
    ACE_CString & command,  // inout
    int retry               // in
    );


  int next_replica (void);

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
  ACE_TCHAR ** argv_;
  const ACE_TCHAR * inFileName_;
  FILE *inFile_;
  FILE *commandIn_;

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
  : inFile_(0)
  , commandIn_(stdin)
  , verbose_(NORMAL)
  , replica_pos_(0)
  , replica_name_("none")
{
}

FTClientMain::~FTClientMain ()
{
  if (this->inFile_)
  {
    ACE_OS::fclose (this->inFile_);
    this->inFile_ = 0;
  }
}

void FTClientMain::commandUsage(FILE* out)
{
  ACE_OS::fprintf (out,
    "Each command must be at the beginning of a separate line.\n"
    "Everything after the command (and operand if any) is ignored.\n"
    "Valid commands are:\n"
    "  Access via method call:\n"
    "    =N    set counter to N\n"
    "    cN    get counter and compare to N (c stands for \"check\"\n"
    "    +N    increment counter by N\n"
    "    -N    decrement counter by N\n"
    "  Access as attribute:\n"
    "    >N    set attribute to N\n"
    "    <     get attribite\n"
    "  Try methods to be used by fault tolerant infrastructure:\n"
    "    !     is_alive\n"
    "    s     get_state\n"
    "    S     set_state\n"
    "    u     get_update\n"
    "    U     set_update\n"
    "  Simulate failure:\n"
    "    dN    die on condition:\n"
    "        d%d don't die\n"
    "        d%d immediately\n"
    "        d%d while idle\n"
    "      (FT_TestReplica interface)\n"
    "        d%d before state change\n"
    "        d%d after state change, before replication\n"
    "        d%d after replication, before reply\n"
    "      (Monitorable interface)\n"
    "        d%d during is alive\n"
    "        d%d is_alive returns false\n"
    "      (Updatable interface)\n"
    "        d%d during get update\n"
    "        d%d before set update\n"
    "        d%d after set update\n"
    "      (Checkpointable interface)\n"
    "        d%d during get state\n"
    "        d%d before set state\n"
    "        d%d after set state\n"
    " Logistics commands:\n"
    "   #    ignore this line (comment).\n"
    "   v    set verbosity:\n"
    "     0 don't check counter value.\n"
    "     1 only display counter value mismatch.\n"
    "     2 display counter value after every command (default).\n"
    "     3 display commands.\n"
    "     4 display method calls.\n"
    "   zN   sleep N seconds.\n"
    "   q    quit (end the client, not the replica(s).)\n"
    "   q1   quit (end the client, and shutdown the currently active replica.)\n"
    "   ?    help (this message)\n",
    FT_TEST::TestReplica::NOT_YET,
    FT_TEST::TestReplica::RIGHT_NOW,
    FT_TEST::TestReplica::WHILE_IDLE,
    FT_TEST::TestReplica::BEFORE_STATE_CHANGE,
    FT_TEST::TestReplica::BEFORE_REPLICATION,
    FT_TEST::TestReplica::BEFORE_REPLY,
    FT_TEST::TestReplica::DURING_IS_ALIVE,
    FT_TEST::TestReplica::DENY_IS_ALIVE,
    FT_TEST::TestReplica::DURING_GET_UPDATE,
    FT_TEST::TestReplica::BEFORE_SET_UPDATE,
    FT_TEST::TestReplica::AFTER_SET_UPDATE,
    FT_TEST::TestReplica::DURING_GET_STATE,
    FT_TEST::TestReplica::BEFORE_SET_STATE,
    FT_TEST::TestReplica::AFTER_SET_STATE);
}

int
FTClientMain::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;
  int result = 0;

  // note: dfnkx are simple_util options
  // include them here so we can detect bad args
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:f:"));
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'c':
      {
        this->inFileName_ = get_opts.opt_arg ();
        this->inFile_ = ACE_OS::fopen (this->inFileName_, "r");
        if(this->inFile_ && !ferror (this->inFile_))
        {
          ACE_OS::fprintf (stdout, "FT Client: Reading commands from %s\n", this->inFileName_);
          this->commandIn_ = this->inFile_;
        }
        else
        {
          ACE_OS::fprintf (stderr, "FT Client: Can't open input file: %s\n", this->inFileName_);
          result = -1;
        }
        break;
      }
      case 'f':
      {
        replica_iors_.push_back(ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }

      default:
      case '?':
        usage(stderr);
        result = 1;
    }
  }
  return result;
}

void FTClientMain::usage(FILE* out)const
{
  ACE_OS::fprintf (out, "usage"
                   " -c <command file>"
                   " [-f <ior file>]...\n");
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

  while(more && result == 0 &&  !feof (this->commandIn_))
  {
    if (! retry || command.length () == 0 )
    {
      char buffer[1000];
      char *str_ = ACE_OS::fgets (buffer, sizeof(buffer)-1, this->commandIn_);
      if ( str_ )
      {
        str_ = ACE_OS::strrchr (str_, '\n');
        if ( str_ )
          *str_ = '\0';
      }
      command = buffer;
    }
    retry = 0;

    if (command.length() >0)
    {
      char op = command[0];
      ACE_CString cdr = command.substr(1);
      char * junque;
      long operand = ACE_OS::strtol(cdr.c_str(),&junque, 10);

      if (this->verbose_ >= NOISY)
      {
        ACE_OS::fprintf (stdout,  "FT Client: %s\n", command.c_str());
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
            ACE_OS::fprintf (stdout, "FT Client: ->set(%ld);\n", operand);
          }
          this->replica_->set(operand);
          counter = operand;
          break;
        }
        case 'c':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->get();\n");
          }
          long value = this->replica_->counter();
          if (value == operand)
          {
            ACE_OS::fprintf (stdout, "FT Client: Good: Read %ld expecting %ld\n", value, operand);
            counter = operand;
          }
          else
          {
            ACE_OS::fprintf (stdout, "FT Client: Error: Read %ld expecting %ld\n", value, operand);
          }
          echo = 0;
          break;

        }
        case '>':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->counter(%ld);\n", operand);
          }
          this->replica_->counter(operand);
          counter = operand;
          break;
        }
        case '+':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->increment(%ld);\n", operand);
          }
          this->replica_->increment(operand);
          counter += operand;
          break;
        }
        case '-':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->increment(%ld);\n", -operand);
          }
          this->replica_->increment(-operand);
          counter -= operand;
          break;
        }
        case '<':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->counter();\n");
          }
          long attribute = this->replica_->counter();
          ACE_OS::fprintf (stdout, "FT Client: Attribute: %ld\n", attribute);
          echo = 0;
          break;
        }
        case '!':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->is_alive();\n");
          }
          int alive = this->replica_->is_alive();
          ACE_OS::fprintf (stdout, "FT Client: Is alive?  %d\n", alive);
          break;
        }
        case 'd':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->die(%ld);\n", operand);
          }
          this->replica_->die(static_cast<FT_TEST::TestReplica::Bane> (operand));
          echo = 0;
          break;
        }
        case 's':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->get_state();\n");
          }
          state = this->replica_->get_state();
          stateValue = counter;
          break;
        }
        case 'S':
        {
          if (state->length () > 0)
          {
            if (this->verbose_ >= LOUD)
            {
              ACE_OS::fprintf (stdout, "FT Client: ->set_state(saved_state);\n");
            }
            this->replica_->set_state (state.in ());
            counter = stateValue;
          }
          else
          {
            ACE_OS::fprintf (stdout, "FT Client: Error: no saved state.\n");
          }
          break;
        }
        case 'u':
        {
          if (this->verbose_ >= LOUD)
          {
            ACE_OS::fprintf (stdout, "FT Client: ->get_update();\n");
          }
          update = this->replica_->get_update();
          updateValue = counter;
          break;
        }
        case 'U':
        {
          if (update->length () > 0)
          {
            if (this->verbose_ >= LOUD)
            {
              ACE_OS::fprintf (stdout, "FT Client: ->set_update(saved_update);\n");
            }
            this->replica_->set_update(update.in ());
            counter = updateValue;
          }
          else
          {
            ACE_OS::fprintf (stdout, "FT Client: ERROR: No saved update information.\n");
          }
          break;
        }
        case 'v':
        {
          this->verbose_ = static_cast<Verbosity> (operand);
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
            try
            {
              if (this->verbose_ >= LOUD)
              {
                ACE_OS::fprintf (stdout, "FT Client: ->shutdown();\n");
              }
              this->replica_->shutdown();
              // @@ Note: this is here because the corba event loop seems to go to sleep
              // if there's nothing for it to do.
              // not quite sure why, yet.  Dale
              this->replica_->is_alive();
            }
            catch (const CORBA::Exception&)
            {
              ACE_OS::fprintf (stdout, "FT Client: Ignoring expected exception during shutdown.\n");
              ; // ignore exception during shutdown
            }
          }
          echo = 0;
          more = 0;
          break;
        }
        default:
        {
          if (op != '?')
          {
            ACE_OS::fprintf (stdout, "FT Client: Unknown: %s\n", command.c_str());
          }
          commandUsage(stderr);
          break;
        }
      }
      if (echo && this->verbose_ >= QUIET)
      {
        if (this->verbose_ >= LOUD)
        {
          ACE_OS::fprintf (stdout, "FT Client: ->get();\n");
        }

        long value = this->replica_->get();
        if (value == counter)
        {
          if (this->verbose_ >= NORMAL)
          {
            ACE_OS::fprintf (stdout, "FT Client: %ld\n", counter);
          }
        }
        else
        {
          ACE_OS::fprintf (stdout, "FT Client: Error: read %ld expecting %ld\n", value, counter);
          result = -1;
        }
      }
    }
  }
  return result;
}

int FTClientMain::next_replica (void)
{
  int result = 0;
  if (this->replica_pos_ < this->replica_iors_.size())
  {
    this->replica_name_ = this->replica_iors_[this->replica_pos_].c_str();
    this->replica_pos_ += 1;
    CORBA::Object_var rep_obj = this->orb_->string_to_object (this->replica_name_);
    this->replica_ = FT_TEST::TestReplica::_narrow (rep_obj.in ());
    if (! CORBA::is_nil (replica_.in ()))
    {
      result = 1;
    }
    else
    {
      ACE_OS::fprintf (stderr, "FT Client: Can't resolve IOR: %s\n", this->replica_name_);
    }
  }
  else
  {
    ACE_OS::fprintf (stderr,
                     "***OUT_OF_REPLICAS*** "
                     ACE_SIZE_T_FORMAT_SPECIFIER_ASCII
                     "\n",
                     this->replica_pos_);
  }
  return result;
}


int FTClientMain::run (void)
{
  int result = 0;

  this->orb_ = CORBA::ORB_init(this->argc_, this->argv_);

  int ok = next_replica ();
  if (ok)
  {
    // retry information
    ACE_CString command;
    int retry = 0;
    long counter = this->replica_->get();

    if (this->verbose_ >= NORMAL)
    {
      ACE_OS::fprintf (stdout, "FT Client: Initial counter %ld\n", counter);
    }
    if (ACE_OS::isatty(ACE_OS::fileno(stdin)))
    {
      ACE_OS::fprintf (stdout, "FT Client: Commands(? for help):\n");
    }

    int more = 1;
    while (more && result == 0 &&  !feof (this->commandIn_))
    {
      try
      {
        result = pass (counter, more, command, retry);
      }
      catch (const CORBA::SystemException& sysex)
      {
        ACE_OS::fprintf (stdout, "FT Client: Caught system exception:\n");
        sysex._tao_print_exception ("FT Client");

        retry = 0;
        int handled = 0;

        handled = next_replica();
        if (handled)
        {
          ACE_OS::fprintf (stdout, "FT Client: Recovering from fault.\n");
          ACE_OS::fprintf (stdout, "FT Client:   Activate %s\n", this->replica_name_);
          if (command.length () == 0)
          {
            ACE_OS::fprintf (stdout, "FT Client:   No command to retry.\n");
          }
          else if (command[0] == 'd')
          {
            ACE_OS::fprintf (stdout, "FT Client:   Not retrying \"die\" command.\n");
          }
          else if (sysex.completed () == CORBA::COMPLETED_YES)
          {
            ACE_OS::fprintf (stdout, "FT Client:   Last command completed.  No retry needed.\n");
          }
          else
          {
            if (sysex.completed () == CORBA::COMPLETED_MAYBE)
            {
              ACE_OS::fprintf (stdout, "FT Client:   Last command may have completed.  Retrying anyway.\n");
            }
            retry = 1;
            ACE_OS::fprintf (stdout, "FT Client:   Retrying command: %s\n", command.c_str());
          }
        }
        if (! handled)
        {
          ACE_OS::fprintf (stdout, "FT Client: Exception not handled.  Rethrow. ");
          throw;
        }
      }
    }
  }
  else
  {
    ACE_OS::fprintf (stderr, "FT Client: Can't connect to replica.\n");
  }
  return result;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  FTClientMain app;
  int result = app.parse_args(argc, argv);
  if (result == 0)
  {
    try
    {
      result = app.run ();
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("FT_Client::main\t\n");
      result = -1;
    }
  }
  return result;
}
