//$Id$
#ifndef SIMPLE_UTIL_C
#define SIMPLE_UTIL_C
# include "Simple_util.h"

// Constructor.

template <class Servant>
Server<Servant>::Server (void)
    : ior_output_file_ (0)
{
  // no-op.
}

// Destructor.

template <class Servant>
Server<Servant>::~Server (void)
{
}

// Parse the command-line arguments and set options.

template <class Servant> int 
Server<Servant>::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:n");
  int c = 0;
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;

      case 'n': //Use naming service
        this->naming_ = 1;
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           " [-n] "
                           "\n",
                           argv_ [0]),
                          -1);
      }
  
  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.
template <class Servant> int 
Server<Servant>::init (const char *servant_name, 
                       int argc, 
                       char *argv[], 
                       CORBA::Environment &ACE_TRY_ENV)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
	
  ACE_CHECK_RETURN (-1);
  
  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;
  
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  if (this->naming_ == 1)
    {

      // Save name in case we use TAO Naming Service.
      name = servant_name;	
      
      // Call naming service 
      if (this->register_name () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n Naming Service\n"),-1);
      return 0;
    }
  // Activate the servant in its own child POA.

  // Make sure that you check for failures here via the ACE_TRY
  // macros?!
  ACE_TRY
    {
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa (servant_name,
                                                     &this->servant_,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

     ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n",
                  str.in ()));
     if (this->ior_output_file_)
       {
         ACE_OS::fprintf (this->ior_output_file_,
                          "%s",
                          str.in ());
         ACE_OS::fclose (this->ior_output_file_);
       }

    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("\tException in activation of POA");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

template <class Servant>int 
Server<Servant>::run (CORBA::Environment &env)
{
    // Run the main event loop for the ORB.
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);
  
  return 0;
}

template <class Servant> int 
Server<Servant>::register_name (void)
{
  this->namingServer.init (this->orb_manager_.orb(),
                           this->orb_manager_.child_poa ());
  // create the name for the naming service
  
  CosNaming::Name bindName;
  bindName.length (1);
  bindName[0].id = CORBA::string_dup (name);

  // (re)Bind the object.
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object = servant_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      namingServer->rebind (bindName,
                            object.in(),       
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;  
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ex)
    {
      ACE_TRY_ENV.clear ();
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Unable to bind %s \n",
                         name),
                        -1);
    }
  ACE_ENDTRY;

  return 0;
}

// Constructor.

template <class InterfaceObj, class Var>
Client<InterfaceObj, Var>::Client (void)
  : ior_ (0)
{
  //no-op
}

// Reads the Server ior from a file

template <class InterfaceObj, class Var> int 
Client<InterfaceObj, Var>::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  
  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);
  
  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

template <class InterfaceObj, class Var> int 
Client<InterfaceObj, Var>::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:nk:x");
  int c = 0;
  int result = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'n': // Use naming service
        this->naming_ = 1;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;
      case 'x': // read the flag for shutting down
        this->shutdown_ = 1;
        break;
      }
  
  // Indicates successful parsing of command line.
  return 0;
}

template <class InterfaceObj, class Var>
Client<InterfaceObj, Var>::~Client (void)
{
  ACE_OS::free (this->ior_);
}

template <class InterfaceObj, class Var> int 
Client<InterfaceObj, Var>::init (const char *name,
                                 int argc, 
                                 char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  
  
  ACE_TRY_NEW_ENV
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    0,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;
      


      if(this->ior_ != 0)
        {
          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_, ACE_TRY_ENV);
          ACE_TRY_CHECK;
      
      
          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               this->ior_),
                              -1);      
          this->server_ = InterfaceObj::_narrow (server_object.in (),
                                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else if (this->naming_ == 1)
        { 
          // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     "Using the Naming Service \n"));
          this->name_ = ACE_const_cast (char *, name);
          this->obtain_initial_references (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "no ior or naming options  specified\n"),
                          -1);
        
      
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  ACE_ENDTRY;
  
  
  return 0;
}


template <class InterfaceObj, class Var> int 
Client<InterfaceObj, Var>::obtain_initial_references (CORBA::Environment &ACE_TRY_ENV)
{
  
  ACE_TRY
    {
      // Initialize the naming services.
      if (namingClient.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name server_name (1);
      server_name.length (1);
      server_name[0].id =
      CORBA::string_dup (this->name_);

      CORBA::Object_var obj =
        namingClient->resolve (server_name,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      this->server_ = InterfaceObj::_narrow (obj.in (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Bank::obtain_initial_references");
      return -1;
    }
  ACE_ENDTRY;
  
  return 0;
}

template <class InterfaceObj, class Var> int 
Client<InterfaceObj, Var>::shutdown (void )
{
  // Returns the shutdwon flag
  return shutdown_;
}

template <class InterfaceObj, class Var> void  
Client<InterfaceObj, Var>::shutdown (int flag)
{
  // Fills the flag
  shutdown_ = flag;
}

#endif
