// $Id$

#include "tao_ir_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo_Service, tao_ir_i, "$Id:")

// How many servers should we get at once?
const size_t IR_LIST_CHUNK = 4;


// Constructor

TAO_IR_i::TAO_IR_i (void)
: implrepo_ (ImplementationRepository::Administration::_nil ()),
  op_ (0)
{
  // Nothing
}


// Destructor

TAO_IR_i::~TAO_IR_i (void)
{
  delete this->op_;
}

int 
TAO_IR_i::run ()
{
  if (this->op_ == 0)
  {
    ACE_ERROR ((LM_ERROR, "Unknown operation"));
    return -1;
  }
  
  return this->op_->run ();
}

int 
TAO_IR_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  const char *exception_message = "Null Message";

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the ImplRepo object

      CORBA::Object_var implrepo_object =
        orb_->resolve_initial_references ("ImplRepoService");

      if (CORBA::is_nil (implrepo_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to resolve the Implementation Repository.\n"));
          ACE_OS::exit (-1);
        }

      exception_message = "While narrowing ImplRepo";
      this->implrepo_ =
        ImplementationRepository::Administration::_narrow (implrepo_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "TAO_IR_i::init - %s\n", exception_message));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Go through and figure out which operation we should do.

int
TAO_IR_i::parse_args (void)
{
  // Make sure one command was given
  if (this->argc_ < 2)
  {
    this->print_usage ();
    return -1;
  }
  
  this->op_ = TAO_IR_Op::make_op (this->argv_[1], this->implrepo_.in ());
  
  // Check for unrecognized operation

  if (this->op_ == 0)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: Unrecognized command: <%s>\n", this->argv_[1]));
    this->print_usage ();
    return -1;    
  }

  // Adjust argc and argv so only the command specific args are passed
  return this->op_->parse (this->argc_ - 2, this->argv_ + 2);
}


// Print out information about all operations.

void 
TAO_IR_i::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] command [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where command is one of the following:\n"
                        "    activate Activates a server through the IR\n"
                        "    add      Add an entry to the IR\n"
                        "    list     List the entries in the IR\n"
                        "    remove   Remove an entry from the IR\n"
                        "    shutdown Shuts down a server through the IR\n"
                        "    update   Update an entry in the IR\n"
                        "  where [command-arguments] depend on the command\n"));
}


// Factory for operations

TAO_IR_Op *
TAO_IR_Op::make_op (const ASYS_TCHAR *op_name, ImplementationRepository::Administration_ptr ir)
{
  if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("activate")) == 0)
    return new TAO_IR_Op_Activate (ir);
  else if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("add")) == 0)
    return new TAO_IR_Op_Add (ir);
  else if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("list")) == 0)
    return new TAO_IR_Op_List (ir);
  else if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("remove")) == 0)
    return new TAO_IR_Op_Remove (ir);
  else if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("shutdown")) == 0)
    return new TAO_IR_Op_Shutdown (ir);
  else if (ACE_OS::strcasecmp (op_name, ASYS_TEXT ("update")) == 0)
    return new TAO_IR_Op_Update (ir);
  
  return 0;
}
 

// ============================================================================
// = Constructors.


TAO_IR_Op::TAO_IR_Op (ImplementationRepository::Administration_ptr implrepo)
: implrepo_ (implrepo)
{
  // Nothing
}

TAO_IR_Op_Activate::TAO_IR_Op_Activate (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo)
{
  // Nothing
}

TAO_IR_Op_Add::TAO_IR_Op_Add (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo)
{
  // Nothing
}

TAO_IR_Op_List::TAO_IR_Op_List (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo),
  verbose_server_information_ (0)
{
  // Nothing
}

TAO_IR_Op_Remove::TAO_IR_Op_Remove (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo)
{
  // Nothing
}

TAO_IR_Op_Shutdown::TAO_IR_Op_Shutdown (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo)
{
  // Nothing
}

TAO_IR_Op_Update::TAO_IR_Op_Update (ImplementationRepository::Administration_ptr implrepo)
: TAO_IR_Op (implrepo),
  set_command_line_ (0),
  set_working_dir_ (0)
{
  // Nothing
}


// ============================================================================
// = Virtual Destructors.


TAO_IR_Op::~TAO_IR_Op ()
{
  // Nothing
}

TAO_IR_Op_Activate::~TAO_IR_Op_Activate (void)
{
  // Nothing
}

TAO_IR_Op_Add::~TAO_IR_Op_Add (void)
{
  // Nothing
}

TAO_IR_Op_List::~TAO_IR_Op_List (void)
{
  // Nothing
}

TAO_IR_Op_Remove::~TAO_IR_Op_Remove (void)
{
  // Nothing
}

TAO_IR_Op_Shutdown::~TAO_IR_Op_Shutdown (void)
{
  // Nothing
}

TAO_IR_Op_Update::~TAO_IR_Op_Update (void)
{
  // Nothing
}


// ============================================================================
// = Parse methods


int 
TAO_IR_Op_Activate::parse (int argc, ASYS_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "activate" command
  ACE_Get_Opt get_opts (argc, argv, "h");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}

int
TAO_IR_Op_Add::parse (int argc, ASYS_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "add" command
  ACE_Get_Opt get_opts (argc, argv, "hc:w:");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':  // Command line arguments
        this->command_line_ = get_opts.optarg;
        break;
      case 'w':  // Working Directory
        this->working_dir_ = get_opts.optarg;
        break;
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}

int
TAO_IR_Op_List::parse (int argc, ASYS_TCHAR **argv)
{
  int server_flag = 0;

  if (argc > 0 && argv[0][0] != '-')
  {
    this->server_name_ = argv[0];
    server_flag = 1;
  }
  
  // Skip both the program name and the "list" command
  ACE_Get_Opt get_opts (argc, argv, "vh", server_flag);

  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v': // verbose server display
        this->verbose_server_information_ = 1;
        break;
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}

int 
TAO_IR_Op_Remove::parse (int argc, ASYS_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "remove" command
  ACE_Get_Opt get_opts (argc, argv, "h");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}

int 
TAO_IR_Op_Shutdown::parse (int argc, ASYS_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "shutdown" command
  ACE_Get_Opt get_opts (argc, argv, "h");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}

int 
TAO_IR_Op_Update::parse (int argc, ASYS_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "update" command
  ACE_Get_Opt get_opts (argc, argv, "hc:w:");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':  // Command line arguments
        this->set_command_line_ = 1;
        this->command_line_ = get_opts.optarg;
        break;
      case 'w':  // Working Directory
        this->set_working_dir_ = 1;
        this->working_dir_ = get_opts.optarg;
        break;
      case 'h':  // display help
      default:
        this->print_usage ();
        return -1;
      }

  // Success
  return 0;
}


// ============================================================================
// = Run methods


int
TAO_IR_Op_Activate::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->implrepo_->activate_server (this->server_name_.c_str (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG, "Successfully Activated server <%s>\n", this->server_name_.c_str ()));
    } 
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Activating Server");
      return -1;
    }
  ACE_ENDTRY;
  
  // Success
  return 0;
}

int
TAO_IR_Op_Add::run (void)
{
  ImplementationRepository::StartupOptions startup_options;

  startup_options.command_line = CORBA::string_dup (this->command_line_.c_str ());
  // @@ add environment
  startup_options.working_directory = CORBA::string_dup (this->working_dir_.c_str ());

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->implrepo_->register_server (this->server_name_.c_str (), startup_options, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG, "Successfully registered server <%s>\n", this->server_name_.c_str ()));
    } 
  ACE_CATCH (ImplementationRepository::Administration::AlreadyRegistered, ex)
    {
      ACE_ERROR ((LM_ERROR, "Server <%s> already registered!\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Adding server");
      return -1;
    }
  ACE_ENDTRY;
  
  // Success
  return 0;
}

int
TAO_IR_Op_List::run (void)
{
  ImplementationRepository::ServerInformationList_var server_list;
  ImplementationRepository::ServerInformationIterator_var server_iter;
  
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If there is a server name, list only that server.  Otherwise, look
      // at all of them.
      if (this->server_name_.length () == 0)
        {
          this->implrepo_->list (IR_LIST_CHUNK, server_list, server_iter, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          for (size_t i = 0; i < server_list->length (); i++)
            this->display_server_information (server_list[i]);

          // Check for more to be displayed
          if (!CORBA::is_nil (server_iter.in ()))
          {
            int flag = 1;

            while (flag)
              {
                flag = server_iter->next_n (IR_LIST_CHUNK, server_list, ACE_TRY_ENV);
                ACE_TRY_CHECK;

                for (size_t i = 0; i < server_list->length (); i++)
                  this->display_server_information (server_list[i]);
              }

            // We are done with the iterator, so it can go away now.
            server_iter->destroy ();
          }
      }
      else
        {
          ImplementationRepository::ServerInformation_var server_information;

          this->implrepo_->find (this->server_name_.c_str (), server_information, ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          // Display verbosely
          this->verbose_server_information_ = 1;

          this->display_server_information (server_information.in ());
      }
    } 
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "List");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int
TAO_IR_Op_Remove::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->implrepo_->remove_server (this->server_name_.c_str (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG, "Successfully removed server <%s>\n", this->server_name_.c_str ()));
    } 
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Removing Server");
      return -1;
    }
  ACE_ENDTRY;
  
  // Success
  return 0;
}

int
TAO_IR_Op_Shutdown::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->implrepo_->shutdown_server (this->server_name_.c_str (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG, "Successfully shut down server <%s>\n", this->server_name_.c_str ()));
    } 
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Shutting Down Server");
      return -1;
    }
  ACE_ENDTRY;
  
  // Success
  return 0;
}

int
TAO_IR_Op_Update::run (void)
{
  ImplementationRepository::ServerInformation_var server_information;
  
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->implrepo_->find (this->server_name_.c_str (), server_information, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Conditionally update the startup options
      if (this->set_command_line_ == 1)
        server_information->startup.command_line = 
          CORBA::string_dup (this->command_line_.c_str ());
      if (this->set_working_dir_ == 1)
        server_information->startup.working_directory = 
          CORBA::string_dup (this->working_dir_.c_str ());
      // @@ add environment and logical server

      this->implrepo_->reregister_server (this->server_name_.c_str (), 
                                          server_information->startup, 
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      this->display_server_information (server_information.in ());
    } 
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>\n", this->server_name_.c_str ()));
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Updating server");
      return -1;
    }
  ACE_ENDTRY;
  
  // Success
  return 0;
}


// ============================================================================
// = Print Usage methods


void
TAO_IR_Op_Activate::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Activates a server\n"
                        "\n"
                        "Usage: tao_ir [options] activate <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IR_Op_Add::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: tao_ir [options] add <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"
                        "    -c command    Startup command\n"
                        "    -w dir        Working directory\n"));
}

void
TAO_IR_Op_List::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Lists all or one of the servers in the Implementation Repository\n"
                        "\n"
                        "Usage: tao_ir [options] list [name] [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where [name] is the optional server name to search for\n"
                        "  where [command-arguments] can be\n"
                        "    -v            Verbose: Displays more info for each server when\n"
                        "                  displaying more than one server\n"
                        "    -h            Displays this\n"));
}

void
TAO_IR_Op_Remove::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Removes a server entry\n"
                        "\n"
                        "Usage: tao_ir [options] remove <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IR_Op_Shutdown::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Shuts down a server\n"
                        "\n"
                        "Usage: tao_ir [options] shutdown <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IR_Op_Update::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Updates a server entry\n"
                        "\n"
                        "Usage: tao_ir [options] update <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"
                        "    -c command    Startup command\n"
                        "    -w dir        Working directory\n"));
}



// Prints out information in a ServerInformation structure.

void
TAO_IR_Op::display_server_information (const ImplementationRepository::ServerInformation &info)
{
  // Print out information
  ACE_DEBUG ((LM_DEBUG, "Server <%s>\n", info.server.in ()));
  ACE_DEBUG ((LM_DEBUG, 
              "  Command Line: %s\n"
              "  Working Directory: %s\n",
              info.startup.command_line.in (),
              info.startup.working_directory.in ()));
  // @@ add environment and logical server once implemented

  // I am assuming that a blank host means currently not running.
  if (ACE_OS::strlen (info.location.host) > 0)
    ACE_DEBUG ((LM_DEBUG, 
                "  Running at \n"
                "    Host: %s\n"
                "    Port: %d\n",
                info.location.host.in (),
                info.location.port));
  else
    ACE_DEBUG ((LM_DEBUG, 
                "  Not currently running\n"));
}


// ============================================================================
// = Display Server Information methods

void
TAO_IR_Op_List::display_server_information (const ImplementationRepository::ServerInformation &info)
{
  if (this->verbose_server_information_)
    TAO_IR_Op::display_server_information (info);
  else
    ACE_DEBUG ((LM_DEBUG, "<%s>\n", info.server.in ()));
}
