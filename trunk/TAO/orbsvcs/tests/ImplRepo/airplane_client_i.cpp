// $Id$

#include "airplane_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, airplane_client_i, "$Id$")

// Constructor.
Airplane_Client_i::Airplane_Client_i (void)
  : server_key_ (ACE_OS::strdup ("key0")),
    loop_count_ (10),
    server_ (Paper_Airplane_Server::_nil ())
{
}

// Reads the Server factory ior from a file

int
Airplane_Client_i::read_ior (char *filename)
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
Airplane_Client_i::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:");
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
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f server-obj-ref-key-file]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Retreives <count> paper airplanes from the server.

void
Airplane_Client_i::get_planes (size_t count)
{
  for (size_t i = 0; i < count; i++)
    {
      TAO_TRY
        {
          CORBA::String_var response =
            this->server_->get_plane (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_DEBUG ((LM_DEBUG, "Plane %d is %s\n", i, response.in ()));
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("get_planes");
          return;
        }
      TAO_ENDTRY;
    }
}


// Execute client example code.

int
Airplane_Client_i::run ()
{
  this->get_planes (this->loop_count_);

  return 0;
}

Airplane_Client_i::~Airplane_Client_i (void)
{
  // Free resources
  // Close the ior files
  CORBA::release (this->server_);

  if (this->server_key_ != 0)
    delete this->server_key_;
}


int
Airplane_Client_i::init (int argc, char **argv)
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

      this->server_ = Paper_Airplane_Server::_narrow (server_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid server key <%s>\n",
                           this->server_key_),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Airplane_Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
