// -*- C++ -*-
// $Id$
#include "Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Constructor.
Client_i::Client_i (void)
  : ior_ (0),
    shutdown_ (0),
    server_ (),
    grid_ (0),
    height_ (0),
    width_ (0),
    setx_ (0),
    sety_ (0),
    setvalue_ (0)
{
  // no-op
}

// Reads the Server factory ior from a file

int
Client_i::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to open %s for writing: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to read ior: %p\n"),
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
  // Gets the command line options
  // The string enclosed in " " essentially tells us
  // what to expect from the command line options.
  ACE_Get_Opt get_opts (argc_, argv_, "df:sk:w:h:x:y:v:");
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
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;
      case 'h': // Height of the grid.
        height_ =  (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'w': // Width of the grid.
        width_ =  (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'x': // A x value of the grid where a value needs
                // to be stored.
        setx_ =  (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'y': // A y value of the grid where a value needs
                // to be stored.
        sety_ =  (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'v': // A value that needs to be stored a grid[x][y].
        setvalue_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 's': // Shutdown server.
        this->shutdown_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f ior-file]"
                           " [-w width]"
                           " [-h height]"
                           " [-k ior]"
                           " [-s]"
                           " [-x cell location]"
                           " [-y cell location]"
                           " [-v value ]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }
  if (setx_ == 0 )
    setx_ = Grid::DEFAULT_LOCATION;
  if (sety_ == 0 )
    sety_ = Grid::DEFAULT_LOCATION;
  if (setvalue_ == 0 )
    setvalue_ = Grid::DEFAULT_VALUE;

  // Indicates successful parsing of command line.
  return 0;
}

// Get the Grid on the server

void
Client_i::test_grid (void)
{
  // Make the RMI.
  TAO_TRY
    {
      this->grid_ = this->server_->make_grid (width_,
                                              height_,
                                              this->env_);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid succesfully\n"));
      // Set a value for a grid.
      this->grid_->set (setx_,
                        sety_,
                        setvalue_,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the grid\n"));

      // Get the value of the grid
      CORBA::Long ret_val = this->grid_->get (setx_,
                                              sety_,
                                              TAO_TRY_ENV);
      ACE_ASSERT (ret_val == setvalue_);
      ACE_UNUSED_ARG (ret_val); // To avoid compile warning with ACE_NDEBUG.
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::UserException, range_ex)
    {
      ACE_UNUSED_ARG (range_ex);
      TAO_TRY_ENV.print_exception (" From set and get grid ");
    }
  TAO_CATCH (CORBA::SystemException, memex)
    {
      ACE_UNUSED_ARG (memex);
      TAO_TRY_ENV.print_exception (" Cannot make grid as Memory exhausted");
    }
  TAO_ENDTRY;
}

// Execute client example code.

int
Client_i::run (void)
{
  // Make the grid.
  this->test_grid ();

  // Destroy the grid.
  this->grid_->destroy ();

  // Shutdown the server
  if (this->shutdown_)
    this->server_->shutdown (this->env_);

  return 0;
}

Client_i::~Client_i (void)
{
  ACE_OS::free (this->ior_);
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
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->ior_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %s: no ior specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->ior_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) invalid ior <%s>\n",
                           this->ior_),
                          -1);
      this->server_ = Grid_Factory::_narrow (server_object.in (),
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
