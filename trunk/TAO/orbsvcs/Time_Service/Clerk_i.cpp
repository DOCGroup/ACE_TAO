// $Id$

#include "Clerk_i.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Time_Service, Clerk_i, "$Id$")

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
Clerk_i::read_ior (const char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  else
    this->ior_fp_ = 1;

  ACE_Read_Buffer ior_buffer (f_handle);

  char *data = ior_buffer.read (EOF,'\n','\n');
  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior: %p\n"),
                      -1);

  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (char *str = ACE_OS::strtok (data, "\n");
           str != 0 ;
           str = ACE_OS::strtok (0, "\n"))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "iors -> |%s|\n",
                      str));

          CORBA::Object_var objref =
            this->orb_->string_to_object (str
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Return if the server reference is nil.
          if (CORBA::is_nil (objref.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "IOR for the server is Null\n"));
              result = -1;
              break;
            }

          CosTime::TimeService_ptr server =
            CosTime::TimeService::_narrow (objref.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->insert_server (server);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
    }
  ACE_ENDTRY;

  ACE_OS::close (f_handle);
  ior_buffer.alloc ()->free (data);

  return result;
}

// Parse the command-line arguments and set options.

int
Clerk_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "dt:u:f:o:");

  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 't':  // time in secs after which the clerk should update time.
        this->timer_value_ = atoi (get_opts.opt_arg ());
        break;

      case 'u':
        // time in usecs after which the clerk should update time.
        // Continues the precision of the -t option.
        this->timer_value_usecs_ = atoi (get_opts.opt_arg ());
        break;

      case 'f':  // read the server IORs from a file.
        result = this->read_ior (get_opts.opt_arg ());

        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "[CLERK] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;

     case 'o':  // output the Clerk IOR to a file.
       this->ior_output_file_ =
         ACE_OS::fopen (get_opts.opt_arg (), "w");

       if (this->ior_output_file_ == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "[SERVER] Process/Thread Id : (%P/%t)Unable to open %s for writing: %\n",
                            get_opts.opt_arg ()), -1);
       break;

      case '?':  // display help for use of the server.
        /* FALLTHRU */
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[SERVER] Process/Thread Id : (%P/%t)"
                           "usage:  %s"
                           " [-d]"
                           " [-t] <Timer value inn Secs>"
                           " [-u] <Timer value in uSecs>"
                           " [-f] <ior_input_file>"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
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
        this->my_name_server_->resolve (server_context_name
                                        ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      CosNaming::NamingContext_var server_context =
        CosNaming::NamingContext::_narrow (temp_object.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server_context.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Time_Service_Clerk::get_server_IORs:"
                    "No Active Servers in the Network\n"));

      // Get the first element and an iterator over the other
      // elements.
      server_context->list (1,
                            bindings_list.out (),
                            iter.out ());
      CosNaming::Name server_name;
      server_name.length (1);
      server_name[0].id = bindings_list[0u].binding_name[0].id;

      temp_object =
        server_context->resolve (server_name
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosTime::TimeService_var obj =
        CosTime::TimeService::_narrow (temp_object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLERK] Process/Thread Id : (%P/%t) Unable to Resolve "
                           "Server Reference\n"),
                          -1);

      // Insert the first server IOR into the unbounded set of server
      // IORs.

      this->insert_server (obj.in ());

      // Iterate over the server context to get the next N IORs.
      if (next_n_IORs (iter,
                       server_context) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLERK] Process/Thread Id : (%P/%t) Unable to get next N IORs "),
                          -1);;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Get the next n IORs of the time servers active in the Network and
// registered with the Naming Service. This is done by iterating over
// the naming context.

int
Clerk_i::next_n_IORs (CosNaming::BindingIterator_var iter,
                      CosNaming::NamingContext_var server_context)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CosNaming::Binding_var binding;

      if (!CORBA::is_nil (iter.in ()))
        {
          while (iter->next_one (binding.out ()
                                 ACE_ENV_ARG_PARAMETER))
            {
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "Getting IOR of the server: %s\n\n",
                          binding->binding_name[0].id.in ()));

              CosNaming::Name server_name;
              server_name.length (1);
              server_name[0].id = binding->binding_name[0].id;

              CORBA::Object_var temp_object =
                server_context->resolve (server_name
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              CosTime::TimeService_ptr server =
                CosTime::TimeService::_narrow (temp_object.in ()
                                               ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              this->insert_server (server);
            }

          ACE_TRY_CHECK;
        }

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception in next_n_IORs\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Initialise the Naming Service.

int
Clerk_i::init_naming_service (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the POA.
      this->orb_manager_.init_child_poa (this->argc_,
                                         this->argv_,
                                         "my_child_poa"
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Create an instance of the clerk with appropriate parameters.

int
Clerk_i::create_clerk (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
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
        this->orb_->object_to_string (this->time_service_clerk_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Print the clerk IOR on the console.
      ACE_DEBUG ((LM_DEBUG,
                  "[SERVER] Process/Thread Id : (%P/%t) The Time Service CLERK IOR is: <%s>\n",
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Check if this is the first Clerk to bind to the Naming
// Service. If yes, then 1 is returned else 0 is returned.

int
Clerk_i::if_first_clerk (CosNaming::Name clerk_context_name)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->my_name_server_->resolve
        (clerk_context_name ACE_ENV_ARG_PARAMETER);
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

// Binds the clerk in the context ClerkContext with the name
// Clerk:<hostname>.

int
Clerk_i::register_clerk (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Bind the Clerk in its appropriate Context.

      CosNaming::Name clerk_context_name;
      clerk_context_name.length (1);
      clerk_context_name[0].id = CORBA::string_dup ("ClerkContext");

      CosNaming::NamingContext_var clerk_context;

      if (if_first_clerk (clerk_context_name))
        {
          clerk_context = this->my_name_server_->new_context (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->my_name_server_->rebind_context (clerk_context_name,
                                                 clerk_context.in ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      char host_name[MAXHOSTNAMELEN];
      char clerk_mc_name[MAXHOSTNAMELEN];

      ACE_OS::hostname (host_name,
                        MAXHOSTNAMELEN);

      //CosNaming::Name clerk_name (clerk_context_name);
      CosNaming::Name clerk_name;
      clerk_name.length (2);

      ACE_OS::strcpy (clerk_mc_name, "Clerk:");
      ACE_OS::strcat (clerk_mc_name, host_name);

      clerk_name[0].id = CORBA::string_dup ("ClerkContext");
      clerk_name[1].id = CORBA::string_dup (clerk_mc_name);

      this->my_name_server_->rebind (clerk_name,
                                     this->time_service_clerk_.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Exception from init_naming_service ()\n");
    }
  ACE_ENDTRY;

  return 0;
}

// Initialize the Clerk.

int
Clerk_i::init (int argc,
               char *argv[]
               ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->argc_ = argc;
      this->argv_ = argv;

      // Set the size of the Server IOR Array.
      this->server_.max_size (10);
      this->server_.size (0);

      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.


      this->orb_manager_.init (argc,
                               argv
                               ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      if (this->orb_manager_.init_child_poa (argc,
                                             argv,
                                             "child_poa"
                                             ACE_ENV_ARG_PARAMETER) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init_child_poa"),
                          -1);
      ACE_TRY_CHECK;

      // Get the ORB.
      this->orb_ = this->orb_manager_.orb ();

      // Parse commandline arguments.
      if (this->parse_args () !=0 )
        return -1;

      // If IOR file has not been specified then try the Naming
      // Service.

      if (!this->ior_fp_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "IOR file not specified. Using the Naming Service instead\n"));

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Exception in Clerk_i::init ()\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Clerk_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      // Run the main event loop for the ORB.
      int r = this->orb_manager_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (r == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[SERVER] Process/Thread Id : (%P/%t) Clerk_i::run"),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Exception in Clerk_i::run ()\n");
    }
  ACE_ENDTRY;

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
