// $Id$

#include "Blob.h"
#include "Blob_Handler.h"

ACE_RCSID(Blobby, Blob, "$Id$")

ACE_Blob::ACE_Blob (void)
{
}

ACE_Blob::~ACE_Blob (void)
{
  this->close ();
}

// initialize address and filename. No network i/o in open

int
ACE_Blob::open (ACE_TCHAR *filename, const ACE_TCHAR *hostname , u_short port)
{
  filename_ = ACE_OS_String::strdup (filename);
  inet_addr_.set (port, hostname);
  return 0;
}

// read from connection length bytes from offset, into Message block

int
ACE_Blob::read (ACE_Message_Block *mb, size_t length, size_t offset)
{

  // Create a Blob Reader
  ACE_Blob_Reader blob_reader (mb, length, offset, filename_);
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
  ACE_Blob_Writer blob_writer (mb, length, offset, filename_);
  ACE_Blob_Handler *bwp = &blob_writer;

  // Connect to the server
  if (connector_.connect (bwp, inet_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Blob::write():Connector error"), -1);

  return blob_writer.byte_count ();
}

// close down the blob

int
ACE_Blob::close (void)
{

  if (filename_)
    {
      ACE_OS::free ((void *) filename_);
      filename_ = 0;
    }
  return 0;

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<ACE_Blob_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple <ACE_Blob_Handler>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<ACE_Blob_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple <ACE_Blob_Handler>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple <ACE_Blob_Handler> *, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
