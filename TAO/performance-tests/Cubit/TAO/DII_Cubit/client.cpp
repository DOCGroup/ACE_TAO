// $Id$


// ============================================================================
//
// = LIBRARY
//	TAO/tests/Cubit/TAO/DII_Cubit
//
// = FILENAME
//	client.cpp
//
// = DESCRIPTION
//      This class implements a simple CORBA client of the Cubit
//      interface using DII functionality.
//
// = AUTHOR
//	Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "ace/streams.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"

// Since we don't yet have an interface repository or dynamic-Any, we
// just get the info from the IDL-generated files, since we're mainly
// interested in timing comparisons anyway.
#include "../IDL_Cubit/cubitC.h"

ACE_RCSID(DII_Cubit, client, "$Id$")

  // Some constants used below.
const CORBA::ULong DEFAULT_LOOP_COUNT = 250;
const char *DEFAULT_FACTORY_IOR = "ior00";
const int SMALL_OCTET_SEQ_LENGTH = 16;
const int LARGE_OCTET_SEQ_LENGTH = 4096;
const int SMALL_LONG_SEQ_LENGTH = 4;
const int LARGE_LONG_SEQ_LENGTH = 1024;
const int NUMBER_OF_TESTS = 10;

// Some macros for env checks used identically in each operation.
#define CUBIT_CHECK_ENV_RETURN_VOID(PRINT_STRING) \
if (this->env_.exception () != 0) \
{ \
    this->error_count_++; \
 this->env_.print_exception (PRINT_STRING); \
 return; \
}

#define CUBIT_CHECK_ENV_RELEASE_RETURN_VOID(REQ, PRINT_STRING) \
if (this->env_.exception () != 0) \
{ \
    this->error_count_++; \
 this->env_.print_exception (PRINT_STRING); \
 CORBA::release (REQ); \
 return; \
}

#define REQUEST_CHECK_ENV_RETURN_VOID(REQ, PRINT_STRING) \
if (REQ->env ()->exception () != 0) \
{ \
    this->error_count_++; \
 REQ->env ()->print_exception (PRINT_STRING); \
 CORBA::release (REQ); \
 return; \
}

class DII_Cubit_Client 
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of a Cubit client
  //    that makes requests using DII rather than stubs.
  //
  // = DESCRIPTION
  //    This class declares an interface to run an example client for
  //    a Cubit CORBA server.  All the complexity for initializing the
  //    client is hidden in the class.  Just the run() interface is
  //    needed.
public:
  // = Constructor and destructor.
  DII_Cubit_Client (void);
  // Constructor.

  ~DII_Cubit_Client (void);
  // Destructor.

  int init (int argc, char **argv);
  // Initialize the ORB and gets the Cubit objref.

  int run (void);
  // Execute client example code.

private:
  int init_naming_service (void);
  // Gets objref through naming service.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int read_ior (char *filename);
  // Function to read the cubit factory IOR from a file.

  void print_stats (const char *call_name,
                    ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);
  // Prints the timing stats.

  // = DII versions of Cubit operations:
	
  void cube_short_dii (void);
  void cube_long_dii (void);
  void cube_octet_dii (void);
  void cube_union_dii (void);
  void cube_struct_dii (void);
  void cube_octet_seq_dii (int length);
  void cube_long_seq_dii (int length);

  // = Wrappers for cubing small and large sequences w/o args:

  void cube_small_long_seq (void);
  void cube_large_long_seq (void);
  void cube_small_octet_seq (void);
  void cube_large_octet_seq (void);
  void cube_mixin (void);
  // Wrapper for the mixin call, just to be neat.

  void (DII_Cubit_Client::*op_array_[NUMBER_OF_TESTS])(void);
  // Array of pointers to the operation functions.

  static char *stats_messages_[];
  // Array of labels for passing to print_stats.
  
  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CORBA::ULong loop_count_;
  // # of calls in test loop.

  int shutdown_;
  // Flag to tell server to exit.

  int use_naming_service_;
  // Flag toggling use of naming service to get IOR.

  CORBA::Environment env_;
  // Environment variable.

  CORBA::ORB_var orb_var_;
  // Storage of the ORB reference.

  CORBA::Object_var factory_var_;
  // Storage of the Cubit_factory objref

  CORBA::Object_var obj_var_;
  // Storage of the Cubit objref.

  CORBA::ULong call_count_;
  // # of calls made to functions.

  CORBA::ULong error_count_;
  // # of errors incurred in the lifetime of the application.

  char *factory_IOR_;
  // IOR of the factory used to make a Cubit object.

  FILE *cubit_factory_ior_file_;
  // File from which to obtain the IOR.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.
};

// Constructor
DII_Cubit_Client::DII_Cubit_Client (void)
  : loop_count_ (DEFAULT_LOOP_COUNT),			
    shutdown_ (0),
    use_naming_service_ (1),
    orb_var_ (0),
    factory_var_ (CORBA::Object::_nil ()),
    obj_var_ (CORBA::Object::_nil ()),
    call_count_ (0),
    error_count_ (0),
    factory_IOR_ (CORBA::string_dup (DEFAULT_FACTORY_IOR))
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
  CORBA::string_free (this->factory_IOR_);
}

// An array of messages to pass to print_stats, so we can step through
// this along with op_array_.
char *DII_Cubit_Client::stats_messages_[] =
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
DII_Cubit_Client::init (int argc, char **argv)
{
  // Nice and safe.
  this->argc_ = argc;
  this->argv_ = argv;

  // Exits gracefully when no IOR is provided and use_naming_service_
  // is toggled off.
  if (!ACE_OS::strcmp (this->factory_IOR_,
		       DEFAULT_FACTORY_IOR) 
      && !this->use_naming_service_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: Must supply IOR, read it from a file, or use naming service."
                       "\n",
                       this->argv_ [0]),
                      -1);

  TAO_TRY
    {
      // Initialize the ORB.
      this->orb_var_ = CORBA::ORB_init (this->argc_,
                                        this->argv_,
                                        "internet",
                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->use_naming_service_)
        {
          // Get a factory object reference from the naming service.
          if (this->init_naming_service () == -1)
            return -1;
        }
      else
        {
          // Get a factory object reference from the factory IOR.
          this->factory_var_ = 
            this->orb_var_->string_to_object (this->factory_IOR_, 
                                              TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

      // Get a Cubit object with a DII request on the Cubit factory.
      CORBA::Request_ptr mc_req =
        this->factory_var_->_request ("make_cubit", TAO_TRY_ENV);

      TAO_CHECK_ENV;
	
      // make_cubit takes a char* arg that it doesn't use, but we must
      // still include it in the request.
      CORBA::String dummy = "";

      CORBA::Any string_arg (CORBA::_tc_string,
                             &dummy,
                             0);
   
      // @@ Jeff, is it possible to use the operator<< for this in
      // order to simplify the code?
      mc_req->arguments ()->add_value (0,
				       string_arg,
				       CORBA::ARG_IN,
				       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Insert the result-holding variable into the request.
      mc_req->result ()->value ()->replace (CORBA::_tc_Object, 
                                            &this->obj_var_,
                                            0, 
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Invoke the <make_cubit> operation to ask the Cubit factory
      // for a Cubit object.
      mc_req->invoke ();

      CORBA::release (mc_req);

      if (CORBA::is_nil (this->obj_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not obtain Cubit object from Cubit factory <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("DII_Cubit_Client::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Get the factory IOR via a DII request on the naming service.

int
DII_Cubit_Client::init_naming_service (void)
{
  TAO_TRY
    {
      // Initialize the naming services
      if (my_name_client_.init (orb_var_.in ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);
      /*
	// Get the naming service from the orb.
	CORBA::Object_var naming_obj =
	this->orb_var_->resolve_initial_references ("NameService");
	
	if (CORBA::is_nil (naming_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
	" (%P|%t) Unable to resolve the Name Service.\n"),
	-1);
      */
      
      // Build a Name object.
      CosNaming::Name cubit_factory_name (2);
      cubit_factory_name.length (2);
      cubit_factory_name[0].id = CORBA::string_dup ("IDL_Cubit");
      cubit_factory_name[1].id = CORBA::string_dup ("cubit_factory");

      // Build up the <resolve> operation using the DII!
      CORBA::Request_ptr req =
        my_name_client_->_request ("resolve", TAO_TRY_ENV);

      TAO_CHECK_ENV;

      CORBA::Any name_arg (CosNaming::_tc_Name,
                           &cubit_factory_name,
                           0);
  
      req->arguments ()->add_value (0,
				    name_arg,
				    CORBA::ARG_IN,
				    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      req->result ()->value ()->replace (CORBA::_tc_Object, 
                                         &this->factory_var_,
                                         0, 
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      req->invoke ();
      CORBA::release (req);

      if (CORBA::is_nil (this->factory_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve cubit factory in Naming service <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("DII_Cubit_Client::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Sort out the args in the command line.

int
DII_Cubit_Client::parse_args (void)
{
  ACE_Get_Opt opts (argc_, argv_, "dn:i:f:x");
  int c;
  int result;
   
  while ((c = opts ()) != -1)
    switch (c) 
      {
      case 'd':   // debug flag
        TAO_debug_level++;
        break;
      case 'n':	  // loop count
        this->loop_count_ = ACE_OS::atoi (opts.optarg);
        break;        
      case 'i':	  // Get the IOR from the command line.	
	this->use_naming_service_ = 0;
        this->factory_IOR_ = opts.optarg;
        break;
      case 'f':   // Read the IOR from the file.
	this->use_naming_service_ = 0;
        result = this->read_ior (opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             opts.optarg),
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
DII_Cubit_Client::read_ior (char *filename)
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

  if (this->factory_IOR_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
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
#if defined (ACE_LACKS_FLOATING_POINT)
      // elapsed_time.real_time is in units of microseconds.
      const u_int calls_per_sec =
        this->call_count_ * 1000000u / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\treal_time\t= %u ms,\n"
                  "\t%u calls/second\n",
                  elapsed_time.real_time / 1000u,
                  calls_per_sec));
#else  /* ! ACE_LACKS_FLOATING_POINT */
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
#endif /* ! ACE_LACKS_FLOATING_POINT */
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
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_short",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_short_dii request create");

  CORBA::Short ret_short = 0;
  CORBA::Short arg_short = -3;

  // Make an Any out of the short and add it to the request arg list.
  CORBA::Any arg_holder (CORBA::_tc_short,
                         &arg_short, 
			 0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_short_dii request arg add");

  // Initialize the result variable.
  req->result ()->value ()->replace (CORBA::_tc_short, 
                                     &ret_short, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_short_dii result type");

  // Invoke, check for an exception and verify the result.

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, 
                                 "cube_short_dii invoke");

  if (ret_short != arg_short * arg_short * arg_short) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_short_dii -- bad results\n"));
      this->error_count_++;
    }

  CORBA::release (req);
}

void
DII_Cubit_Client::cube_long_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_long",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_long_dii request create");

  CORBA::Long ret_long = 0;
  CORBA::Long arg_long = -7;

  // Make an Any out of the long and add it to the request arg list.
  CORBA::Any arg_holder (CORBA::_tc_long,
                         &arg_long,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_long_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (CORBA::_tc_long, 
                                     &ret_long, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_long_dii result type");

  // Invoke, check for an exception and verify the result.

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, 
                                 "cube_long_dii invoke");

  if (ret_long != arg_long * arg_long * arg_long) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_long_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
}

void
DII_Cubit_Client::cube_octet_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_octet",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_octet_dii request create");

  CORBA::Octet ret_octet = 0;
  CORBA::Octet arg_octet = 5;

  // Make an Any out of the octet and add it to the request arg list.
  CORBA::Any arg_holder (CORBA::_tc_octet,
                         &arg_octet,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_octet_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (CORBA::_tc_octet, 
                                     &ret_octet, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_octet_dii result type");

  // Invoke, check for an exception and verify the result.
  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, 
                                 "cube_octet_dii invoke");

  if (ret_octet != arg_octet * arg_octet * arg_octet) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_octet_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
}

void
DII_Cubit_Client::cube_union_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_union",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_union_dii request create");

  Cubit::oneof arg_union, ret_union;

  arg_union._d(Cubit::e_3rd);
  arg_union.cm ().l = 5;
  arg_union.cm ().s = -7;
  arg_union.cm ().o = 3;

  // A different discrim value than arg_ret, just to be ornery.
  ret_union._d(Cubit::e_1st);
  ret_union.s (0);
   
  // Make an Any out of the union and add it to the request arg list.
  CORBA::Any arg_holder (Cubit::_tc_oneof,
                         &arg_union,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_union_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (Cubit::_tc_oneof, 
                                     &ret_union, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_union_dii result type");

  // Invoke, check for an exception and verify the result

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, "cube_union_dii invoke");

  if (ret_union.cm ().l != arg_union.cm ().l * arg_union.cm ().l * arg_union.cm ().l 
      || ret_union.cm ().s != arg_union.cm ().s * arg_union.cm ().s * arg_union.cm ().s 
      || ret_union.cm ().o != arg_union.cm ().o * arg_union.cm ().o * arg_union.cm ().o) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_union_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
}

void
DII_Cubit_Client::cube_struct_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_struct",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_struct_dii request create");

  Cubit::Many arg_struct, ret_struct;

  ret_struct.l = 0;
  ret_struct.s = 0;
  ret_struct.o = 0;
  
  arg_struct.l = 5;
  arg_struct.s = -7;
  arg_struct.o = 3;

  // Make an Any out of the struct and add it to the request arg list.
  CORBA::Any arg_holder (Cubit::_tc_Many,
                         &arg_struct,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_struct_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (Cubit::_tc_Many, 
                                     &ret_struct, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_struct_dii result type");

  // Invoke, check for an exception and verify the result.

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, "cube_struct_dii invoke");

  if (ret_struct.l != arg_struct.l * arg_struct.l * arg_struct.l 
      || ret_struct.s != arg_struct.s * arg_struct.s * arg_struct.s 
      || ret_struct.o != arg_struct.o * arg_struct.o * arg_struct.o) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_struct_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
}

void
DII_Cubit_Client::cube_octet_seq_dii (int length)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_octet_sequence",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_octet_seq_dii request create");

  // Same length as in IDL_Cubit tests so timings can be compared.
  // Return value holder is set to a different length to test resizing.
  Cubit::octet_seq ret_octet_seq (1), arg_octet_seq (length);
  arg_octet_seq.length (length);
  arg_octet_seq[0] = 4;  
  ret_octet_seq[0] = 0;

  // Make an Any out of the octet_seq and add it to the request arg list
  CORBA::Any arg_holder (Cubit::_tc_octet_seq,
                         &arg_octet_seq,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_octet_seq_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (Cubit::_tc_octet_seq, 
                                     &ret_octet_seq, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_octet_seq_dii result type");

  // Invoke, check for an exception and verify the result.

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req, 
                                 "cube_octet_seq_dii invoke");

  // Check for correct length.
  if (ret_octet_seq.length () != arg_octet_seq.length ()) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_octet_seq_dii -- bad length\n"));
      this->error_count_++;
    }

  // Check for correct value(s).
  if (ret_octet_seq[0] != arg_octet_seq[0] * arg_octet_seq[0] * arg_octet_seq[0]) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_octet_seq_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
}

void
DII_Cubit_Client::cube_long_seq_dii (int length)
{
  // Create the request ...
  CORBA::Request_ptr req;

  req = this->obj_var_->_request ("cube_long_sequence",
                                  this->env_);

  CUBIT_CHECK_ENV_RETURN_VOID ("cube_long_seq_dii request create");

  // Same length as in IDL_Cubit tests so timings can be compared.
  // Return value holder is set to a different length to test
  // resizing.
  Cubit::long_seq ret_long_seq (1);
  Cubit::long_seq arg_long_seq (length);
  arg_long_seq.length (length);
  arg_long_seq[0] = 4;
  ret_long_seq[0] = 0;

  // Make an Any out of the long_seq and add it to the request arg
  // list.
  CORBA::Any arg_holder (Cubit::_tc_long_seq,
                         &arg_long_seq,
                         0);
   
  req->arguments ()->add_value (0,
                                arg_holder,
                                CORBA::ARG_IN,
                                this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_long_seq_dii request arg add");

  // Insert the result variable.
  req->result ()->value ()->replace (Cubit::_tc_long_seq, 
                                     &ret_long_seq, 
                                     0, 
                                     this->env_);

  CUBIT_CHECK_ENV_RELEASE_RETURN_VOID (req,
                                       "cube_long_seq_dii result type");

  // Invoke, check for an exception and verify the result.

  this->call_count_++;

  req->invoke ();

  REQUEST_CHECK_ENV_RETURN_VOID (req,
                                 "cube_long_seq_dii invoke");

  // Check for correct length.
  if (ret_long_seq.length () != arg_long_seq.length ()) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_long_seq_dii -- bad length\n"));
      this->error_count_++;
    }

  // Check for correct value(s).
  if (ret_long_seq[0] != arg_long_seq[0] * arg_long_seq[0] * arg_long_seq[0]) 
    {
      ACE_ERROR ((LM_ERROR,
                  "cube_long_seq_dii -- bad results\n"));
      this->error_count_++;
    }
   
  CORBA::release (req);
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
      this->print_stats (this->stats_messages_[j], dii_elapsed_time);
    }

  TAO_TRY
    {
      // Shut down server via a DII request.
      if (this->shutdown_)
        {
          CORBA::Request_ptr req;

          req = this->obj_var_->_request ("shutdown",
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Cubit::shutdown () is a oneway operation.
          req->send_oneway ();

          CORBA::release (req);

          ACE_DEBUG ((LM_DEBUG,
                      "\n\t Shutting down IDL_Cubit server \n\n"));
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("DII_Cubit_Client: server shutdown");
      return -1;
    }
  TAO_ENDTRY;

  return this->error_count_ == 0 ? 0 : 1;
}

// Start the test.

int main (int argc, char *argv[])
{
  DII_Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\t DII_Cubit::CLIENT \n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  return cubit_client.run ();
}
