// $Id$

#include "TAO_IR_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo_Service, TAO_IR_i, "$Id")

// Constructor

TAO_IR_i::TAO_IR_i (void)
{
  // Nothing
}


// Destructor

TAO_IR_i::~TAO_IR_i (void)
{
  // Nothing
}

int 
TAO_IR_i::run ()
{
  return 0;
}

int 
TAO_IR_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("TAO_IR_i::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Go through and figure out which operation we should do.

int
TAO_IR_i::parse_args (void)
{
  if (this->argc_ < 2)
  {
    this->print_usage (ALL);
    return -1;
  }
  
  if (ACE_OS::strcasecmp (this->argv_[1], ASYS_TEXT ("add")) == 0)
  {
    if (this->argc_ < 3)
    {
      this->print_usage (ADD);
      return -1;
    }

    return this->parse_add_args ();
  }
  else if (ACE_OS::strcasecmp (this->argv_[1], ASYS_TEXT ("remove")) == 0)
  {
    if (this->argc_ < 3)
    {
      this->print_usage (REMOVE);
      return -1;
    }
  }
  else if (ACE_OS::strcasecmp (this->argv_[1], ASYS_TEXT ("update")) == 0)
  {
    if (this->argc_ < 3)
    {
      this->print_usage (UPDATE);
      return -1;
    }
  }
  else if (ACE_OS::strcasecmp (this->argv_[1], ASYS_TEXT ("list")) == 0)
  {
    if (this->argc_ > 2)
    {
      ACE_ERROR ((LM_ERROR, "Unrecognized options\n"));
      this->print_usage (LIST);
      return -1;
    }
  }

  // Indicates successful parsing of command line.
  return 0;
}


// Look at the arguments assuming that it is an add command.
int
TAO_IR_i::parse_add_args ()
{
  // Skip both the program name and the "add" command
  ACE_Get_Opt get_opts (this->argc_ - 2, this->argv_ + 2, "hc:w:");

  ACE_DEBUG ((LM_DEBUG, "Server name: %s\n", this->argv_[2]));

  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':  // Command line arguments
        ACE_DEBUG ((LM_DEBUG, "Command line argument: %s\n", get_opts.optarg));
        break;
      case 'w':  // Working Directory
        ACE_DEBUG ((LM_DEBUG, "Working directory: %s\n", get_opts.optarg));
        break;
      case 'h':  // display help
      default:
        this->print_usage (ADD);
        return -1;
      }

  // Success
  return 0;
}

// Print out information about an operation.

void 
TAO_IR_i::print_usage (Operation op)
{
  switch (op)
  {
    case ADD:
      ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] add <name> [command-arguments]\n"));
      ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
      ACE_ERROR ((LM_ERROR, "  where <name> is the POA name used by the server object\n"));
      ACE_ERROR ((LM_ERROR, "  where [command-arguments] can be\n"));
      ACE_ERROR ((LM_ERROR, "    -h            Displays this\n"));
      ACE_ERROR ((LM_ERROR, "    -c command    Startup command\n"));
      ACE_ERROR ((LM_ERROR, "    -w dir        Working directory\n"));
      break;
    case REMOVE:
      ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] remove <name> [command-arguments]\n"));
      ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
      ACE_ERROR ((LM_ERROR, "  where <name> is the POA name used by the server object\n"));
      ACE_ERROR ((LM_ERROR, "  where [command-arguments] can be\n"));
      ACE_ERROR ((LM_ERROR, "    -h            Displays this\n"));
      break;
    case UPDATE:
      ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] update <name> [command-arguments]\n"));
      ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
      ACE_ERROR ((LM_ERROR, "  where <name> is the POA name used by the server object\n"));
      ACE_ERROR ((LM_ERROR, "  where [command-arguments] can be\n"));
      ACE_ERROR ((LM_ERROR, "    -h            Displays this\n"));
      ACE_ERROR ((LM_ERROR, "    -c command    Startup command\n"));
      ACE_ERROR ((LM_ERROR, "    -w dir        Working directory\n"));
      break;
    case LIST:
      ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] list [command-arguments]\n"));
      ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
      ACE_ERROR ((LM_ERROR, "  where [command-arguments] can be\n"));
      ACE_ERROR ((LM_ERROR, "    -h            Displays this\n"));
      break;
    default:
      ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] command [command-arguments]\n"));
      ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
      ACE_ERROR ((LM_ERROR, "  where command is one of the following:\n"));
      ACE_ERROR ((LM_ERROR, "    add      Add an entry to the IR\n"));
      ACE_ERROR ((LM_ERROR, "    remove   Remove an entry from the IR\n"));
      ACE_ERROR ((LM_ERROR, "    update   Update an entry in the IR\n"));
      ACE_ERROR ((LM_ERROR, "    list     List the entries in the IR\n"));
      ACE_ERROR ((LM_ERROR, "  where [command-arguments] depend on the command\n"));
      break;
  }
}
