// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "Clerk_i.h"

ACE_RCSID(Time_Service, Clerk_i, "$Id$")

// Constructor.

Clerk_i::Clerk_i (void)
  : ior_output_file_ (0),
    timer_value_ (3),
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
  this->ior_fp_ = ACE_OS::fopen (filename, "r");

  if ((this->ior_fp_ = ACE_OS::fopen (filename, "r")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Clerk_i::read_ior () "
                         "Failed to open the IOR file for reading\n"),
                        -1);
    }

  // @@ Vishal, please sure you don't use magic numbers like 1024 *
  // 10.  Instead, add a const or an enum to the Clerk_i class.  Check
  // other applications in TAO that read IORs and see if there's a
  // default size for these things.  Please update Andy's stuff to
  // also use a const.

  char str[1024 * 10];
  ACE_OS::memset (str, 0, sizeof (str));

  while (fscanf (this->ior_fp_, "%s", str) != EOF)
    {
      // Get the IOR and insert it into the set of server IORs.
      TAO_TRY
        {
          ACE_DEBUG ((LM_DEBUG,
                      "iors -> %s\n",
                      str));
          CORBA::Object_var objref =
            this->orb_->string_to_object (str,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Return if the server reference is nil.
          if (CORBA::is_nil (objref.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "IOR for the server is Null\n"),
                              -1);

          CosTime::TimeService_ptr server =
            CosTime::TimeService::_narrow (objref.in (),
                                           TAO_TRY_ENV);
          TAO_CHECK_ENV;
          this->insert_server (server);
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("Exception");
        }
      TAO_ENDTRY;
    }
  return 0;

}

// Parse the command-line arguments and set options.

int
Clerk_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "dt:f:o:");

  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 't':  // time after which the clerk should update time.
        this->timer_value_ = atoi (get_opts.optarg);
        break;

      case 'f':  // read the server IORs from a file.
        result = this->read_ior (get_opts.optarg);

        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "[CLERK] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;

     case 'o':  // output the Clerk IOR to a file.
       this->ior_output_file_ =
         ACE_OS::fopen (get_opts.optarg, "w");

       if (this->ior_output_file_ == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "[SERVER] Process/Thread Id : (%P/%t)Unable to open %s for writing: %p\n",
                            get_opts.optarg), -1);
       break;

      case '?':  // display help for use of the server.
        /* FALLTHRU */
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[SERVER] Process/Thread Id : (%P/%t)"
                           "usage:  %s"
                           " [-d]"
                           " [-t] <Timer value>"
                           " [-f] <ior_input_file>"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Yet to be integrated and tested.
/*
int
Clerk_i::init_IR (void)
{
  TAO_TRY
    {

    ACE_NEW_RETURN (this->ir_helper_, IR_Helper ( "my_child_poa",
    this->orb_manager_.child_poa (),
    this->orb_manager_.orb (),
    TAO_debug_level),
    -1);
      this->ir_helper_->register_server ("clerk -t 5");

      this->ir_helper_->change_object (this->time_service_clerk_,
                                       TAO_TRY_ENV);

      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      // Convert the IR clerk reference to a string.
      CORBA::String_var objref_clerk =
        this->orb_manager_.orb ()->object_to_string (this->time_service_clerk_.in (),
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      // Print the IR clerk IOR on the console.
      ACE_DEBUG ((LM_DEBUG,
                  "[SERVER] Process/Thread Id : (%P/%t) The Time Service IREPO CLERK IOR is: <%s>\n",
                  objref_clerk.in ()));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception:");
    }
  TAO_ENDTRY;

  return 0;
}
*/

// Get a reference to the Server Naming context and the first IOR.
// The iterator returned from this is used to get the next n IORs.

int
Clerk_i::get_first_IOR (void)
{
  TAO_TRY
    {
      char host_name[MAXHOSTNAMELEN];
      //char machine_clerk_name[MAXHOSTNAMELEN];
      int index = 0;

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
        this->my_name_server_->resolve (server_context_name,
                                        TAO_TRY_ENV);

      TAO_CHECK_ENV;

      CosNaming::NamingContext_var server_context =
        CosNaming::NamingContext::_narrow (temp_object.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
      server_name[0].id = bindings_list[0].binding_name[0].id;

      temp_object =
        server_context->resolve (server_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosTime::TimeService_var obj =
        CosTime::TimeService::_narrow (temp_object.in (),
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Get the next n IORs of the time servers active in the Network and
// registered with the Naming Service. This is done by iterating over
// the naming context.

int
Clerk_i::next_n_IORs (CosNaming::BindingIterator_var iter,
                      CosNaming::NamingContext_var server_context)
{
  TAO_TRY
    {
      CosNaming::Binding_var binding;

      if (!CORBA::is_nil (iter.in ()))
        {
          while (iter->next_one (binding.out (),
                                 TAO_TRY_ENV))
            {
              TAO_CHECK_ENV;

              ACE_DEBUG ((LM_DEBUG,
                          "Getting IOR of the server: %s\n\n",
                          binding->binding_name[0].id.in ()));

              CosNaming::Name server_name;
              server_name.length (1);
              server_name[0].id = binding->binding_name[0].id;

              CORBA::Object_var temp_object =
                server_context->resolve (server_name,
                                         TAO_TRY_ENV);
              TAO_CHECK_ENV;

              CosTime::TimeService_ptr server =
                CosTime::TimeService::_narrow (temp_object.in (),
                                               TAO_TRY_ENV);
              TAO_CHECK_ENV;

              this->insert_server (server);
            }

          TAO_CHECK_ENV;
        }

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Unexpected exception in next_n_IORs\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Initialise the Naming Service.

int
Clerk_i::init_naming_service (CORBA::Environment& env)
{
  TAO_TRY
    {
      // Initialize the POA.
      this->orb_manager_.init_child_poa (this->argc_,
                                         this->argv_,
                                         "my_child_poa",
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Create an instance of the clerk with appropriate parameters.

int
Clerk_i::create_clerk (void)
{
  TAO_TRY
    {

      // Create a new clerk object. Pass it the timer value, the set
      // of server IORs and the no. of servers.
      ACE_NEW_RETURN (this->time_service_clerk_impl_,
                      TAO_Time_Service_Clerk (this->timer_value_,
                                              this->server_),
                      0);
      // Generate IOR of the Clerk and register with POA.
      this->time_service_clerk_ =
        this->time_service_clerk_impl_->_this ();

      // Convert the clerk reference to a string.
      CORBA::String_var objref_clerk =
        this->orb_->object_to_string (this->time_service_clerk_.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int 
Clerk_i::if_first_clerk (CosNaming::Name clerk_context_name)
{
	TAO_TRY
	{
		this->my_name_server_->resolve
			 (clerk_context_name, TAO_TRY_ENV);
		TAO_CHECK_ENV;
	}
	TAO_CATCH (CORBA::UserException, userex)
	{
      ACE_UNUSED_ARG (userex);
	  printf("First clerk\n");
      //TAO_TRY_ENV.print_exception ("User Exception");
      return 1;
    }
	TAO_ENDTRY;
	return 0;
}

// Binds the clerk in the context ClerkContext with the name
// Clerk:<hostname>.

int
Clerk_i::register_clerk (void)
{
  TAO_TRY
    {
      // Bind the Clerk in its appropriate Context.

      CosNaming::Name clerk_context_name;
      clerk_context_name.length (1);
      clerk_context_name[0].id = CORBA::string_dup ("ClerkContext");

	  
	  CosNaming::NamingContext_var clerk_context;

     // if (CORBA::is_nil (this->my_name_server_->resolve
       //                  (clerk_context_name, TAO_TRY_ENV)))
	  if (if_first_clerk (clerk_context_name))
            {
              //TAO_TRY_ENV.clear ();

              clerk_context = this->my_name_server_->new_context (TAO_TRY_ENV);
              TAO_CHECK_ENV;

              this->my_name_server_->rebind_context (clerk_context_name,
                                                     clerk_context.in (),
                                                     TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }

      //TAO_CHECK_ENV;

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
                                   this->time_service_clerk_.in (),
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
 TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("(%P|%t) Exception from init_naming_service ()\n");
    }
 TAO_ENDTRY;

  return 0;
}

// Initialize the Clerk.

int
Clerk_i::init (int argc,
               char *argv[],
               CORBA::Environment &env)
{
  TAO_TRY
    {
      this->argc_ = argc;
      this->argv_ = argv;

      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      this->orb_manager_.init (argc,
                               argv,
                               TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      if (this->orb_manager_.init_child_poa (argc,
                                             argv,
                                             "child_poa",
                                             TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init_child_poa"),
                          -1);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

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
          if (this->init_naming_service (TAO_TRY_ENV) !=0 )
            return -1;

          // Set the size of the Server IOR Array.
          this->server_.max_size (10);
          this->server_.size (0);

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
        if (this->register_clerk () != 0)
          return -1;

      // Close the open file handler.
      // ACE_OS::fclose (this->ior_fp_);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("(%P|%t) Exception in Clerk_i::init ()\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Clerk_i::run (CORBA::Environment &env)
{
  TAO_TRY
    {
      // Run the main event loop for the ORB.
      if (this->orb_manager_.run (TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[SERVER] Process/Thread Id : (%P/%t) Clerk_i::run"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("(%P|%t) Exception in Clerk_i::run ()\n");
    }
  TAO_ENDTRY;

  return 0;
}

void
Clerk_i::insert_server (CosTime::TimeService_ptr server)
{

  // We duplicate the capacity of the Array.
  size_t s = this->server_.size ();

  if (this->server_.max_size () == s)
    {
      this->server_.max_size (2 * s);
    }
  this->server_[s] = CosTime::TimeService::_duplicate (server);
  this->server_.size (s + 1);
}
