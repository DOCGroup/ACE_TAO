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
//    WIN32_Asynch_IO.h
//
// = DESCRIPTION
//
//    These classes only works on Win32 platforms.
//
//    The implementation of <ACE_Asynch_Transmit_File> and
//    <ACE_Asynch_Accept> are only supported if ACE_HAS_WINSOCK2 is
//    defined or you are on WinNT 4.0 or higher.
//
// = AUTHOR
//
//    Irfan Pyarali (irfan@cs.wustl.edu),
//    Tim Harrison (harrison@cs.wustl.edu) and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_WIN32_ASYNCH_IO_H)
#define ACE_WIN32_ASYNCH_IO_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))

#include "ace/Asynch_IO_Impl.h"

// Forward declaration
class ACE_WIN32_Proactor;

class ACE_Export ACE_WIN32_Asynch_Result : public virtual ACE_Asynch_Result_Impl,
                                           public OVERLAPPED
{
  // = TITLE
  //
  //     An abstract class which adds information to the OVERLAPPED
  //     structure to make it more useful.
  //
  // = DESCRIPTION
  //
  //     An abstract base class from which you can obtain some basic
  //     information like the number of bytes transferred, the ACT
  //     associated with the asynchronous operation, indication of
  //     success or failure, etc.  Subclasses may want to store more
  //     information that is particular to the asynchronous operation
  //     it represents.

  friend class ACE_WIN32_Asynch_Accept;
  // Factory class has special permissions.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

  virtual ~ACE_WIN32_Asynch_Result (void);
  // Destructor.

protected:
  ACE_WIN32_Asynch_Result (ACE_Handler &handler,
                           const void* act,
                           ACE_HANDLE event,
                           u_long offset,
                           u_long offset_high,
                           int priority);
  // Constructor.

  ACE_Handler &handler_;
  // Handler that will be called back.

  const void *act_;
  // ACT for this operation.

  u_long bytes_transferred_;
  // Bytes transferred by this operation.

  int success_;
  // Success indicator.

  const void *completion_key_;
  // ACT associated with handle.

  u_long error_;
  // Error if operation failed.
};

class ACE_Export ACE_WIN32_Asynch_Operation : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  //
  //     This class abstracts out the common things needed for
  //     implementing Asynch_Operation for WIN32 platform.
  //
  // = DESCRIPTION
  //
public:
  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  // = Access methods.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  ACE_WIN32_Asynch_Operation (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  virtual ~ACE_WIN32_Asynch_Operation (void);
  // Destructor.

  ACE_WIN32_Proactor *win32_proactor_;
  // WIn32 Proactor.

  ACE_Proactor *proactor_;
  // Proactor that this asynch IO is registered with.

  ACE_Handler *handler_;
  // Handler that will receive the callback.

  ACE_HANDLE handle_;
  // I/O handle used for reading.
};

class ACE_Export ACE_WIN32_Asynch_Read_Stream_Result : public virtual ACE_Asynch_Read_Stream_Result_Impl,
                                                       public virtual ACE_WIN32_Asynch_Result
{
  // = TITLE
  //
  //     This is class provides concrete implementation for
  //     ACE_Asynch_Read_Stream::Result class.
  //
  // = DESCRIPTION
  //

  friend class ACE_WIN32_Asynch_Read_Stream;
  // Factory class will have special permissions.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  u_long bytes_to_read (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous read.

  ACE_Message_Block &message_block (void) const;
  // Message block which contains the read data.

  ACE_HANDLE handle (void) const;
  // I/O handle used for reading.

  // Base class operations. These operations are here to kill some
  // warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Read_Stream_Result (ACE_Handler &handler,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block &message_block,
                                       u_long bytes_to_read,
                                       const void* act,
                                       ACE_HANDLE event,
                                       int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Read_Stream factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // Proactor will call this method when the read completes.

  virtual ~ACE_WIN32_Asynch_Read_Stream_Result (void);
  // Destructor.

  u_long bytes_to_read_;
  // Bytes requested when the asynchronous read was initiated.

  ACE_Message_Block &message_block_;
  // Message block for reading the data into.

  ACE_HANDLE handle_;
  // I/O handle used for reading.
};

class ACE_Export ACE_WIN32_Asynch_Read_Stream : public virtual ACE_Asynch_Read_Stream_Impl,
                                                public ACE_WIN32_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a stream.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class.  An ACE_Asynch_Read_Stream::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_stream>
  //     callback.

public:
  ACE_WIN32_Asynch_Read_Stream (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.

  virtual ~ACE_WIN32_Asynch_Read_Stream (void);
  // Destructor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_read (ACE_WIN32_Asynch_Read_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Read_File class can use it too.
};

class ACE_Export ACE_WIN32_Asynch_Write_Stream_Result : public virtual ACE_Asynch_Write_Stream_Result_Impl,
                                                        public ACE_WIN32_Asynch_Result
{
  // = TITLE
  //
  //     This is class provides concrete implementation for
  //     ACE_Asynch_Write_Stream::Result class.
  //
  // = DESCRIPTION
  //

  friend class ACE_WIN32_Asynch_Write_Stream;
  // Factory class willl have special permissions.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  u_long bytes_to_write (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block (void) const;
  // Message block that contains the data to be written.

  ACE_HANDLE handle (void) const;
  // I/O handle used for writing.

  // Base class operations. These operations are here to kill some
  // warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Write_Stream_Result (ACE_Handler &handler,
                                        ACE_HANDLE handle,
                                        ACE_Message_Block &message_block,
                                        u_long bytes_to_write,
                                        const void* act,
                                        ACE_HANDLE event,
                                        int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Write_Stream factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // ACE_Proactor will call this method when the write completes.

  virtual ~ACE_WIN32_Asynch_Write_Stream_Result (void);
  // Destructor.

  u_long bytes_to_write_;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block_;
  // Message block that contains the data to be written.

  ACE_HANDLE handle_;
  // I/O handle used for writing.
};

class ACE_Export ACE_WIN32_Asynch_Write_Stream : public virtual ACE_Asynch_Write_Stream_Impl,
                                                 public ACE_WIN32_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous writes
  //     on a stream.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <writes>s can
  //     started using this class.  A ACE_Asynch_Write_Stream::Result
  //     will be passed back to the <handler> when the asynchronous
  //     write completes through the
  //     <ACE_Handler::handle_write_stream> callback.

public:
  ACE_WIN32_Asynch_Write_Stream (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be written from the <message_block>.

  virtual ~ACE_WIN32_Asynch_Write_Stream (void);
  // Destructor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_write (ACE_WIN32_Asynch_Write_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Write_File class can use it too.
};

class ACE_Export ACE_WIN32_Asynch_Read_File_Result : public virtual ACE_Asynch_Read_File_Result_Impl,
                                                     public ACE_WIN32_Asynch_Read_Stream_Result
{
  // = TITLE
  //
  //     This is class provides concrete implementation for
  //     ACE_Asynch_Read_File::Result class.
  //
  // = DESCRIPTION
  //

  friend class ACE_WIN32_Asynch_Read_File;
  // Factory class will have special permissions.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  // These methods belong to ACE_WIN32_Asynch_Result class base class.
  // These operations are here to kill some warnings. These methods
  // call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  // The following methods belong to
  // ACE_WIN32_Asynch_Read_Stream_Result. They are here to avoid VC++
  // warnings. These methods route their call to the
  // ACE_WIN32_Asynch_Read_Stream_Result base class.

  u_long bytes_to_read (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous read.

  ACE_Message_Block &message_block (void) const;
  // Message block which contains the read data.

  ACE_HANDLE handle (void) const;
  // I/O handle used for reading.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Read_File_Result (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block &message_block,
                                     u_long bytes_to_read,
                                     const void* act,
                                     u_long offset,
                                     u_long offset_high,
                                     ACE_HANDLE event,
                                     int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Read_File factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // ACE_Proactor will call this method when the read completes.

  virtual ~ACE_WIN32_Asynch_Read_File_Result (void);
  // Destructor.
};

class ACE_Export ACE_WIN32_Asynch_Read_File : public virtual ACE_Asynch_Read_File_Impl,
                                              public ACE_WIN32_Asynch_Read_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a file.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class.  A ACE_Asynch_Read_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_file>
  //     callback.
  //
  //     This class differs slightly from ACE_Asynch_Read_Stream as it
  //     allows the user to specify an offset for the read.

public:
  ACE_WIN32_Asynch_Read_File (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            u_long offset,
            u_long offset_high,
            const void *act,
            int priority);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.  The read will start
  // at <offset> from the beginning of the file.

  virtual ~ACE_WIN32_Asynch_Read_File (void);
  // Destructor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This method belongs to ACE_WIN32_Asynch_Read_Stream. It is here
  // to avoid the compiler warnings. We forward this call to the
  // ACE_WIN32_Asynch_Read_Stream class.
};

class ACE_Export ACE_WIN32_Asynch_Write_File_Result : public virtual ACE_Asynch_Write_File_Result_Impl,
                                                      public ACE_WIN32_Asynch_Write_Stream_Result
{
  // = TITLE
  //
  //     This class provides implementation for
  //      ACE_Asynch_Write_File_Result for WIN32 platforms.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous write.
  //
  //     This class differs slightly from
  //     ACE_Asynch_Write_Stream::Result as it calls back
  //     <ACE_Handler::handle_write_file> on the <handler> instead
  //     of <ACE_Handler::handle_write_stream>.  No additional state
  //     is required by this class as ACE_Asynch_Result can store
  //     the <offset>.

  friend class ACE_WIN32_Asynch_Write_File;
  // Factory class will have special permission.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  // Base class operations. These operations are here to kill some
  // warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  // The following methods belong to
  // ACE_WIN32_Asynch_Read_Stream_Result. They are here to avoid VC++
  // warnings. These methods route their call to the
  // ACE_WIN32_Asynch_Read_Stream_Result base class.

  u_long bytes_to_write (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block (void) const;
  // Message block that contains the data to be written.

  ACE_HANDLE handle (void) const;
  // I/O handle used for writing.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Write_File_Result (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block &message_block,
                                      u_long bytes_to_write,
                                      const void* act,
                                      u_long offset,
                                      u_long offset_high,
                                      ACE_HANDLE event,
                                      int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Write_File factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // ACE_Proactor will call this method when the write completes.

  virtual ~ACE_WIN32_Asynch_Write_File_Result (void);
  // Destructor.
};

class ACE_Export ACE_WIN32_Asynch_Write_File : public virtual ACE_Asynch_Write_File_Impl,
                                               public ACE_WIN32_Asynch_Write_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous writes
  //     on a file.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <write>s can be
  //     started using this class.  A ACE_Asynch_Write_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     writes completes through the <ACE_Handler::handle_write_file>
  //     callback.

public:
  ACE_WIN32_Asynch_Write_File (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             u_long offset,
             u_long offset_high,
             const void *act,
             int priority);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be write and stored in the <message_block>.  The write will
  // start at <offset> from the beginning of the file.

  virtual ~ACE_WIN32_Asynch_Write_File (void);
  // Destrcutor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This method belongs to ACE_WIN32_Asynch_Write_Stream. It is here
  // to avoid compiler warnings. This method is forwarded to the
  // ACE_WIN32_Asynch_Write_Stream class.
};

class ACE_Export ACE_WIN32_Asynch_Accept_Result : public virtual ACE_Asynch_Accept_Result_Impl,
                                                  public ACE_WIN32_Asynch_Result
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Accept::Result for WIN32
  //     platform.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous accept.

  friend class ACE_WIN32_Asynch_Accept;
  // Factory will have special permission.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  u_long bytes_to_read (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous accept.

  ACE_Message_Block &message_block (void) const;
  // Message block which contains the read data.

  ACE_HANDLE listen_handle (void) const;
  // I/O handle used for accepting new connections.

  ACE_HANDLE accept_handle (void) const;
  // I/O handle for the new connection.

  // Base class operations. These operations are here to kill some
  // warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Accept_Result (ACE_Handler &handler,
                                  ACE_HANDLE listen_handle,
                                  ACE_HANDLE accept_handle,
                                  ACE_Message_Block &message_block,
                                  u_long bytes_to_read,
                                  const void* act,
                                  ACE_HANDLE event,
                                  int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Accept factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // ACE_Proactor will call this method when the accept completes.

  virtual ~ACE_WIN32_Asynch_Accept_Result (void);
  // Destructor.

  u_long bytes_to_read_;
  // Bytes requested when the asynchronous read was initiated.

  ACE_Message_Block &message_block_;
  // Message block for reading the data into.

  ACE_HANDLE listen_handle_;
  // I/O handle used for accepting new connections.

  ACE_HANDLE accept_handle_;
  // I/O handle for the new connection.
};

class ACE_Export ACE_WIN32_Asynch_Accept : public virtual ACE_Asynch_Accept_Impl,
                                           public ACE_WIN32_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous accepts
  //     on a listen handle.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <accept>s can
  //     started using this class.  A ACE_Asynch_Accept::Result will
  //     be passed back to the <handler> when the asynchronous accept
  //     completes through the <ACE_Handler::handle_accept>
  //     callback.

public:
  ACE_WIN32_Asynch_Accept (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int accept (ACE_Message_Block &message_block,
              u_long bytes_to_read,
              ACE_HANDLE accept_handle,
              const void *act,
              int priority);
  // This starts off an asynchronous accept.  The asynchronous accept
  // call also allows any initial data to be returned to the
  // <handler>.  Upto <bytes_to_read> will be read and stored in the
  // <message_block>.  The <accept_handle> will be used for the
  // <accept> call.  If (<accept_handle> == INVALID_HANDLE), a new
  // handle will be created.
  //
  // <message_block> must be specified. This is because the address of
  // the new connection is placed at the end of this buffer.

  ~ACE_WIN32_Asynch_Accept (void);
  // Destructor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.
};

class ACE_Export ACE_WIN32_Asynch_Transmit_File_Result : public virtual ACE_Asynch_Transmit_File_Result_Impl,
                                                         public ACE_WIN32_Asynch_Result
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Transmit_File::Result for
  //     WIN32 platforms.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous transmit file.

  friend class ACE_WIN32_Asynch_Transmit_File;
  // Factory class will have special permission.

  friend class ACE_WIN32_Proactor;
  // Proactor class has special permission.

public:
  ACE_HANDLE socket (void) const;
  // Socket used for transmitting the file.

  ACE_HANDLE file (void) const;
  // File from which the data is read.

  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer (void) const;
  // Header and trailer data associated with this transmit file.

  u_long bytes_to_write (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous transmit file.

  u_long bytes_per_send (void) const;
  // Number of bytes per send requested at the start of the transmit
  // file.

  u_long flags (void) const;
  // Flags which were passed into transmit file.

  // Base class operations. These operations are here to kill some
  // warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This returns the ACT associated with the handle when it was
  // registered with the I/O completion port.  This ACT is not the
  // same as the ACT associated with the asynchronous operation.

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // Event associated with the OVERLAPPED structure.

  u_long offset (void) const;
  // This really make sense only when doing file I/O.

  u_long offset_high (void) const;
  // Offset_high associated with the OVERLAPPED structure.

  int priority (void) const;
  // The priority of the asynchronous operation. Currently, this is
  // not supported on Win32.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor's completion port.

protected:
  ACE_WIN32_Asynch_Transmit_File_Result (ACE_Handler &handler,
					 ACE_HANDLE socket,
					 ACE_HANDLE file,
					 ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
					 u_long bytes_to_write,
					 u_long offset,
					 u_long offset_high,
					 u_long bytes_per_send,
					 u_long flags,
					 const void *act,
					 ACE_HANDLE event,
					 int priority);
  // Constructor is protected since creation is limited to
  // ACE_Asynch_Transmit_File factory.

  virtual void complete (u_long bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error);
  // Proactor will call this method when the write completes.

  virtual ~ACE_WIN32_Asynch_Transmit_File_Result (void);
  // Destructor.

  ACE_HANDLE socket_;
  // Network I/O handle.

  ACE_HANDLE file_;
  // File I/O handle.

  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer_;
  // Header and trailer data associated with this transmit file.

  u_long bytes_to_write_;
  // The number of bytes which were requested at the start of the
  // asynchronous transmit file.

  u_long bytes_per_send_;
  // Number of bytes per send requested at the start of the transmit
  // file.

  u_long flags_;
  // Flags which were passed into transmit file.
};

class ACE_Export ACE_WIN32_Asynch_Transmit_File : public virtual ACE_Asynch_Transmit_File_Impl,
                                                  public ACE_WIN32_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous
  //     transmit files on a stream.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <transmit_file>s
  //     can started using this class.  A
  //     ACE_Asynch_Transmit_File::Result will be passed back to the
  //     <handler> when the asynchronous transmit file completes
  //     through the <ACE_Handler::handle_transmit_file> callback.
  //
  //     The transmit_file function transmits file data over a
  //     connected network connection. The function uses the operating
  //     system's cache manager to retrieve the file data. This
  //     function provides high-performance file data transfer over
  //     network connections.  This function would be of great use in
  //     a Web Server, Image Server, etc.

public:
  ACE_WIN32_Asynch_Transmit_File (ACE_WIN32_Proactor *win32_proactor);
  // Constructor.

  int transmit_file (ACE_HANDLE file,
                     ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                     u_long bytes_to_write,
                     u_long offset,
                     u_long offset_high,
                     u_long bytes_per_send,
                     u_long flags,
                     const void *act,
                     int priority);
  // This starts off an asynchronous transmit file. The <file> is a
  // handle to an open file.  <header_and_trailer> is a pointer to a
  // data structure that contains pointers to data to send before and
  // after the file data is sent.  Set this parameter to 0 if you only
  // want to transmit the file data.  Upto <bytes_to_write> will be
  // written to the <socket>.  If you want to send the entire file,
  // let <bytes_to_write> = 0.  <bytes_per_send> is the size of each
  // block of data sent per send operation.  Please read the Win32
  // documentation on what the flags should be.

  ~ACE_WIN32_Asynch_Transmit_File (void);
  // Destructor.

  // Methods belong to ACE_WIN32_Asynch_Operation base class. These
  // methods are defined here to avoid VC++ warnings. They route the
  // call to the ACE_WIN32_Asynch_Operation base class.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel asynchronous
  // operations issued by other threads.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.
};

#if defined (__ACE_INLINE__)
#include "ace/WIN32_Asynch_IO.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
#endif /* ACE_WIN32_ASYNCH_IO_H */
