// $Id$

#include "nestea_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, nestea_client_i, "$Id$")

// Constructor.
Nestea_Client_i::Nestea_Client_i (void)
  : server_key_ (ACE_OS::strdup ("key0")),
    server_ (Nestea_Bookshelf::_nil ())
{
}

// Reads the Server factory ior from a file

int
Nestea_Client_i::read_ior (char *filename)
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
Nestea_Client_i::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
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
  
  ACE_DEBUG ((LM_DEBUG, "Cans: %d\n"
                        "Praise: %s\n", 
                        this->server_->bookshelf_size (),
                        this->server_->get_praise ()));

  this->server_->drink (500);
  this->server_->crush (200);

  ACE_DEBUG ((LM_DEBUG, "Cans: %d\n"
                        "Praise: %s\n", 
                        this->server_->bookshelf_size (),
                        this->server_->get_praise ()));

  return 0;
}

Nestea_Client_i::~Nestea_Client_i (void)
{
  // Free resources
  // Close the ior files
  CORBA::release (this->server_);

  if (this->server_key_ != 0)
    delete this->server_key_;
}


int
Nestea_Client_i::init (int argc, char **argv)
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

      this->server_ = Nestea_Bookshelf::_narrow (server_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid server key <%s>\n",
                           this->server_key_),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Nestea_Client_i::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
