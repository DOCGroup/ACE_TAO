// $Id$

#include "Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Echo, Client_i, "$Id$")

// Constructor.

Client_i::Client_i (void)
  : ior_ (0),
    loop_count_ (10),
    shutdown_ (0),
    use_naming_service_ (0),
    server_ ()
{
}

// Reads the Server factory IOR from a file.

int
Client_i::read_ior (char *filename)
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

int
Client_i::parse_args (void)
{
    ACE_Get_Opt get_opts (argc_, argv_, "dn:f:xk:xs");

  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++; //****
        break;

        case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;

      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;

      case 's': // don't use the naming service
        this->use_naming_service_ = 1;
        break;

      case 'x':
        this->shutdown_ = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f ior-file]"
                           " [-k ior]"
                           " [-x]"
                           " [-s]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

void
Client_i::echo_list (const char *message)
{
  TAO_TRY
    {
      // Make the RMI.
      Echo::List_var l = this->server_->echo_list
        (message,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      for (u_int i = 0; i < l->length (); i++)
        {
          CORBA::String_var ior = orb_->object_to_string (l[i]);

          ACE_DEBUG ((LM_DEBUG,
                      "list elem %d = %s\n",
                      i,
                      ior));
        }
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Exception raised!\n"));
    }
  TAO_ENDTRY;
}

void
Client_i::echo_string (const char *message)
{
  TAO_TRY
    {
      // Make the RMI.
      CORBA::String_var s = this->server_->echo_string
        (message,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  s.in ()));
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Exception raised!\n"));
    }
  TAO_ENDTRY;
}

//Display the message on the screen

void
Client_i::echo (const char *message)
{
  this->echo_list (message);
  this->echo_string (message);
}

// Execute client example code.

int
Client_i::run (void)
{

  for (;;)
    {
      char buf[BUFSIZ];

      // Get the input message which has to be displayed.
      ACE_DEBUG ((LM_DEBUG,
                  "ECHO? "));

      if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
        break;

      // Make a call to the method which will display the input
      // string.
      this->echo (buf);
    }

  // End of the job, so tata! ;)

  if (this->shutdown_)
    this->server_->shutdown (this->env_);

  return 0;
}

Client_i::~Client_i (void)
{
  ACE_OS::free (this->ior_);
}

int
Client_i::via_naming_service(void)
{
  TAO_TRY
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);
      CosNaming::Name echo_ref_name (1);
      echo_ref_name.length (1);
      echo_ref_name[0].id = CORBA::string_dup ("Echo");

      CORBA::Object_var echo_obj =
        this->naming_services_client_->resolve (echo_ref_name,
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to Echo_var using
      // the <_narrow> method.
      this->server_ =
        Echo::_narrow (echo_obj.in (),
                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Echo::via_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Init function.
int
Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      TAO_debug_level = 1; //****

       if (this->use_naming_service_)
        return via_naming_service ();

      if (this->ior_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no ior specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->ior_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);

      // The downcasting from CORBA::Object_var to Echo_var is done
      // using the <_narrow> method.
      this->server_ = Echo::_narrow (server_object.in (),
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
