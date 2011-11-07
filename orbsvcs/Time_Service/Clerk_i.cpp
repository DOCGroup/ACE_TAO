// $Id$

#include "Clerk_i.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/os_include/os_netdb.h"



// Constructor.

Clerk_i::Clerk_i (void)
  : ior_output_file_ (0),
    timer_value_ (3),
    timer_value_usecs_ (0),
    server_ (Clerk_i::DEFAULT_SERVER_COUNT),
    ior_fp_ (0)
{
  // no-op.
}

// Destructor.

Clerk_i::~Clerk_i (void)
{
  // no-op.
}

// Reads the Time Service Server iors from a file instead of using a
// naming service.

int
Clerk_i::read_ior (const ACE_TCHAR* filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("[CLIENT] Process/Thread Id : (%P/%t) Unable to open %s for writing: %p\n"),
                       filename),
                      -1);
  else
    this->ior_fp_ = 1;

  ACE_Read_Buffer ior_buffer (f_handle);

  char *data = ior_buffer.read (EOF,'\n','\n');
  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior: %p\n")),
                      -1);

  int result = 0;

  try
    {
      for (char *str = ACE_OS::strtok (data, "\n");
           str != 0 ;
           str = ACE_OS::strtok (0, "\n"))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("iors -> |%C|\n"),
                      str));

          CORBA::Object_var objref =
            this->orb_->string_to_object (str);

          // Return if the server reference is nil.
          if (CORBA::is_nil (objref.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("IOR for the server is Null\n")));
              result = -1;
              break;
            }

          CosTime::TimeService_ptr server =
            CosTime::TimeService::_narrow (objref.in ());

          this->insert_server (server);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Exception"));
    }

  ACE_OS::close (f_handle);
  ior_buffer.alloc ()->free (data);

  return result;
}

// Parse the command-line arguments and set options.

int
Clerk_i::parse_args (int argc,
                     ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dt:u:f:o:"));

  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 't':  // time in secs after which the clerk should update time.
        this->timer_value_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'u':
        // time in usecs after which the clerk should update time.
        // Continues the precision of the -t option.
        this->timer_value_usecs_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'f':  // read the server IORs from a file.
        result = this->read_ior (get_opts.opt_arg ());

        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("[CLERK] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n"),
                             get_opts.opt_arg ()),
                            -1);
        break;

     case 'o':  // output the Clerk IOR to a file.
       this->ior_output_file_ =
         ACE_OS::fopen (get_opts.opt_arg (), ACE_TEXT("w"));

       if (this->ior_output_file_ == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t)Unable to open %s for writing: %\n"),
                            get_opts.opt_arg ()), -1);
       break;

      case '?':  // display help for use of the server.
        /* FALLTHRU */
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t)")
                           ACE_TEXT("usage:  %s")
                           ACE_TEXT(" [-d]")
                           ACE_TEXT(" [-t] <Timer value inn Secs>")
                           ACE_TEXT(" [-u] <Timer value in uSecs>")
                           ACE_TEXT(" [-f] <ior_input_file>")
                           ACE_TEXT(" [-o] <ior_output_file>")
                           ACE_TEXT("\n"),
                           argv[0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Get a reference to the Server Naming context and the first IOR.
// The iterator returned from this is used to get the next n IORs.

int
Clerk_i::get_first_IOR (void)
{
  try
    {
      char host_name[MAXHOSTNAMELEN];

      ACE_OS::hostname (host_name,
                        MAXHOSTNAMELEN);
      CosNaming::BindingList_var bindings_list;
      CosNaming::BindingIterator_var iter;

      // Construct the server context name.
      CosNaming::Name server_context_name;
      server_context_name.length (1);
      server_context_name[0].id = CORBA::string_dup ("ServerContext");

      // Resolve name.
      CORBA::Object_var temp_object =
        this->naming_client_->resolve (server_context_name);


      CosNaming::NamingContext_var server_context =
        CosNaming::NamingContext::_narrow (temp_object.in ());

      if (CORBA::is_nil (server_context.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO_Time_Service_Clerk::get_server_IORs:")
                    ACE_TEXT("No Active Servers in the Network\n")));

      // Get the first element and an iterator over the other
      // elements.
      server_context->list (1,
                            bindings_list.out (),
                            iter.out ());
      CosNaming::Name server_name;
      server_name.length (1);
      server_name[0].id = bindings_list[0u].binding_name[0].id;

      temp_object =
        server_context->resolve (server_name);

      CosTime::TimeService_var obj =
        CosTime::TimeService::_narrow (temp_object.in ());

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("[CLERK] Process/Thread Id : (%P/%t) Unable to Resolve ")
                           ACE_TEXT("Server Reference\n")),
                          -1);

      // Insert the first server IOR into the unbounded set of server
      // IORs.

      this->insert_server (obj.in ());

      // Iterate over the server context to get the next N IORs.
      if (next_n_IORs (iter,
                       server_context) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("[CLERK] Process/Thread Id : (%P/%t) Unable to get next N IORs ")),
                          -1);;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Exception"));
      return -1;
    }

  return 0;
}

// Get the next n IORs of the time servers active in the Network and
// registered with the Naming Service. This is done by iterating over
// the naming context.

int
Clerk_i::next_n_IORs (CosNaming::BindingIterator_var iter,
                      CosNaming::NamingContext_var server_context)
{
  try
    {
      CosNaming::Binding_var binding;

      if (!CORBA::is_nil (iter.in ()))
        {
          while (iter->next_one (binding.out ()))
            {

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("Getting IOR of the server: %C\n\n"),
                          binding->binding_name[0].id.in ()));

              CosNaming::Name server_name;
              server_name.length (1);
              server_name[0].id = binding->binding_name[0].id;

              CORBA::Object_var temp_object =
                server_context->resolve (server_name);

              CosTime::TimeService_ptr server =
                CosTime::TimeService::_narrow (temp_object.in ());

              this->insert_server (server);
            }

        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Unexpected exception in next_n_IORs\n"));
      return -1;
    }

  return 0;
}

// Initialise the Naming Service.

int
Clerk_i::init_naming_service ()
{
  // Initialize the Naming Client.
  return (this->naming_client_.init (this->orb_.in ()));
}

// Create an instance of the clerk with appropriate parameters.

int
Clerk_i::create_clerk (void)
{
  try
    {

      // Create a new clerk object. Pass it the timer value, the set
      // of server IORs and the no. of servers.
      ACE_NEW_RETURN (this->time_service_clerk_impl_,
                      TAO_Time_Service_Clerk (this->timer_value_,
                                              this->timer_value_usecs_,
                                              this->server_),
                      0);

      // Generate IOR of the Clerk and register with POA.
      this->time_service_clerk_ =
        this->time_service_clerk_impl_->_this ();

      // Convert the clerk reference to a string.
      CORBA::String_var objref_clerk =
        this->orb_->object_to_string (this->time_service_clerk_.in ());

      // Print the clerk IOR on the console.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t) The Time Service CLERK IOR is: <%C>\n"),
                  objref_clerk.in ()));

      // Print the Time Service clerk IOR to a file.
      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           objref_clerk.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

      // Register the clerk implementation with the Interface
      // Repository.  init_IR();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Exception"));
      return -1;
    }

  return 0;
}

// Binds the clerk in the context ClerkContext with the name
// Clerk:<hostname>.

int
Clerk_i::register_clerk (void)
{
  try
    {
      // Bind the Clerk in its appropriate Context.

      CosNaming::Name clerk_context_name;
      clerk_context_name.length (1);
      clerk_context_name[0].id = CORBA::string_dup ("ClerkContext");

      try
        {
          CosNaming::NamingContext_var clerk_context =
            this->naming_client_->bind_new_context(clerk_context_name);
        }
      catch (const CosNaming::NamingContext::AlreadyBound& )
        {
          // OK, naming context already exists.
        }

      char host_name[MAXHOSTNAMELEN];
      char clerk_mc_name[MAXHOSTNAMELEN];
      ACE_OS::hostname (host_name, MAXHOSTNAMELEN);

      //CosNaming::Name clerk_name (clerk_context_name);
      CosNaming::Name clerk_name;
      clerk_name.length (2);

      ACE_OS::strcpy (clerk_mc_name, "Clerk:");
      ACE_OS::strcat (clerk_mc_name, host_name);

      clerk_name[0].id = CORBA::string_dup ("ClerkContext");
      clerk_name[1].id = CORBA::string_dup (clerk_mc_name);

      this->naming_client_->rebind (clerk_name,
                                     this->time_service_clerk_.in ());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("(%P|%t) Exception from init_naming_service ()\n"));
    }

  return 0;
}

// Initialize the Clerk.

int
Clerk_i::init (int argc,
               ACE_TCHAR *argv[])
{
  try
    {
      // Make a copy of command line parameter.
      ACE_Argv_Type_Converter command(argc, argv);

      // Set the size of the Server IOR Array.
      this->server_.max_size (10);
      this->server_.size (0);

      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      this->orb_manager_.init (command.get_argc(),
                               command.get_TCHAR_argv());

      if (this->orb_manager_.init_child_poa (command.get_argc(),
                                             command.get_TCHAR_argv(),
                                             "child_poa") == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("%p\n"),
                           ACE_TEXT("init_child_poa")),
                          -1);

      // Get the ORB.
      this->orb_ = this->orb_manager_.orb ();

      // Parse commandline arguments.
      if (this->parse_args (command.get_argc(), command.get_TCHAR_argv()) !=0 )
        return -1;

      // If IOR file has not been specified then try the Naming
      // Service.

      if (!this->ior_fp_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("IOR file not specified. Using the Naming Service instead\n")));

          // Initialize the Naming Service.
          if (this->init_naming_service () !=0 )
            return -1;

          // Get a reference to the Server Naming context and the
          // first IOR.
          if (this->get_first_IOR () != 0)
            return -1;

        }

      // Create an instance of the Clerk.
      if (this->create_clerk () != 0)
        return -1;


      // Register the clerk with the Naming Service.
      if (this->ior_fp_ == 0)
        {
          if (this->register_clerk () != 0)
            return -1;
        }

      // Close the open file handler.
      // ACE_OS::fclose (this->ior_fp_);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("(%P|%t) Exception in Clerk_i::init ()\n"));
      return -1;
    }

  return 0;
}

int
Clerk_i::run (void)
{
  try
    {
      // Run the main event loop for the ORB.
      int r = this->orb_manager_.run ();

      if (r == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("[SERVER] Process/Thread Id : (%P/%t) Clerk_i::run")),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("(%P|%t) Exception in Clerk_i::run ()\n"));
    }

  return 0;
}

void
Clerk_i::insert_server (CosTime::TimeService_ptr server)
{
  // We duplicate the capacity of the Array.
  size_t s = this->server_.size ();

  if (this->server_.max_size () == s)
    this->server_.max_size (2 * s);

  this->server_[s] =
    CosTime::TimeService::_duplicate (server);

  this->server_.size (s + 1);
}
