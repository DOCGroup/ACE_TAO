#include "airplane_client_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

// Parses the command line arguments and returns an error status.

int
Airplane_Client_i::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dn:k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':  // loop count
        this->loop_count_ = static_cast<size_t> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'k':  // ior provide on command line
        this->server_key_ = get_opts.opt_arg ();
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
  int rc {};
  for (size_t i = 0; i < count; ++i)
    {
      try
        {
          CORBA::String_var const response =
            this->server_->get_plane ();

          ACE_DEBUG ((LM_DEBUG, "Plane %B is %C\n", i, response.in ()));
        }
      catch (CORBA::Exception const &ex)
        {
          ACE_ERROR ((LM_ERROR, "Plane %B exception:\n", i));
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

      CORBA::Object_var const server_object =
        this->orb_->string_to_object (this->server_key_.c_str ());

      this->server_ = Paper_Airplane_Server::_narrow (server_object.in ());

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
          "Error: invalid server key <%s>\n", this->server_key_.c_str ()), -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Airplane_Client_i::init");
      return -1;
    }

  return 0;
}
