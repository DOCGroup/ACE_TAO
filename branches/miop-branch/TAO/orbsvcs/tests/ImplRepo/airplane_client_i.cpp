// $Id$

#include "airplane_client_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, airplane_client_i, "$Id$")

// Constructor.
Airplane_Client_i::Airplane_Client_i (void)
  : server_key_ (ACE::strnew ("key0")),
    loop_count_ (10),
    server_ (Paper_Airplane_Server::_nil ())
{
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
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'k':  // ior provide on command line
        this->server_key_ = ACE::strnew (get_opts.opt_arg ());
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
      TAO_ENV_DECLARE_NEW_ENV;
      ACE_TRY
        {
          CORBA::String_var response =
            this->server_->get_plane (TAO_ENV_SINGLE_ARG_PARAMETER);
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
  CORBA::release (this->server_);

  delete [] this->server_key_;
}


int
Airplane_Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet"
                                    TAO_ENV_ARG_PARAMETER);
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
        this->orb_->string_to_object (this->server_key_ TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->server_ = Paper_Airplane_Server::_narrow (server_object.in() TAO_ENV_ARG_PARAMETER);
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
