// $Id$

#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "client_i.h"

ACE_RCSID(IDL_Cubit, client_i, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *Cubit_Client_Timeprobe_Description[] =
{
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
  CUBIT_CLIENT_CUBE_OCTET_START = 10000,
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
  ACE_Get_Opt get_opts (argc_, argv_, "n:f:k:xs");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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

// void
// Cubit_Client::cube_union (void)
// {
//   Cubit::oneof u;

//   this->call_count_++;

//   u.l (3); // use the long union branch.

//   // Cube a "union" ...
//   Cubit::oneof r;
//   {
//     ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);

//     try
//       {
//         r = this->cubit_->cube_union (u);
//       }
//     catch (...)
//       {
//         cout << "An unknown exception in cube_long\n";
//         this->error_count_++;
//         return;
//       }
//   }
//   u.l (u.l () * u.l () * u.l ()) ;

//   if (u.l () != r.l ())
//     {
//       ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
//       this->error_count_++;
//     }

//   this->call_count_++;
      
//   u._d (Cubit::e_3rd);
      
//   u.cm ().l = 7;
//   u.cm ().s = 5;
//   u.cm ().o = 3;
  
//       // Cube another "union" which uses the default arm ... NOT tested yet
//   {
//     ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);
//     try
//       {
//         r = this->cubit_->cube_union (u);
//       }
//     catch (...)
//       {
//         cout << "An unknown exception in cube_long\n";
//         this->error_count_++;
//         return;
//       }
//   }
//   u.cm ().l = u.cm ().l * u.cm ().l * u.cm ().l;
//   u.cm ().s = u.cm ().s * u.cm ().s * u.cm ().s;
//   u.cm ().o = u.cm ().o * u.cm ().o * u.cm ().o;
      
//   if (u.cm ().l != r.cm ().l
//       || u.cm ().s != r.cm ().s
//       || u.cm ().o != r.cm ().o)
//     {
//       ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
//       this->error_count_++;
//     }
// }

// Cube a short.

void
Cubit_Client::cube_short (int i)
{
  CORBA::Short arg_short = this->func (i);

  this->call_count_++;
  // Cube a short.
  CORBA::Short ret_short;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_SHORT_START);
      try
        {
          ret_short = this->cubit_->cube_short (arg_short);
        }
      catch (...)
        {
          cout << "An unknown exception in cube_short\n";
          this->error_count_++;
          return;
        }
      arg_short = arg_short * arg_short * arg_short;
      if (arg_short != ret_short)
        {
          ACE_ERROR ((LM_ERROR, "** cube_short (%d) ERROR (--> %d)\n",
                      (CORBA::Short) i-117,
                      ret_short));
          this->error_count_++;
        }
  }
}

void
Cubit_Client::cube_oneway (int i)
{
  try
    {
      this->cubit_->cube_oneway ();
    }
  catch (...)
    {
      cout << "An unknown exception in cube_short\n";
      this->error_count_++;
      return;
    }

  this->call_count_++;
  return;
}

void
Cubit_Client::cube_void (int i)
{
  try
    {
      this->cubit_->cube_void ();
    }
  catch (...)
    {
      cout << "An unknown exception in cube_short\n";
      this->error_count_++;
      return;
    }
  this->call_count_++;
}


// Cube an octet

void
Cubit_Client::cube_octet (int i)
{
  CORBA::Octet arg_octet = this->func (i);

  this->call_count_++;
  // Cube an octet.
  CORBA::Octet ret_octet;
  {
    ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_START);
      try
        {
          ret_octet = this->cubit_->cube_octet (arg_octet);
        }
      catch (...)
        {
          cout << "An unknown exception in cube_long\n";
          this->error_count_++;
          return;
        }
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
      try
        {
          ret_long = this->cubit_->cube_long (arg_long);
          this->call_count_++;
        }
      catch (...)
        {
          cout << "An unknown exception in cube_long\n";
          this->error_count_++;
          this->call_count_++;
          return;
        }
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

// // Cube the numbers in a struct

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
    try
      {
        ret_struct = this->cubit_->cube_struct (arg_struct);
      }
    catch (...)
      {
        cout << "An unknown exception in cube_struct\n";
        this->error_count_++;
        return;
      }
  }

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


// Cube the numbers in a sequence

// void
// Cubit_Client::cube_long_sequence (int i, int l)
// {
//   this->call_count_++;

//   Cubit::long_seq input (l);
//   input.length (l);

//   // Just set the first item, otherwise it is hard to compare the
//   // results for longer sequences, i.e. more than just marshalling
//   // gets in the way.
//   input[0] = 4;

//   Cubit::long_seq_var output ;
  
//   // Cube the sequence
//   {
//     ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_LONG_SEQUENCE_START);
//     try
//       {
//         this->cubit_->cube_long_sequence (input,output);
//       }
//     catch (...)
//       {
//         cout << "An unknown exception in cube_long_sequence\n";
//         this->error_count_++;
//         return;
//       }
//   }
//   if (output->length () != input.length ())
//     {
//       ACE_ERROR ((LM_ERROR, "** cube sequence, wrong length\n"));
//       this->error_count_++;
//     }
  
//   u_int rl = output->length ();
//   if (input.length () < rl)
//     rl = input.length ();
//   CORBA::Long x = input[0];
//   if (x * x *x != output[0])
//     {
//       ACE_ERROR ((LM_ERROR, "** cube_long_sequence ERROR\n"));
//       this->error_count_++;
//     }
// }

// void
// Cubit_Client::cube_octet_sequence (int i, int l)
// {
//   this->call_count_++;

//   Cubit::octet_seq input (l);
//   input.length (l);

//   // Just set the first item, otherwise it is hard to compare the
//   // results for longer sequences, i.e. more than just marshalling
//   // gets in the way.
//   input[0] = 4;

//   Cubit::octet_seq* output;

//   // Cube the sequence
//   {
//     ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_START);

//     try
//       {
//         this->cubit_->cube_octet_sequence (input, output);
//       }
//     catch (...)
//       {
//         cout << "An unknown exception in cube_octec_sequence\n";
//         this->error_count_++;
//         return;
//       }
//   }
//   u_int rl = output->length ();
//   if (rl != input.length ())
//     {
//       ACE_ERROR ((LM_ERROR, "** cube octet, wrong length\n"));
//       this->error_count_++;
//     }

//   if (input.length () < rl)
//     rl = input.length ();
//   CORBA::Octet x = input[0];
//   if (x * x *x != output[0])
//     {
//       ACE_ERROR ((LM_ERROR, "** cube_octet ERROR\n"));
//       this->error_count_++;
//     }
// }

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

//   // UNION
//   this->call_count_ = 0;
//   this->error_count_ = 0;
//   timer.start ();
//   for (i = 0; i < this->loop_count_; i++)
//     this->cube_union ();
//   timer.stop ();
//   timer.elapsed_time (elapsed_time);
//   this->print_stats ("cube_union_stub call", elapsed_time);

//   // SMALL LONG SEQUENCES
//   this->call_count_ = 0;
//   this->error_count_ = 0;
//   timer.start ();
//   for (i = 0; i < this->loop_count_; i++)
//     this->cube_long_sequence (this->loop_count_, 4);
//   timer.stop ();
//   timer.elapsed_time (elapsed_time);
//   this->print_stats ("cube_small_sequence<long>", elapsed_time);

//   // LARGE LONG SEQUENCES
//   this->call_count_ = 0;
//   this->error_count_ = 0;
//   timer.start ();
//   for (i = 0; i < this->loop_count_; i++)
//     this->cube_long_sequence (this->loop_count_, 1024);
//   timer.stop ();
//   timer.elapsed_time (elapsed_time);
//   this->print_stats ("cube_large_sequence<long>", elapsed_time);

//   // SMALL OCTET SEQUENCES
//   this->call_count_ = 0;
//   this->error_count_ = 0;
//   timer.start ();
//   for (i = 0; i < this->loop_count_; i++)
//     this->cube_octet_sequence (this->loop_count_, 16);
//   timer.stop ();
//   timer.elapsed_time (elapsed_time);
//   this->print_stats ("cube_small_sequence<octet>", elapsed_time);

//   // LARGE OCTET SEQUENCES
//   this->call_count_ = 0;
//   this->error_count_ = 0;
//   timer.start ();
//   for (i = 0; i < this->loop_count_; i++)
//     this->cube_octet_sequence (this->loop_count_, 4096);
//   timer.stop ();
//   timer.elapsed_time (elapsed_time);
//   this->print_stats ("cube_large_sequence<octet>", elapsed_time);


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
  CORBA::Object_ptr		dummy_object;
  CosNaming::NamingContext_var  nameserver;
  CosNaming::Name		name;

  try {
    // Resolve initial reference to the nameserver
    dummy_object = this->orb_-> resolve_initial_references("NameService");
    // Narrow the initial reference to the nameserver
    nameserver = CosNaming::NamingContext::_narrow(dummy_object);
  } catch (CORBA::BAD_PARAM) {
    cout << "Bad parameter exception caught - narrow on the NameService" << endl;
    exit(-1);
  } catch (CORBA::SystemException) {
    cout << "A general system exception was caught - resolve_initial_references" << endl;
    exit(-1);
  } catch (...) {
    cout << "An exception of unknown type was caught - initializing the NameService" << endl;
    exit(-1);    
  }

  // Resolve the server interface object which was bound under the name "cubit"
  name.length(2);
  
  name[0].id          = "cubit_S";
  name[0].kind        = "ASE";
  name[1].id          = "cubit";
  name[1].kind        = "interface";
  
  try {
    dummy_object = nameserver -> resolve(name);
    this->cubit_ = Cubit::_narrow(dummy_object);
  } catch (CORBA::BAD_PARAM) {
    cout << "Bad parameter exception caught - resolve or narrow on the cubit server object" << endl;
    exit(-1);
  } catch (...) {
    cout << "An exception of unknown type was caught - resolve or narrow on the cubit server object" << endl;
    exit(-1);
  }

  cout << "cubit object resolved successfully\n";

  return 0;
}

int
Cubit_Client::init (int argc, char **argv)
{
  int  argct;
  char *argvt[1];
  int naming_result;
  this->argc_ = argc;
  this->argv_ = argv;
  cout << "Starting the cubit client." << endl;

  try {
    // Obtain an object reference to the ORB
    argvt[0] = "-ORBase cubit_client";
    argct = 1;
    this->orb_ = CORBA::ORB_init(argct, argvt, ORBIDENT);
  } catch (CORBA::BAD_PARAM) {
    cout << "Bad parameter exception caught - ORB_init" << endl;
    exit(-1);
  } catch (CORBA::INITIALIZE) {
    cout << "Initialize exception caught - ORB_init" << endl;
    exit(-1);
  } catch (CORBA::INTERNAL) {
    cout << "Internal exception caught - ORB_init" << endl;
    exit(-1);
  } catch (CORBA::UNKNOWN) {
    cout << "Unknown exception caught - ORB_init" << endl;
    exit(-1);
  } catch(...) {
    cout << "An exception of unknown type was caught - ORB_init" << endl;
    exit(-1);
  }

  try {
    // Obtain an object reference to the BOA
    argct = 0;
    this->boa_ = this->orb_-> BOA_init(argct, argvt, BOAIDENT);
  } catch (CORBA::UNKNOWN) {
    cout << "Unknown exception caught - ORB_init" << endl;
    exit(-1);
  } catch (CORBA::SystemException) {
    cout << "General system exception caught - BOA_init" << endl;
    exit(-1);
  } catch (...) {
    cout << "An exception of unknown type was caught - BOA_init" << endl;
    exit(-1);
  } 
  naming_result = this->init_naming_service ();
  if (naming_result < 0)
    return naming_result;
  
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Env_Value<unsigned long>;
template class ACE_Env_Value<unsigned short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Env_Value<unsigned long>
#pragma instantiate ACE_Env_Value<unsigned short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
