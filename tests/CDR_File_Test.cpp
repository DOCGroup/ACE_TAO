// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    CDR_File_Test.cpp
//
// = DESCRIPTION
//    Checks the functionality of the ACE CDR streams used for file
//    I/O.
//
// = AUTHORS
//    Giga Giguashvili <gregoryg@ParadigmGeo.com> and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/CDR_Stream.h"
#include "ace/FILE_Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/Truncate.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"



#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

class CDR_Test
{
  // = TITLE
  //   Simple class that's used to read and write CDR streams.

  friend ostream& operator << (ostream &os, const CDR_Test &t);
  // Output the state of a <CDR_Test> object to the <ostream>.

  friend void operator << (ACE_OutputCDR &os, const CDR_Test &t);
  // Convert the state of this object into an <ACE_OutputCDR>.

  friend void operator >> (ACE_InputCDR &is, CDR_Test &);
  // Convert the <ACE_InputCDR> into the state of this object.

public:
  CDR_Test (void);
  // Default constructor.

  CDR_Test (ACE_CDR::Char o,
            ACE_CDR::Short s,
            ACE_CDR::Long w,
            ACE_CDR::ULongLong lw,
            ACE_CDR::Float f,
            ACE_CDR::Double d);
  // Constructor.

  bool operator == (const CDR_Test &rhs) const;
  // Compare <rhs> for equality with <this>.

private:
  ACE_CDR::Char char_;
  ACE_CDR::Short word2_;
  ACE_CDR::Long word4_;
  ACE_CDR::ULongLong word8_;
  ACE_CDR::Float fpoint_;
  ACE_CDR::Double dprec_;
};

ostream &
operator << (ostream &os,
             const CDR_Test &t)
{
#if defined (ACE_OPENVMS)
  // to circumvent some obscure bug with OpenVMS iostreams digit conversions
  // combined with shared libraries????
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n"
                        "Char:               %c\n"
                        "Short:              %u\n"
                        "Long:               %d\n"),
              t.char_,
              t.word2_,
              t.word4_));

  ACE_CDR::ULongLong hi = (t.word8_ >> 32);
  ACE_CDR::ULongLong lo = (t.word8_ & 0xffffffff);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n"
                        "ULongLong 1st half: %x\n"
                        "ULongLong 2nd half: %x\n"
                        "Float:              %f\n"
                        "Double:             %f\n"),
              ACE_Utils::truncate_cast<ACE_UINT32> (hi),
              ACE_Utils::truncate_cast<ACE_UINT32> (lo),
              t.fpoint_,
              t.dprec_));
#else
  os << "Char:              " << t.char_ << endl
     << "Short:             " << t.word2_ << endl
     << "Long:              " << t.word4_ << endl;

  ACE_CDR::ULongLong hi = (t.word8_ >> 32);
  ACE_CDR::ULongLong lo = (t.word8_ & 0xffffffff);

  os << "ULongLong 1st half: "
     << hex
     << ACE_Utils::truncate_cast<ACE_UINT32> (hi)
     << dec << endl
     << "ULongLong 2nd half: "
     << hex
     << ACE_Utils::truncate_cast<ACE_UINT32> (lo)
     << dec << endl
     << "Float:             " << t.fpoint_ << endl
     << "Double:            " << t.dprec_ << endl;
#endif
  return os;
}

CDR_Test::CDR_Test (void)
  : char_ (0),
    word2_ (0),
    word4_ (0),
    word8_ (0),
    fpoint_ (0.0),
    dprec_ (0.0)
{
}

CDR_Test::CDR_Test (ACE_CDR::Char o,
                    ACE_CDR::Short s,
                    ACE_CDR::Long w,
                    ACE_CDR::ULongLong lw,
                    ACE_CDR::Float f,
                    ACE_CDR::Double d)
  : char_ (o),
    word2_ (s),
    word4_ (w),
    word8_ (lw),
    fpoint_ (f),
    dprec_ (d)
{
}

void
operator << (ACE_OutputCDR &os, const CDR_Test &t)
{
  os << t.char_;
  os << t.word2_;
  os << t.word4_;
  os << t.word8_;
  os << t.fpoint_;
  os << t.dprec_;
}

void
operator >> (ACE_InputCDR &is, CDR_Test &t)
{
  is >> t.char_;
  is >> t.word2_;
  is >> t.word4_;
  is >> t.word8_;
  is >> t.fpoint_;
  is >> t.dprec_;
}

bool
CDR_Test::operator == (const CDR_Test &rhs) const
{
  // @@ Workaround bug in egcs-1.1.1 using a single && expression
  // results in UMR errors in purify.
  if (this->char_ != rhs.char_)
    return false;
  if (this->word2_ != rhs.word2_)
    return false;
  if (this->word4_ != rhs.word4_)
    return false;
  if (this->word8_ != rhs.word8_)
    return false;
  if (!ACE::is_equal (this->fpoint_, rhs.fpoint_))
    return false;
  if (!ACE::is_equal (this->dprec_, rhs.dprec_))
    return false;
  return true;
}

static int
run_test (int write_file,
          ACE_FILE_IO &file,
          const ACE_TCHAR *filename,
          CDR_Test &cdr_test)
{
  if (write_file)
    {
      char byte_order = ACE_CDR_BYTE_ORDER;
      size_t n = file.send (&byte_order, 1);
      if (n != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("send failed on %p\n"),
                           filename),
                          -1);

      ACE_OutputCDR output_cdr (0,
                                ACE_CDR_BYTE_ORDER,
                                0,
                                0,
                                0,
                                ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                                ACE_CDR_GIOP_MAJOR_VERSION,
                                ACE_CDR_GIOP_MINOR_VERSION);
      // Marshal the <CDR_Test> object data to the output CDR stream.
      output_cdr << cdr_test;

      // Output the data to cout.
      *ace_file_stream::instance ()->output_file () << cdr_test;

      // Save the data.
      const ACE_Message_Block *output_mb =
        output_cdr.begin ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Writing file %s in %s endian format...\n"),
                  filename,
                  ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big")));

      n = file.send (output_mb->rd_ptr (),
                     output_mb->length ());
      if (n != (size_t) output_mb->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("send failed on %p\n"),
                           filename),
                          -1);
    }
  else // We're reading from the file.
    {
      ACE_FILE_Info info;
      if (file.get_info (info) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("get_info failed on %p\n"),
                           filename),
                          -1);

      ACE_OFF_T msgsize = info.size_ - 1;

      // Allocate the input buffer
      char *buffer = 0;
      ACE_NEW_RETURN (buffer,
                      char[msgsize],
                      -1);
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
      ACE_OS::memset(buffer, 0, sizeof (buffer));
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

      // Make sure <buffer> is released automagically.
      ACE_Auto_Basic_Array_Ptr<char> b (buffer);

      // Move the file pointer back to the beginning of the file.
      if (file.seek (0,
                     SEEK_SET) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           filename),
                          -1);

      char byte_order;
      ssize_t size = file.recv (&byte_order, 1);
      if (size != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Read %d bytes, rather than expected ")
                           ACE_TEXT ("1 bytes\n"),
                           size),
                          -1);

      // Read the cdr data from the file into the buffer.
      size = file.recv (buffer, msgsize);
      if (size != msgsize)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Read %d bytes, rather than expected ")
                           ACE_TEXT ("%d bytes\n"),
                           size,
                           msgsize),
                          -1);

      // Create message block for the whole file.  Ensure that it is
      // aligned to properly handle the double.
      ACE_Message_Block mb (ACE_CDR::MAX_ALIGNMENT + msgsize);
      ACE_CDR::mb_align (&mb);

      mb.copy (buffer, msgsize);

      // Create CDR input stream from the message block.

      ACE_InputCDR input_cdr (&mb);
      input_cdr.reset_byte_order ((int) byte_order);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reading file %s in %s endian format...\n"),
                  filename,
                  ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big")));

      CDR_Test temp;

      // Demarshal the data from the input CDR stream into the
      // <CDR_Test> object.
      input_cdr >> temp;

      *ace_file_stream::instance ()->output_file () << temp;

      if (!(temp == cdr_test))
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Data mismatch across file\n")));
    }

  return 0;
}

static void
usage (const ACE_TCHAR *cmd)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s ")
              ACE_TEXT ("[-f filename [-w|-r]]"),
              cmd));
  ACE_OS::exit (1);
}

// Main function

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_File_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version (),
              ACE::minor_version (),
              ACE::beta_version ()));

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:rw"));
  int opt;
  int reading = 1;
  int writing = 1;
  ACE_TCHAR* fn = 0;
  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'f':
          fn = get_opt.opt_arg ();
          break;
        case 'r':
          writing = 0;
          break;
        case 'w':
          reading = 0;
          break;
        case '?':
        default:
          usage (ACE_TEXT("CDR_File_Test"));
        }
    }

  if ((!reading || !writing) && fn == 0)
    usage (ACE_TEXT("CDR_File_Test"));

  if (!reading && !writing)
    usage (ACE_TEXT("CDR_File_Test"));

  // Create a temporary filename.
  ACE_FILE_Addr filename (ACE_sap_any_cast (ACE_FILE_Addr &));
  if (fn != 0)
    filename.set (fn);


  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  // Open up the file.
  if (connector.connect (file,
                         filename,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         ((writing) ? (O_RDWR | O_CREAT) : O_RDONLY),
                         ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("connect failed for %p\n"),
                       filename.get_path_name ()),
                      1);

  CDR_Test cdr_test ('a',
                     0x00ff,
                     0xaabbccdd,
                     0x01234567,
                     1.54321f,
                     1.12345);

  if (writing)
    {
      // write the file.
      run_test (1,
                file,
                filename.get_path_name (),
                cdr_test);
    }

  if (reading)
    {
      // read the file.
      run_test (0,
                file,
                filename.get_path_name (),
                cdr_test);
    }

  if (fn == 0)
    {
      file.close ();
      if (file.unlink () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("unlink failed for %p\n"),
                           filename.get_path_name ()),
                          1);
    }

  ACE_END_TEST;
  return 0;
}

#else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_File_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("iostreams not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
