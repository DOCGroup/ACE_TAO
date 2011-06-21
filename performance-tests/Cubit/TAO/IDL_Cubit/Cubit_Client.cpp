// $Id$

#include "Cubit_Client.h"
#include "RTI_IO.h"

#include "tao/Timeprobe.h"
#include "tao/TAO_Internal.h"
#include "tao/debug.h"

#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

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
  "Cubit_Client::cube_octet_sequence - end",

  "Cubit_Client::cube_many_sequence - start",
  "Cubit_Client::cube_many_sequence - end",

  "Cubit_Client::cube_rti_data - start",
  "Cubit_Client::cube_rti_data - end",

  "Cubit_Client::cube_any - start",
  "Cubit_Client::cube_any - end",

  "Cubit_Client::cube_any_struct - start",
  "Cubit_Client::cube_any_struct - end"
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
  CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_END,

  CUBIT_CLIENT_CUBE_MANY_SEQUENCE_START,
  CUBIT_CLIENT_CUBE_MANY_SEQUENCE_END,

  CUBIT_CLIENT_CUBE_RTI_DATA_START,
  CUBIT_CLIENT_CUBE_RTI_DATA_END,

  CUBIT_CLIENT_CUBE_ANY_START,
  CUBIT_CLIENT_CUBE_ANY_END,

  CUBIT_CLIENT_CUBE_ANY_STRUCT_START,
  CUBIT_CLIENT_CUBE_ANY_STRUCT_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Cubit_Client_Timeprobe_Description,
                                  CUBIT_CLIENT_CUBE_ONEWAY_START);

#endif /* ACE_ENABLE_TIMEPROBES */


// Bitmasks for each of the Cubit tests
#define TAO_ENABLE_CUBIT_VOID              1 << 0
#define TAO_ENABLE_CUBIT_OCTET             1 << 1
#define TAO_ENABLE_CUBIT_SHORT             1 << 2
#define TAO_ENABLE_CUBIT_LONG              1 << 3
#define TAO_ENABLE_CUBIT_STRUCT            1 << 4
#define TAO_ENABLE_CUBIT_UNION             1 << 5
#define TAO_ENABLE_CUBIT_SMALL_OCTET_SEQ   1 << 6
#define TAO_ENABLE_CUBIT_LARGE_OCTET_SEQ   1 << 7
#define TAO_ENABLE_CUBIT_SMALL_LONG_SEQ    1 << 8
#define TAO_ENABLE_CUBIT_LARGE_LONG_SEQ    1 << 9
#define TAO_ENABLE_CUBIT_SMALL_STRUCT_SEQ  1 << 10
#define TAO_ENABLE_CUBIT_LARGE_STRUCT_SEQ  1 << 11
#define TAO_ENABLE_CUBIT_MIXIN             1 << 12
#define TAO_ENABLE_CUBIT_RTI_DATA          1 << 13
#define TAO_ENABLE_CUBIT_ONE_WAY           1 << 14
#define TAO_ENABLE_CUBIT_ANY               1 << 15
#define TAO_ENABLE_CUBIT_ANY_STRUCT        1 << 16
// Bitmask to enable all tests
#define TAO_ENABLE_CUBIT_ALL \
        TAO_ENABLE_CUBIT_VOID | \
        TAO_ENABLE_CUBIT_OCTET | \
        TAO_ENABLE_CUBIT_SHORT | \
        TAO_ENABLE_CUBIT_LONG | \
        TAO_ENABLE_CUBIT_STRUCT | \
        TAO_ENABLE_CUBIT_UNION | \
        TAO_ENABLE_CUBIT_SMALL_OCTET_SEQ | \
        TAO_ENABLE_CUBIT_LARGE_OCTET_SEQ | \
        TAO_ENABLE_CUBIT_SMALL_LONG_SEQ | \
        TAO_ENABLE_CUBIT_LARGE_LONG_SEQ | \
        TAO_ENABLE_CUBIT_SMALL_STRUCT_SEQ | \
        TAO_ENABLE_CUBIT_LARGE_STRUCT_SEQ | \
        TAO_ENABLE_CUBIT_MIXIN | \
        TAO_ENABLE_CUBIT_RTI_DATA |  \
        TAO_ENABLE_CUBIT_ONE_WAY  |  \
        TAO_ENABLE_CUBIT_ANY      |  \
        TAO_ENABLE_CUBIT_ANY_STRUCT

// Constructor.
Cubit_Client::Cubit_Client (int shutdown)
  : cubit_factory_key_ (0),
    loop_count_ (250),
    shutdown_ (shutdown),
    quiet_ (0),
    cubit_ (Cubit::_nil ()),
    call_count_ (0),
    error_count_ (0),
    cubit_factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    bytes_in_octet_sequence_ (0),
    test_enable_bitset_ (0),
    test_disable_bitset_ (0)
{
}

// Simple function that returns the subtraction of 117 from the
// parameter.

int
Cubit_Client::func (u_int i)
{
  return i - 117;
}

// Reads the Cubit factory ior from a file

int
Cubit_Client::read_ior (const ACE_TCHAR *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to open <%s> for reading: %p\n",
                         filename),
                        -1);
    }

  ACE_Read_Buffer ior_buffer (this->f_handle_);

  char *data = ior_buffer.read ();

  if (data == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to allocate memory to read ior: %p\n"),
                        -1);
    }

  this->cubit_factory_key_ = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR(data));

  ior_buffer.alloc ()->free (data);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Cubit_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("b:t:z:ovdn:f:k:xsq"));
  int c = 0;
  int result = 0;
  u_int test_mask = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        // bytes in octet sequence
        result = ACE_OS::atoi (get_opts.opt_arg ());

        if (result <= 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Invalid number of bytes entered: <%s>\n",
                             get_opts.opt_arg ()),
                            -1);
        else
          this->bytes_in_octet_sequence_ = (u_int) result;
        break;
      case 't':
        test_mask = this->opt_to_mask (get_opts.opt_arg ());
        if (test_mask == 0)
          return -1;
        this->enable_test (test_mask);
        break;
      case 'z':
        test_mask = this->opt_to_mask (get_opts.opt_arg ());
        if (test_mask == 0)
          return -1;
        this->disable_test (test_mask);
        break;
      case 'v':
        // For backward compatibility
        test_mask = this->opt_to_mask (ACE_TEXT("void"));
        if (test_mask == 0)
          return -1;
        this->enable_test (test_mask);
        break;
      case 'o':
        // For backward compatibility
        test_mask = this->opt_to_mask (ACE_TEXT("one_way"));
        if (test_mask == 0)
          return -1;
        this->enable_test (test_mask);
        break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':                 // loop count
        result = ACE_OS::atoi (get_opts.opt_arg ());

        if (result <= 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Invalid number of iterations entered: <%s>\n",
                             get_opts.opt_arg ()),
                            -1);
        else
          this->loop_count_ = (u_int) result;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from <%s> : %p\n",
                             get_opts.opt_arg (),
                             ""),
                            -1);
        break;
      case 'k': // read the cubit IOR from the command-line.
        this->cubit_factory_key_ =
          ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'x':
        ACE_DEBUG ((LM_DEBUG, "We will shutdown the server\n"));
        this->shutdown_ = 1;
        break;
      case 'q':
        this->quiet_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-b bytes-in-sequence]"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f cubit_factory-obj-ref-key-file]"
                           " [-k cubit-obj-ref-key]"
                           " [-x]"
                           " [-s]"
                           " [-t test-to-enable]"
                           " [-z test-to-disable]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

u_int
Cubit_Client::opt_to_mask (const ACE_TCHAR *test_type)
{
  if (ACE_OS::strcasecmp (test_type, ACE_TEXT("void")) == 0)
    return TAO_ENABLE_CUBIT_VOID;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("octet")) == 0)
    return TAO_ENABLE_CUBIT_OCTET;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("short")) == 0)
    return TAO_ENABLE_CUBIT_SHORT;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("long")) == 0)
    return TAO_ENABLE_CUBIT_LONG;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("struct")) == 0)
    return TAO_ENABLE_CUBIT_STRUCT;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("union")) == 0)
    return TAO_ENABLE_CUBIT_UNION;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("small_octet_seq")) == 0)
    return TAO_ENABLE_CUBIT_SMALL_OCTET_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("small_long_seq")) == 0)
    return TAO_ENABLE_CUBIT_SMALL_LONG_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("small_struct_seq")) == 0)
    return TAO_ENABLE_CUBIT_SMALL_STRUCT_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("large_octet_seq")) == 0)
    return TAO_ENABLE_CUBIT_LARGE_OCTET_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("large_long_seq")) == 0)
    return TAO_ENABLE_CUBIT_LARGE_LONG_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("large_struct_seq")) == 0)
    return TAO_ENABLE_CUBIT_LARGE_STRUCT_SEQ;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("mixin")) == 0)
    return TAO_ENABLE_CUBIT_MIXIN;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("rti_data")) == 0)
    return TAO_ENABLE_CUBIT_RTI_DATA;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("one_way")) == 0)
    return TAO_ENABLE_CUBIT_ONE_WAY;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("any")) == 0)
    return TAO_ENABLE_CUBIT_ANY;
  else if (ACE_OS::strcasecmp (test_type, ACE_TEXT("any_struct")) == 0)
    return TAO_ENABLE_CUBIT_ANY_STRUCT;

  ACE_ERROR_RETURN ((LM_ERROR,
                     "Incorrect test: %s\n"
                     "Valid tests are:\n"
                     "  void, octet, short, long, struct, union,\n"
                     "  small_octet_seq, small_long_seq, small_struct_seq\n"
                     "  large_octet_seq, large_long_seq, large_struct_seq\n"
                     "  mixin, rti_data, one_way, any, any_struct\n",
                     test_type),
                    0);  // Zero since u_int return type
}

void
Cubit_Client::enable_test (u_int mask)
{
  ACE_SET_BITS (this->test_enable_bitset_, mask);
}

void
Cubit_Client::disable_test (u_int mask)
{
  ACE_SET_BITS (this->test_disable_bitset_, mask);
}


int
Cubit_Client::check_enabled (u_int mask) const
{
  return ACE_BIT_ENABLED (this->test_enable_bitset_, mask);
}

// Exercise the union.  Cube a union.

void
Cubit_Client::cube_union (void)
{
  try
    {
      Cubit::oneof u;

      this->call_count_++;

      u.l (3); // use the long union branch.

      // Cube a "union" ...
      Cubit::oneof r;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);

        r = this->cubit_->cube_union (u);
      }


      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG, "cube union ..."));
      u.l (u.l () * u.l () * u.l ()) ;

      if (u.l () != r.l ())
        {
          ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
          this->error_count_++;
        }

      this->call_count_++;

      u._d (Cubit::e_3rd);

      u.cm ().l = 7;
      u.cm ().s = 5;
      u.cm ().o = 3;

      // Cube another "union" which uses the default arm ... NOT tested yet
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_UNION_START);

        r = this->cubit_->cube_union (u);
      }


      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG, "cube union ..."));
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_union");
      this->error_count_++;
    }
}

// Cube a short.

void
Cubit_Client::cube_short (int i)
{
  try
    {
      CORBA::Short arg_short = this->func (i);

      // Cube a short.
      CORBA::Short ret_short;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_SHORT_START);

        ret_short = cubit_->cube_short (arg_short);
      }


      this->call_count_++;

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube short:  %d --> %d\n",
                      arg_short,
                      ret_short));
        }
      arg_short = arg_short * arg_short * arg_short;

      if (arg_short != ret_short)
        {
          ACE_ERROR ((LM_ERROR, "** cube_short (%d) ERROR (--> %d)\n",
                      (CORBA::Short) this->func (i),
                      ret_short));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_short");
      this->error_count_++;
    }
}

// Oneway test.

void
Cubit_Client::cube_oneway (int)
{
  try
    {
      ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_ONEWAY_START);
      this->cubit_->cube_oneway ();
      this->call_count_++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_oneway");
      this->error_count_++;
    }
}

void
Cubit_Client::cube_void (int)
{
  try
    {
      ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_VOID_START);
      this->cubit_->cube_void ();
      this->call_count_++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_void");
      this->error_count_++;
    }
}

// Cube an octet

void
Cubit_Client::cube_octet (int i)
{
  try
    {
      CORBA::Octet arg_octet = this->func (i);

      // Cube an octet.
      CORBA::Octet ret_octet;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_START);

        ret_octet = this->cubit_->cube_octet (arg_octet);
      }


      this->call_count_++;

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube octet:  %d --> %d\n",
                      arg_octet, ret_octet));
        }

      arg_octet = arg_octet * arg_octet * arg_octet;

      if (arg_octet != ret_octet)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "** cube_octet (%d) ERROR (--> %d)\n",
                      (CORBA::Octet) this->func (i),
                      ret_octet));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_octet");
      this->error_count_++;
    }
}

// calculate the cube from a long

void
Cubit_Client::cube_long (int i)
{
  try
    {
      CORBA::Long arg_long = this->func (i);

      // Cube a long.
      CORBA::Long ret_long;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_LONG_START);

        ret_long = this->cubit_->cube_long (arg_long);
      }


      this->call_count_++;

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube long:  %d --> %d\n",
                      arg_long,
                      ret_long));
        }

      arg_long = arg_long * arg_long * arg_long;

      if (arg_long != ret_long)
        {
          // CORBA::Long is 32 bits, which can be handled by %d on
          // most platforms.
          ACE_ERROR ((LM_ERROR,
                      "** cube_long (%d) ERROR (--> %d)\n",
                      (CORBA::Long) this->func (i),
                      ret_long));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_long");
      this->error_count_++;
    }
}

// Cube the numbers in a struct

void
Cubit_Client::cube_struct (int i)
{
  try
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

        ret_struct = this->cubit_->cube_struct (arg_struct);
      }


      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube struct ..."));
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_struct");
      this->error_count_++;
    }
}

// Cube the numbers in a sequence

void
Cubit_Client::cube_long_sequence (int,
                                  int l)
{
  try
    {
      this->call_count_++;

      Cubit::long_seq input (l);
      input.length (l);

      // Just set the first item, otherwise it is hard to compare the
      // results for longer sequences, i.e. more than just marshalling
      // gets in the way.
      input[0] = 4;

#if defined (ACE_HAS_MEMORY_PROFILER)
      for (int i = 1; i < l; i++)
        input[i]=11;
#endif /* ACE_HAS_MEMORY_PROFILER */

      Cubit::long_seq_var output;
      Cubit::long_seq_out vout (output);

      // Cube the sequence
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_LONG_SEQUENCE_START);

        this->cubit_->cube_long_sequence (input,
                                          vout);
      }


      if (output->length () != input.length ())
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube sequence, wrong length\n"));

          this->error_count_++;

          return;
        }

      u_int rl = output->length ();

      if (input.length () < rl)
        {
          rl = input.length ();
        }

      CORBA::Long x = input[0];

      if (x * x *x != output[0])
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube_long_sequence ERROR\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_long_sequence");
      this->error_count_++;
    }
}

void
Cubit_Client::cube_octet_sequence (int,
                                   int l)
{
  try
    {
      this->call_count_++;

      ACE_Message_Block mb (l);
      mb.wr_ptr (l);
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      Cubit::octet_seq input (l, &mb);
#else
      // If the form of the constructor is not available, we will need
      // to do the copy manually.  First, set the octet sequence length.
      Cubit::octet_seq input;
      CORBA::ULong length = mb.length ();
      input.length (length);

      // Now copy over each byte.
      char* base = mb.data_block ()->base ();
      for(CORBA::ULong i = 0; i < length; i++)
        {
          input[i] = base[i];
        }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */


      // Just set the first item, otherwise it is hard to compare the
      // results for longer sequences, i.e. more than just marshalling
      // gets in the way.
      input[0] = 4;

#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
      for (int i = 1 ; i < l; i++)
        input[i]=10;
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

      Cubit::octet_seq_var output;
      Cubit::octet_seq_out vout (output);

      // Cube the sequence
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_OCTET_SEQUENCE_START);

        this->cubit_->cube_octet_sequence (input,
                                           vout);
      }


      if (output->length () != input.length ())
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube octet, wrong length\n"));

          this->error_count_++;

          return;
        }

      u_int rl = output->length ();

      if (input.length () < rl)
        {
          rl = input.length ();
        }
      CORBA::Octet x = input[0];

      if (x * x *x != output[0])
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube_octet ERROR\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_octet_sequence");
      this->error_count_++;
    }
}

// Cube the many in a sequence

void
Cubit_Client::cube_many_sequence (int,
                                  int l)
{
  try
    {
      this->call_count_++;

      Cubit::many_seq input (l);
      input.length (l);

      // Just set the first item, otherwise it is hard to compare the
      // results for longer sequences, i.e. more than just marshalling
      // gets in the way.
      Cubit::Many &in = input[0];
      in.l = 4;
      in.s = 5;
      in.o = 6;

#if defined (ACE_HAS_PURIFY) && (ACE_HAS_PURIFY == 1)
      for(int i=1; i < l; i++)
        {
          Cubit::Many &inb = input[i];
          inb.l=7;
          inb.s=8;
          inb.o=9;
        }
#endif /* ACE_HAS_PURIFY == 1 */

      Cubit::many_seq_var output;
      Cubit::many_seq_out vout (output);

      // Cube the sequence
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_MANY_SEQUENCE_START);

        this->cubit_->cube_many_sequence (input,
                                          vout);
      }


      if (output->length () != input.length ())
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube sequence, wrong length\n"));

          this->error_count_++;

          return;
        }

      u_int rl = output->length ();

      if (input.length () < rl)
        {
          rl = input.length ();
        }

      Cubit::Many &ret_in  = input[0];
      Cubit::Many &out = output[0];

      if (ret_in.l * ret_in.l * ret_in.l != out.l ||
          ret_in.s * ret_in.s * ret_in.s != out.s ||
          ret_in.o * ret_in.o * ret_in.o != out.o)
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube_long_sequence ERROR\n"));

          this->error_count_++;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_many_sequence");
      this->error_count_++;
    }
}

void
Cubit_Client::cube_rti_data (int,
                             int numUpdates,
                             int numAttrs)
{
  try
    {
      this->call_count_++;

      Cubit::RtiPacket input;
      input.packetHeader.packetLength = 1; // this is probably redundant
      input.packetHeader.federationHandle = 2;
      input.packetHeader.channelHandle = 3;
      input.packetHeader.packetColor = 4;

      input.msgs.length (numUpdates);

      for (int j = 0; j < numUpdates; ++j)
        {
#if defined (ACE_HAS_PURIFY) && (ACE_HAS_PURIFY == 1)
          Cubit::RtiObjectUpdateMessageHeader o;
          ACE_OS::memset (&o, 0, sizeof(Cubit::RtiObjectUpdateMessageHeader));
          input.msgs[j].oumh (o);
#else
          input.msgs[j].oumh (Cubit::RtiObjectUpdateMessageHeader ());
#endif /* ACE_HAS_PURIFY == 1 */
          Cubit::RtiObjectUpdateMessageHeader & oumh = input.msgs[j].oumh ();
          oumh.updateLength = 2001; // redundant
          oumh.updateTag = 2002;
          oumh.objectHandle = 2003;
          oumh.timestamp = 3.14159;
          oumh.eventRetractionHandle = 2004;
          oumh.classHandle = 2005;
          oumh.sendingFederateHandle = 2006;
          oumh.userTag = CORBA::string_dup ("beefcake!");
          oumh.regionData.length(0);
          oumh.transportationHandle = 1;
          oumh.orderingHandle = 1;
          oumh.messagePayload.length (numAttrs);

          for (int k = 0; k < numAttrs; ++k)
            {
#if defined (ACE_HAS_PURIFY) && (ACE_HAS_PURIFY == 1)
              Cubit::HandleValuePair h;
              ACE_OS::memset (&h, 0, sizeof(Cubit::HandleValuePair));
              oumh.messagePayload[k] = h;
#else
              oumh.messagePayload[k] = Cubit::HandleValuePair ();
#endif /* ACE_HAS_PURIFY == 1 */
              Cubit::HandleValuePair &hvp = oumh.messagePayload[k];
              hvp.handle = k * k;
              const char *d1 = "somedata";
              hvp.data.length (ACE_OS::strlen (d1)+1);
              ACE_OS::strcpy ((char *) hvp.data.get_buffer (), d1);
            }
        }

      Cubit::RtiPacket_var output;
      Cubit::RtiPacket_out vout (output);

      // Cube the sequence
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Input:\n"));

          print_RtiPacket (input);
        }

      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_RTI_DATA_START);

        this->cubit_->cube_rti_data (input,
                                     vout);
      }


      if (TAO_debug_level > 0)

        {
          ACE_DEBUG ((LM_DEBUG,
                      "Output:\n"));

          print_RtiPacket (*vout.ptr ());

          ACE_DEBUG ((LM_DEBUG,
                      "need to check whether cubing happened\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_rtti_data");
      this->error_count_++;
    }
}


// calculate the cube from long contained in an any

void
Cubit_Client::cube_any (int i)
{
  try
    {
      CORBA::Long arg_long = this->func (i);

      // Cube a long inside an any.
      CORBA::Long ret_long;
      CORBA::Any  arg_any;
      CORBA::Any * ret_any;
      // Should the timing include the packing and unpacking of the any? NO.
      arg_any <<= arg_long;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_ANY_START);
        ret_any = this->cubit_->cube_any (arg_any);
      }

      *ret_any >>= ret_long;

      this->call_count_++;

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube any (long):  %d --> %d\n",
                      arg_long,
                      ret_long));
        }

      arg_long = arg_long * arg_long * arg_long;

      if (arg_long != ret_long)
        {
          ACE_ERROR ((LM_ERROR,
                      "** cube_any(%d)  ERROR (got %d, expect %d)\n",
                      (CORBA::Long) this->func (i),
                      ret_long, arg_long));

          this->error_count_++;
        }

      delete ret_any;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_any");
      this->error_count_++;
    }
}


// Cube the numbers in a struct

void
Cubit_Client::cube_any_struct (int i)
{
  try
    {
      Cubit::Many arg_struct;
      Cubit::Many * ret_struct;

      this->call_count_++;

      arg_struct.l = this->func (i);
      arg_struct.s = this->func (i);
      arg_struct.o = this->func (i);

      // Cube a "struct" in an any ...
      CORBA::Any arg_any;
      CORBA::Any * ret_any;
      arg_any <<= arg_struct;
      {
        ACE_FUNCTION_TIMEPROBE (CUBIT_CLIENT_CUBE_ANY_STRUCT_START);
        ret_any = this->cubit_->cube_any_struct (arg_any);
      }

      *ret_any >>= ret_struct;

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "cube any struct ..."));
        }

      arg_struct.l = arg_struct.l * arg_struct.l * arg_struct.l;
      arg_struct.s = arg_struct.s * arg_struct.s * arg_struct.s;
      arg_struct.o = arg_struct.o * arg_struct.o * arg_struct.o;

      if (arg_struct.l != ret_struct->l
          || arg_struct.s != ret_struct->s
          || arg_struct.o != ret_struct->o)
        {
          ACE_ERROR ((LM_ERROR, "** cube_any_struct ERROR -- %d should be %d\n",
                      ret_struct->l, arg_struct.l));

          this->error_count_++;
        }

      delete ret_any;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from cube_any_struct");
      this->error_count_++;
    }
}


void
Cubit_Client::print_stats (const char *call_name,
                           ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time)
{
  ACE_DEBUG ((LM_DEBUG,
              "%C:\n",
              call_name));

  if (this->quiet_)
    {
      return;
    }

  if (!this->quiet_)
    {
      if (this->call_count_ > 0 && this->error_count_ == 0)
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
        {
          ACE_ERROR ((LM_ERROR,
                      "\tNo time stats printed."
                      "  Call count zero or error ocurred.\n"));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "\t%d calls, %d errors\n",
              this->call_count_,
              this->error_count_));
}

// Execute client example code.

int
Cubit_Client::run ()
{
  u_int i;

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
  //  ACE_Time_Value before;

  // Show the results one type at a time.

  try
    {
      // VOID
      if (this->check_enabled (TAO_ENABLE_CUBIT_VOID))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_void (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_void",
                             elapsed_time);
        }

      // SHORT
      if (this->check_enabled (TAO_ENABLE_CUBIT_SHORT))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_short (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_short",
                             elapsed_time);
        }

      // OCTET
      if (this->check_enabled (TAO_ENABLE_CUBIT_OCTET))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_octet (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_octet",
                             elapsed_time);
        }

      // LONG
      if (this->check_enabled (TAO_ENABLE_CUBIT_LONG))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_long (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);
          this->print_stats ("cube_long",
                             elapsed_time);
        }

      // STRUCT
      if (this->check_enabled (TAO_ENABLE_CUBIT_STRUCT))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_struct (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_struct",
                             elapsed_time);
        }

      // UNION
      if (this->check_enabled (TAO_ENABLE_CUBIT_UNION))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_union ();
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_union_stub call",
                             elapsed_time);
        }

      // SMALL LONG SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_SMALL_LONG_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_long_sequence (this->loop_count_,
                                        4);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_small_sequence<long>",
                             elapsed_time);
        }

      // LARGE LONG SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_LARGE_LONG_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_long_sequence (this->loop_count_,
                                        1024);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_large_sequence<long>",
                             elapsed_time);
        }

      // SMALL OCTET SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_SMALL_OCTET_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          if (bytes_in_octet_sequence_ > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Running sequence<octet> test with %u bytes "
                          "in the sequence.\n", bytes_in_octet_sequence_));
              for (i = 0; i < this->loop_count_; ++i)
                {
                  this->cube_octet_sequence (this->loop_count_,
                                             bytes_in_octet_sequence_);
                }
            }
          else
            {
              for (i = 0; i < this->loop_count_; ++i)
                {
                  this->cube_octet_sequence (this->loop_count_,
                                             16);
                }
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_small_sequence<octet>",
                             elapsed_time);
        }

      // LARGE OCTET SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_LARGE_OCTET_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_octet_sequence (this->loop_count_,
                                         4096);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_large_sequence<octet>",
                             elapsed_time);
        }

      // SMALL MANY SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_SMALL_STRUCT_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_many_sequence (this->loop_count_,
                                        4);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_small_sequence<many>",
                             elapsed_time);
        }

      // LARGE MANY SEQUENCES
      if (this->check_enabled (TAO_ENABLE_CUBIT_LARGE_STRUCT_SEQ))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_many_sequence (this->loop_count_,
                                        1024);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_large_sequence<many>",
                             elapsed_time);
        }

      // MIXIN
      if (this->check_enabled (TAO_ENABLE_CUBIT_MIXIN))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_short (i);

              this->cube_octet (i);

              this->cube_long (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);
          this->print_stats ("cube mixin (short/octet/long)",
                             elapsed_time);
        }

      // RTI
      if (this->check_enabled (TAO_ENABLE_CUBIT_RTI_DATA))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_rti_data (this->loop_count_,
                                   2,
                                   5);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_rti_data",
                             elapsed_time);
        }

      // ONEWAY
      if (this->check_enabled (TAO_ENABLE_CUBIT_ONE_WAY))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_oneway (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_oneway",
                             elapsed_time);
        }

      // ANY - LONG
      if (this->check_enabled (TAO_ENABLE_CUBIT_ANY))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_any (i);
            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);
          this->print_stats ("cube_any",
                             elapsed_time);
        }

      // ANY - STRUCT
      if (this->check_enabled (TAO_ENABLE_CUBIT_ANY_STRUCT))
        {
          this->call_count_ = 0;
          this->error_count_ = 0;
          timer.start ();

          for (i = 0; i < this->loop_count_; ++i)
            {
              this->cube_any_struct (i);

            }

          timer.stop ();
          timer.elapsed_time (elapsed_time);

          this->print_stats ("cube_any_struct",
                             elapsed_time);
        }

      this->shutdown_server (this->shutdown_);

      try
        {
          this->cubit_->ping ();
        }
      catch (const CORBA::Exception&)
        {
          // Dont do anything..
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Cubit_Client::run");
    }

  return this->error_count_ == 0 ? 0 : 1;
}

int
Cubit_Client::shutdown_server (int do_shutdown)
{
  if (do_shutdown)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "shutdown on cubit object\n"));

      try
        {
          this->cubit_->shutdown ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Cubit::shutdown_server");
        }
    }

  return 0;
}

Cubit_Client::~Cubit_Client (void)
{
  // Free resources and close the IOR files.
  if (this->cubit_factory_ior_file_)
    {
      ACE_OS::fclose (this->cubit_factory_ior_file_);
    }

  if (this->f_handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::close (this->f_handle_);
    }

  if (this->cubit_factory_key_ != 0)
    {
      ACE_OS::free (this->cubit_factory_key_);
    }

  // Release the objects, to have a clean shutdown.
  this->factory_ = Cubit_Factory::_nil ();
  this->cubit_ = Cubit::_nil ();

  // Now release the ORB
  this->orb_ = CORBA::ORB::_nil ();
}


int
Cubit_Client::init (int argc, ACE_TCHAR **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        {
          return -1;
        }
      else
        {
          // Enable all tests if none were specified.
          if (this->test_enable_bitset_ == 0)
            this->enable_test (TAO_ENABLE_CUBIT_ALL);

          // Disable tests that were explicitly disabled.
          if (this->test_disable_bitset_ != 0)
            this->test_enable_bitset_ &= ~this->test_disable_bitset_;
        }


      if (this->cubit_factory_key_ == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: no cubit factory key specified\n",
                             this->argv_[0]),
                            -1);
        }

      CORBA::Object_var factory_object =
        this->orb_->string_to_object (this->cubit_factory_key_);

      this->factory_ =
        Cubit_Factory::_narrow (factory_object.in());

      if (CORBA::is_nil (this->factory_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "invalid factory key <%s>\n",
                             this->cubit_factory_key_),
                            -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Factory received OK\n"));

      // Now retrieve the Cubit obj ref corresponding to the key.
      this->cubit_ =
        this->factory_->make_cubit ();

      if (CORBA::is_nil (this->cubit_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "null cubit objref returned by factory\n"),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Cubit::init");
      return -1;
    }

  return 0;
}
