// $Id$

#include "Server_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Time_Service, Server_i, "$Id$")

// Constructor.
Server_i::Server_i (void)
  : ior_output_file_ (0)
{
  // no-op.
}

// Destructor.

Server_i::~Server_i (void)
{
  // no-op.
}

// Parse the command-line arguments and set options.

int
Server_i::parse_args (int argc,
                      ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_LIB_TEXT("do:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ =
          ACE_OS::fopen (get_opts.opt_arg (), ACE_LIB_TEXT("a"));

        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT("[SERVER] Process/Thread Id : (%P/%t)Unable to open %s for writing: %p\n"),
                             get_opts.opt_arg ()), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT("[SERVER] Process/Thread Id : (%P/%t)")
                           ACE_LIB_TEXT("usage:  %s")
                           ACE_LIB_TEXT(" [-d]")
                           ACE_LIB_TEXT(" [-o] <ior_output_file>")
                           ACE_LIB_TEXT("\n"),
                           argv[0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialise the Naming Service and register the TimeService Object
// with it.

int
Server_i::init_naming_service (int argc,
                               char* argv[]
                               ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      // Initialize the POA.
      this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_ptr child_poa
        = this->orb_manager_.child_poa ();

      // Initialize the Naming Server. Note the Naming Server cannot
      // be initialized with the Root POA because it has to be a
      // persistent object reference.  Hence the need for child
      // POA. The servants need not be registered in the same POA. We
      // use the Root POA for the servants.

      if (this->my_name_server_.init (this->orb_.in (),
                                      child_poa) == -1)
        return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_LIB_TEXT("(%P|%t) Exception from init_naming_service ()\n"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Create a new time server object and register it with the child POA.
// Print the IOR of the registered server on the console and in a file.

int
Server_i::create_server (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // Create a new server object.
      ACE_NEW_RETURN (this->time_service_server_impl_,
                      TAO_Time_Service_Server,
                      0);

      // Register a servant with the child poa.
      CORBA::String_var server_str =
        this->orb_manager_.activate_under_child_poa ("server",
                                                     this->time_service_server_impl_
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("server");

      CORBA::Object_var server_ref =
        this->orb_manager_.child_poa ()->id_to_reference (id.in ()
                                                          ACE_ENV_ARG_PARAMETER);

      this->time_service_server_ = CosTime::TimeService::_narrow (server_ref.in ()
                                                                  ACE_ENV_ARG_PARAMETER);


      ACE_TRY_CHECK;

      // All this !! just to register a servant with the child poa.
      // Instead of using _this ().

      //Convert the server reference to a string.

      CORBA::String_var objref_server =
        this->orb_->object_to_string (server_ref.in ()
                                      ACE_ENV_ARG_PARAMETER);

      // Print the server IOR on the console.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT("[SERVER] Process/Thread Id : (%P/%t) The Time Service ")
                  ACE_LIB_TEXT("SERVER IOR: <%s>\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(objref_server.in ())));

      // Print the IOR to a file.

      if (this->ior_output_file_)
        {
          // Write the IOR to the file.
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s\n",
                           objref_server.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_LIB_TEXT("Exception in Server_i::create_server ()"));
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}


// This function checks if this is the first server being executed. If yes,
// the call to resolve raises an exception and 1 is returned. If there is
// no exception 0 is returned.

int
Server_i::if_first_server (CosNaming::Name &server_context_name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->my_name_server_->resolve
        (server_context_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// Bind the Server in the context 'ServerContext' with the name
// 'Server:<hostname>'.

int
Server_i::register_server (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CosNaming::Name server_context_name;
      server_context_name.length (1);
      server_context_name[0].id = CORBA::string_dup ("ServerContext");

      CosNaming::NamingContext_var server_context;

      // If this is the first server then get a new Naming Context
      // and bind it to the Naming Server.

      if (if_first_server (server_context_name))
        {
          // Get context.
          server_context =
            this->my_name_server_->new_context (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Bind.
          this->my_name_server_->rebind_context (server_context_name,
                                                 server_context.in ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      char host_name[MAXHOSTNAMELEN];
      char server_mc_name[MAXHOSTNAMELEN];
      ACE_OS::hostname (host_name,MAXHOSTNAMELEN);

      CosNaming::Name server_name (server_context_name);

      server_name.length (2);
      strcpy (server_mc_name, "Server:");
      strcat (server_mc_name, host_name);
      server_name[1].id = CORBA::string_dup (server_mc_name);

      // Bind the compound name (ServerContext(Server:<hostname>))
      // to the Naming Server.

      this->my_name_server_->rebind (server_name,
                                     this->time_service_server_.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT("Binding ServerContext -> %s\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(server_name[1].id.in ())));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_LIB_TEXT("(%P|%t) Exception from Register Server ()\n"));
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Initialize the server. If a filename is specified with the -f
// commandline option, the server writes its IOR to the file besides
// binding itself with the Naming Service.

int
Server_i::init (int argc,
                ACE_TCHAR *argv[]
                ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // Make a copy of command line parameter.
      ACE_Argv_Type_Converter command(argc, argv);

      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.

      if (this->orb_manager_.init_child_poa (command.get_argc(),
                                             command.get_ASCII_argv(),
                                             "time_server"
                                             ACE_ENV_ARG_PARAMETER) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT("%p\n"),
                           ACE_LIB_TEXT("init_child_poa")),
                           -1);
      ACE_TRY_CHECK;

      // Activate the POA Manager.
      this->orb_manager_.activate_poa_manager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result = this->parse_args (command.get_argc(), command.get_TCHAR_argv());

      if (result != 0)
        return result;

      // Get the orb.
      this->orb_ = this->orb_manager_.orb ();

      // Use the Naming Service Register the above implementation with the Naming Service.
      this->init_naming_service (command.get_argc(),
                                 command.get_ASCII_argv()
                                 ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      // Create the server object.
      this->create_server ();

      // Register the server object with the Naming Service.
      this->register_server ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, ACE_LIB_TEXT("Exception:"));
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Run the event loop for ORB.

int
Server_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int retval = this->orb_manager_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT("[SERVER] Process/Thread Id : (%P/%t) Server_i::run")),
                      -1);
  return 0;
}
