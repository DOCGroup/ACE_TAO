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

#include "ace/CDR_Stream.h"
#include "ace/FILE_Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "test_config.h"

ACE_RCSID(tests, CDR_File_Test, "$Id$")

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
            ACE_CDR::LongLong lw,
            ACE_CDR::Float f,
            ACE_CDR::Double d);
  // Constructor.

  int operator == (const CDR_Test &rhs) const;
  // Compare <rhs> for equality with <this>.

private:
  ACE_CDR::Char char_;
  ACE_CDR::Short word2_;
  ACE_CDR::Long word4_;
  ACE_CDR::LongLong word8_;
  ACE_CDR::Float fpoint_;
  ACE_CDR::Double dprec_;
};

ostream &
operator << (ostream &os,
             const CDR_Test &t)
{
  os << "Char:              " << t.char_ << endl
     << "Short:             " << t.word2_ << endl
     << "Long:              " << t.word4_ << endl
#if !defined(_MSC_VER)
     << "LongLong:          " << t.word8_ << endl
#else
     << "LongLong 1st half: "
        << hex
        << ACE_reinterpret_cast(ACE_UINT32, (t.word8_ >> 32))
        << dec << endl
     << "LongLong 2nd half: "
        << hex
        << ACE_reinterpret_cast(ACE_UINT32, (t.word8_ & 0xffffffff))
        << dec << endl
#endif
     << "Float:             " << t.fpoint_ << endl
     << "Double:            " << t.dprec_ << endl;
  return os;
}

CDR_Test::CDR_Test (void)
  : char_  (0),
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
                    ACE_CDR::LongLong lw,
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

int
CDR_Test::operator == (const CDR_Test &rhs) const
{
  // @@ Workaround bug in egcs-1.1.1 using a single && expression
  // results in UMR errors in purify.
  if (this->char_ != rhs.char_)
    return 0;
  if (this->word2_ != rhs.word2_)
    return 0;
  if (this->word4_ != rhs.word4_)
    return 0;
  if (this->word8_ != rhs.word8_)
    return 0;
  if (this->fpoint_ != rhs.fpoint_)
    return 0;
  if (this->dprec_ != rhs.dprec_)
    return 0;
  return 1;
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
                                ACE_DEFAULT_CDR_MEMCPY_TRADEOFF);
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
                  ACE_CDR_BYTE_ORDER ? "little" : "big"));

      n = file.send (output_mb->rd_ptr (),
                     output_mb->length ());
      if (n != (size_t) output_mb->length())
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

      size_t msgsize = info.size_ - 1;

      // Allocate the input buffer
      char *buffer;
      ACE_NEW_RETURN (buffer,
                      char[msgsize],
                      -1);
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
      size_t size = file.recv (&byte_order, 1);
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
                  ACE_CDR_BYTE_ORDER ? "little" : "big"));

      CDR_Test temp;

      // Demarshal the data from the input CDR stream into the
      // <CDR_Test> object.
      input_cdr >> temp;

      *ace_file_stream::instance ()->output_file () << temp;
      ACE_ASSERT (temp == cdr_test);
    }

  return 0;
}

static void
usage(ACE_TCHAR* cmd)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s ")
              ACE_TEXT ("[-f filename [-w|-r]]"),
              cmd));
  ACE_OS::exit(1);
}

// Main function

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_File_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

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
          fn = get_opt.optarg;
          break;
        case 'r':
          writing = 0;
          break;
        case 'w':
          reading = 0;
          break;
        case '?':
        default:
          usage(argv[0]);
        }
    }

  if ((!reading || !writing) && fn == 0)
    {
      usage(argv[0]);
    }

  if (!reading && !writing)
    {
      usage(argv[0]);
    }

  // Create a temporary filename.
  ACE_FILE_Addr filename(ACE_sap_any_cast (ACE_FILE_Addr &));
  if (fn != 0)
    {
      filename.set (fn);
    }

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

  if (fn == 0)
    {
      // Unlink this file right away so that it is automatically removed
      // when the process exits.
      if (file.unlink () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("unlink failed for %p\n"),
                           filename.get_path_name ()),
                          1);
    }

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

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Not necessary here, because it's instantiated in
// ace/Memory_Pool.cpp.
// template class ACE_Auto_Basic_Array_Ptr<ACE_CDR::Char>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Not necessary here, because it's instantiated in
// ace/Memory_Pool.cpp.
// #pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_CDR::Char>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_File_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("iostreams not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
