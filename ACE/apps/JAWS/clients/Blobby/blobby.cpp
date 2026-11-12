
//=============================================================================
/**
 *  @file    blobby.cpp
 *
 *   Simple client application to illustrate the use of the ACE_Blob class
 *
 *   It reads "length" number of bytes, after skipping offset "offset"
 *   from hostname, port and filename as specified. (if -r specified)
 *
 *   It writes "length" number of bytes, after skipping offset "offset"
 *   to hostname, port and filename as specified (if -w specified)
 *
 *  @author Prashant Jain and Sumedh Mungee
 */
//=============================================================================

#include "Options.h"
#include "Blob.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Options is a singleton
  Options * const options = Options::instance ();
  options->parse_args (argc, argv);

  // Explain what is going to happen
  if (options->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("hostname = %C, port = %d, filename = %s, ")
                ACE_TEXT ("length = %d, offset = %d, operation = %c\n"),
                options->hostname_.c_str (),
                options->port_,
                options->filename_.c_str (),
                options->length_,
                options->offset_,
                options->operation_));

  // Create a blob
  ACE_Blob blob;

  // User requested a read
  if (options->operation_ == 'r')
    {
      ACE_Message_Block mb (static_cast<char const *> (nullptr), options->length_);

      // Open the blob
      if (blob.open (options->filename_.c_str (),
                     options->hostname_.c_str (),
                     options->port_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("open error")),
                          -1);

      // Read from it
      if (blob.read (&mb, options->length_, options->offset_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("read error")),
                          -1);

      // Write to STDOUT
      if (ACE_OS::write (ACE_STDOUT, mb.rd_ptr(), mb.length()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write error")),
                          -1);
    }
  else
    {
      int const total = options->length_ + options->offset_;
      ACE_Message_Block mb (total);

      // Open the file to be sent
      std::unique_ptr<FILE, decltype (&ACE_OS::fclose)> input_file (
        ACE_OS::fopen (options->filename_.c_str (), ACE_TEXT ("rb")),
        &ACE_OS::fclose);
      if (!input_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("file open error")),
                          -1);

      // Open the blob
      if (blob.open (options->filename_.c_str (), options->hostname_.c_str (), options->port_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("connection open error")),
                          -1);

      // Read from the file
      if (ACE_OS::fread (mb.wr_ptr (), 1, total, input_file.get ()) != static_cast<size_t> (total))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("file read error")),
                          -1);

      // Adjust the offset
      mb.wr_ptr (mb.size ());

      // Write to the blob
      if (blob.write (&mb, options->length_, options->offset_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("network write error")),
                          -1);
    }

  blob.close ();
  return 0;
}
