// $Id$

// ============================================================================
//
// = LIBRARY
//		TAO/tests/Cubit/TAO/DII_Cubit
//
// = FILENAME
//		client.cpp
//
// = DESCRIPTION
//      This class implements a simple CORBA client for the Cubit
//      example using DII functionality
//
// = AUTHORS
//		Jeff Parsons 
//
// ============================================================================

#include "client.h"

	// Some magic numbers used below.
const CORBA::ULong DEFAULT_LOOP_COUNT = 250;
#define DEFAULT_FACTORY_IOR "ior00"
#define DEFAULT_FACTORY_KEY "key00"

	// Constructor
DII_Cubit_Client::DII_Cubit_Client (void)
  : exit_later_ (0),
    obj_var_ (CORBA::Object::_nil ()),
	factory_var_ (CORBA::Object::_nil ()),
    orb_var_ (0),
    call_count_ (0),
    error_count_ (0),
		// The following can be set from the command line
    loop_count_ (DEFAULT_LOOP_COUNT),			
    factory_IOR_ (DEFAULT_FACTORY_IOR),
		// Either the previous one or the following three must be set
		// (if not using the naming service). It is redundant at best to
		// set both groups.
	factory_key_ (DEFAULT_FACTORY_KEY),
    hostname_ (ACE_DEFAULT_SERVER_HOST),	
    portnum_ (TAO_DEFAULT_SERVER_PORT)
{
}

	// Destructor
DII_Cubit_Client::~DII_Cubit_Client (void)
{
}

int
DII_Cubit_Client::init (int argc, char **argv)
{
	// Nice and safe.
	this->argc_ = argc;
	this->argv_ = argv;

	// Parse command line and verify parameters.
	if (this->parse_args () == -1)
		return -1;
  
	// Construct the IOR from the component args, if necessary.
	if (ACE_OS::strcmp (this->factory_key_, DEFAULT_FACTORY_KEY) &&
		!ACE_OS::strcmp (this->factory_IOR_, DEFAULT_FACTORY_IOR))
	{
		ACE_OS::sprintf (this->factory_IOR_, 
						"iiop:1.0//%s:%d/%s", 
						this->hostname_, 
						this->portnum_, 
						this->factory_key_);
	}

	TAO_TRY
	{
		// Initialize the ORB.
		orb_var_ = CORBA::ORB_init (this->argc_, this->argv_, 0, TAO_TRY_ENV);
		TAO_CHECK_ENV;

		// Get a factory object reference from the factory IOR.
		factory_var_ = this->orb_var_->string_to_object (this->factory_IOR_, 
														 TAO_TRY_ENV);
		TAO_CHECK_ENV;
			
		// Get a Cubit object with a DII request on the Cubit factory.
		CORBA::Request_ptr mc_req;

		mc_req = this->factory_var_->_request ("make_cubit", TAO_TRY_ENV);
		TAO_CHECK_ENV;

		// make_cubit takes a char* arg that it doesn't use, but we
		// must include it in the request.
		CORBA::String dummy = "";

  		CORBA::Any string_arg (CORBA::_tc_string, &dummy, CORBA::B_FALSE);
   
		mc_req->arguments ()->add_value (0, string_arg, CORBA::ARG_IN, TAO_TRY_ENV);
		TAO_CHECK_ENV;

		// Insert the result-holding variable into the request.
		mc_req->result ()->value ()->replace (CORBA::_tc_Object, 
											  &obj_var_,
											  CORBA::B_FALSE, 
											  TAO_TRY_ENV);
		TAO_CHECK_ENV;

		// Ask the Cubit factory for a Cubit object.
		mc_req->invoke ();

//		*mc_req->result ()->value () >>= this->obj_var_;

		CORBA::release (mc_req);

	}
	TAO_CATCHANY
	{
		TAO_TRY_ENV.print_exception ("DII_Cubit_Client::init");
		return -1;
	}
	TAO_ENDTRY;

	return 0;
}

int
DII_Cubit_Client::parse_args (void)
{
	ACE_Get_Opt opts (argc_, argv_, "dn:i:h:p:k:x");
	int c;
   
	while ((c = opts ()) != -1)
		switch (c) 
		{
			case 'd':  // debug flag
				TAO_debug_level++;
			break;

			case 'n':			// loop count
				this->loop_count_ = ACE_OS::atoi (opts.optarg);
			break;

				// The next 3 are for building the IOR from pieces.
			case 'h':
				this->hostname_ = opts.optarg;
			break;

			case 'p':
				this->portnum_ = ACE_OS::atoi (opts.optarg);
			break;

			case 'k':
				this->factory_key_ = opts.optarg;
			break;

				// and if the whole IOR is in one arg...
			case 'i':			
				this->factory_IOR_ = opts.optarg;
			break;

			case 'x':
				this->exit_later_++;
			break;

			case '?':
			default:
				ACE_ERROR_RETURN ((LM_ERROR,
							"usage:  %s"
							" [-d]"
							" [-n loopcount]"
							" [-i cubit-factory-IOR]"
							" [-h hostname]"
							" [-p port]"
							" [-k cubit-factory-key]"
							" [-x]"
							"\n", argv_ [0]
								 ), -1);
		}
   
	return 0;  // Indicates successful parsing of command line.
}

// Formats and prints time statistics.
// Identical to function in IDL_Cubit, for ease of comparison.
void
DII_Cubit_Client::print_stats (const char *call_name,
							   ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time)
{
	ACE_DEBUG ((LM_DEBUG,
				"%s:\n",
				call_name));

	if (this->call_count_ > 0 && this->error_count_ == 0)
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
		double calls_per_sec = this->call_count_ / elapsed_time.real_time;

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
					"\tNo time stats printed.  Call count zero or error ocurred.\n"));

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

	req = this->obj_var_->_request ("cube_short", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_short_dii request create");
		return;
    }

	CORBA::Short ret_short, arg_short = -3;

	// Make an Any out of the short and add it to the request arg list.
	CORBA::Any arg_holder (CORBA::_tc_short, &arg_short, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_short_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Initialize the result variable.
	req->result ()->value ()->replace (CORBA::_tc_short, 
									   &ret_short, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_short_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_short_dii invoke");
		CORBA::release (req);
		return;
    }

	if (ret_short != arg_short * arg_short * arg_short) 
    {
		ACE_ERROR ((LM_ERROR, "cube_short_dii -- bad results\n"));
		this->error_count_++;
    }

	CORBA::release (req);

}

void
DII_Cubit_Client::cube_long_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_long", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_dii request create");
		return;
    }

	CORBA::Long ret_long, arg_long = -7;

	// Make an Any out of the long and add it to the request arg list.
	CORBA::Any arg_holder (CORBA::_tc_long, &arg_long, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (CORBA::_tc_long, 
									   &ret_long, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_long_dii invoke");
		CORBA::release (req);
		return;
    }

	if (ret_long != arg_long * arg_long * arg_long) 
    {
		ACE_ERROR ((LM_ERROR, "cube_long_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

void
DII_Cubit_Client::cube_octet_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_octet", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_dii request create");
		return;
    }

	CORBA::Octet ret_octet, arg_octet = 5;

	// Make an Any out of the octet and add it to the request arg list.
	CORBA::Any arg_holder (CORBA::_tc_octet, &arg_octet, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (CORBA::_tc_octet, 
									   &ret_octet, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_octet_dii invoke");
		CORBA::release (req);
		return;
    }

	if (ret_octet != arg_octet * arg_octet * arg_octet) 
    {
		ACE_ERROR ((LM_ERROR, "cube_octet_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

void
DII_Cubit_Client::cube_union_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_union", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_union_dii request create");
		return;
    }

	Cubit::oneof arg_union, ret_union;
   
	arg_union._d(Cubit::e_3rd);
	arg_union.cm ().l = 5;
	arg_union.cm ().s = -7;
	arg_union.cm ().o = 3;

	// Make an Any out of the union and add it to the request arg list.
	CORBA::Any arg_holder (Cubit::_tc_oneof, &arg_union, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_union_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (Cubit::_tc_oneof, 
									   &ret_union, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_union_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_union_dii invoke");
		CORBA::release (req);
		return;
    }

	if (ret_union.cm ().l != arg_union.cm ().l * arg_union.cm ().l * arg_union.cm ().l ||
		ret_union.cm ().s != arg_union.cm ().s * arg_union.cm ().s * arg_union.cm ().s ||
		ret_union.cm ().o != arg_union.cm ().o * arg_union.cm ().o * arg_union.cm ().o) 
    {
		ACE_ERROR ((LM_ERROR, "cube_union_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

void
DII_Cubit_Client::cube_struct_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_struct", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_struct_dii request create");
		return;
    }

	Cubit::Many arg_struct, ret_struct;
   
	arg_struct.l = 5;
	arg_struct.s = -7;
	arg_struct.o = 3;

	// Make an Any out of the struct and add it to the request arg list.
	CORBA::Any arg_holder (Cubit::_tc_Many, &arg_struct, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_struct_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (Cubit::_tc_Many, 
									   &ret_struct, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_struct_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_struct_dii invoke");
		CORBA::release (req);
		return;
    }

	if (ret_struct.l != arg_struct.l * arg_struct.l * arg_struct.l ||
		ret_struct.s != arg_struct.s * arg_struct.s * arg_struct.s ||
		ret_struct.o != arg_struct.o * arg_struct.o * arg_struct.o) 
    {
		ACE_ERROR ((LM_ERROR, "cube_struct_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

void
DII_Cubit_Client::cube_octet_seq_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_octet_sequence", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_seq_dii request create");
		return;
    }

	// Same length as in IDL_Cubit tests so timings can be compared.
	Cubit::octet_seq ret_octet_seq (1), arg_octet_seq;
	arg_octet_seq.length (1);
	arg_octet_seq[0] = 4;

	// Make an Any out of the raw and add it to the request arg list
	CORBA::Any arg_holder (Cubit::_tc_octet_seq, &arg_octet_seq, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_seq_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (Cubit::_tc_octet_seq, 
									   &ret_octet_seq, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_octet_seq_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_octet_seq_dii invoke");
		CORBA::release (req);
		return;
    }

	// Check for correct length.
	if (ret_octet_seq.length () != arg_octet_seq.length ()) 
    {
		ACE_ERROR ((LM_ERROR, "cube_octet_seq_dii -- bad length\n"));
		this->error_count_++;
    }

	// Check for correct value(s).
	if (ret_octet_seq[0] != arg_octet_seq[0] * arg_octet_seq[0] * arg_octet_seq[0]) 
    {
		ACE_ERROR ((LM_ERROR, "cube_octet_seq_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

void
DII_Cubit_Client::cube_long_seq_dii (void)
{
	// Create the request ...
	CORBA::Request_ptr req;

	req = this->obj_var_->_request ((CORBA::String) "cube_long_sequence", this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_seq_dii request create");
		return;
    }

	// Same length as in IDL_Cubit tests so timings can be compared.
	Cubit::long_seq ret_long_seq (1), arg_long_seq;
	arg_long_seq.length (1);
	arg_long_seq[0] = 4;

	// Make an Any out of the sequence and add it to the request arg list.
	CORBA::Any arg_holder (Cubit::_tc_long_seq, &arg_long_seq, CORBA::B_FALSE);
   
	req->arguments ()->add_value (0, arg_holder, CORBA::ARG_IN, this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_seq_dii request arg add");
		CORBA::release (req);
		return;
    }

	// Insert the result variable.
	req->result ()->value ()->replace (Cubit::_tc_long_seq, 
									   &ret_long_seq, 
									   CORBA::B_TRUE, 
									   this->env_);

	if (this->env_.exception () != 0) 
    {
		this->error_count_++;
		this->env_.print_exception ("cube_long_seq_dii result type");
		CORBA::release (req);
		return;
    }

	// Invoke, check for an exception and verify the result.

	this->call_count_++;

	req->invoke ();

	if (req->env ()->exception () != 0) 
    {
		this->error_count_++;
		req->env ()->print_exception ("cube_long_seq_dii invoke");
		CORBA::release (req);
		return;
    }

	// Check for correct length.
	if (ret_long_seq.length () != arg_long_seq.length ()) 
    {
		ACE_ERROR ((LM_ERROR, "cube_long_seq_dii -- bad length\n"));
		this->error_count_++;
    }

	// Check for correct value(s).
	if (ret_long_seq[0] != arg_long_seq[0] * arg_long_seq[0] * arg_long_seq[0]) 
    {
		ACE_ERROR ((LM_ERROR, "cube_long_seq_dii -- bad results\n"));
		this->error_count_++;
    }
   
	CORBA::release (req);
}

int
DII_Cubit_Client::run (void)
{
	// NOTE - default loop_count_ is 250 but can be set from the command line.
//#if 0
	// loop counter.
	u_int i;

	// Make a timer and an elapsed time holder.
	ACE_Profile_Timer dii_timer;
	ACE_Profile_Timer::ACE_Elapsed_Time dii_elapsed_time;

	dii_timer.start ();

	for (i = 0; i < this->loop_count_; i++)
    {
		this->cube_short_dii ();
		this->cube_octet_dii ();
		this->cube_long_dii ();
		this->cube_struct_dii ();
		this->cube_octet_seq_dii ();
		this->cube_long_seq_dii ();
    }

	dii_timer.stop ();

	// Put the stats into the holding variable and display.
	dii_timer.elapsed_time (dii_elapsed_time);  
	this->print_stats ("DII cube average call", dii_elapsed_time);

	this->call_count_ = 0;
	this->error_count_ = 0;

	dii_timer.start ();

	for (i = 0; i < this->loop_count_; i++)
    {
		this->cube_union_dii ();
    }

	dii_timer.stop ();

	// Put the stats into the holding variable and display.
	dii_timer.elapsed_time (dii_elapsed_time);  
	this->print_stats ("cube_union_dii call", dii_elapsed_time);
//#endif
// Using the single calls below to track down memory leaks.
// They will be gone eventually.
#if 0
	this->cube_short_dii ();
//	this->cube_long_dii ();
//	this->cube_octet_dii ();
//	this->cube_union_dii ();
//	this->cube_struct_dii ();
//	this->cube_raw_dii ();
//	this->cube_sequence_dii ();
#endif
	return this->error_count_ == 0 ? 0 : 1;
}

	// Start the test.

int main (int argc, char **argv)
{

	DII_Cubit_Client cubit_client;

	if (cubit_client.init (argc, argv) == -1)
		return 1;
	else
		return cubit_client.run ();

	return 0;
}