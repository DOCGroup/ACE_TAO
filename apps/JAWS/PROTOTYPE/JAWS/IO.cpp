// $Id$

#include "JAWS/JAWS.h"

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/IO_Acceptor.h"

// #include "HTTP_Helpers.h"

#include "ace/Message_Block.h"
#include "ace/SOCK_Stream.h"
#include "ace/Filecache.h"

JAWS_IO::JAWS_IO (void)
  : handle_ (ACE_INVALID_HANDLE),
    handler_ (0),
    inet_addr_ (0),
    acceptor_ (0)
{
}

JAWS_IO::~JAWS_IO (void)
{
}

#if 0
ACE_HANDLE
JAWS_IO::handle (void)
{
  return this->handle_;
}

void
JAWS_IO::handle (ACE_HANDLE handle)
{
  this->handle_ = handle;
}

void
JAWS_IO::handler (JAWS_IO_Handler *handler)
{
  this->handler_ = handler;
}

void
JAWS_IO::acceptor (JAWS_IO_Acceptor *acceptor)
{
  this->acceptor_ = acceptor;
}
#endif /* 0 */

JAWS_Synch_IO::JAWS_Synch_IO (void)
{
  this->acceptor_ = JAWS_IO_Synch_Acceptor_Singleton::instance ();
}

JAWS_Synch_IO::~JAWS_Synch_IO (void)
{
  ACE_OS::closesocket (this->handle_);
}

void
JAWS_Synch_IO::accept (JAWS_IO_Handler *ioh,
                       ACE_Message_Block *,
                       unsigned int size)
{
  ACE_SOCK_Stream new_stream;
  new_stream.set_handle (ACE_INVALID_HANDLE);
  if (this->acceptor_->accept (new_stream) == -1)
    ioh->accept_error ();
  else
    ioh->accept_complete (new_stream.get_handle ());
}

void
JAWS_Synch_IO::read (JAWS_IO_Handler *ioh,
                     ACE_Message_Block *mb,
                     unsigned int size)
{
  JAWS_TRACE ("JAWS_Synch_IO::read");

  ACE_SOCK_Stream stream;

  stream.set_handle (ioh->handle ());
  int result = stream.recv (mb->wr_ptr (), size);

  if (result <= 0)
    ioh->read_error ();
  else
    {
      JAWS_TRACE ("JAWS_Synch_IO::read success");
      mb->wr_ptr (result);
      ioh->read_complete (mb);
    }
}

void
JAWS_Synch_IO::receive_file (JAWS_IO_Handler *ioh,
                             const char *filename,
                             void *initial_data,
                             unsigned int initial_data_length,
                             unsigned int entire_length)
{
  ACE_Filecache_Handle handle (filename, entire_length);

  int result = handle.error ();

  if (result == ACE_Filecache_Handle::SUCCESS)
    {
      ACE_SOCK_Stream stream;
      stream.set_handle (ioh->handle ());

      int bytes_to_memcpy = ACE_MIN (entire_length, initial_data_length);
      ACE_OS::memcpy (handle.address (), initial_data, bytes_to_memcpy);

      int bytes_to_read = entire_length - bytes_to_memcpy;

      int bytes = stream.recv_n ((char *)
                                 handle.address () + initial_data_length,
                                 bytes_to_read);
      if (bytes == bytes_to_read)
        ioh->receive_file_complete ();
      else
        result = -1;
    }

  if (result != ACE_Filecache_Handle::SUCCESS)
    ioh->receive_file_error (result);
}

void
JAWS_Synch_IO::transmit_file (JAWS_IO_Handler *ioh,
                              const char *filename,
                              const char *header,
                              unsigned int header_size,
                              const char *trailer,
                              unsigned int trailer_size)
{
  ACE_Filecache_Handle handle (filename);

  int result = handle.error ();

  if (result == ACE_Filecache_Handle::SUCCESS)
    {
#if defined (ACE_JAWS_BASELINE)
      ACE_SOCK_Stream stream;
      stream.set_handle (ioh->handle ());

      if ((stream.send_n (header, header_size) == header_size)
          && ((u_long) stream.send_n (handle.address (), handle.size ())
              == handle.size ())
          && (stream.send_n (trailer, trailer_size) == trailer_size))
        this->handler_->transmit_file_complete ();
      else
        result = -1;
#else
      // Attempting to use writev
      // Is this faster?
      struct iovec iov[3];
      int iovcnt = 0;
      if (header_size > 0)
        {
          iov[iovcnt].iov_base = (char *) header;
          iov[iovcnt].iov_len = header_size;
          iovcnt++;
        }
      if (handle.size () > 0)
        {
          iov[iovcnt].iov_base = (char *) handle.address ();
          iov[iovcnt].iov_len = handle.size ();
          iovcnt++;
        }
      if (trailer_size > 0)
        {
          iov[iovcnt].iov_base = (char *) trailer;
          iov[iovcnt].iov_len = trailer_size;
          iovcnt++;
        }
      if (ACE_OS::writev (ioh->handle (), iov, iovcnt) < 0)
        result = -1;
      else
        ioh->transmit_file_complete ();
#endif /* ACE_JAWS_BASELINE */
    }

  if (result != ACE_Filecache_Handle::SUCCESS)
    ioh->transmit_file_error (result);
}

void
JAWS_Synch_IO::send_confirmation_message (JAWS_IO_Handler *ioh,
                                          const char *buffer,
                                          unsigned int length)
{
  this->send_message (ioh, buffer, length);
  ioh->confirmation_message_complete ();
}

void
JAWS_Synch_IO::send_error_message (JAWS_IO_Handler *ioh,
                                   const char *buffer,
                                   unsigned int length)
{
  this->send_message (ioh, buffer, length);
  ioh->error_message_complete ();
}

void
JAWS_Synch_IO::send_message (JAWS_IO_Handler *ioh,
                             const char *buffer,
                             unsigned int length)
{
  ACE_SOCK_Stream stream;
  stream.set_handle (ioh->handle ());
  stream.send_n (buffer, length);
}

// This only works on Win32
#if defined (ACE_WIN32)

JAWS_Asynch_IO::JAWS_Asynch_IO (void)
{
}

JAWS_Asynch_IO::~JAWS_Asynch_IO (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->handle_);
}

void
JAWS_Asynch_IO::accept (JAWS_IO_Handler *ioh,
                        ACE_Message_Block* mb,
                        unsigned int size)
{
  JAWS_Asynch_IO_Handler *aioh =
    ACE_dynamic_cast (JAWS_Asynch_IO_Handler *, ioh);

  ACE_Asynch_Accept aa;

  if (aa.open (*(aioh->handler ()), aioh->handle ()) == -1
      || aa.accept (*mb, size) == -1)
    aioh->accept_error ();
}

void
JAWS_Asynch_IO::read (JAWS_IO_Handler *ioh,
                      ACE_Message_Block* mb,
                      unsigned int size)
{
  JAWS_Asynch_IO_Handler *aioh =
    ACE_dynamic_cast (JAWS_Asynch_IO_Handler *, ioh);

  ACE_Asynch_Read_Stream ar;

  if (ar.open (*(aioh->handler ()), aioh->handle ()) == -1
      || ar.read (*mb, size) == -1)
    aioh->read_error ();
}

void
JAWS_Asynch_IO::receive_file (JAWS_IO_Handler *ioh,
                              const char *filename,
                              void *initial_data,
                              unsigned int initial_data_length,
                              unsigned int entire_length)
{
  JAWS_Asynch_IO_Handler *aioh =
    ACE_dynamic_cast (JAWS_Asynch_IO_Handler *, ioh);

  ACE_Message_Block *mb = 0;
  ACE_Filecache_Handle *handle;

  ACE_NEW (handle, ACE_Filecache_Handle (filename, entire_length, NOMAP));

  int result = handle->error ();

  if (result == ACE_Filecache_Handle::SUCCESS)
    {
      ACE_OS::memcpy (handle->address (),
                      initial_data,
                      initial_data_length);

      int bytes_to_read = entire_length - initial_data_length;

      ACE_NEW (mb, ACE_Message_Block ((char *)handle->address ()
                                      + initial_data_length, bytes_to_read));

      if (mb == 0)
        {
          errno = ENOMEM;
          result = -1;
        }
      else
        {
          ACE_Asynch_Read_Stream ar;

          if (ar.open (*(aioh->handler ()), aioh->handle ()) == -1
              || ar.read (*mb, mb->size () - mb->length (), handle) == -1)
            result = -1;
        }
    }

  if (result != ACE_Filecache_Handle::SUCCESS)
    {
      this->handler_->receive_file_error (result);
      delete mb;
      delete handle;
    }
}

void
JAWS_Asynch_IO::transmit_file (JAWS_IO_Handler *ioh,
                               const char *filename,
                               const char *header,
                               unsigned int header_size,
                               const char *trailer,
                               unsigned int trailer_size)
{
  JAWS_Asynch_IO_Handler *aioh =
    ACE_dynamic_cast (JAWS_Asynch_IO_Handler *, ioh);

  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer = 0;
  ACE_Filecache_Handle *handle = new ACE_Filecache_Handle (filename, NOMAP);

  int result = handle->error ();

  if (result == ACE_Filecache_Handle::SUCCESS)
    {
      ACE_Message_Block header_mb (header, header_size);
      ACE_Message_Block trailer_mb (trailer, trailer_size);

      header_and_trailer = new ACE_Asynch_Transmit_File::Header_And_Trailer
        (&header_mb, header_size, &trailer_mb, trailer_size);

      ACE_Asynch_Transmit_File tf;

      if (tf.open (*(aioh->handler ()), aioh->handle ()) == -1
          || tf.transmit_file (handle->handle (), // file handle
                               header_and_trailer, // header and trailer data
                               0,  // bytes_to_write
                               0,  // offset
                               0,  // offset_high
                               0,  // bytes_per_send
                               0,  // flags
                               handle // act
                               ) == -1)
        result = -1;
    }

  if (result != ACE_Filecache_Handle::SUCCESS)
    {
      this->handler_->transmit_file_error (result);
      delete header_and_trailer;
      delete handle;
    }
}

void
JAWS_Asynch_IO::send_confirmation_message (JAWS_IO_Handler *ioh,
                                           const char *buffer,
                                           unsigned int length)
{
  this->send_message (ioh, buffer, length, CONFIRMATION);
}

void
JAWS_Asynch_IO::send_error_message (JAWS_IO_Handler *ioh,
                                    const char *buffer,
                                    unsigned int length)
{
  this->send_message (ioh, buffer, length, ERROR_MESSAGE);
}

void
JAWS_Asynch_IO::send_message (JAWS_IO_Handler *ioh,
                              const char *buffer,
                              unsigned int length,
                              int act)
{
  JAWS_Asynch_IO_Handler *aioh =
    ACE_dynamic_cast (JAWS_Asynch_IO_Handler *, ioh);

  ACE_Message_Block *mb;
  ACE_NEW (mb, ACE_Message_Block (buffer, length));

  if (mb == 0)
    {
      this->handler_->error_message_complete ();
      return;
    }

  ACE_Asynch_Write_Stream aw;
  if (aw.open (*(aioh->handler ()), aioh->handle ()) == -1
      || aw.write (*mb, length, (void *) act) == -1)
    {
      mb->release ();

      if (act == CONFORMATION)
        ioh->confirmation_message_complete ();
      else
        ioh->error_message_complete ();
    }
}

#endif /* ACE_WIN32 */

// #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// template class ACE_Singleton<JAWS_VFS, ACE_SYNCH_MUTEX>;
// #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// #pragma instantiate ACE_Singleton<JAWS_VFS, ACE_SYNCH_MUTEX>
// #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_Synch_IO, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_Synch_IO, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
