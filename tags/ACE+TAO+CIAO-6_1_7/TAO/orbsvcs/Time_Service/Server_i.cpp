// $Id$

#include "Server_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"



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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("do:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ =
          ACE_OS::fopen (get_opts.opt_arg (), ACE_TEXT("a"));

        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t)Unable to open %s for writing: %p\n"),
                             get_opts.opt_arg ()), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t)")
                           ACE_TEXT("usage:  %s")
                           ACE_TEXT(" [-d]")
                           ACE_TEXT(" [-o] <ior_output_file>")
                           ACE_TEXT("\n"),
                           argv[0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialise the Naming Service and register the TimeService Object
// with it.

int
Server_i::init_naming_service ()
{
  // Initialize the Naming Client.
  return (this->naming_client_.init (this->orb_.in ()));
}

// Create a new time server object and register it with the child POA.
// Print the IOR of the registered server on the console and in a file.

int
Server_i::create_server (void)
{
  try
    {

      // Create a new server object.
      ACE_NEW_RETURN (this->time_service_server_impl_,
                      TAO_Time_Service_Server,
                      0);

      // Register a servant with the child poa.
      CORBA::String_var server_str =
        this->orb_manager_.activate_under_child_poa ("server",
                                                     this->time_service_server_impl_);

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("server");

      CORBA::Object_var server_ref =
        this->orb_manager_.child_poa ()->id_to_reference (id.in ());

      this->time_service_server_ = CosTime::TimeService::_narrow (server_ref.in ());

      // All this !! just to register a servant with the child poa.
      // Instead of using _this ().

      //Convert the server reference to a string.

      CORBA::String_var objref_server =
        this->orb_->object_to_string (server_ref.in ());

      // Print the server IOR on the console.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t) The Time Service ")
                  ACE_TEXT("SERVER IOR: <%C>\n"),
                  objref_server.in ()));

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception in Server_i::create_server ()"));
      return -1;
    }
  return 0;
}


// Bind the Server in the context 'ServerContext' with the name
// 'Server:<hostname>'.

int
Server_i::register_server (void)
{
  try
    {
      CosNaming::Name server_context_name;
      server_context_name.length (1);
      server_context_name[0].id = CORBA::string_dup ("ServerContext");

      try
        {
          CosNaming::NamingContext_var server_context =
            this->naming_client_->bind_new_context(server_context_name);
        }
      catch (const CosNaming::NamingContext::AlreadyBound& )
        {
          // OK, naming context already exists.
        }

      char host_name[MAXHOSTNAMELEN];
      char server_mc_name[MAXHOSTNAMELEN];
      ACE_OS::hostname (host_name,MAXHOSTNAMELEN);

      CosNaming::Name server_name (server_context_name);

      server_name.length (2);
      ACE_OS::strcpy (server_mc_name, "Server:");
      ACE_OS::strcat (server_mc_name, host_name);
      server_name[1].id = CORBA::string_dup (server_mc_name);

      // Bind the compound name (ServerContext(Server:<hostname>))
      // to the Naming Server.

      this->naming_client_->rebind (server_name,
                                    this->time_service_server_.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Binding ServerContext -> %C\n"),
                  server_name[1].id.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("(%P|%t) Exception from Register Server ()\n"));
      return -1;
    }

  return 0;
}

// Initialize the server. If a filename is specified with the -f
// commandline option, the server writes its IOR to the file besides
// binding itself with the Naming Service.

int
Server_i::init (int argc,
                ACE_TCHAR *argv[])
{
  try
    {
      // Make a copy of command line parameter.
      ACE_Argv_Type_Converter command(argc, argv);

      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.

      int retval = this->orb_manager_.init_child_poa (
                      command.get_argc(),
                      command.get_TCHAR_argv(),
                      "time_server");

      if (retval == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("%p\n"),
                           ACE_TEXT("init_child_poa")),
                           -1);

      // Activate the POA Manager.
      this->orb_manager_.activate_poa_manager ();

      int result = this->parse_args (command.get_argc(), command.get_TCHAR_argv());

      if (result != 0)
        return result;

      // Get the orb.
      this->orb_ = this->orb_manager_.orb ();

      // Initialize Naming Service.
      this->init_naming_service ();

      // Create the server object.
      this->create_server ();

      // Register the server object with the Naming Service.
      this->register_server ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Exception:"));
      return -1;
    }

  return 0;
}


// Run the event loop for ORB.

int
Server_i::run (void)
{
  int retval = this->orb_manager_.run ();

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t) Server_i::run")),
                      -1);
  return 0;
}
