/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
// 
//    ace
//
// = FILENAME
// 
//    Asynch_IO_Impl.h
//
// = DESCRIPTION
// 
//    This class contains asbtract base classes for all the concrete
//    implementation classes for the various asynchronous operations
//    that are used with the Praoctor.
//
// = AUTHOR
// 
//    Irfan Pyarali (irfan@cs.wustl.edu),
//    Tim Harrison (harrison@cs.wustl.edu) and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_ASYNCH_IO_IMPL_H)
#define ACE_ASYNCH_IO_IMPL_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))
// This only works on Win32 platforms and on Unix platforms supporting
// aio calls. 

#include "ace/Asynch_IO.h"

class ACE_Export ACE_Asynch_Result_Impl
{
  // = TITLE
  //
  //     Abstract base class for the all the classes that provide
  //     concrete implementations for ACE_Asynch_Result.
  // 
  // = DESCRIPTION
  // 
public:
  virtual ~ACE_Asynch_Result_Impl (void) {}

  virtual u_long bytes_transferred (void) const = 0;
  // Number of bytes transferred by the operation.

  virtual const void *act (void) const = 0;
  // ACT associated with the operation.

  virtual int success (void) const = 0;
  // Did the operation succeed?

  virtual const void *completion_key (void) const = 0;
  // This ACT is not the same as the ACT associated with the
  // asynchronous operation. 

  virtual u_long error (void) const = 0;
  // Error value if the operation fail.

  virtual ACE_HANDLE event (void) const = 0;
  // Event associated with the OVERLAPPED structure.

  virtual u_long offset (void) const = 0;
  virtual u_long offset_high (void) const = 0;
  // This really make sense only when doing file I/O.

  virtual int priority (void) const = 0;
  // Priority of the operation.
  
  // protected:
  //
  // These two should really be protected.  But sometimes it
  // simplifies code to be able to "fake" a result.  Use carefully.
  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0) = 0;
  // This is called when the asynchronous operation completes.

protected:
  ACE_Asynch_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Operation_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Operation.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Operation_Impl () {}
  
  virtual int open (ACE_Handler &handler,
                    ACE_HANDLE handle,
                    const void *completion_key,
                    ACE_Proactor *proactor) = 0;
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  virtual int cancel (void) = 0;
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  // = Access methods.

  virtual ACE_Proactor* proactor (void) const = 0;
  // Return the underlying proactor.

protected:
  ACE_Asynch_Operation_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Read_Stream_Impl : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Read_Stream
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Read_Stream_Impl (void) {}
  
  virtual int read (ACE_Message_Block &message_block,
                    u_long bytes_to_read,
                    const void *act,
                    int priority) = 0;
  // This starts off an asynchronous read. Upto <bytes_to_read> will
  // be read and stored in the <message_block>.

protected:
  ACE_Asynch_Read_Stream_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Read_Stream_Result_Impl : public virtual ACE_Asynch_Result_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Read_Stream::Result class.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Read_Stream_Result_Impl () {}

  virtual u_long bytes_to_read (void) const = 0;
  // The number of bytes which were requested at the start of the
  // asynchronous read.
  
  virtual ACE_Message_Block &message_block (void) const = 0;
  // Message block which contains the read data.
  
  virtual ACE_HANDLE handle (void) const = 0;
  // I/O handle used for reading.

protected:
  ACE_Asynch_Read_Stream_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Write_Stream_Impl : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Write_Stream class.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Write_Stream_Impl (void) {}
  
  virtual int write (ACE_Message_Block &message_block,
                     u_long bytes_to_write,
                     const void *act,
                     int priority) = 0;
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be written from the <message_block>.

protected:
  ACE_Asynch_Write_Stream_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Write_Stream_Result_Impl : public virtual ACE_Asynch_Result_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Write_Stream::Result.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Write_Stream_Result_Impl () {}

  virtual u_long bytes_to_write (void) const = 0;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  virtual ACE_Message_Block &message_block (void) const = 0;
  // Message block that contains the data to be written.

  virtual ACE_HANDLE handle (void) const = 0;
  // I/O handle used for writing.

protected:
  ACE_Asynch_Write_Stream_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Read_File_Impl : public virtual ACE_Asynch_Read_Stream_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Read_File::Result.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Read_File_Impl () {}
  
  virtual int read (ACE_Message_Block &message_block,
                    u_long bytes_to_read,
                    u_long offset,
                    u_long offset_high,
                    const void *act,
                    int priority) = 0;
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.  The read will start
  // at <offset> from the beginning of the file.

protected:
  ACE_Asynch_Read_File_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Read_File_Result_Impl : public virtual ACE_Asynch_Read_Stream_Result_Impl
{
  // = TITLE
  //     This is the abstract base class for all the concrete
  //     implementation classes for ACE_Asynch_Read_File::Result.
  //
  // = DESCRIPTION
  // 
public:
  virtual ~ACE_Asynch_Read_File_Result_Impl () {}
  // Destructor.

protected:
  ACE_Asynch_Read_File_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Write_File_Impl : public virtual ACE_Asynch_Write_Stream_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Write_File.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Write_File_Impl (void) {}
  
  virtual int write (ACE_Message_Block &message_block,
                     u_long bytes_to_write,
                     u_long offset,
                     u_long offset_high,
                     const void *act,
                     int priority) = 0;
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be write and stored in the <message_block>.  The write will
  // start at <offset> from the beginning of the file.

protected:
  ACE_Asynch_Write_File_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Write_File_Result_Impl : public virtual ACE_Asynch_Write_Stream_Result_Impl
{
  // = TITLE
  // 
  //     This is the abstract base class for all the concrete
  //     implementation classes that provide different implementations
  //     for the ACE_Asynch_Write_File::Result.
  //
  // = DESCRIPTION
  // 
public:
  virtual ~ACE_Asynch_Write_File_Result_Impl () {}

protected:
  ACE_Asynch_Write_File_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Accept_Impl : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Accept.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Accept_Impl (void) {}

  virtual int accept (ACE_Message_Block &message_block,
                      u_long bytes_to_read,
                      ACE_HANDLE accept_handle,
                      const void *act,
                      int priority) = 0;
  // This starts off an asynchronous accept.  The asynchronous accept
  // call also allows any initial data to be returned to the
  // <handler>.  Upto <bytes_to_read> will be read and stored in the
  // <message_block>.  The <accept_handle> will be used for the
  // <accept> call.  If (<accept_handle> == INVALID_HANDLE), a new
  // handle will be created.
  //
  // <message_block> must be specified. This is because the address of
  // the new connection is placed at the end of this buffer.

protected:
  ACE_Asynch_Accept_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Accept_Result_Impl : public virtual ACE_Asynch_Result_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Accept.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Accept_Result_Impl () {}

  virtual u_long bytes_to_read (void) const = 0;
  // The number of bytes which were requested at the start of the 
  // asynchronous accept.

  virtual ACE_Message_Block &message_block (void) const = 0;
  // Message block which contains the read data.

  virtual ACE_HANDLE listen_handle (void) const = 0;
  // I/O handle used for accepting new connections.

  virtual ACE_HANDLE accept_handle (void) const = 0;
  // I/O handle for the new connection.

protected:
  ACE_Asynch_Accept_Result_Impl (void);
  // Do-nothing constructor.
};

class ACE_Asynch_Transmit_File_Impl : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Transmit_File.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Transmit_File_Impl () {}
  
  virtual int transmit_file (ACE_HANDLE file,
                             ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                             u_long bytes_to_write,
                             u_long offset,
                             u_long offset_high,
                             u_long bytes_per_send,
                             u_long flags,
                             const void *act,
                             int priority) = 0;
  // This starts off an asynchronous transmit file.

protected:
  ACE_Asynch_Transmit_File_Impl (void);
  // Do-nothing constructor.
};

class ACE_Export ACE_Asynch_Transmit_File_Result_Impl : public virtual ACE_Asynch_Result_Impl
{
  // = TITLE
  // 
  //     Abstract base class for all the concrete implementation
  //     classes that provide different implementations for the
  //     ACE_Asynch_Transmit_File::Result.
  // 
  // = DESCRIPTION
  //
public:
  virtual ~ACE_Asynch_Transmit_File_Result_Impl () {}
  
  virtual ACE_HANDLE socket (void) const = 0;
  // Socket used for transmitting the file.
  
  virtual ACE_HANDLE file (void) const = 0;
  // File from which the data is read.
  
  virtual ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer (void) const = 0;
  // Header and trailer data associated with this transmit file.
  
  virtual u_long bytes_to_write (void) const = 0;
  // The number of bytes which were requested at the start of the
  // asynchronous transmit file.
  
  virtual u_long bytes_per_send (void) const = 0;
  // Number of bytes per send requested at the start of the transmit
  // file.
    
  virtual u_long flags (void) const = 0;
  // Flags which were passed into transmit file.

protected:
  ACE_Asynch_Transmit_File_Result_Impl (void);
  // Do-nothing constructor.
};

#if defined (__ACE_INLINE__)
#include "ace/Asynch_IO_Impl.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS  || !ACE_HAS_WINCE && ACE_WIN32 */
#endif /* ACE_ASYNCH_IO_IMPL_H */
