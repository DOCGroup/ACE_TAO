// $Id$

#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "client_i.h"
#include "orbsvcs/CosNamingC.h"

#include "tao/Timeprobe.h"

ACE_RCSID(IDL_Cubit, client_i, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *Cubit_Client_Timeprobe_Description[] =
{
  "Cubit_Client::cube_oneway - start",
  "Cubit_Client::cube_oneway - end",

  "Cubit_Client::cube_void - start",
  "Cubit_Client::cube_void - end",

  "Cubit_Client::cube_octet - start",
  "Cubit_Client::cube_octet - end",

  "Cubit_Client::cube_short - start",
  "Cubit_Client::cube_short - end",

  "Cubit_Client::cube_long - start",
  "Cubit_Client::cube_long - end",

  "Cubit_Client::cube_struct - start",
  "Cubit_Client::cube_struct - end",

  "Cubit_Client::cube_union - start",
  "Cubit_Client::cube_union - end",

  "Cubit_Client::cube_long_sequence - start",
  "Cubit_Client::cube_long_sequence - end",

  "Cubit_Client::cube_octet_sequence - start",
  "Cubit_Client::cube_octet_sequence - end"
};

enum
{
  // Timeprobe description table start key 
  CUBIT_CLIENT_CUBE_ONEWAY_START = 10000,
  CUBIT_CLIENT_CUBE_ONEWAY_END,

  CUBIT_CLIENT_CUBE_VOID_START,
  CUBIT_CLIENT_CUBE_VOID_END,

  CUBIT_CLIENT_CUBE_OCTET_START,
  CUBIT_CLIENT_CUBE_OCTET_END,

  CUBIT_CLIENT_CUBE_SHORT_START,
  CUBIT_CLIENT_CUBE_SHORT_END,

  CUBIT_CLIENT_CUBE_LONG_START,
  CUBIT_CLIENT_CUBE_LONG_END,

  CUBIT_CLIENT_CUBE_STRUCT_START,
  CUBIT_CLIENT_CUBE_STRUCT_END,

  CUBIT_CLIENT_CUBE_UNION_START,
  CUBIT_CLIENT_CUBE_UNION_END,

  CUBIT_CLIENT_CUBE_LONG_SEQUENCE_START,
  CUBIT_CLIENT_CUBE_LONG_SEQUENCE_END,

  CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_START,
  CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Cubit_Client_Timeprobe_Description,
                                  CUBIT_CLIENT_CUBE_OCTET_START);

#endif /* ACE_ENABLE_TIMEPROBES */

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
    use_naming_service_ (1),
    only_void_ (0),
    only_oneway_ (0)
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
  this->cubit_factory_key_ =
    ior_buffer.read ();

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
  ACE_Get_Opt get_opts (argc_, argv_, "ovdn:f:k:xs");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
	this->only_void_ = 1;
	break;
      case 'o':
	this->only_oneway_ = 1;
	break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':                 // loop count
        this->loop_count_ =
          (u_int) ACE_OS::atoi (get_opts.optarg);
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
Cubit_Client::cube_union (void)
{
  CORBA::Environment env;
  Cubit::oneof u;

  this->call_count_++;

  u.l (3); // use the long union branch.

  // Cube a "union" ...
  Cubit::oneof r;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);

    r = this->cubit_->cube_union (u, this->env_);
  }

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
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);

    r = this->cubit_->cube_union (u, this->env_);
  }

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

// Cube a short.

void
Cubit_Client::cube_short (int i)
{
  CORBA::Short arg_short = this->func (i);

  // Cube a short.
  CORBA::Short ret_short;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_SHORT_START);

    ret_short = cubit_->cube_short (arg_short, this->env_);
  }

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

// Oneway test.

void
Cubit_Client::cube_oneway (int i)
{
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_ONEWAY_START);

    this->cubit_->cube_oneway (this->env_);
  }

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_oneway");
      this->error_count_++;
    }
}

void
Cubit_Client::cube_void (int i)
{
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_VOID_START);

    this->cubit_->cube_void (this->env_);
  }

  this->call_count_++;

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("from cube_void");
      this->error_count_++;
    }
}

// Cube an octet

void
Cubit_Client::cube_octet (int i)
{
  CORBA::Octet arg_octet = this->func (i);

   // Cube an octet.
  CORBA::Octet ret_octet;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_START);

    ret_octet = this->cubit_->cube_octet (arg_octet, this->env_);
  }

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
  CORBA::Long ret_long;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_LONG_START);

    ret_long = this->cubit_->cube_long (arg_long, this->env_);
  }

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
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_STRUCT_START);

    ret_struct = this->cubit_->cube_struct (arg_struct, this->env_);
  }

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
Cubit_Client::cube_long_sequence (int i, int l)
{
  this->call_count_++;

  Cubit::long_seq input (l);
  input.length (l);

#if 0
  // Fill in the input sequence...
  for (int j = 0; j < l; ++j)
    input[j] = j;
#else
  // Just set the first item, otherwise it is hard to compare the
  // results for longer sequences, i.e. more than just marshalling
  // gets in the way.
  input[0] = 4;
#endif

  Cubit::long_seq_var output;
  Cubit::long_seq_out vout (output);

  // Cube the sequence
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_LONG_SEQUENCE_START);

    this->cubit_->cube_long_sequence (input, vout, this->env_);
  }

  //  Cubit::long_seq& output = *vout.ptr ();
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
#if 0
      for (u_int j = 0; j < rl; ++j)
        {
          CORBA::Long x = input[j];
          if (x*x*x != output[j])
            {
              ACE_ERROR ((LM_ERROR, "** cube_long_sequence ERROR\n"));
              this->error_count_++;
            }
        }
#else
      CORBA::Long x = input[0];
      if (x * x *x != output[0])
        {
          ACE_ERROR ((LM_ERROR, "** cube_long_sequence ERROR\n"));
          this->error_count_++;
        }
#endif
    }
}

void
Cubit_Client::cube_octet_sequence (int i, int l)
{
  this->call_count_++;

  Cubit::octet_seq input (l);
  input.length (l);

#if 0
  // Fill in the input sequence...
  for (int j = 0; j < l; ++j)
    input[j] = j;
#else
  // Just set the first item, otherwise it is hard to compare the
  // results for longer sequences, i.e. more than just marshalling
  // gets in the way.
  input[0] = 4;
#endif

  Cubit::octet_seq_var output;
  Cubit::octet_seq_out vout (output);

  // Cube the sequence
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_START);

    this->cubit_->cube_octet_sequence (input, vout, this->env_);
  }

  //  Cubit::long_seq& output = *vout.ptr ();
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
          ACE_ERROR ((LM_ERROR, "** cube octet, wrong length\n"));
          this->error_count_++;
        }

      u_int rl = output->length ();
      if (input.length () < rl)
        rl = input.length ();
#if 0
      for (u_int j = 0; j < rl; ++j)
        {
          CORBA::Octet x = input[j];
          if (x*x*x != output[j])
            {
              ACE_ERROR ((LM_ERROR, "** cube_octet ERROR\n"));
              this->error_count_++;
            }
        }
#else
      CORBA::Octet x = input[0];
      if (x * x *x != output[0])
        {
          ACE_ERROR ((LM_ERROR, "** cube_octet ERROR\n"));
          this->error_count_++;
        }
#endif
    }
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
Cubit_Client::run (int testing_collocation)
{
  if (this->only_void_)
    return this->run_void ();

  if (this->only_oneway_)
    return this->run_oneway ();

  u_int i;

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
  //  ACE_Time_Value before;


  //
  // Show the results one type at a time.
  //

  // ONEWAY
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_oneway (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_oneway", elapsed_time);

  // VOID
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_void (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_void", elapsed_time);

  // SHORT
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_short (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_short", elapsed_time);

  // OCTET
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_octet (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_octet", elapsed_time);

  // LONG
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_long (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_long", elapsed_time);

  // STRUCT
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_struct (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_struct", elapsed_time);

  // UNION
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_union ();
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_union_stub call", elapsed_time);

  // SMALL LONG SEQUENCES
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_long_sequence (this->loop_count_, 4);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_small_sequence<long>", elapsed_time);

  // LARGE LONG SEQUENCES
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_long_sequence (this->loop_count_, 1024);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_large_sequence<long>", elapsed_time);

  // SMALL OCTET SEQUENCES
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_octet_sequence (this->loop_count_, 16);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_small_sequence<octet>", elapsed_time);

  // LARGE OCTET SEQUENCES
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_octet_sequence (this->loop_count_, 4096);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_large_sequence<octet>", elapsed_time);

  // MIXIN
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    {
      this->cube_short (i);
      this->cube_octet (i);
      this->cube_long (i);
    }
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube mixin (short/octet/long)", elapsed_time);

  if (testing_collocation)
    {
      // @@ Nanbor, this code should be split into a separate method.
      TAO_ORB_Core_instance ()->using_collocation (CORBA::B_FALSE);
      // Make sure we call the following method "remotely" so
      // the right ORB could be used.

      TAO_TRY
        {
          // @@ This code should be replaced with the
          // TAO_Naming_Client helper class.
          CORBA::Object_var naming_obj =
            this->orb_->resolve_initial_references ("NameService");

          if (CORBA::is_nil (naming_obj.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to resolve the NameService.\n"),
                              -1);

          CosNaming::NamingContext_var naming_context =
            CosNaming::NamingContext::_narrow (naming_obj.in (),
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CosNaming::Name cubit_shutdown_name (2);
          cubit_shutdown_name.length (2);
          cubit_shutdown_name[0].id = 
            CORBA::string_dup ("IDL_Cubit");
          cubit_shutdown_name[1].id =
            CORBA::string_dup ("shutdown");

          CORBA::Object_var shutdown_obj =
            naming_context->resolve (cubit_shutdown_name,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;

          Cubit_Shutdown_var shutdown =
            Cubit_Shutdown::_narrow (shutdown_obj.in (),
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (shutdown.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "null cubit objref returned by factory\n"),
                              -1);
          shutdown->shutdown (this->env_);
          TAO_CHECK_ENV;

          dexc (this->env_,
                "server, please ACE_OS::exit");
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("Cubit::init");
          return -1;
        }
      TAO_ENDTRY;

    }
  else if (this->shutdown_)
    {
      this->cubit_->shutdown (this->env_);
      dexc (this->env_, "server, please ACE_OS::exit");
    }


  return this->error_count_ == 0 ? 0 : 1;
}

int
Cubit_Client::run_oneway (void)
{
  u_int i;

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
  //  ACE_Time_Value before;


  // ONEWAY
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_oneway (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_oneway", elapsed_time);

  return this->error_count_ == 0 ? 0 : 1;
}

int
Cubit_Client::run_void (void)
{
  u_int i;

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
  //  ACE_Time_Value before;

  // VOID
  this->call_count_ = 0;
  this->error_count_ = 0;
  timer.start ();
  for (i = 0; i < this->loop_count_; i++)
    this->cube_void (i);
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  this->print_stats ("cube_void", elapsed_time);

  return this->error_count_ == 0 ? 0 : 1;
}

Cubit_Client::~Cubit_Client (void)
{
  // Free resources and close the IOR files.
  if (this->cubit_factory_ior_file_)
    ACE_OS::fclose (this->cubit_factory_ior_file_);

  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  CORBA::release (this->cubit_);

  if (this->cubit_factory_key_ != 0)
    delete [] (this->cubit_factory_key_);

  if (this->cubit_key_ != 0)
    ACE_OS::free (this->cubit_key_);
}

int
Cubit_Client::init_naming_service (void)
{
  TAO_TRY
    {
      // @@ This code should use the new TAO_Naming_Client helper
      // class.
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the NameService.\n"),
                          -1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name cubit_factory_name (2);
      cubit_factory_name.length (2);
      cubit_factory_name[0].id =
        CORBA::string_dup ("IDL_Cubit");
      cubit_factory_name[1].id =
        CORBA::string_dup ("cubit_factory");
      CORBA::Object_var factory_obj =
        naming_context->resolve (cubit_factory_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->factory_ =
        Cubit_Factory::_narrow (factory_obj.in (),
                                TAO_TRY_ENV);
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
          if (this->init_naming_service () == -1)
            return -1;
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
            Cubit_Factory::_narrow (factory_object.in(),
                                    TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->cubit_factory_key_),
                              -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Factory received OK\n"));

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Env_Value<unsigned long>;
template class ACE_Env_Value<unsigned short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Env_Value<unsigned long>
#pragma instantiate ACE_Env_Value<unsigned short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
