// $Id$

#include "client_impl.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, client_impl, "$Id$")

// Constructor.
Client_i::Client_i (void)
  : server_key_ (ACE_OS::strdup ("key0")),
    loop_count_ (10),
    shutdown_ (0),
    server_ (simple_object::_nil ())
{
}

// Reads the Server factory ior from a file

int
Client_i::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename, 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  this->server_key_ = ior_buffer.read ();

  if (this->server_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (f_handle_);
  return 0;
}

// Parses the command line arguments and returns an error status.

int
Client_i::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:x");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
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
                           " [-f server-obj-ref-key-file]"
                           " [-x]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// calculate the cube from a long

void
Client_i::cube_long (int i)
{
  // Cube a long.
  CORBA::Long ret_long;
  ACE_OS::sleep (2);

  TAO_TRY 
  {
    ret_long = this->server_->simple_method (i, TAO_TRY_ENV);
    TAO_CHECK_ENV;

    ACE_DEBUG ((LM_DEBUG, "The cube of %d is %d\n", i, ret_long));
  }
  TAO_CATCHANY
  {
      TAO_TRY_ENV.print_exception ("from cube_long");
      return;
  }
  TAO_ENDTRY;

  CORBA::Long arg_long = i * i * i;

  if (arg_long != ret_long)
  {
    ACE_ERROR ((LM_ERROR,
                "** cube_long (%ld) ERROR (--> %ld)\n",
                ret_long,
                arg_long));
  }
}


// Execute client example code.

int
Client_i::run ()
{
  u_int i;

  for (i = 1; i <= this->loop_count_; i++)
    this->cube_long (i);

  if (this->shutdown_)
    this->server_->shutdown (this->env_);

  return 0;
}

Client_i::~Client_i (void)
{
  // Free resources
  // Close the ior files
  CORBA::release (this->server_);

  if (this->server_key_ != 0)
    ACE_OS::free (this->server_key_);
}


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
                                    "internet",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->server_key_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no server key specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->server_key_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->server_ = simple_object::_narrow (server_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid server key <%s>\n",
                           this->server_key_),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
