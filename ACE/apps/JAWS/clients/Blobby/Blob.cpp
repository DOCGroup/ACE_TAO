#include "Blob.h"
#include "Blob_Handler.h"

ACE_Blob::~ACE_Blob ()
{
  this->close ();
}

// initialize address and filename. No network i/o in open

int
ACE_Blob::open (ACE_TCHAR const *filename, ACE_TCHAR const *hostname, u_short port)
{
  this->filename_ = filename;
  inet_addr_.set (port, hostname);
  return 0;
}

// read from connection length bytes from offset, into Message block

int
ACE_Blob::read (ACE_Message_Block *mb, size_t length, size_t offset)
{
  // Create a Blob Reader
  ACE_Blob_Reader blob_reader (mb, length, offset, this->filename_.c_str ());
  ACE_Blob_Handler *brp = &blob_reader;

  // Connect to the server
  if (connector_.connect (brp, inet_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Blob::read():Connector error"), -1);

  return blob_reader.byte_count ();
}

// write to connection length bytes from offset, into Message block

int
ACE_Blob::write (ACE_Message_Block *mb, size_t length, size_t offset)
{
  // Create a Blob Writer
  ACE_Blob_Writer blob_writer (mb, length, offset, this->filename_.c_str ());
  ACE_Blob_Handler *bwp = &blob_writer;

  // Connect to the server
  if (connector_.connect (bwp, inet_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Blob::write():Connector error"), -1);

  return blob_writer.byte_count ();
}

// close down the blob

int
ACE_Blob::close ()
{
  this->filename_.clear ();
  return 0;
}

