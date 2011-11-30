

//=============================================================================
/**
 *  @file      client.cpp
 *
 *  $Id$
 *
 *    This class implements a simple CORBA client of the Cubit
 *    interface using DII functionality.
 *
 *
 *  @author   Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "tao/DynamicInterface/Request.h"
#include "tao/debug.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"

// Since we don't yet have an interface repository or dynamic-Any, we
// just get the info from the IDL-generated files, since we're mainly
// interested in timing comparisons anyway.
#include "../IDL_Cubit/cubitC.h"

  // Some constants used below.
const CORBA::ULong DEFAULT_LOOP_COUNT = 250;
const char *DEFAULT_FACTORY_IOR = "ior00";
const int SMALL_OCTET_SEQ_LENGTH = 16;
const int LARGE_OCTET_SEQ_LENGTH = 4096;
const int SMALL_LONG_SEQ_LENGTH = 4;
const int LARGE_LONG_SEQ_LENGTH = 1024;
const int NUMBER_OF_TESTS = 10;

/**
 * @class DII_Cubit_Client
 *
 * @brief Defines a class that encapsulates behaviour of a Cubit client
 * that makes requests using DII rather than stubs.
 *
 * This class declares an interface to run an example client for
 * a Cubit CORBA server.  All the complexity for initializing the
 * client is hidden in the class.  Just the run() interface is
 * needed.
 */
class DII_Cubit_Client
{
public:
  // = Constructor and destructor.
  /// Constructor.
  DII_Cubit_Client (void);

  /// Destructor.
  ~DII_Cubit_Client (void);

  /// Initialize the ORB and gets the Cubit objref.
  int init (int argc, ACE_TCHAR **argv);

  /// Execute client example code.
  int run (void);

private:
  /// Gets objref through naming service.
  int init_naming_service (void);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// Function to read the cubit factory IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Prints the timing stats.
  void print_stats (const char *call_name,
                    ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);

  // = DII versions of Cubit operations:

  void cube_short_dii (void);
  void cube_long_dii (void);
  void cube_octet_dii (void);
  void cube_union_dii (void);
  void cube_struct_dii (void);
  void cube_octet_seq_dii (int length);
  void cube_long_seq_dii (int length);

  // = Wrappers for cubing small and large sequences w/o args:

  /// Wrapper for the mixin call, just to be neat.
  void cube_small_long_seq (void);
  void cube_large_long_seq (void);
  void cube_small_octet_seq (void);
  void cube_large_octet_seq (void);
  void cube_mixin (void);

  /// Array of pointers to the operation functions.
  void (DII_Cubit_Client::*op_array_[NUMBER_OF_TESTS])(void);

  /// Array of labels for passing to print_stats.
  static const char *stats_messages_[];

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// # of calls in test loop.
  CORBA::ULong loop_count_;

  /// Flag to tell server to exit.
  int shutdown_;

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_var_;

  /// Storage of the Cubit_factory objref
  CORBA::Object_var factory_var_;

  /// Storage of the Cubit objref.
  CORBA::Object_var obj_var_;

  /// # of calls made to functions.
  CORBA::ULong call_count_;

  /// # of errors incurred in the lifetime of the application.
  CORBA::ULong error_count_;

  /// IOR of the factory used to make a Cubit object.
  ACE_CString factory_IOR_;

  /// File from which to obtain the IOR.
  FILE *cubit_factory_ior_file_;

  /// File handle to read the IOR.
  ACE_HANDLE f_handle_;
};

// Constructor
DII_Cubit_Client::DII_Cubit_Client (void)
  : loop_count_ (DEFAULT_LOOP_COUNT),
    shutdown_ (0),
    orb_var_ (0),
    factory_var_ (CORBA::Object::_nil ()),
    obj_var_ (CORBA::Object::_nil ()),
    call_count_ (0),
    error_count_ (0),
    factory_IOR_ (DEFAULT_FACTORY_IOR)
{
  // Initialize the array of pointers-to-member-functions.
  this->op_array_[0] = &DII_Cubit_Client::cube_short_dii;
  this->op_array_[1] = &DII_Cubit_Client::cube_octet_dii;
  this->op_array_[2] = &DII_Cubit_Client::cube_long_dii;
  this->op_array_[3] = &DII_Cubit_Client::cube_struct_dii;
  this->op_array_[4] = &DII_Cubit_Client::cube_union_dii;
  this->op_array_[5] = &DII_Cubit_Client::cube_small_long_seq;
  this->op_array_[6] = &DII_Cubit_Client::cube_large_long_seq;
  this->op_array_[7] = &DII_Cubit_Client::cube_small_octet_seq;
  this->op_array_[8] = &DII_Cubit_Client::cube_large_octet_seq;
  this->op_array_[9] = &DII_Cubit_Client::cube_mixin;
}

// Destructor
DII_Cubit_Client::~DII_Cubit_Client (void)
{
}

// An array of messages to pass to print_stats, so we can step through
// this along with op_array_.
const char *DII_Cubit_Client::stats_messages_[] =
{
  "DII cube_short",
  "DII cube_octet",
  "DII cube_long",
  "DII cube_struct",
  "DII cube_union",
  "DII cube_small_sequence<long>",
  "DII cube_large_sequence<long>",
  "DII cube_small_sequence<octet>",
  "DII cube_large_sequence<octet>",
  "DII cube mixin (short/octet/long)"
};

int
DII_Cubit_Client::init (int argc, ACE_TCHAR **argv)
{
  // Nice and safe.
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Initialize the ORB.
      this->orb_var_ = CORBA::ORB_init (this->argc_,
                                        this->argv_,
                                        "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "DII_Cubit_Client::parse_args failed"),
                          -1);

      // Get a factory object reference from the factory IOR.
      this->factory_var_ =
        this->orb_var_->string_to_object (this->factory_IOR_.c_str ());

      // Get a Cubit object with a DII request on the Cubit factory.
      CORBA::Request_var mc_req (this->factory_var_->_request ("make_cubit"));


      // make_cubit takes a char* arg that it doesn't use, but we must
      // still include it in the request.
      const char * dummy = "";

      mc_req->add_in_arg () <<= dummy;

      mc_req->set_return_type (CORBA::_tc_Object);

      // Invoke the <make_cubit> operation to ask the Cubit factory
      // for a Cubit object.
      mc_req->invoke ();

      // Extract the returned object reference from the request.
      mc_req->return_value () >>= CORBA::Any::to_object (this->obj_var_.out ());

      if (CORBA::is_nil (this->obj_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not obtain Cubit object from Cubit factory <%s>\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DII_Cubit_Client::init");
      return -1;
    }

  return 0;
}

// Sort out the args in the command line.

int
DII_Cubit_Client::parse_args (void)
{
  ACE_Get_Opt opts (argc_, argv_, ACE_TEXT("dn:i:f:x"));
  int c;
  int result;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':   // debug flag
        TAO_debug_level++;
        break;
      case 'n':   // loop count
        this->loop_count_ = ACE_OS::atoi (opts.opt_arg ());
        break;
      case 'i':   // Get the IOR from the command line.
        this->factory_IOR_ = ACE_TEXT_ALWAYS_CHAR(opts.opt_arg ());
        break;
      case 'f':   // Read the IOR from the file.
        result = this->read_ior (opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             opts.opt_arg ()),
                            -1);
        break;
      case 'x':   // Shut down server after test run.
        this->shutdown_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s"
                           " [-d]"
                           " [-n calls/loop]"
                           " [-i cubit-factory-IOR]"
                           " [-f filename]"
                           " [-x]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }
  return 0;  // Indicates successful parsing of command line.
}

// Get the factory IOR from the file created by the server.
int
DII_Cubit_Client::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (this->f_handle_);

  this->factory_IOR_ = ior_buffer.read ();

  if (this->factory_IOR_.length () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to allocate memory to read ior (%p)\n"),
                       ACE_TEXT ("read")),
                      -1);
  return 0;
}

// Formats and prints time statistics.  Identical to function in
// IDL_Cubit, for ease of comparison.

void
DII_Cubit_Client::print_stats (const char *call_name,
                               ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s:\n",
              call_name));

  if (this->call_count_ > 0
      && this->error_count_ == 0)
    {
      // elapsed_time.real_time is in units of seconds.
      double calls_per_sec =
        this->call_count_ / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\treal_time\t= %0.06f ms, \n\t"
                  "user_time\t= %0.06f ms, \n\t"
                  "system_time\t= %0.06f ms\n"
                  "\t%0.00f calls/second\n",
                  elapsed_time.real_time < 0.0 ? 0.0
                  : elapsed_time.real_time * ACE_ONE_SECOND_IN_MSECS,
                  elapsed_time.user_time < 0.0 ? 0.0
                  : elapsed_time.user_time * ACE_ONE_SECOND_IN_MSECS,
                  elapsed_time.system_time < 0.0 ? 0.0
                  : elapsed_time.system_time * ACE_ONE_SECOND_IN_MSECS,
                  calls_per_sec < 0.0 ? 0.0 : calls_per_sec));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  "
                "Call count zero or error ocurred.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "\t%d calls, %d errors\n",
              this->call_count_,
              this->error_count_));
}

void
DII_Cubit_Client::cube_short_dii (void)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_short"));


      CORBA::Short ret_short = 0;
      CORBA::Short arg_short = -3;

      // Add the short to the request arg list.
      req->add_in_arg () <<= arg_short;

      req->set_return_type (CORBA::_tc_short);

      this->call_count_++;

      req->invoke ();

      req->return_value () >>= ret_short;

      if (ret_short != arg_short * arg_short * arg_short)
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_short_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_short_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_long_dii (void)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_long"));


      CORBA::Long ret_long = 0;
      CORBA::Long arg_long = -7;

      // Add the long to the request arg list.
      req->add_in_arg () <<= arg_long;

      req->set_return_type (CORBA::_tc_long);

      this->call_count_++;

      req->invoke ();


      req->return_value () >>= ret_long;

      if (ret_long != arg_long * arg_long * arg_long)
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_long_dii -- bad results\n"));

            this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_long_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_octet_dii (void)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_octet"));


      CORBA::Octet ret_octet = 0;
      CORBA::Octet arg_octet = 5;

      // Add the octet to the request arg list.
      CORBA::Any::from_octet from_arg_octet (arg_octet);
      req->add_in_arg () <<= from_arg_octet;

      req->set_return_type (CORBA::_tc_octet);

      // Invoke, check for an exception and verify the result.
      this->call_count_++;

      req->invoke ();


      CORBA::Any::to_octet to_ret_octet (ret_octet);
      req->return_value () >>= to_ret_octet;

      if (ret_octet != arg_octet * arg_octet * arg_octet)
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_octet_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_octet_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_union_dii (void)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_union"));


      Cubit::oneof arg_union;

      arg_union._d(Cubit::e_3rd);
      arg_union.cm ().l = 5;
      arg_union.cm ().s = -7;
      arg_union.cm ().o = 3;

      // Add the union to the request arg list and set the return type.
      req->add_in_arg () <<= arg_union;

      req->set_return_type (Cubit::_tc_oneof);

      // Invoke, check for an exception and verify the result

      this->call_count_++;

      req->invoke ();


      // Extract the result and check validity.
      Cubit::oneof* ret_ptr;
      req->return_value () >>= ret_ptr;

      if (ret_ptr->cm ().l != arg_union.cm ().l * arg_union.cm ().l * arg_union.cm ().l
          || ret_ptr->cm ().s != arg_union.cm ().s * arg_union.cm ().s * arg_union.cm ().s
          || ret_ptr->cm ().o != arg_union.cm ().o * arg_union.cm ().o * arg_union.cm ().o)
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_union_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_union_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_struct_dii (void)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_struct"));


      Cubit::Many arg_struct, *ret_struct_ptr;

      arg_struct.l = 5;
      arg_struct.s = -7;
      arg_struct.o = 3;

      // Add struct to the request arg list.
      req->add_in_arg () <<= arg_struct;

      req->set_return_type (Cubit::_tc_Many);

      // Invoke, check for an exception and verify the result.

      this->call_count_++;

      req->invoke ();


      req->return_value () >>= ret_struct_ptr;

      if (ret_struct_ptr->l != arg_struct.l * arg_struct.l * arg_struct.l
          || ret_struct_ptr->s != arg_struct.s * arg_struct.s * arg_struct.s
          || ret_struct_ptr->o != arg_struct.o * arg_struct.o * arg_struct.o)
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_struct_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_struct_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_octet_seq_dii (int length)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_octet_sequence"));


      // Same length as in IDL_Cubit tests so timings can be compared.
      // Return value holder is set to a different length to test resizing.
      Cubit::octet_seq arg_octet_seq (length), *ret_octet_seq_ptr;
      arg_octet_seq.length (length);
      arg_octet_seq[0] = 4;

      // Add octet sequence to the request arg list

      req->add_in_arg () <<= arg_octet_seq;

      req->set_return_type (Cubit::_tc_octet_seq);

      // Invoke, check for an exception and verify the result.

      this->call_count_++;

      req->invoke ();


      req->return_value () >>= ret_octet_seq_ptr;

      // Check for correct length.
      if (ret_octet_seq_ptr->length () != arg_octet_seq.length ())
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_octet_seq_dii -- bad length\n"));

          this->error_count_++;
        }

      // Check for correct value(s).
      if ((*ret_octet_seq_ptr)[0] != arg_octet_seq[0] * arg_octet_seq[0] * arg_octet_seq[0])
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_octet_seq_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_octet_seq_dii");
      return;
    }
}

void
DII_Cubit_Client::cube_long_seq_dii (int length)
{
  try
    {
      // Create the request ...
      CORBA::Request_var req (this->obj_var_->_request ("cube_long_sequence"));


      // Same length as in IDL_Cubit tests so timings can be compared.
      // Return value holder is set to a different length to test
      // resizing.
      Cubit::long_seq *ret_long_seq_ptr;
      Cubit::long_seq arg_long_seq (length);
      arg_long_seq.length (length);
      arg_long_seq[0] = 4;

      // Add the long_seq to the request arg list.
      req->add_in_arg () <<= arg_long_seq;

      req->set_return_type (Cubit::_tc_long_seq);

      // Invoke, check for an exception and verify the result.

      this->call_count_++;

      req->invoke ();


      req->return_value () >>= ret_long_seq_ptr;

      // Check for correct length.
      if (ret_long_seq_ptr->length () != arg_long_seq.length ())
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_long_seq_dii -- bad length\n"));

          this->error_count_++;
        }

      // Check for correct value(s).
      if ((*ret_long_seq_ptr)[0] != arg_long_seq[0] * arg_long_seq[0] * arg_long_seq[0])
        {
          ACE_ERROR ((LM_ERROR,
                      "cube_long_seq_dii -- bad results\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      this->error_count_++;
      ex._tao_print_exception ("cube_long_seq_dii");
      return;
    }
}

// Wrappers for operations with non-void arg lists and the 3-in-1
// mixin test, so an array of pointers to functions can be stepped
// through in run ().

void
DII_Cubit_Client::cube_small_long_seq (void)
{
  this->cube_long_seq_dii (SMALL_LONG_SEQ_LENGTH);
}

void
DII_Cubit_Client::cube_large_long_seq (void)
{
  this->cube_long_seq_dii (LARGE_LONG_SEQ_LENGTH);
}

void
DII_Cubit_Client::cube_small_octet_seq (void)
{
  this->cube_octet_seq_dii (SMALL_OCTET_SEQ_LENGTH);
}

void
DII_Cubit_Client::cube_large_octet_seq (void)
{
  this->cube_octet_seq_dii (LARGE_OCTET_SEQ_LENGTH);
}

void
DII_Cubit_Client::cube_mixin (void)
{
  this->cube_short_dii ();
  this->cube_octet_dii ();
  this->cube_long_dii ();
}

int
DII_Cubit_Client::run (void)
{
  // loop counter.
  CORBA::ULong i;

  // Make a timer and an elapsed time holder.
  ACE_Profile_Timer dii_timer;
  ACE_Profile_Timer::ACE_Elapsed_Time dii_elapsed_time;

  // Order and format of test calls matches that of SII (IDL) tests
  // for easy comparison.

  for (int j = 0; j < NUMBER_OF_TESTS; j++)
    {
      this->call_count_ = 0;

      this->error_count_ = 0;

      dii_timer.start ();

      for (i = 0; i < this->loop_count_; i++)
        (this->*op_array_[j])();

      dii_timer.stop ();

      dii_timer.elapsed_time (dii_elapsed_time);

      this->print_stats (this->stats_messages_[j],
                         dii_elapsed_time);
    }

  try
    {
      // Shut down server via a DII request.
      if (this->shutdown_)
        {
          CORBA::Request_var req (this->obj_var_->_request ("shutdown"));


          // Cubit::shutdown () is a oneway operation.
          req->send_oneway ();


          ACE_DEBUG ((LM_DEBUG,
                      "\n\t Shutting down IDL_Cubit server \n\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DII_Cubit_Client: server shutdown");
      return -1;
    }

  return this->error_count_ == 0 ? 0 : 1;
}

// Start the test.

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  DII_Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\t DII_Cubit::CLIENT \n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  return cubit_client.run ();
}
