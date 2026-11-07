#include "Client_i.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/os_include/os_netdb.h"

// The following test throws an exception that the operation
// <secure_universal_time> has not been currently implemented in the
// given server.

void
Client_i::test_for_secure_universal_time ()
{
  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing secure_time()\n"));

  try
    {
      CosTime::UTO_var const uto_server = this->clerk_->secure_universal_time ();
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) test_for_secure_universal_time() successful !!\n"));
        }
}

// The following test retrieves the current universal time as a UTO
// from the clerk_ and prints out the various attributes of the UTO.

void
Client_i::test_for_universal_time ()
{
  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing universal_time()\n"));
  try
    {
      CosTime::UTO_var const uto_server =
        this->clerk_->universal_time ();

      ACE_DEBUG ((LM_DEBUG,
                  "\nTime = %Q\nInaccuracy = %Q\nTimeDiff = %d\nstruct.time = %Q"
                  "\nstruct.inacclo = %d\nstruct.inacchi = %d\nstruct.Tdf = %d\n",
                  uto_server->time (),
                  uto_server->inaccuracy (),
                  uto_server->tdf (),
                  (uto_server->utc_time ()).time,
                  (uto_server->utc_time ()).inacclo,
                  (uto_server->utc_time ()).inacchi,
                  (uto_server->utc_time ()).tdf));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Error:");
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) test_for_universal_time()\n"));
    }
}

void
Client_i::test_for_new_universal_time ()
{
  TimeBase::TimeT const time = 999999999;
  TimeBase::InaccuracyT const inaccuracy = 9999;
  TimeBase::TdfT const tdf = 99;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_universal_time()\n"));

  try
    {
      CosTime::UTO_var const uto_server = this->clerk_->new_universal_time (time,
                                                                            inaccuracy,
                                                                            tdf);

      ACE_ASSERT (uto_server->time () == 999999999);
      ACE_ASSERT (uto_server->inaccuracy () == 9999);
      ACE_ASSERT (uto_server->tdf () == 99);
      ACE_ASSERT ((uto_server->utc_time ()).time == 999999999);
      ACE_ASSERT ((uto_server->utc_time ()).inacchi == 0);
      ACE_ASSERT ((uto_server->utc_time ()).inacclo == 9999);
      ACE_ASSERT ((uto_server->utc_time ()).tdf == 99);
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_universal_time () fails.\n"));
      ex._tao_print_exception ("Exception :\n");
    }
}

void
Client_i::test_for_uto_from_utc ()
{
  TimeBase::UtcT const utc_struct {999999999, 50, 50, 99};

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing uto_from_utc ()\n"));

  try
    {
      CosTime::UTO_var const uto_server = this->clerk_->uto_from_utc (utc_struct);

      TimeBase::InaccuracyT const inaccuracy =
        (static_cast<TimeBase::InaccuracyT> (utc_struct.inacchi) << 32)
        | utc_struct.inacclo;

      ACE_ASSERT (uto_server->time () == 999999999);
      ACE_ASSERT (uto_server->inaccuracy () == inaccuracy);
      ACE_ASSERT (uto_server->tdf () == 99);
      ACE_ASSERT ((uto_server->utc_time ()).time == 999999999);
      ACE_ASSERT ((uto_server->utc_time ()).inacclo == 50);
      ACE_ASSERT ((uto_server->utc_time ()).inacchi == 50);
      ACE_ASSERT ((uto_server->utc_time ()).tdf == 99);
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test uto_from_utc () fails.\n"));
      ex._tao_print_exception ("Exception :\n");
    }
}

void
Client_i::test_for_new_interval ()
{
  TimeBase::TimeT const lower = 666666666;
  TimeBase::TimeT const upper = 999999999;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Testing new_interval ()\n"));

  try
    {
      CosTime::TIO_var const tio_server = this->clerk_->new_interval (lower,
                                                                      upper);

      ACE_ASSERT ((tio_server->time_interval ()).lower_bound == 666666666);
      ACE_ASSERT ((tio_server->time_interval ()).upper_bound == 999999999);
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Test new_interval () fails.\n"));
      ex._tao_print_exception ("Exception :\n");
    }
}

// Reads the Time Service Server ior from a file

int
Client_i::read_ior (ACE_TCHAR const *filename)
{
  // Open the file for reading.
  ACE_HANDLE const f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("[CLIENT] Process/Thread Id : (%P/%t) Unable to open %s for reading: %p\n"),
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char * const data = ior_buffer.read ();

  if (data == nullptr)
    {
      ACE_OS::close (f_handle);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior: %p\n")),
                        -1);
    }

  this->ior_ = data;
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Client_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dn:f:xk:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        TAO_debug_level++;
        break;
      case 'n':
        this->loop_count_ = static_cast<u_int> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'k':
        this->ior_ = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        break;
      case 'f':
        if (this->read_ior (get_opts.opt_arg ()) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n"),
                             get_opts.opt_arg ()),
                            -1);
        break;
      case 'x':
        this->shutdown_ = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s [-d] [-n loopcount] [-f ior-file] [-k ior] [-x]\n"),
                           argv[0]),
                          -1);
      }

   // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
Client_i::run ()
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

int
Client_i::obtain_initial_references ()
{
  try
    {
      // Initialize the naming services.
      if (my_name_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "DONE\n"));

      char host_name[MAXHOSTNAMELEN] {};

      ACE_OS::hostname (host_name, MAXHOSTNAMELEN);

      CosNaming::Name clerk_name;
      clerk_name.length (2);
      clerk_name[0].id = CORBA::string_dup ("ClerkContext");
      std::string const name = std::string ("Clerk:") + host_name;
      clerk_name[1].id = CORBA::string_dup (name.c_str ());

      ACE_DEBUG ((LM_DEBUG,
                  "%C|\n",
                  name.c_str ()));

      CORBA::Object_var temp_object =
        my_name_client_->resolve (clerk_name);

      clerk_ = CosTime::TimeService::_narrow (temp_object.in ());

      if (CORBA::is_nil (clerk_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to Resolve "
                           "Clerk Reference\n"),
                           -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Client :: obtain_initial_references\n");
      return -1;
    }

  return 0;
}

// This method uses the IOR if specified on the command line or in a
// file, else it uses the Naming Service

int
Client_i::init (int argc, ACE_TCHAR **argv)
{
  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc, argv);

      // Parse command line and verify parameters.
      if (this->parse_args (argc, argv) != 0)
        return -1;

      if (!this->ior_.empty ())
        {
          // An ior is specified for the client through a commandline
          // option or a file.

          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_.c_str ());

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%C>\n",
                               this->ior_.c_str ()),
                              -1);
          this->clerk_ =
            CosTime::TimeService::_narrow (server_object.in ());

          ACE_DEBUG ((LM_DEBUG,
                      "[CLIENT] Process/Thread Id : (%P/%t) Using the IOR provided\n"));
        }
      else
        { // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     "[CLIENT] Process/Thread Id : (%P/%t) Using the Naming Service\n"));

          if (this->obtain_initial_references () == -1)
            return -1;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client_i::init\n");
      return -1;
    }

  return 0;
}
