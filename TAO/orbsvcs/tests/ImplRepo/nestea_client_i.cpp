#include "nestea_client_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

// Parses the command line arguments and returns an error status.

int
Nestea_Client_i::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dsn:k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        ++TAO_debug_level;
        break;
      case 'k':  // ior provide on command line
        this->server_key_ = get_opts.opt_arg ();
        break;
      case 's': // shutdown server before exiting
        this->shutdown_server_ = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-s]"
                           " [-k server-object-key]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
Nestea_Client_i::run ()
{
  this->server_->drink (40);
  this->server_->drink (100);

  CORBA::String_var praise = this->server_->get_praise ();
  ACE_DEBUG ((LM_DEBUG, "Cans: %d\n"
                        "Praise: %s\n",
                        this->server_->bookshelf_size (),
                        praise.in ()));

  this->server_->drink (500);
  this->server_->crush (200);

  praise = this->server_->get_praise ();
  ACE_DEBUG ((LM_DEBUG, "Cans: %d\n"
                        "Praise: %s\n",
                        this->server_->bookshelf_size (),
                        praise.in ()));

  if (this->shutdown_server_)
    this->server_->shutdown ();

  return 0;
}

int
Nestea_Client_i::init (int argc, ACE_TCHAR **argv)
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

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->server_key_.c_str ());

      this->server_ = Nestea_Bookshelf::_narrow (server_object.in ());

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
          "Error: invalid server key <%s>\n", this->server_key_.c_str ()), -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Nestea_Client_i::init");
      return -1;
    }

  return 0;
}
