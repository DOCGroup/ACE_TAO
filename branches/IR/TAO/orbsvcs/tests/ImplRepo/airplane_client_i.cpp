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
  ACE_Get_Opt get_opts (argc_, argv_, "dn:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'k':  // ior provide on command line
        this->server_key_ = ACE_OS::strdup (get_opts.optarg);
        break;  
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-k server-obj-key]"
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
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          ACE_TRY_ENV.clear ();
          CORBA::String_var response =
            this->server_->get_plane (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Plane %d is %s\n", i, response.in ()));
        }
      ACE_CATCHANY
        {
          ACE_ERROR ((LM_ERROR, "Plane %d exception:\n", i));
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "get_planes");
        }
      ACE_ENDTRY;
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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->server_key_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no server key specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->server_key_, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->server_ = Paper_Airplane_Server::_narrow (server_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid server key <%s>\n",
                           this->server_key_),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Airplane_Client_i::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
