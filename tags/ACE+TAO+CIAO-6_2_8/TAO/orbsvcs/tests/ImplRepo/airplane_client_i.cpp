// $Id$

#include "airplane_client_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/ACE.h"



// Constructor.
Airplane_Client_i::Airplane_Client_i (void)
  : argc_ (0),
    argv_ (0),
    server_key_ (ACE::strnew (ACE_TEXT("key0"))),
    loop_count_ (10),
    server_ (Paper_Airplane_Server::_nil ())
{
}


// Parses the command line arguments and returns an error status.

int
Airplane_Client_i::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dn:k:"));
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
        delete [] this->server_key_;
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

int
Airplane_Client_i::get_planes (size_t count)
{
  int rc = 0;
  for (size_t i = 0; i < count; i++)
    {
      try
        {
          CORBA::String_var response =
            this->server_->get_plane ();

          ACE_DEBUG ((LM_DEBUG, "Plane %d is %C\n", i, response.in ()));
        }
      catch (const CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR, "Plane %d exception:\n", i));
          ex._tao_print_exception ("get_planes");
          rc = 1;
        }
    }
  return rc;
}


// Execute client example code.

int
Airplane_Client_i::run ()
{
  return this->get_planes (this->loop_count_);
}

Airplane_Client_i::~Airplane_Client_i (void)
{
  // Free resources
  CORBA::release (this->server_);

  delete [] this->server_key_;
}


int
Airplane_Client_i::init (int argc, ACE_TCHAR **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->server_key_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no server key specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->server_key_);

      this->server_ = Paper_Airplane_Server::_narrow (server_object.in());

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
          "Error: invalid server key <%s>\n", this->server_key_), -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Airplane_Client_i::init");
      return -1;
    }

  return 0;
}
