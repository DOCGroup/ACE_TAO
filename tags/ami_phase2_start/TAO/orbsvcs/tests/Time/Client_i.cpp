// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

#include "Client_i.h"

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
  TAO_TRY
    {
      CosTime::UTO_var UTO_server =
 	   this->clerk_->secure_universal_time (TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      ACE_DEBUG ((LM_DEBUG,
 		  "[CLIENT] Process/Thread Id : (%P/%t) test_for_secure_universal_time() successful !!\n"));
	}
  TAO_ENDTRY;
}

// The following test retrieves the current universal time as a UTO
// from the clerk_ and prints out the various attributes of the UTO.

void
Client_i::test_for_universal_time (void)
{
  ACE_DEBUG ((LM_DEBUG,
 	      "[CLIENT] Process/Thread Id : (%P/%t) Testing universal_time()\n"));
  TAO_TRY
    {
      CosTime::UTO_var UTO_server =
 	this->clerk_->universal_time (TAO_TRY_ENV);

	  TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
 		  "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q"
 		  "\nstruct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
 		  UTO_server->time (TAO_TRY_ENV),
 		  UTO_server->inaccuracy (TAO_TRY_ENV),
 		  UTO_server->tdf (TAO_TRY_ENV),
 		  (UTO_server->utc_time ()).time,
 		  (UTO_server->utc_time ()).inacclo,
 		  (UTO_server->utc_time ()).inacchi,
 		  (UTO_server->utc_time ()).tdf));

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Error:");
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) test_for_universal_time()\n"));
    }
  TAO_ENDTRY;
}

void
Client_i::test_for_new_universal_time (void)
{
  // @@ Vishal, please consider replacing these numbers with consts or enums.
  TimeBase::TimeT time = 999999999;
  TimeBase::InaccuracyT inaccuracy = 9999;
  TimeBase::TdfT tdf = 99;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_universal_time()\n"));

  TAO_TRY
    {
      CosTime::UTO_var UTO_server = this->clerk_->new_universal_time (time,
 								      inaccuracy,
                                                                      tdf,
                                                                      TAO_TRY_ENV);

      ACE_ASSERT (UTO_server->time (TAO_TRY_ENV) == 999999999);
      ACE_ASSERT (UTO_server->inaccuracy (TAO_TRY_ENV) == 9999);
      ACE_ASSERT (UTO_server->tdf (TAO_TRY_ENV) == 99);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).time == 999999999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).inacchi == 0);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).inacclo == 9999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).tdf == 99);
		  
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_universal_time () fails.\n"));
    }
  TAO_ENDTRY;
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

  TAO_TRY
    {
      CosTime::UTO_var UTO_server = this->clerk_->uto_from_utc (utc_struct,
                                                                TAO_TRY_ENV);
      
      TimeBase::InaccuracyT inaccuracy = utc_struct.inacchi;
      inaccuracy <<= 32;
      inaccuracy |= utc_struct.inacclo;
      
      ACE_ASSERT (UTO_server->time (TAO_TRY_ENV) == 999999999);
      ACE_ASSERT (UTO_server->inaccuracy (TAO_TRY_ENV) == inaccuracy);
      ACE_ASSERT (UTO_server->tdf (TAO_TRY_ENV) == 99);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).time == 999999999);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).inacclo == 50);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).inacchi == 50);
      ACE_ASSERT ((UTO_server->utc_time (TAO_TRY_ENV)).tdf == 99);
      
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test uto_from_utc () fails.\n"));
    }
  TAO_ENDTRY;
}

void
Client_i::test_for_new_interval (void)
{
  TimeBase::TimeT lower = 666666666;
  TimeBase::TimeT upper = 999999999;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_interval ()\n"));

  TAO_TRY
    {
      CosTime::TIO_var TIO_server = this->clerk_->new_interval (lower,
 								upper,
                                                                TAO_TRY_ENV);

      ACE_ASSERT ((TIO_server->time_interval (TAO_TRY_ENV)).lower_bound == 666666666);
      ACE_ASSERT ((TIO_server->time_interval (TAO_TRY_ENV)).upper_bound == 999999999);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_interval () fails.\n"));
    }
  TAO_ENDTRY;
  
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
     //		   ));
     switch (c)
       {
       case 'd':  // debug flag
         TAO_debug_level++;
         break;
       case 'n':  // loop count
	 this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
         break;
       case 'k':  // ior provide on command line
         this->ior_ = ACE_OS::strdup (get_opts.optarg);
         break;
       case 'f': // read the IOR from the file.
         result = this->read_ior (get_opts.optarg);
         if (result < 0)
           ACE_ERROR_RETURN ((LM_ERROR,
                              "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n",
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
  TAO_TRY
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
        my_name_client_->resolve (clerk_name,
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      clerk_ = CosTime::TimeService::_narrow (temp_object.in (),
					      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (clerk_.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   "[CLIENT] Process/Thread Id : (%P/%t) Unable to Resolve "
			   "Clerk Reference\n"),
			   -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client :: obtain_initial_references");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This method uses the IOR if specified on the command line or in a
// file, else it uses the Naming Service

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

      if (this->ior_)
	{
	  // An ior is specified for the client through a commandline
	  // option or a file.

	  CORBA::Object_var server_object =
	  this->orb_->string_to_object (this->ior_,
					TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (CORBA::is_nil (server_object.in ()))
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "invalid ior <%s>\n",
			       this->ior_),
			      -1);
	  this->clerk_ =
	    CosTime::TimeService::_narrow (server_object.in (),
					    TAO_TRY_ENV);

	  ACE_DEBUG ((LM_DEBUG,
		      "[CLIENT] Process/Thread Id : (%P/%t) Using the IOR provided\n"));
	  TAO_CHECK_ENV;
	}
      else
	{ // No IOR specified. Use the Naming Service
	  ACE_DEBUG((LM_DEBUG,
		     "[CLIENT] Process/Thread Id : (%P/%t) Using the Naming Service\n"));

	  if (this->obtain_initial_references () == -1)
	    return -1;

	  TAO_CHECK_ENV;
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
