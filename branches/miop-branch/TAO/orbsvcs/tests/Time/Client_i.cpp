// $Id$

#include "Client_i.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

ACE_RCSID(Time_Service, Client_i, "$Id$")

// Constructor.

Client_i::Client_i (void)
  : ior_ (0),
    loop_count_ (10),
    shutdown_ (0),
    clerk_ ()
{
}

// The following test throws an exception that the operation
// <secure_universal_time> has not been currently implemented in the
// given server.

void
Client_i::test_for_secure_universal_time (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing secure_time()\n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosTime::UTO_var UTO_server =
           this->clerk_->secure_universal_time (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) test_for_secure_universal_time() successful !!\n"));
        }
  ACE_ENDTRY;
}

// The following test retrieves the current universal time as a UTO
// from the clerk_ and prints out the various attributes of the UTO.

void
Client_i::test_for_universal_time (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing universal_time()\n"));
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosTime::UTO_var UTO_server =
        this->clerk_->universal_time (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

#ifndef ACE_LACKS_LONGLONG_T
      ACE_DEBUG ((LM_DEBUG,
                  "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q"
                  "\nstruct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
                  UTO_server->time (),
                  UTO_server->inaccuracy (),
                  UTO_server->tdf (),
                  (UTO_server->utc_time ()).time,
                  (UTO_server->utc_time ()).inacclo,
                  (UTO_server->utc_time ()).inacchi,
                  (UTO_server->utc_time ()).tdf));
#else
      ACE_DEBUG ((LM_DEBUG,
                  "\nTime = %u\nInaccuracy = %u\nTimeDiff = %d\nstruct.time = %u"
                  "\nstruct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
                  UTO_server->time ().lo(),
                  UTO_server->inaccuracy ().lo(),
                  UTO_server->tdf (),
                  (UTO_server->utc_time ()).time.lo(),
                  (UTO_server->utc_time ()).inacclo,
                  (UTO_server->utc_time ()).inacchi,
                  (UTO_server->utc_time ()).tdf));
#endif

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error:");
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) test_for_universal_time()\n"));
    }
  ACE_ENDTRY;
}

void
Client_i::test_for_new_universal_time (void)
{
  TimeBase::TimeT time = 999999999;
  TimeBase::InaccuracyT inaccuracy = 9999;
  TimeBase::TdfT tdf = 99;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_universal_time()\n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosTime::UTO_var UTO_server = this->clerk_->new_universal_time (time,
                                                                      inaccuracy,
                                                                      tdf
                                                                      TAO_ENV_ARG_PARAMETER);

      ACE_ASSERT (UTO_server->time (TAO_ENV_SINGLE_ARG_PARAMETER) == 999999999);
      ACE_ASSERT (UTO_server->inaccuracy (TAO_ENV_SINGLE_ARG_PARAMETER) == 9999);
      ACE_ASSERT (UTO_server->tdf (TAO_ENV_SINGLE_ARG_PARAMETER) == 99);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).time == 999999999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).inacchi == 0);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).inacclo == 9999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).tdf == 99);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_universal_time () fails.\n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception :\n");
    }
  ACE_ENDTRY;
}

void
Client_i::test_for_uto_from_utc (void)
{
  TimeBase::UtcT utc_struct;

  utc_struct.time = 999999999;
  utc_struct.inacclo = 50;
  utc_struct.inacchi = 50;
  utc_struct.tdf = 99;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing uto_from_utc ()\n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosTime::UTO_var UTO_server = this->clerk_->uto_from_utc (utc_struct
                                                                TAO_ENV_ARG_PARAMETER);

      TimeBase::InaccuracyT inaccuracy = utc_struct.inacchi;
      inaccuracy <<= 32;
      inaccuracy |= utc_struct.inacclo;

      ACE_ASSERT (UTO_server->time (TAO_ENV_SINGLE_ARG_PARAMETER) == 999999999);
      ACE_ASSERT (UTO_server->inaccuracy (TAO_ENV_SINGLE_ARG_PARAMETER) == inaccuracy);
      ACE_ASSERT (UTO_server->tdf (TAO_ENV_SINGLE_ARG_PARAMETER) == 99);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).time == 999999999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).inacclo == 50);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).inacchi == 50);
      ACE_ASSERT ((UTO_server->utc_time (TAO_ENV_SINGLE_ARG_PARAMETER)).tdf == 99);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test uto_from_utc () fails.\n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception :\n");
    }
  ACE_ENDTRY;
}

void
Client_i::test_for_new_interval (void)
{
  TimeBase::TimeT lower = 666666666;
  TimeBase::TimeT upper = 999999999;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_interval ()\n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosTime::TIO_var TIO_server = this->clerk_->new_interval (lower,
                                                                upper
                                                                TAO_ENV_ARG_PARAMETER);

      ACE_ASSERT ((TIO_server->time_interval (TAO_ENV_SINGLE_ARG_PARAMETER)).lower_bound == 666666666);
      ACE_ASSERT ((TIO_server->time_interval (TAO_ENV_SINGLE_ARG_PARAMETER)).upper_bound == 999999999);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_interval () fails.\n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception :\n");
    }
  ACE_ENDTRY;

  return;
}

// Reads the Time Service Server ior from a file

int
Client_i::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to open %s for writing: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior: %p\n"),
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
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:xk:");
  int c;
  int result;

   while ((c = get_opts ()) != -1)
     // ACE_DEBUG((LM_DEBUG,"bal =1%c",c
     //            ));
     switch (c)
       {
       case 'd':  // debug flag
         TAO_debug_level++;
         break;
       case 'n':  // loop count
         this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
         break;
       case 'k':  // ior provide on command line
         this->ior_ = ACE_OS::strdup (get_opts.opt_arg ());
         break;
       case 'f': // read the IOR from the file.
         result = this->read_ior (get_opts.opt_arg ());
         if (result < 0)
           ACE_ERROR_RETURN ((LM_ERROR,
                              "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n",
                              get_opts.opt_arg ()),
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
                            " [-f ior-file]"
                           " [-k ior]"
                            " [-x]"
                           "\n",
                            this->argv_ [0]),
                           -1);
       }

   // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
Client_i::run (void)
{
  // Retrieves the current universal time.
  test_for_universal_time ();

  // Asserts the validity of the new_universal_time.
  test_for_new_universal_time ();

  // Asserts the validity of the UTO created from a given UTC.
  test_for_uto_from_utc ();

  // Asserts the validity of the TIO created using the given
  // bounds.
  test_for_new_interval ();

  // Throws an exception because the method has not been
  // implemented currently.
  test_for_secure_universal_time ();

  return 0;
}

Client_i::~Client_i (void)
{
  ACE_OS::free (this->ior_);
}

int
Client_i::obtain_initial_references (void)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the naming services.
      if (my_name_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "DONE\n"));

      char host_name[MAXHOSTNAMELEN];
      char name[MAXHOSTNAMELEN];

      ACE_OS::hostname (host_name, MAXHOSTNAMELEN);

      CosNaming::Name clerk_name;
      clerk_name.length (2);
      clerk_name[0].id = CORBA::string_dup ("ClerkContext");
      strcpy (name, "Clerk:");
      strcat (name, host_name);
      clerk_name[1].id = CORBA::string_dup (name);

      ACE_DEBUG ((LM_DEBUG,
                  "%s|\n",
                  name));

      CORBA::Object_var temp_object =
        my_name_client_->resolve (clerk_name
                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      clerk_ = CosTime::TimeService::_narrow (temp_object.in ()
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (clerk_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to Resolve "
                           "Clerk Reference\n"),
                           -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Client :: obtain_initial_references\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// This method uses the IOR if specified on the command line or in a
// file, else it uses the Naming Service

int
Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    0
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->ior_)
        {
          // An ior is specified for the client through a commandline
          // option or a file.

          CORBA::Object_var server_object =
          this->orb_->string_to_object (this->ior_
                                        TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               this->ior_),
                              -1);
          this->clerk_ =
            CosTime::TimeService::_narrow (server_object.in ()
                                           TAO_ENV_ARG_PARAMETER);

          ACE_DEBUG ((LM_DEBUG,
                      "[CLIENT] Process/Thread Id : (%P/%t) Using the IOR provided\n"));
          ACE_TRY_CHECK;
        }
      else
        { // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     "[CLIENT] Process/Thread Id : (%P/%t) Using the Naming Service\n"));

          if (this->obtain_initial_references () == -1)
            return -1;

          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Client_i::init\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
