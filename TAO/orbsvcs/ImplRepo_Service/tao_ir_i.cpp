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

int
TAO_IR_i::parse_args (void)
{
  if (this->argc_ < 2)
  {
    ACE_ERROR ((LM_ERROR, 
                "Usage: %s [options] command [command-arguments]\n",
                this->argv[0]));
    ACE_ERROR ((LM_ERROR, "  where [options] are ORB options\n"));
    ACE_ERROR ((LM_ERROR, "  where command is:\n"));
    ACE_ERROR ((LM_ERROR, "    add    - Add an entry to the IR\n"));
    ACE_ERROR ((LM_ERROR, "    remove - Remove an entry from the IR\n"));
    ACE_ERROR ((LM_ERROR, "    update - Update an entry in the IR\n"));
    ACE_ERROR ((LM_ERROR, "    list   - List the entries in the IR\n"));
    ACE_ERROR ((LM_ERROR, "  where [command-arguments] depend on the command\n"));
    return -1;
  }
  
  if (ACE_OS::stricmp (this->argv[1], ASYS_TEXT ("add")))
  {
  }
  else if (ACE_OS::stricmp (this->argv[1], ASYS_TEXT ("remove")))
  {
  }
  else if (ACE_OS::stricmp (this->argv[1], ASYS_TEXT ("update")))
  {
  }
  else if (ACE_OS::stricmp (this->argv[1], ASYS_TEXT ("list")))
  {
  }

  // Indicates successful parsing of command line.
  return 0;
}

