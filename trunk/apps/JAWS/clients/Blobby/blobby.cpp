/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    JAWS
// 
// = FILENAME
//    blobby.c
//
// = DESCRIPTION
//     Simple client application to illustrate the use of the ACE_Blob class
//
//     It reads "length" number of bytes, after skipping offset "offset" 
//     from hostname, port and filename as specified. (if -r specified)
//
//     It writes "length" number of bytes, after skipping offset "offset"
//     to hostname, port and filename as specified (if -w specified)
//
// = AUTHOR
//    Prashant Jain and Sumedh Mungee
// 
//============================================================================

#include "Options.h"

int
main (int argc, char *argv[]) 
{
  // Options is a singleton
  Options *options = Options::instance ();
  options->parse_args (argc, argv);

  // Explain what is going to happen
  if (options->debug_)
    ACE_DEBUG ((LM_DEBUG, 
		"hostname = %s, port = %d, filename = %s, length = %d, offset = %d, operation = %c\n", 
		options->hostname_,
		options->port_,
		options->filename_,
		options->length_,
		options->offset_,
		options->operation_));

  // Create a blob
  ACE_Blob blob;

  // User requested a read
  if (options->operation_ == 'r') 
    {
      ACE_Message_Block mb (0, options->length_);

      // Open the blob
      if (blob.open (options->filename_, options->hostname_, options->port_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open error"), -1);

      // Read from it
      if (blob.read (&mb, options->length_, options->offset_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "read error"), -1);
      
      // Write to STDOUT
      if (ACE_OS::write (ACE_STDOUT, mb.rd_ptr(), mb.length()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write error"), -1);        
    }
  else 
    {
      int total = options->length_ + options->offset_;
      ACE_Message_Block mb (total);

      // Open the file to be sent
      ACE_HANDLE h = ACE_OS::open (options->filename_, O_RDONLY);
      if (h == ACE_INVALID_HANDLE) 
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "file open error"), -1);

      // Open the blob 
      if (blob.open (options->filename_, options->hostname_, options->port_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connection open error"), -1);

      // Read from the file
      if (ACE_OS::read (h, mb.wr_ptr (), total) != total)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "file read error"), -1);

      // Close the file
      ACE_OS::close (h);

      // Adjust the offset
      mb.wr_ptr (mb.size ());

      // Write to the blob
      if (blob.write (&mb, options->length_, options->offset_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "network write error"), -1);
    }

  blob.close ();
  return 0;
}
