// $Id$

#include "tao_imr_i.h"

#include "tao/Stub.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// How many servers should we get at once?
const size_t IR_LIST_CHUNK = 10;

// exception return codes
const int NORMAL                  = 0;
const int UNKNOWN                 = 1;
const int NO_PERMISSION           = 2;
const int ALREADY_REGISTERED      = 3;
const int CANNOT_ACTIVATE         = 4;
const int NOT_FOUND               = 5;

// Constructor

TAO_IMR_i::TAO_IMR_i (void)
: implrepo_ (ImplementationRepository::Administration::_nil ()),
  op_ (0)
{
  // Nothing
}


// Destructor

TAO_IMR_i::~TAO_IMR_i (void)
{
  delete this->op_;
}

int
TAO_IMR_i::run ()
{
  if (this->op_ == 0)
  {
    ACE_ERROR ((LM_ERROR, "Unknown operation"));
    return UNKNOWN;
  }

  return this->op_->run ();
}

int
TAO_IMR_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  const char *exception_message = "Null Message";

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet"
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      // Get the ImplRepo object

      CORBA::Object_var implrepo_object =
        orb_->resolve_initial_references ("ImplRepoService" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (implrepo_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to resolve the Implementation Repository.\n"));
          ACE_OS::exit (-1);
        }

      exception_message = "While narrowing ImplRepo";
      this->implrepo_ =
        ImplementationRepository::Administration::_narrow
                               (implrepo_object.in() TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->op_->set_imr (this->implrepo_.in ());
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "TAO_IMR_i::init - %s\n", exception_message));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Go through and figure out which operation we should do.

int
TAO_IMR_i::parse_args (void)
{
  // Make sure one command was given
  if (this->argc_ < 2)
  {
    this->print_usage ();
    return -1;
  }

  this->op_ = TAO_IMR_Op::make_op (this->argv_[1]);

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
TAO_IMR_i::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: tao_imr [options] command [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where command is one of the following:\n"
                        "    activate  Activates a server through the IR\n"
                        "    add       Add an entry to the IR\n"
                        "    autostart Activates all AUTO_START servers\n"
                        "    ior       Creates a simplified IOR\n"
                        "    list      List the entries in the IR\n"
                        "    remove    Remove an entry from the IR\n"
                        "    shutdown  Shuts down a server through the IR\n"
                        "    update    Update an entry in the IR\n"
                        "  where [command-arguments] depend on the command\n"));
}


// Factory for operations

TAO_IMR_Op *
TAO_IMR_Op::make_op (const ACE_TCHAR *op_name)
{
  if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("activate")) == 0)
    return new TAO_IMR_Op_Activate ();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("add")) == 0)
    return new TAO_IMR_Op_Add ();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("autostart")) == 0)
    return new TAO_IMR_Op_Autostart();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("ior")) == 0)
    return new TAO_IMR_Op_IOR();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("list")) == 0)
    return new TAO_IMR_Op_List ();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("remove")) == 0)
    return new TAO_IMR_Op_Remove ();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("shutdown")) == 0)
    return new TAO_IMR_Op_Shutdown ();
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("update")) == 0)
    return new TAO_IMR_Op_Update ();

  return 0;
}


// Sets the implrepo pointer.

void
TAO_IMR_Op::set_imr (ImplementationRepository::Administration_ptr imr)
{
  this->implrepo_ = imr;
}


// ============================================================================
// = Constructors.


TAO_IMR_Op::TAO_IMR_Op (void)
{
  // Nothing
}

TAO_IMR_Op_Activate::TAO_IMR_Op_Activate (void)
{
  // Nothing
}

TAO_IMR_Op_Add::TAO_IMR_Op_Add (void)
: activation_ (ImplementationRepository::NORMAL)
{
  // Nothing
}

TAO_IMR_Op_Autostart::TAO_IMR_Op_Autostart (void)
{
  // Nothing
}

TAO_IMR_Op_IOR::TAO_IMR_Op_IOR (void)
{
  // Nothing
}

TAO_IMR_Op_List::TAO_IMR_Op_List (void)
: verbose_server_information_ (0)
{
  // Nothing
}

TAO_IMR_Op_Remove::TAO_IMR_Op_Remove (void)
{
  // Nothing
}

TAO_IMR_Op_Shutdown::TAO_IMR_Op_Shutdown (void)
{
  // Nothing
}

TAO_IMR_Op_Update::TAO_IMR_Op_Update (void)
: set_command_line_ (0),
  set_working_dir_ (0),
  set_activation_ (0)
{
  // Nothing
}


// ============================================================================
// = Virtual Destructors.


TAO_IMR_Op::~TAO_IMR_Op ()
{
  // Nothing
}

TAO_IMR_Op_Activate::~TAO_IMR_Op_Activate (void)
{
  // Nothing
}

TAO_IMR_Op_Add::~TAO_IMR_Op_Add (void)
{
  // Nothing
}

TAO_IMR_Op_Autostart::~TAO_IMR_Op_Autostart (void)
{
  // Nothing
}

TAO_IMR_Op_IOR::~TAO_IMR_Op_IOR (void)
{
  // Nothing
}

TAO_IMR_Op_List::~TAO_IMR_Op_List (void)
{
  // Nothing
}

TAO_IMR_Op_Remove::~TAO_IMR_Op_Remove (void)
{
  // Nothing
}

TAO_IMR_Op_Shutdown::~TAO_IMR_Op_Shutdown (void)
{
  // Nothing
}

TAO_IMR_Op_Update::~TAO_IMR_Op_Update (void)
{
  // Nothing
}


// ============================================================================
// = Parse methods


int
TAO_IMR_Op_Activate::parse (int argc, ACE_TCHAR **argv)
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

void
TAO_IMR_Op_Add::setenv (ACE_TCHAR *opt)
{
   CORBA::ULong length = this->environment_vars_.length ();

   // Increase the length of the sequence
   this->environment_vars_.length (length + 1);
   ACE_TString tokens (opt);
   int index = tokens.find ("=");
   // Insert at position length since that is our new element
   this->environment_vars_ [length].name =
         CORBA::string_dup (tokens.substr (0, index).c_str ());
   this->environment_vars_ [length].value =
         CORBA::string_dup (tokens.substr (index + 1).c_str ());
}

int
TAO_IMR_Op_Add::parse (int argc, ACE_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
    {
      this->print_usage ();
      return -1;
    }

  // Skip both the program name and the "add" command
  ACE_Get_Opt get_opts (argc, argv, "hc:w:a:e:");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':  // Command line arguments
        this->command_line_ = get_opts.opt_arg ();
        break;
      case 'e':  // set environment variables
        this->setenv( get_opts.opt_arg () ) ;
        break;
      case 'w':  // Working Directory
        this->working_dir_ = get_opts.opt_arg ();
        break;
      case 'a':  // Activation Mode
        if (ACE_OS::strcasecmp (get_opts.opt_arg (), "NORMAL") == 0)
          this->activation_ = ImplementationRepository::NORMAL;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "MANUAL") == 0)
          this->activation_ = ImplementationRepository::MANUAL;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "PER_CLIENT") == 0)
          this->activation_ = ImplementationRepository::PER_CLIENT;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "AUTO_START") == 0)
          this->activation_ = ImplementationRepository::AUTO_START;
        else
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unknown Activation Mode <%s>!\n",
                             get_opts.opt_arg ()),
                            -1);
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
TAO_IMR_Op_Autostart::parse (int argc, ACE_TCHAR **argv)
{
  // Skip the "autostart" command
  ACE_Get_Opt get_opts (argc, argv, "h", 0);

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
TAO_IMR_Op_IOR::parse (int argc, ACE_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
  {
    this->print_usage ();
    return -1;
  }

  // Skip both the program name and the "ior" command
  ACE_Get_Opt get_opts (argc, argv, "hf:");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':  // File name
        this->filename_ = get_opts.opt_arg ();
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
TAO_IMR_Op_List::parse (int argc, ACE_TCHAR **argv)
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
TAO_IMR_Op_Remove::parse (int argc, ACE_TCHAR **argv)
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
TAO_IMR_Op_Shutdown::parse (int argc, ACE_TCHAR **argv)
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

void
TAO_IMR_Op_Update::setenv (ACE_TCHAR *opt)
{
   this->set_environment_vars_ = 1;
   CORBA::ULong length = this->environment_vars_.length ();
   // Increase the length of the sequence
   this->environment_vars_.length (length + 1);
   ACE_TString tokens (opt);
   int index = tokens.find ("=");
   // Insert at position length since that is our new element
   this->environment_vars_ [length].name =
         CORBA::string_dup (tokens.substr (0, index).c_str ());
   this->environment_vars_ [length].value =
         CORBA::string_dup (tokens.substr (index + 1).c_str ());
}

int
TAO_IMR_Op_Update::parse (int argc, ACE_TCHAR **argv)
{
  // Check for enough arguments (we need at least one for the server name)
  if (argc < 1)
    {
      this->print_usage ();
      return -1;
    }

  // Skip both the program name and the "update" command
  ACE_Get_Opt get_opts (argc, argv, "hc:w:a:e:");

  this->server_name_ = argv[0];
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':  // Command line arguments
        this->set_command_line_ = 1;
        this->command_line_ = get_opts.opt_arg ();
        break;
      case 'e':  // set environment variables
        this->setenv( get_opts.opt_arg () );
        break;
      case 'w':  // Working Directory
        this->set_working_dir_ = 1;
        this->working_dir_ = get_opts.opt_arg ();
        break;
      case 'a':  // Activation Mode
        this->set_activation_ = 1;
        if (ACE_OS::strcasecmp (get_opts.opt_arg (), "NORMAL") == 0)
          this->activation_ = ImplementationRepository::NORMAL;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "MANUAL") == 0)
          this->activation_ = ImplementationRepository::MANUAL;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "PER_CLIENT") == 0)
          this->activation_ = ImplementationRepository::PER_CLIENT;
        else if (ACE_OS::strcasecmp (get_opts.opt_arg (), "AUTO_START") == 0)
          this->activation_ = ImplementationRepository::AUTO_START;
        else
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unknown Activation Mode <%s>!\n",
                             get_opts.opt_arg ()),
                            -1);
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
TAO_IMR_Op_Activate::run (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->activate_server
                        (this->server_name_.c_str () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Successfully Activated server <%s>\n", this->server_name_.c_str ()));
    }
  ACE_CATCH (ImplementationRepository::Administration::CannotActivate, ex)
    {
      ACE_ERROR ((LM_ERROR, "Cannot activate server <%s>, reason: <%s>\n",
                            this->server_name_.c_str (),
                            ex.reason.in ()));
      return CANNOT_ACTIVATE;
    }
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return NOT_FOUND;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Activating Server");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  // Success
  return NORMAL;
}

int
TAO_IMR_Op_Add::run (void)
{
  ImplementationRepository::StartupOptions startup_options;

  startup_options.command_line = CORBA::string_dup (this->command_line_.c_str ());
  startup_options.environment = this->environment_vars_;
  startup_options.working_directory = CORBA::string_dup (this->working_dir_.c_str ());
  startup_options.activation = this->activation_;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->register_server
          (this->server_name_.c_str (), startup_options TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Successfully registered server <%s>\n", this->server_name_.c_str ()));
    }
  ACE_CATCH (ImplementationRepository::Administration::AlreadyRegistered, ex)
    {
      ACE_ERROR ((LM_ERROR, "Server <%s> already registered!\n", this->server_name_.c_str ()));
      return ALREADY_REGISTERED;
    }
  ACE_CATCH (CORBA::NO_PERMISSION, ex)
    {
      ACE_ERROR ((LM_ERROR, "No Permission: ImplRepo is in Locked mode\n"));
      return NO_PERMISSION;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Adding server");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  // Success
  return NORMAL;
}

int
TAO_IMR_Op_Autostart::run (void)
{
  ImplementationRepository::ServerInformationList_var server_list;
  ImplementationRepository::ServerInformationIterator_var server_iter;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->list (0, server_list, server_iter TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check for more to be displayed
      if (!CORBA::is_nil (server_iter.in ()))
        {
          int flag = 1;

          while (flag)
            {
              flag = server_iter->next_n (IR_LIST_CHUNK,
                                          server_list
                                          TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              for (size_t i = 0; i < server_list->length (); i++)
                {
                  ACE_TRY_EX (inside)
                    {
                      this->implrepo_->activate_server (server_list[i].server.in ()
                                                        TAO_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK_EX (inside);
                    }
                  ACE_CATCHANY
                    {
                      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                           server_list[i].server.in ());
                      // Ignore exception
                    }
                  ACE_ENDTRY;
                }
            }

          // We are done with the iterator, so it can go away now.
          server_iter->destroy ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "autostart");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  return NORMAL;
}

int
TAO_IMR_Op_IOR::run (void)
{
  ACE_TRY_NEW_ENV
    {
      if (CORBA::is_nil (this->implrepo_)
          || !this->implrepo_->_stubobj ()
          || !this->implrepo_->_stubobj ()->profile_in_use ())
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("Invalid Implementation Repository IOR\n")
            ),
            -1
          );
        }

      CORBA::String_var imr_str =
        this->implrepo_->_stubobj ()->profile_in_use ()->to_string (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Search for "corbaloc:" alone, without the protocol.  This code
      // should be protocol neutral.
      const char corbaloc[] = "corbaloc:";
      char *pos = ACE_OS::strstr (imr_str.inout (), corbaloc);

      if (pos == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not parse IMR IOR\n"),
                          -1);
      else
        {
          pos = ACE_OS::strchr (pos + sizeof (corbaloc), ':');
          pos = ACE_OS::strchr (pos + 1,
                                this->implrepo_->_stubobj ()->profile_in_use ()->object_key_delimiter ());

          if (pos)
            *(pos + 1) = 0;  // Crop the string
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Could not parse IMR IOR\n"),
                              -1);
        }
      ACE_TString ior (imr_str.in ());

      // Add the key
      ior += this->server_name_;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  ior.c_str ()));

      if (this->filename_.length () > 0)
        {
          FILE *file = ACE_OS::fopen (this->filename_.c_str (),
                                      "w");

          if (file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Error: Unable to open %s for writing: %p\n",
                                 this->filename_.c_str ()),
                                -1);
            }

          ACE_OS::fprintf (file,
                           "%s",
                           ior.c_str ());
          ACE_OS::fclose (file);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Ior");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  return NORMAL;
}

int
TAO_IMR_Op_List::run (void)
{
  ImplementationRepository::ServerInformationList_var server_list;
  ImplementationRepository::ServerInformationIterator_var server_iter;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // If there is a server name, list only that server.  Otherwise, look
      // at all of them.
      if (this->server_name_.length () == 0)
        {
          this->implrepo_->list
               (IR_LIST_CHUNK, server_list, server_iter TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          for (size_t i = 0; i < server_list->length (); i++)
            this->display_server_information (server_list[i]);

          // Check for more to be displayed
          if (!CORBA::is_nil (server_iter.in ()))
            {
              int flag = 1;

              while (flag)
                {
                  flag = server_iter->next_n
                           (IR_LIST_CHUNK, server_list TAO_ENV_ARG_PARAMETER);
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

          this->implrepo_->find
             (this->server_name_.c_str (), server_information TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Display verbosely
          this->verbose_server_information_ = 1;

          this->display_server_information (server_information.in ());
        }
    }
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return NOT_FOUND;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "List");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  return NORMAL;
}

int
TAO_IMR_Op_Remove::run (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->remove_server
                         (this->server_name_.c_str () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Successfully removed server <%s>\n", this->server_name_.c_str ()));
    }
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return NOT_FOUND;
    }
  ACE_CATCH (CORBA::NO_PERMISSION, ex)
    {
      ACE_ERROR ((LM_ERROR, "No Permission: ImplRepo is in Locked mode\n"));
      return NO_PERMISSION;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Removing Server");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  // Success
  return NORMAL;
}

int
TAO_IMR_Op_Shutdown::run (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->shutdown_server
                         (this->server_name_.c_str () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Successfully shut down server <%s>\n", this->server_name_.c_str ()));
    }
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>!\n", this->server_name_.c_str ()));
      return NOT_FOUND;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Shutting Down Server");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  // Success
  return NORMAL;
}

int
TAO_IMR_Op_Update::run (void)
{
  ImplementationRepository::ServerInformation_var server_information;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      this->implrepo_->find
        (this->server_name_.c_str (), server_information TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Conditionally update the startup options
      if (this->set_command_line_ == 1)
        server_information->startup.command_line =
          CORBA::string_dup (this->command_line_.c_str ());
      if (this->set_environment_vars_ == 1)
        server_information->startup.environment = this->environment_vars_;
      if (this->set_working_dir_ == 1)
        server_information->startup.working_directory =
          CORBA::string_dup (this->working_dir_.c_str ());
      if (this->set_activation_ == 1)
        server_information->startup.activation = this->activation_;

      // @@ Add logical server support here also

      this->implrepo_->reregister_server (this->server_name_.c_str (),
                                          server_information->startup
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->display_server_information (server_information.in ());
    }
  ACE_CATCH (ImplementationRepository::Administration::NotFound, ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not find server <%s>\n", this->server_name_.c_str ()));
      return NOT_FOUND;
    }
  ACE_CATCH (CORBA::NO_PERMISSION, ex)
    {
      ACE_ERROR ((LM_ERROR, "No Permission: ImplRepo is in Locked mode\n"));
      return NO_PERMISSION;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Updating server");
      return UNKNOWN;
    }
  ACE_ENDTRY;

  // Success
  return NORMAL;
}


// ============================================================================
// = Print Usage methods


void
TAO_IMR_Op_Activate::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Activates a server\n"
                        "\n"
                        "Usage: tao_imr [options] activate <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_Add::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: tao_imr [options] add <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"
                        "    -c command    Startup command\n"
                        "    -w dir        Working directory\n"
                        "    -a mode       Set activate mode (NORMAL|MANUAL|PER_CLIENT|AUTO_START)"));
}

void
TAO_IMR_Op_Autostart::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: tao_imr [options] autostart [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_IOR::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Creates an IOR for a server that is registered with the IMR and uses\n"
                        "the InterOperable Naming Service.  Please see the documentation for\n"
                        "more information on which server configurations work with this command.\n"
                        "\n"
                        "Usage: tao_imr [options] ior <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name of the server\n"
                        "  where [command-arguments] can be\n"
                        "    -f filename   filename to output the IOR to\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_List::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Lists all or one of the servers in the Implementation Repository\n"
                        "\n"
                        "Usage: tao_imr [options] list [name] [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where [name] is the optional server name to search for\n"
                        "  where [command-arguments] can be\n"
                        "    -v            Verbose: Displays more info for each server when\n"
                        "                  displaying more than one server\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_Remove::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Removes a server entry\n"
                        "\n"
                        "Usage: tao_imr [options] remove <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_Shutdown::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Shuts down a server\n"
                        "\n"
                        "Usage: tao_imr [options] shutdown <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"));
}

void
TAO_IMR_Op_Update::print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Updates a server entry\n"
                        "\n"
                        "Usage: tao_imr [options] update <name> [command-arguments]\n"
                        "  where [options] are ORB options\n"
                        "  where <name> is the POA name used by the server object\n"
                        "  where [command-arguments] can be\n"
                        "    -h            Displays this\n"
                        "    -c command    Startup command\n"
                        "    -w dir        Working directory\n"
                        "    -a mode       Set activate mode (NORMAL|MANUAL|PER_CLIENT|AUTO_START)"));
}



// Prints out information in a ServerInformation structure.

void
TAO_IMR_Op::display_server_information (const ImplementationRepository::ServerInformation &info)
{
  // Figure out what the activation string is.
  const char *act = "UNKNOWN STARTUP";
  if (info.startup.activation == ImplementationRepository::NORMAL)
    act = "NORMAL";
  else if (info.startup.activation == ImplementationRepository::MANUAL)
    act = "MANUAL";
  else if (info.startup.activation == ImplementationRepository::PER_CLIENT)
    act = "PER_CLIENT";
  else if (info.startup.activation == ImplementationRepository::AUTO_START)
    act = "AUTO_START";

  // Print out information
  ACE_DEBUG ((LM_DEBUG, "Server <%s>\n", info.server.in ()));
  ACE_DEBUG ((LM_DEBUG,
              "  Command Line: %s\n"
              "  Working Directory: %s\n"
              "  Activation Mode: %s\n",
              info.startup.command_line.in (),
              info.startup.working_directory.in (),
              act));
  for (size_t i = 0; i < info.startup.environment.length (); ++i)
    ACE_DEBUG ((LM_DEBUG, "Environment Variable: %s=%s \n",
                info.startup.environment[i].name.in (),
                info.startup.environment[i].value.in ()));

  // @@ add logical server once implemented


  if (info.startup.activation == ImplementationRepository::PER_CLIENT)
    ACE_DEBUG ((LM_DEBUG, "  No running info available for PER_CLIENT mode\n"));
  else if (ACE_OS::strlen (info.location) > 0)
    ACE_DEBUG ((LM_DEBUG,
                "  Running at endpoint: %s\n",
                info.location.in ()));
  else   // I am assuming that a blank location means currently not running.
    ACE_DEBUG ((LM_DEBUG,
                "  Not currently running\n"));
}


// ============================================================================
// = Display Server Information methods

void
TAO_IMR_Op_List::display_server_information (const ImplementationRepository::ServerInformation &info)
{
  if (this->verbose_server_information_)
    TAO_IMR_Op::display_server_information (info);
  else
    ACE_DEBUG ((LM_DEBUG, "<%s>\n", info.server.in ()));
}
