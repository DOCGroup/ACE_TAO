// $Id$

#include "ace/Profile_Timer.h"
#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "client.h"
#include "orbsvcs/CosNamingC.h"

#define quote(x) #x

const int max_sequence_length = 100;
// Limit the sequence length, otherwise the time to run the test
// increases to fast....

// Constructor.
Cubit_Client::Cubit_Client (void)
  : cubit_factory_key_ (0),
    cubit_key_ (ACE_OS::strdup ("key0")),
    loop_count_ (250),
    shutdown_ (0),
    cubit_ (Cubit::_nil ()),
    call_count_ (0),
    error_count_ (0),
    cubit_factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    use_naming_service_ (1)
{
}

// Simple function that returns the substraction of 117 from the
// parameter.

int
Cubit_Client::func (u_int i)
{
  return i - 117;
}

// Reads the Cubit factory ior from a file

int
Cubit_Client::read_ior (char *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (this->f_handle_);
  this->cubit_factory_key_ = ior_buffer.read ();

  if (this->cubit_factory_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
   return 0;
}

// Parses the command line arguments and returns an error status.

int
Cubit_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:k:xs");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':                 // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
            break;
      case 'k': // read the cubit IOR from the command-line.
        this->cubit_factory_key_ =
          ACE_OS::strdup (get_opts.optarg);
        break;
      case 'x':
        this->shutdown_ = 1;
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_ = 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f cubit_factory-obj-ref-key-file]"
                           " [-k cubit-obj-ref-key]"
                           " [-x]"
                           " [-s]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Exercise the union.  Cube a union.

void
Cubit_Client::cube_union_stub (void)
{
  CORBA::Environment env;
  Cubit::oneof u;

  this->call_count_++;

  u.l (3); // use the long union branch.

  // Cube a "union" ...
  Cubit::oneof r = this->cubit_->cube_union (u, this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_union");
      this->error_count_++;
    }
  else
    {
      dmsg ("cube union ...");
      u.l (u.l () * u.l () * u.l ()) ;

      if (u.l () != r.l ())
        {
          ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
          this->error_count_++;
        }

    }

  this->call_count_++;

  u._d (Cubit::e_3rd);
#if 0
  u.cm ().l = this->func (i);
  u.cm ().s = this->func (i);
  u.cm ().o = this->func (i);
#endif

  u.cm ().l = 7;
  u.cm ().s = 5;
  u.cm ().o = 3;

  // Cube another "union" which uses the default arm ... NOT tested yet
  r = this->cubit_->cube_union (u, this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_union");
      this->error_count_++;
    }
  else
    {
      dmsg ("cube union ...");
      u.cm ().l = u.cm ().l * u.cm ().l * u.cm ().l;
      u.cm ().s = u.cm ().s * u.cm ().s * u.cm ().s;
      u.cm ().o = u.cm ().o * u.cm ().o * u.cm ().o;

      if (u.cm ().l != r.cm ().l
          || u.cm ().s != r.cm ().s
          || u.cm ().o != r.cm ().o)
        {
          ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
          this->error_count_++;
        }
    }
}

// Exercise the union using dii.

void
Cubit_Client::cube_union_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req;

  this->call_count_++;

  req = this->cubit_->_request ((CORBA::String) "cube_union", this->env_);

  if (this->env_.exception () != 0)
    {
      this->error_count_++;

      this->env_.print_exception ("cube_union_dii request create");
      return;
    }

  // ... initialise the argument list and result ...
  Cubit::oneof  *u, *r;

  u = new Cubit::oneof;
  u->_d (Cubit::e_3rd);

  u->cm ().l = 5;
  u->cm ().s = -7;
  u->cm ().o = 3;

  CORBA::Any tmp_arg (Cubit::_tc_oneof, u, CORBA::B_FALSE);

  req->arguments ()->add_value ("values",
                                tmp_arg,
                                CORBA::ARG_IN,
                                this->env_);

  if (this->env_.exception () != 0)
    {
      this->error_count_++;
      this->env_.print_exception ("cube_union_dii request arg add");
      CORBA::release (req);
      return;
    }

  // set the result's typecode to indicate what we expect. Let the ORB allocate
  // the memory
  req->result ()->value ()->replace (Cubit::_tc_oneof,
                                     0,
                                     0,
                                     this->env_);

  if (this->env_.exception () != 0)
    {
      this->error_count_++;
      this->env_.print_exception ("cube_union_dii result type");
      CORBA::release (req);
      return;
    }

  // Make the invocation, verify the result.

  req->invoke ();

  if (req->env ()->exception () != 0)
    {
      this->error_count_++;
      req->env ()->print_exception ("cube_union_dii invoke");
      CORBA::release (req);
      return;
    }

  // retrieve the value in "r". Note that we don't own the memory that the ORb
  // has allocated
  if (!((*req->result ()->value ()) >>= r))
    {
      ACE_ERROR ((LM_ERROR,
                  "Error retrieving union oneof via the >>= operator\n"));
      CORBA::release (req);
      return;
    }

  if (r->cm ().o != 27 || r->cm ().l != 125 || r->cm ().s != -343)
    {
      this->error_count_++;
      ACE_ERROR ((LM_ERROR, "cube_union_dii -- bad results\n"));
    }
  else
    dmsg ("cube_union_dii ... success!!");

  CORBA::release (req);
  delete u;
}

// Cube a short.

void
Cubit_Client::cube_short (int i)
{
  CORBA::Short arg_short = this->func (i);

  // Cube a short.
  CORBA::Short ret_short = cubit_->cube_short (arg_short, this->env_);

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_short");
      this->error_count_++;
    }
  else
    {
      dmsg2 ("cube short:  %d --> %d\n",
             arg_short,
             ret_short);
      arg_short = arg_short * arg_short * arg_short;

      if (arg_short != ret_short)
        {
          ACE_ERROR ((LM_ERROR, "** cube_short (%d) ERROR (--> %d)\n",
                      (CORBA::Short) this->func (i),
                      ret_short));
          this->error_count_++;
        }
    }
}

// Cube an octet

void
Cubit_Client::cube_octet (int i)
{
  CORBA::Octet arg_octet = this->func (i);

   // Cube an octet.
  CORBA::Octet ret_octet = this->cubit_->cube_octet (arg_octet, this->env_);

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_octet");
      this->error_count_++;
    }
  else
    {
      dmsg2 ("cube octet:  %d --> %d\n", arg_octet, ret_octet);
      arg_octet = arg_octet * arg_octet * arg_octet;
      if (arg_octet != ret_octet) {
        ACE_DEBUG ((LM_DEBUG,
                    "** cube_octet (%d) ERROR (--> %d)\n",
                    (CORBA::Octet) this->func (i),
                    ret_octet));
        this->error_count_++;
      }
    }
}

// calculate the cube from a long

void
Cubit_Client::cube_long (int i)
{
  CORBA::Long arg_long = this->func (i);

  // Cube a long.
  CORBA::Long ret_long = this->cubit_->cube_long (arg_long, this->env_);;

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_long");
      this->error_count_++;
    }
  else
    {
      dmsg2 ("cube long:  %d --> %d\n", arg_long, ret_long);
      arg_long = arg_long * arg_long * arg_long;

      if (arg_long != ret_long)
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube_long (%ld) ERROR (--> %ld)\n",
                      (CORBA::Long) this->func (i),
                      ret_long));
          this->error_count_++;
        }
    }
}

// Cube the numbers in a struct

void
Cubit_Client::cube_struct (int i)
{
  Cubit::Many arg_struct;
  Cubit::Many ret_struct;

  this->call_count_++;

  arg_struct.l = this->func (i);
  arg_struct.s = this->func (i);
  arg_struct.o = this->func (i);

  // Cube a "struct" ...
  ret_struct = this->cubit_->cube_struct (arg_struct, this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_struct");
      this->error_count_++;
    }
  else
    {
      dmsg ("cube struct ...");
      arg_struct.l = arg_struct.l * arg_struct.l * arg_struct.l;
      arg_struct.s = arg_struct.s * arg_struct.s * arg_struct.s;
      arg_struct.o = arg_struct.o * arg_struct.o * arg_struct.o;

      if (arg_struct.l != ret_struct.l
          || arg_struct.s != ret_struct.s
          || arg_struct.o != ret_struct.o)
        {
          ACE_ERROR ((LM_ERROR, "** cube_struct ERROR\n"));
          this->error_count_++;
        }
    }
}

// Cube the numbers in a sequence

void
Cubit_Client::cube_sequence (int i)
{
  this->call_count_++;

  int l = i + 1;
  if (l > max_sequence_length)
    l = max_sequence_length;

  Cubit::vector input (l);
  input.length (l);

  // Fill in the input sequence...
  for (int j = 0; j < l; ++j)
    input[j] = j;

  Cubit::vector_var output;
  Cubit::vector_out vout (output);

  // Cube the sequence
  this->cubit_->cube_sequence (input, vout, this->env_);

  //  Cubit::vector& output = *vout.ptr ();
  //  output = vout;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_struct");
      this->error_count_++;
    }
  else
    {
      if (output->length () != input.length ())
        {
          ACE_ERROR ((LM_ERROR, "** cube sequence, wrong length\n"));
          this->error_count_++;
        }

      u_int rl = output->length ();
      if (input.length () < rl)
        rl = input.length ();
      for (u_int j = 0; j < rl; ++j)
        {
          int x = input[j];
          if (x*x*x != output[j])
            {
              ACE_ERROR ((LM_ERROR, "** cube_sequence ERROR\n"));
              this->error_count_++;
            }
        }
    }
}

// Cube the numbers in a struct

void
Cubit_Client::cube_struct_dii (void)
{
  // Create the request ...
  CORBA::Request_ptr req =
    this->cubit_->_request ((CORBA::String) "cube_struct", this->env_);

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("DII request create");
      return;
    }

  // ... initialise the argument list and result ...
  Cubit::Many arg;
  Cubit::Many *result;

  arg.o = 3;
  arg.l = 5;
  arg.s = -7;

  CORBA::Any tmp_arg (Cubit::_tc_Many, &arg, CORBA::B_FALSE);

  req->arguments ()->add_value ("values",
                                tmp_arg,
                                CORBA::ARG_IN, this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("DII request arg add");
      CORBA::release (req);
      return;
    }

  // set the typecode for the result and expect the ORB to allocate the memory
  req->result ()->value ()->replace (Cubit::_tc_Many,
                                     0,
                                     0,
                                     this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("DII request result type");
      CORBA::release (req);
      return;
    }

  // Make the invocation, verify the result

  req->invoke ();

  if (req->env ()->exception () != 0)
    {
      req->env ()->print_exception ("DII invoke sending struct");
      CORBA::release (req);
      return;
    }

  if (!((*req->result ()->value ()) >>= result))
    {
      ACE_ERROR ((LM_ERROR,
                  "Error retrieving struct Many via the >>= operator\n"));
      CORBA::release (req);
      return;
    }

  if (result->o != 27 || result->l != 125 || result->s != -343)
    ACE_ERROR ((LM_ERROR, "DII cube_struct -- bad results\n"));
  else
    dmsg ("DII cube_struct ... success!!");

  CORBA::release (req);
}

void
Cubit_Client::print_stats (const char *call_name,
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

// Execute client example code.

int
Cubit_Client::run (void)
{
  u_int i;

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

  //  ACE_Time_Value before;

  // We start an ACE_Profile_Timer here...
  timer.start ();

  // Make the calls in a loop.
  for (i = 0; i < this->loop_count_; i++)
    {
      this->cube_short (i);
      this->cube_octet (i);
      this->cube_long (i);
      this->cube_struct (i);
      this->cube_sequence (i);
    }

  // stop the timer.
  timer.stop ();

  timer.elapsed_time (elapsed_time);
  // compute call average call time.
  this->print_stats ("cube average call", elapsed_time);

  // Simple test for DII: call "cube_struct". (It's not timed since
  // the copious mallocation of DII would bias numbers against typical
  // stub-based calls).
  this->call_count_ = 0;
  this->error_count_ = 0;

  timer.start ();

  // Make the calls in a loop.
  for (i = 0; i < this->loop_count_; i++)
    this->cube_struct_dii ();

  timer.stop ();

  timer.elapsed_time (elapsed_time);
  // compute call average call time.
  this->print_stats ("cube_struct_dii call", elapsed_time);

  // ------------------>
  // Two more tests, using the "cube_union" function

  this->call_count_ = 0;
  this->error_count_ = 0;

  // unions using stubs
  timer.start ();

  // Make the calls in a loop.
  for (i = 0; i < this->loop_count_; i++)
    this->cube_union_stub ();

  timer.stop ();

  timer.elapsed_time (elapsed_time);
  // compute call average call time.
  this->print_stats ("cube_union_stub call", elapsed_time);

  this->call_count_ = 0;
  this->error_count_ = 0;

  // union DII
  timer.start ();

  // Make the calls in a loop.
  for (i = 0; i < this->loop_count_; i++)
    this->cube_union_dii ();

  timer.stop ();

  timer.elapsed_time (elapsed_time);
  // compute call average call time.
  this->print_stats ("cube_union_dii call", elapsed_time);

  this->call_count_ = 0;
  this->error_count_ = 0;

  // Sequences
  timer.start ();

  // Make the calls in a loop.
  for (i = 0; i < this->loop_count_; i++)
    this->cube_sequence (this->loop_count_);

  timer.stop ();

  timer.elapsed_time (elapsed_time);
  // compute call average call time.
  this->print_stats ("cube_sequence", elapsed_time);

  if (this->shutdown_)
    {
      this->cubit_->shutdown (this->env_);
      dexc (this->env_, "server, please ACE_OS::exit");
    }

  return this->error_count_ == 0 ? 0 : 1;
}

Cubit_Client::~Cubit_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->cubit_factory_ior_file_)
    ACE_OS::fclose (this->cubit_factory_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  CORBA::release (this->cubit_);

  if (this->cubit_factory_key_ != 0)
    ACE_OS::free (this->cubit_factory_key_);
  if (this->cubit_key_ != 0)
    ACE_OS::free (this->cubit_key_);
}

int
Cubit_Client::init_naming_service (void)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name cubit_factory_name (2);
      cubit_factory_name.length (2);
      cubit_factory_name[0].id = CORBA::string_dup ("IDL_Cubit");
      cubit_factory_name[1].id = CORBA::string_dup ("cubit_factory");
      CORBA::Object_var factory_obj =
        naming_context->resolve (cubit_factory_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->factory_ =
        Cubit_Factory::_narrow (factory_obj.in (),TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve cubit factory in Naming service <%s>\n"),
                          -1);
   }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Cubit::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Cubit_Client::init (int argc, char **argv)
{
  int naming_result;
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->use_naming_service_)
        {
          naming_result = this->init_naming_service ();
          if (naming_result < 0)
            return naming_result;
        }
      else
        {
          if (this->cubit_factory_key_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: no cubit factory key specified\n",
                               this->argv_[0]),
                              -1);


          CORBA::Object_var factory_object =
            this->orb_->string_to_object (this->cubit_factory_key_,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->factory_ =
            Cubit_Factory::_narrow (factory_object.in(), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->cubit_factory_key_),
                              -1);
        }

      ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));

      // Now retrieve the Cubit obj ref corresponding to the key.
      this->cubit_ =
        this->factory_->make_cubit (this->cubit_key_,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->cubit_))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null cubit objref returned by factory\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Cubit::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This function runs the test.

int
main (int argc, char **argv)
{
  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t IDL_Cubit: client \n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;
  else
    return cubit_client.run ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Env_Value<unsigned long>;
template class ACE_Env_Value<unsigned short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Env_Value<unsigned long>
#pragma instantiate ACE_Env_Value<unsigned short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
