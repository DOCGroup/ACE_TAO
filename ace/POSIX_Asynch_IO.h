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
//    POSIX_Asynch_IO.h
//
// = DESCRIPTION
//
//    The implementation classes for POSIX implementation of Asynch
//    Operations are defined here in this file.
//
// = AUTHOR
//
//    Irfan Pyarali (irfan@cs.wustl.edu),
//    Tim Harrison (harrison@cs.wustl.edu) and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_POSIX_ASYNCH_IO_H)
#define ACE_POSIX_ASYNCH_IO_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS)

#include "ace/Asynch_IO_Impl.h"
#include "ace/Reactor.h"

// Forward declarations
class ACE_POSIX_SIG_Proactor;
class ACE_POSIX_AIOCB_Proactor;

class ACE_Export ACE_POSIX_Asynch_Result : public virtual ACE_Asynch_Result_Impl,
                                           public aiocb
{
  // = TITLE
  //
  //     This class provides concrete implementation for
  //     ACE_Asynch_Result for POSIX4 platforms. This class extends
  //     <aiocb> and makes it more useful.
  //
  // = DESCRIPTION
  //
public:
  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE on POSIX4 platforms.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // Priority of the operation.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

  virtual ~ACE_POSIX_Asynch_Result (void);
  // Destructor.

protected:
  ACE_POSIX_Asynch_Result (ACE_Handler &handler,
                           const void* act,
                           ACE_HANDLE event,
                           u_long offset,
                           u_long offset_high,
                           int priority);
  // Constructor. <Event> is not used on POSIX.

  ACE_Handler &handler_;
  // Handler that will be called back.

  const void *act_;
  // ACT for this operation.
  // We could use <aiocb::aio_sigevent.sigev_value.sival_ptr> for
  // this. But it doesnot provide the constness, so this may be
  // better.

  // aiocb::aio_resultp.aio_return
  // Bytes transferred by this operation.

  int success_;
  // Success indicator.

  const void *completion_key_;
  // ACT associated with handle.

  // aiocb::aio_resultp.aio_errno
  // Error if operation failed.
};

class ACE_Export ACE_POSIX_Asynch_Operation : public virtual ACE_Asynch_Operation_Impl
{
  // = TITLE
  //
  //     This class abstracts out the common things needed for
  //     implementing Asynch_Operation for POSIX platforms. Specific
  //     implementations such as POSIX_AIOCB_Asynch_Operation and
  //     POSIX_SIG_Asynch_Operation etc., can derive from this class.
  //
  //  = DESCRIPTION
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
  //
  // @@ Not implemented. Returns 0.

  // = Access methods.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  ACE_POSIX_Asynch_Operation (void);
  // No op contructor.

  virtual ~ACE_POSIX_Asynch_Operation (void);
  // Destructor.

  ACE_Proactor *proactor_;
  // Proactor that this Asynch IO will be registered with.

  ACE_Handler *handler_;
  // Handler that will receive the callback.

  ACE_HANDLE handle_;
  // I/O handle used for reading.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Operation : public virtual ACE_POSIX_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Operation for AIOCB
  //     (Asynchronous I/O Control Blocks) based implementation of
  //     Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_AIOCB_Proactor *posix_proactor (void) const;
  // Return the underlying Proactor implementation.

protected:
  ACE_POSIX_AIOCB_Asynch_Operation (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Contructor.

  virtual ~ACE_POSIX_AIOCB_Asynch_Operation (void);
  // Destructor.

  int register_aio_with_proactor (ACE_POSIX_Asynch_Result *result);
  // This call is for the POSIX implementation. This method is used by
  // <ACE_Asynch_Operation> to store some information with the
  // Proactor after an <aio_> call is issued, so that the Proactor can
  // retrive this information to do <aio_return> and <aio_error>.
  // Passing a '0' ptr returns the status, indicating whether there
  // are slots available or no. Passing a valid ptr stores the ptr
  // with the Proactor.

  ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor_;
  // It is easy to get this specific implementation proactor here,
  // since it is the one that creates the correct POSIX_Asynch_*
  // objects. We can use this to get to the implementation proactor
  // directly.
};

class ACE_Export ACE_POSIX_SIG_Asynch_Operation : public virtual ACE_POSIX_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Operation for Real-Time
  //     Signal (<sigtimedwait>) based implementation of Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Proactor *posix_proactor (void) const;
  // Return the  underlying Proactor implemetation.

protected:
  ACE_POSIX_SIG_Asynch_Operation (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
  // Contructor.

  virtual ~ACE_POSIX_SIG_Asynch_Operation (void);
  // Destructor.

  ACE_POSIX_SIG_Proactor *posix_sig_proactor_;
  // It is easy to get this specific implementation proactor here,
  // since it is the one that creates the correct POSIX_Asynch_*
  // objects. We can use this to get to the implementation proactor
  // directly.
};

class ACE_Export ACE_POSIX_Asynch_Read_Stream_Result : public virtual ACE_Asynch_Read_Stream_Result_Impl,
                                                       public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //
  //     This class provides concrete implementation for
  //     ACE_Asynch_Read_Stream::Result class for POSIX platforms.
  //
  // = DESCRIPTION
  //

  friend class ACE_POSIX_AIOCB_Asynch_Read_Stream;
  friend class ACE_POSIX_SIG_Asynch_Read_Stream;
  // Factory classes willl have special permissions.
  
  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

public:
  u_long bytes_to_read (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous read.

  ACE_Message_Block &message_block (void) const;
  // Message block which contains the read data.

  ACE_HANDLE handle (void) const;
  // I/O handle used for reading.
  
  // = Base class operations. These operations are here to kill
  //   dominance warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // The priority of the asynchronous operation.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

protected:
  ACE_POSIX_Asynch_Read_Stream_Result (ACE_Handler &handler,
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
  // Get the data copied to this class, before calling application
  // handler.

  virtual ~ACE_POSIX_Asynch_Read_Stream_Result (void);
  // Destrcutor.

  // aiocb::aio_nbytes
  // Bytes requested when the asynchronous read was initiated.

  ACE_Message_Block &message_block_;
  // Message block for reading the data into.

  // aiocb::aio_filedes
  // I/O handle used for reading.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Read_Stream : public virtual ACE_Asynch_Read_Stream_Impl,
                                                      public ACE_POSIX_AIOCB_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Read_Stream for AIOCB
  //     (Asynchronous I/O Control Blocks) based implementation of
  //      Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_AIOCB_Asynch_Read_Stream (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Constructor.

  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.

  virtual ~ACE_POSIX_AIOCB_Asynch_Read_Stream (void);
  // Destructor.
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_read (ACE_POSIX_Asynch_Read_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Read_File class can use it too.
};


class ACE_Export ACE_POSIX_SIG_Asynch_Read_Stream : public virtual ACE_Asynch_Read_Stream_Impl,
                                                    public ACE_POSIX_SIG_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Read_Stream for Real-Time
  //     Signal (<sigtimedwait>) based implementation of Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Asynch_Read_Stream (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
  // Constructor.

  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.

  virtual ~ACE_POSIX_SIG_Asynch_Read_Stream (void);
  // Destructor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_read (ACE_POSIX_Asynch_Read_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Read_File class can use it too.
};

class ACE_Export ACE_POSIX_Asynch_Write_Stream_Result : public virtual ACE_Asynch_Write_Stream_Result_Impl,
                                                        public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //
  //     This class provides concrete implementation for
  //     ACE_Asynch_Write_Stream::Result on POSIX platforms.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous write.

  friend class ACE_POSIX_AIOCB_Asynch_Write_Stream;
  friend class ACE_POSIX_SIG_Asynch_Write_Stream;
  // Factory classes will have special privilages.

  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

public:
  u_long bytes_to_write (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block (void) const;
  // Message block that contains the data to be written.

  ACE_HANDLE handle (void) const;
  // I/O handle used for writing.
  
  // = Base class operations. These operations are here to kill
  //   dominance warnings. These methods call the base class methods. 

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE on POSIX4 platforms.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // The priority of the asynchronous operation.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

protected:
  ACE_POSIX_Asynch_Write_Stream_Result (ACE_Handler &handler,
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

  virtual ~ACE_POSIX_Asynch_Write_Stream_Result (void);
  // Destructor.

protected:
  // aiocb::aio_nbytes
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block_;
  // Message block that contains the data to be written.

  // aiocb::aio_filedes
  // I/O handle used for writing.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Write_Stream : public virtual ACE_Asynch_Write_Stream_Impl,
                                                       public ACE_POSIX_AIOCB_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Write_Stream for AIOCB
  //     (Asynchronous I/O Control Blocks) based implementation of
  //      Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_AIOCB_Asynch_Write_Stream (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Constructor.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be written from the <message_block>.

  virtual ~ACE_POSIX_AIOCB_Asynch_Write_Stream (void);
  // Destrcutor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_write (ACE_POSIX_Asynch_Write_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Write_File class can use it too.
};

class ACE_Export ACE_POSIX_SIG_Asynch_Write_Stream : public virtual ACE_Asynch_Write_Stream_Impl,
                                                     public ACE_POSIX_SIG_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Write_Stream for Real-Time
  //     Signal (<sigtimedwait>) based implementation of Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Asynch_Write_Stream (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
  // Constrctor.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be written from the <message_block>.

  virtual ~ACE_POSIX_SIG_Asynch_Write_Stream (void);
  // Destructor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

protected:
  int shared_write (ACE_POSIX_Asynch_Write_Stream_Result *result);
  // This is the method which does the real work and is there so that
  // the ACE_Asynch_Write_File class can use it too.
};

class ACE_Export ACE_POSIX_Asynch_Read_File_Result : public virtual ACE_Asynch_Read_File_Result_Impl,
                                                     public ACE_POSIX_Asynch_Read_Stream_Result
{
  // = TITLE
  //
  //     This class provides concrete implementation for
  //     ACE_Asynch_Read_File::Result class for POSIX platforms.
  //
  // = DESCRIPTION
  //

  friend class ACE_POSIX_AIOCB_Asynch_Read_File;
  friend class ACE_POSIX_SIG_Asynch_Read_File;
  // Factory classes willl have special permissions.

  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

public:
  // = These methods belong to ACE_POSIX_Asynch_Result class base
  //   class. These operations are here to kill dominance
  //   warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE on POSIX4 platforms.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // The priority of the asynchronous operation.

  // = The following methods belong to
  //   ACE_POSIX_Asynch_Read_Stream_Result. They are here to avoid 
  //   dominance warnings. These methods route their call to the
  //   ACE_POSIX_Asynch_Read_Stream_Result base class.

  u_long bytes_to_read (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous read.

  ACE_Message_Block &message_block (void) const;
  // Message block which contains the read data.

  ACE_HANDLE handle (void) const;
  // I/O handle used for reading.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

protected:
  ACE_POSIX_Asynch_Read_File_Result (ACE_Handler &handler,
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

  virtual ~ACE_POSIX_Asynch_Read_File_Result (void);
  // Destructor.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Read_File : public virtual ACE_Asynch_Read_File_Impl,
                                                    public ACE_POSIX_AIOCB_Asynch_Read_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a file. This class implements ACE_Asynch_Read_File for
  //     AIOCB (Asynchronous I/O Control Blocks) based implementation
  //     of Proactor.
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
  ACE_POSIX_AIOCB_Asynch_Read_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
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

  virtual ~ACE_POSIX_AIOCB_Asynch_Read_File (void);
  // Destructor.
  
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This belongs to ACE_POSIX_AIOCB_Asynch_Read_Stream. We have
  // defined this here to avoid compiler warnings and forward the
  // method to <ACE_POSIX_AIOCB_Asynch_Read_Stream::read>.
};

class ACE_Export ACE_POSIX_SIG_Asynch_Read_File : public virtual ACE_Asynch_Read_File_Impl,
                                                  public ACE_POSIX_SIG_Asynch_Read_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a file. This class implements ACE_Asynch_Operation for
  //     Real-Time Signal (<sigtimedwait>) based implementation of
  //     Proactor.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class.  A ACE_Asynch_Read_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_file>
  //     callback.

public:
  ACE_POSIX_SIG_Asynch_Read_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
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

  virtual ~ACE_POSIX_SIG_Asynch_Read_File (void);
  // Destructor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act,
            int priority);
  // This belongs to ACE_POSIX_SIG_Asynch_Read_Stream. We have
  // defined this here to avoid compiler warnings and forward the
  // method to <ACE_POSIX_SIG_Asynch_Read_Stream::read>.
};

class ACE_Export ACE_POSIX_Asynch_Write_File_Result : public virtual ACE_Asynch_Write_File_Result_Impl,
                                                      public ACE_POSIX_Asynch_Write_Stream_Result
{
  // = TITLE
  //
  //     This class provides implementation for
  //      ACE_Asynch_Write_File_Result for POSIX platforms.
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

  friend class ACE_POSIX_AIOCB_Asynch_Write_File;
  friend class ACE_POSIX_SIG_Asynch_Write_File;
  // Factory classes will have special permissions.

  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

public:
  // = Base class operations. These operations are here to kill some
  //   warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE on POSIX4 platforms.
  
  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //
  
  int priority (void) const;
  // The priority of the asynchronous operation.

  // = The following methods belong to
  //   ACE_POSIX_Asynch_Write_Stream_Result. They are here to avoid
  //   dominace warnings. These methods route their call to the 
  //   ACE_POSIX_Asynch_Write_Stream_Result base class. 

  u_long bytes_to_write (void) const;
  // The number of bytes which were requested at the start of the
  // asynchronous write.

  ACE_Message_Block &message_block (void) const;
  // Message block that contains the data to be written.

  ACE_HANDLE handle (void) const;
  // I/O handle used for writing.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.
  
protected:
  ACE_POSIX_Asynch_Write_File_Result (ACE_Handler &handler,
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

  virtual ~ACE_POSIX_Asynch_Write_File_Result (void);
  // Destructor.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Write_File : public virtual ACE_Asynch_Write_File_Impl,
                                                     public ACE_POSIX_AIOCB_Asynch_Write_Stream
{
  // = TITLE
  //
  //     This class provides concrete implementation for
  //     ACE_Asynch_Write_File for POSIX platforms where the
  //     completion strategy for Proactor is based on AIOCB (AIO
  //     Control Blocks).
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_AIOCB_Asynch_Write_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
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

  virtual ~ACE_POSIX_AIOCB_Asynch_Write_File (void);
  // Destructor.
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This <write> belongs to ACE_POSIX_SIG_Asynch_Write_Stream. We
  // have put this here to avoid compiler warnings. We forward this
  // method call to the <ACE_POSIX_SIG_Asynch_Write_Stream::write>
  // one.
};

class ACE_Export ACE_POSIX_SIG_Asynch_Write_File : public virtual ACE_Asynch_Write_File_Impl,
                                                   public ACE_POSIX_SIG_Asynch_Write_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a file. This class implements ACE_Asynch_Operation for
  //     Real-Time Signal (<sigtimedwait>) based implementation of
  //     Proactor.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class.  A ACE_Asynch_Read_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_file>
  //     callback.

public:
  ACE_POSIX_SIG_Asynch_Write_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
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

  virtual ~ACE_POSIX_SIG_Asynch_Write_File (void);
  // Destrcutor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act,
             int priority);
  // This <write> belongs to ACE_POSIX_SIG_Asynch_Write_Stream. We
  // have put this here to avoid compiler warnings. We forward this
  // method call to the <ACE_POSIX_SIG_Asynch_Write_Stream::write>
  // one.
};

class ACE_Export ACE_POSIX_Asynch_Accept_Result : public virtual ACE_Asynch_Accept_Result_Impl,
                                                  public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //
  //     This is that class which will be passed back to the
  //     <handler> when the asynchronous accept completes.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous accept.

  friend class ACE_POSIX_AIOCB_Asynch_Accept;
  friend class ACE_POSIX_SIG_Asynch_Accept;
  // Factory classes willl have special permissions.

  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

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

  // = Base class operations. These operations are here to kill
  //   dominance warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE on POSIX4 platforms.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // The priority of the asynchronous operation. 

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

protected:
  ACE_POSIX_Asynch_Accept_Result (ACE_Handler &handler,
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

  virtual ~ACE_POSIX_Asynch_Accept_Result (void);
  // Destructor.

  // aiocb::aio_nbytes
  // Bytes requested when the asynchronous read was initiated.
  // Actually, on POSIX implementation, we dont read any intial data.

  ACE_Message_Block &message_block_;
  // Message block for reading the data into.

  ACE_HANDLE listen_handle_;
  // I/O handle used for accepting new connections.

  // aiocb::aio_filedes
  // I/O handle for the new connection.
};

class ACE_POSIX_AIOCB_Asynch_Accept_Handler;
// Forward declaration. This class is defined the in the cpp file,
// since this is internal  to the implementation.

class ACE_Export ACE_POSIX_AIOCB_Asynch_Accept : public virtual ACE_Asynch_Accept_Impl,
                                                 public ACE_POSIX_AIOCB_Asynch_Operation
{
public:
  ACE_POSIX_AIOCB_Asynch_Accept (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Constructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // This <open> belongs to ACE_AIOCB_Asynch_Operation. We forward
  // this call to that method. We have put this here to avoid the
  // compiler warnings.

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

  virtual ~ACE_POSIX_AIOCB_Asynch_Accept (void);
  // Destructor.

  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.
  
private:
  static void* thread_function  (void* reactor);
  // The thread function that does handle events.

  ACE_Reactor reactor_;
  // Reactor to wait on the <listen_handle>.

  ACE_POSIX_AIOCB_Asynch_Accept_Handler* accept_handler_;
  // The Event Handler to do handle_input.
};

class ACE_POSIX_SIG_Asynch_Accept_Handler;
// Forward declaration. This class is defined the in the cpp file,
// since this is internal  to the implementation.

class ACE_Export ACE_POSIX_SIG_Asynch_Accept : public virtual ACE_Asynch_Accept_Impl,
                                               public ACE_POSIX_SIG_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Accept for Real-Time
  //     Signal (<sigtimedwait>) based implementation of Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Asynch_Accept (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
  // Constructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // This <open> belongs to ACE_SIG_Asynch_Operation. We forward this
  // call to that method. We have put this here to avoid the compiler
  // warnings.

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

  virtual ~ACE_POSIX_SIG_Asynch_Accept (void);
  // Destructor.
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class.
 
  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

private:
  static void* thread_function (void* reactor);
  // The thread function that does handle events.

  ACE_Reactor reactor_;
  // Reactor to wait on the <listen_handle>.

  ACE_POSIX_SIG_Asynch_Accept_Handler* accept_handler_;
  // The Event Handler to do handle_input.
};

class ACE_Export ACE_POSIX_Asynch_Transmit_File_Result : public virtual ACE_Asynch_Transmit_File_Result_Impl,
                                                         public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //
  //     This is that class which will be passed back to the
  //     <handler> when the asynchronous transmit file completes.
  //
  // = DESCRIPTION
  //
  //     This class has all the information necessary for the
  //     <handler> to uniquiely identify the completion of the
  //     asynchronous transmit file.

  friend class ACE_POSIX_AIOCB_Asynch_Transmit_File;
  friend class ACE_POSIX_SIG_Asynch_Transmit_File;
  // Factory classes willl have special permissions.

  friend class ACE_POSIX_Asynch_Transmit_Handler;
  friend class ACE_POSIX_AIOCB_Asynch_Transmit_Handler;
  friend class ACE_POSIX_SIG_Asynch_Transmit_Handler;
  // Handlers do all the job.

  friend class ACE_POSIX_Proactor;
  // The Proactor constructs the Result class for faking results.

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

  // = Base class operations. These operations are here to kill
  //   dominance warnings. These methods call the base class methods.

  u_long bytes_transferred (void) const;
  // Number of bytes transferred by the operation.

  const void *act (void) const;
  // ACT associated with the operation.

  int success (void) const;
  // Did the operation succeed?

  const void *completion_key (void) const;
  // This is the ACT associated with the handle on which the
  // Asynch_Operation takes place.
  //
  // @@ This is not implemented for POSIX4 platforms.
  //

  u_long error (void) const;
  // Error value if the operation fail.

  ACE_HANDLE event (void) const;
  // This returns ACE_INVALID_HANDLE.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using
  //     aiocb64.
  //

  int priority (void) const;
  // The priority of the asynchronous operation.

  int post_completion (ACE_Proactor_Impl *proactor);
  // Post <this> to the Proactor.

protected:
  ACE_POSIX_Asynch_Transmit_File_Result (ACE_Handler &handler,
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
  // ACE_Proactor will call this method when the write completes.

  virtual ~ACE_POSIX_Asynch_Transmit_File_Result (void);
  // Destructor.

  ACE_HANDLE socket_;
  // Network I/O handle.

  // aiocb::aio_filedes
  // File I/O handle.

  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer_;
  // Header and trailer data associated with this transmit file.

  // aiocb::aio_nbytes
  // The number of bytes which were requested at the start of the
  // asynchronous transmit file.

  u_long bytes_per_send_;
  // Number of bytes per send requested at the start of the transmit
  // file.

  u_long flags_;
  // Flags which were passed into transmit file.
};

class ACE_Export ACE_POSIX_AIOCB_Asynch_Transmit_File : public virtual ACE_Asynch_Transmit_File_Impl,
                                                        public ACE_POSIX_AIOCB_Asynch_Operation
{
  // = DESCRIPTION
  //     Implementation for transmit_file will make use of
  //     POSIX_AIOCB_Asynch_Transmit_Handler.
public:
  ACE_POSIX_AIOCB_Asynch_Transmit_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
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
  // block of data sent per send operation. Please read the POSIX
  // documentation on what the flags should be.

  virtual ~ACE_POSIX_AIOCB_Asynch_Transmit_File (void);
  // Destructor.
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.
};

class ACE_Export ACE_POSIX_SIG_Asynch_Transmit_File : public virtual ACE_Asynch_Transmit_File_Impl,
                                                      public ACE_POSIX_SIG_Asynch_Operation
{
  // = TITLE
  //
  //     This class implements ACE_Asynch_Transmit_File for Real-Time
  //     Signal (<sigtimedwait>) based implementation of Proactor.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Asynch_Transmit_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor);
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
  // block of data sent per send operation.

  virtual ~ACE_POSIX_SIG_Asynch_Transmit_File (void);
  // Destructor.
  
  // = Methods belong to ACE_POSIX_Asynch_Operation base class. These  
  //   methods are defined here to avoid dominace warnings. They route 
  //   the call to the ACE_POSIX_Asynch_Operation base class. 

  int open (ACE_Handler &handler,
            ACE_HANDLE handle,
            const void *completion_key,
            ACE_Proactor *proactor);
  // Initializes the factory with information which will be used with
  // each asynchronous call.  If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int cancel (void);
  //
  // @@ Not implemented. Returns 0.
  
  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.
};

#if defined (__ACE_INLINE__)
#include "ace/POSIX_Asynch_IO.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_ASYNCH_IO_H */
