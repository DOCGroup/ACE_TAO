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
// ============================================================================

#include "ace/CDR_Stream.h"
#include "ace/FILE_Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "test_config.h"

ACE_RCSID(tests, CDR_File_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

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
            ACE_CDR::Long w,
            ACE_CDR::Float f,
            ACE_CDR::Double d);
  // Constructor.

  int operator == (const CDR_Test &rhs);
  // Compare <rhs> for equality with <this>.

private:
  ACE_CDR::Char char_;
  ACE_CDR::Long word_;
  ACE_CDR::Float fpoint_;
  ACE_CDR::Double dprec_;
};

ostream &
operator << (ostream &os,
             const CDR_Test &t)
{
  os << "Char:  " << t.char_ << endl
     << "Long:   " << t.word_ << endl
     << "Float:  " << t.fpoint_ << endl
     << "Double: " << t.dprec_ << endl;
  return os;
}

CDR_Test::CDR_Test (void)
  : char_  (0),
    word_ (0),
    fpoint_ (0.0),
    dprec_ (0.0)
{
}

CDR_Test::CDR_Test (ACE_CDR::Char o,
                    ACE_CDR::Long w,
                    ACE_CDR::Float f,
                    ACE_CDR::Double d)
  : char_ (o),
    word_ (w),
    fpoint_ (f),
    dprec_ (d)
{
}

void
operator << (ACE_OutputCDR &os, const CDR_Test &t)
{
  os << t.char_;
  os << t.word_;
  os << t.fpoint_;
  os << t.dprec_;
}

void
operator >> (ACE_InputCDR &is, CDR_Test &t)
{
  is >> t.char_;
  is >> t.word_;
  is >> t.fpoint_;
  is >> t.dprec_;
}

int
CDR_Test::operator == (const CDR_Test &rhs)
{
  return this->char_ == rhs.char_
    && this->word_ == rhs.word_
    && this->fpoint_ == rhs.fpoint_
    && this->dprec_ == rhs.dprec_;
}

static int
run_test (int write_file,
          ACE_FILE_IO &file,
          LPCTSTR filename,
          CDR_Test &cdr_test)
{
  if (write_file)
    {
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
                  ASYS_TEXT ("Writing file %s in %s endian format...\n"),
                  filename,
                  ACE_CDR_BYTE_ORDER ? "little" : "big"));

      ssize_t n = file.send (output_mb->rd_ptr (),
                             output_mb->length ());
      if (n != (ssize_t) output_mb->length())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("send failed on %p\n"),
                           filename),
                          -1);
    }
  else // We're reading from the file.
    {
      ACE_FILE_Info info;
      if (file.get_info (info) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("get_info failed on %p\n"),
                           filename),
                          -1);

      // Allocate the input buffer
      char *buffer;
      ACE_NEW_RETURN (buffer,
                      char[info.size_],
                      -1);
      // Make sure <buffer> is released automagically.
      ACE_Auto_Basic_Array_Ptr<char> b (buffer);

      // Move the file pointer back to the beginning of the file.
      if (file.seek (0,
                     SEEK_SET) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           filename),
                          -1);
      // Read the file into the buffer.
      ssize_t size = file.recv (buffer,
                                info.size_);
      if (size != info.size_)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("Read %d bytes, rather than expected ")
                           ASYS_TEXT ("%d bytes\n"),
                           size,
                           info.size_),
                          -1);

      // Create message block for the whole file.  Ensure that it is
      // aligned to properly handle the double.
      ACE_Message_Block mb (ACE_CDR::MAX_ALIGNMENT + info.size_);
      ACE_CDR::mb_align (&mb);
      mb.copy (buffer,
               info.size_);

      // Create CDR input stream from the message block.

      ACE_InputCDR input_cdr (&mb);
      input_cdr.reset_byte_order (ACE_CDR_BYTE_ORDER);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("Reading file %s in %s endian format...\n"),
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

// Main function

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("CDR_File_Test"));

  // Create a temporary filename.
  ACE_FILE_Addr filename (ACE_sap_any_cast (ACE_FILE_Addr &));

  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  // Open up the file.
  if (connector.connect (file,
                         filename,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_RDWR | O_CREAT,
                         ACE_DEFAULT_FILE_PERMS) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("connect failed for %p\n"),
                         filename.get_path_name ()),
                        1);
  // Unlink this file right away so that it is automatically removed
  // when the process exits.
  else if (file.unlink () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("unlink failed for %p\n"),
                       filename.get_path_name ()),
                      1);
  CDR_Test cdr_test ('a',
                     1000,
                     1.54321f,
                     1.12345);

  // First write the file.
  run_test (1,
            file,
            filename.get_path_name (),
            cdr_test);

  // Then read the file.
  run_test (0,
            file,
            filename.get_path_name (),
            cdr_test);

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
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("CDR_File_Test"));

  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("iostreams not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
