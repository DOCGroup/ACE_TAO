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
//    Asynch_IO.h
//
// = DESCRIPTION
//
//    This works on Win32 (#if defined (ACE_WIN32) && !defined
//    (ACE_HAS_WINCE)) platforms and on POSIX4 platforms with <aio_*>
//    routines (#if defined (ACE_HAS_AIO_CALLS))
//
//    On Win32 platforms, the implementation of
//    <ACE_Asynch_Transmit_File> and <ACE_Asynch_Accept> are only
//    supported if ACE_HAS_WINSOCK2 is defined or you are on WinNT 4.0
//    or higher. 
//
// = AUTHOR
//
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    Tim Harrison <harrison@cs.wustl.edu> and
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_ASYNCH_IO_H)
#define ACE_ASYNCH_IO_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))

// Forward declarations
class ACE_Proactor;
class ACE_Handler;
class ACE_Message_Block;
class ACE_INET_Addr;

// Forward declarations
class ACE_Asynch_Result_Impl;

class ACE_Export ACE_Asynch_Result
{
  // = TITLE
  //
  //     An interface base class which allows users access to common
  //     information related to an asynchronous operation.
  //
  // = DESCRIPTION
  // 
  //     An interface base class from which you can obtain some basic
  //     information like the number of bytes transferred, the ACT
  //     associated with the asynchronous operation, indication of
  //     success or failure, etc. Subclasses may want to store more
  //     information that is particular to the asynchronous operation
  //     it represents.

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
  // On WIN32, this returns the ACT associated with the handle when it
  // was registered with the I/O completion port.
  //
  // @@ This is not implemented for POSIX4 platforms. Returns 0.

  u_long error (void) const;
  // Error value if the operation fails.

  ACE_HANDLE event (void) const;
  // On WIN32, this returns the event associated with the OVERLAPPED
  // structure. 
  //
  // This returns ACE_INVALID_HANDLE on POSIX4-Unix platforms.

  u_long offset (void) const;
  u_long offset_high (void) const;
  // This really make sense only when doing file I/O. 
  // 
  // On WIN32, these are represented in the OVERLAPPED datastructure.
  //
  // @@ On POSIX4-Unix, offset_high should be supported using aiocb64.

  int priority (void) const;
  // Priority of the operation. 
  //
  // On POSIX4-Unix, this is supported. Works like <nice> in
  // Unix. Negative values are not allowed. 0 means priority of the
  // operation same as the process priority. 1 means priority of the
  // operation is one less than process. And so forth.
  //
  // On Win32, this is a no-op.
  
  virtual ~ACE_Asynch_Result (void);
  // Destructor.

protected:
  ACE_Asynch_Result (ACE_Asynch_Result_Impl *implementation);
  // Constructor. This implementation will not be deleted.  The
  // implementation will be deleted by the Proactor.

  ACE_Asynch_Result_Impl *implementation (void) const;
  // Get the implementation class.
  
  ACE_Asynch_Result_Impl *implementation_;
  // Implementation class.
};

// Forward declarations
class ACE_Asynch_Operation_Impl;

class ACE_Export ACE_Asynch_Operation
{
  // = TITLE
  //
  //     This is an interface base class for all asynch
  //     operations. The resposiblility of this class is to forward
  //     all methods to its delegation/implementation class, e.g.,
  //     <ACE_WIN32_Asynch_Operation> or <ACE_POSIX_Asynch_Operation>.
  //
  // = DESCRIPTION
  //
  //     There are some attributes and functionality which is common
  //     to all asychronous operations. The delegation classes of this
  //     class will factor out this code.

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
  // On Win32, this cancels all pending accepts operations that were
  // issued by the calling thread.  The function does not cancel
  // asynchronous operations issued by other threads.
  //
  // @@ POSIX: please implement me.

  // = Access methods.

  ACE_Proactor* proactor (void) const;
  // Return the underlying proactor.

  virtual ~ACE_Asynch_Operation (void);
  // Destructor.

protected:
  ACE_Asynch_Operation (void);
  // Constructor.

  ACE_Asynch_Operation_Impl *implementation (void) const;
  // Return the underlying implementation class.

  void implementation (ACE_Asynch_Operation_Impl *implementation);
  // Set the implementation class.
  
  ACE_Proactor *get_proactor (ACE_Proactor *user_proactor,
                              ACE_Handler &handler) const;
  // Get a proactor for/from the user

  ACE_Asynch_Operation_Impl *implementation_;
  // Implementation class.
};

// Forward declarations
class ACE_Asynch_Read_Stream_Result_Impl;
class ACE_Asynch_Read_Stream_Impl;

class ACE_Export ACE_Asynch_Read_Stream : public ACE_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a stream. This class forwards all methods to its
  //     implementation class.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class.  An ACE_Asynch_Read_Stream::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_stream>
  //     callback.

public:
  ACE_Asynch_Read_Stream (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Read_Stream (void);
  // Destructor

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            const void *act = 0,
            int priority = 0);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.

  ACE_Asynch_Read_Stream_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Read_Stream_Impl *implementation);
  // Set the implementation class.

  ACE_Asynch_Read_Stream_Impl *implementation_;
  // Implementation class that all methods will be forwarded to.

public:
  class ACE_Export Result : public ACE_Asynch_Result
  {
    // = TITLE
    // 
    //     This is the class which will be passed back to the
    //     <handler> when the asynchronous read completes. This class
    //     forwards all the methods to the implementation classes.
    //
    // = DESCRIPTION
    // 
    //     This class has all the information necessary for the
    //     <handler> to uniquiely identify the completion of the
    //     asynchronous read.
    
    friend class ACE_POSIX_Asynch_Read_Stream_Result;
    friend class ACE_WIN32_Asynch_Read_Stream_Result;
    // The concrete implementation result classes only construct this
    // class.

  public:
    u_long bytes_to_read (void) const;
    // The number of bytes which were requested at the start of the
    // asynchronous read.

    ACE_Message_Block &message_block (void) const;
    // Message block which contains the read data.

    ACE_HANDLE handle (void) const;
    // I/O handle used for reading.

    ACE_Asynch_Read_Stream_Result_Impl *implementation (void) const;
    // Get the implementation class.
    
  protected:
    Result (ACE_Asynch_Read_Stream_Result_Impl *implementation);
    // Constructor.
    
    virtual ~Result (void);
    // Destructor.

    ACE_Asynch_Read_Stream_Result_Impl *implementation_;
    // The implementation class.
  };
};

// Forward declarations
class ACE_Asynch_Write_Stream_Impl;
class ACE_Asynch_Write_Stream_Result_Impl;

class ACE_Export ACE_Asynch_Write_Stream : public ACE_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous writes
  //     on a stream. This class forwards all methods to its
  //     implementation class.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <writes>s can
  //     started using this class.  An ACE_Asynch_Write_Stream::Result
  //     will be passed back to the <handler> when the asynchronous
  //     write completes through the
  //     <ACE_Handler::handle_write_stream> callback.

public:
  ACE_Asynch_Write_Stream (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Write_Stream (void);
  // Destructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             const void *act = 0,
             int priority = 0);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be written from the <message_block>. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.

  ACE_Asynch_Write_Stream_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Write_Stream_Impl *implementation);
  // Set the implementation class.

  ACE_Asynch_Write_Stream_Impl *implementation_;
  // Implementation class that all methods will be forwarded to.

public:
  class  ACE_Export Result : public ACE_Asynch_Result
  {
    // = TITLE
    //
    //     This is that class which will be passed back to the
    //     <handler> when the asynchronous write completes. This class
    //     forwards all the methods to the implementation class.
    //
    // = DESCRIPTION
    // 
    //     This class has all the information necessary for the
    //     <handler> to uniquiely identify the completion of the
    //     asynchronous write.

    friend class ACE_POSIX_Asynch_Write_Stream_Result;
    friend class ACE_WIN32_Asynch_Write_Stream_Result;
    // The concrete implementation result classes only construct this
    // class.

  public:
    u_long bytes_to_write (void) const;
    // The number of bytes which were requested at the start of the
    // asynchronous write.

    ACE_Message_Block &message_block (void) const;
    // Message block that contains the data to be written.

    ACE_HANDLE handle (void) const;
    // I/O handle used for writing.

    ACE_Asynch_Write_Stream_Result_Impl *implementation (void) const;
    // Get the implementation class.
    
  protected:
    Result (ACE_Asynch_Write_Stream_Result_Impl *implementation);
    // Constrcutor.
    
    virtual ~Result (void);
    // Destructor.

    ACE_Asynch_Write_Stream_Result_Impl *implementation_;
    // Implementation class.
  };
};  

// Forward declarations
class ACE_Asynch_Read_File_Impl;
class ACE_Asynch_Read_File_Result_Impl;

class ACE_Export ACE_Asynch_Read_File : public ACE_Asynch_Read_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous reads
  //     on a file. This class forwards all methods to its
  //     implementation class.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <read>s can
  //     started using this class. An ACE_Asynch_Read_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     reads completes through the <ACE_Handler::handle_read_file>
  //     callback.
  //
  //     This class differs slightly from ACE_Asynch_Read_Stream as it
  //     allows the user to specify an offset for the read.

public:
  ACE_Asynch_Read_File (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Read_File (void);
  // Destructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.
  
  int read (ACE_Message_Block &message_block,
            u_long bytes_to_read,
            u_long offset = 0,
            u_long offset_high = 0,
            const void *act = 0,
            int priority = 0);
  // This starts off an asynchronous read.  Upto <bytes_to_read> will
  // be read and stored in the <message_block>.  The read will start
  // at <offset> from the beginning of the file. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.

  ACE_Asynch_Read_File_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Read_File_Impl *implementation);
  // Set the implementation class.
  
  ACE_Asynch_Read_File_Impl *implementation_;
  // Delegation/implementation class that all methods will be
  // forwarded to.

public:
  class ACE_Export Result : public ACE_Asynch_Read_Stream::Result
  {
    // = TITLE
    //
    //     This is that class which will be passed back to the
    //     <handler> when the asynchronous read completes. This class
    //     forwards all the methods to the implementation class.
    //
    // = DESCRIPTION
    //
    //     This class has all the information necessary for the
    //     <handler> to uniquiely identify the completion of the
    //     asynchronous read.
    //
    //     This class differs slightly from
    //     ACE_Asynch_Read_Stream::Result as it calls back
    //     <ACE_Handler::handle_read_file> on the <handler> instead of
    //     <ACE_Handler::handle_read_stream>.  No additional state is
    //     required by this class as ACE_Asynch_Result can store the
    //     <offset>.

    friend class ACE_POSIX_Asynch_Read_File_Result;
    friend class ACE_WIN32_Asynch_Read_File_Result;
    // The concrete implementation result classes only construct this
    // class.
    
  public:
    ACE_Asynch_Read_File_Result_Impl *implementation (void) const;
    // Get the implementation class.

  protected:
    Result (ACE_Asynch_Read_File_Result_Impl *implementation);
    // Constructor. This implementation will not be deleted.

    virtual ~Result (void);
    // Destructor.
    
    ACE_Asynch_Read_File_Result_Impl *implementation_;
    // The implementation class.
  };
};

// Forward declarations
class ACE_Asynch_Write_File_Impl;
class ACE_Asynch_Write_File_Result_Impl;

class ACE_Export ACE_Asynch_Write_File : public ACE_Asynch_Write_Stream
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous writes
  //     on a file. This class forwards all methods to its
  //     implementation class.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <write>s can be
  //     started using this class.  A ACE_Asynch_Write_File::Result
  //     will be passed back to the <handler> when the asynchronous
  //     writes completes through the <ACE_Handler::handle_write_file>
  //     callback.
  //
  //     This class differs slightly from ACE_Asynch_Write_Stream as
  //     it allows the user to specify an offset for the write.

public:
  ACE_Asynch_Write_File (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Write_File (void);
  // Destructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.

  int write (ACE_Message_Block &message_block,
             u_long bytes_to_write,
             u_long offset = 0,
             u_long offset_high = 0,
             const void *act = 0,
             int priority = 0);
  // This starts off an asynchronous write.  Upto <bytes_to_write>
  // will be write and stored in the <message_block>.  The write will
  // start at <offset> from the beginning of the file. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.

  ACE_Asynch_Write_File_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Write_File_Impl *implementation);
  // Set the implementation.

  ACE_Asynch_Write_File_Impl *implementation_;
  // Implementation object.

public:
  class ACE_Export Result : public ACE_Asynch_Write_Stream::Result
  {
    // = TITLE
    //
    //     This is that class which will be passed back to the
    //     <handler> when the asynchronous write completes. This class
    //     forwards all the methods to the implementation class. 
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
    
    friend class ACE_POSIX_Asynch_Write_File_Result;
    friend class ACE_WIN32_Asynch_Write_File_Result;
    // The concrete implementation result classes only construct this
    // class.

  public:
    ACE_Asynch_Write_File_Result_Impl *implementation (void) const;
    //  Get the implementation class.
 
  protected:
    Result (ACE_Asynch_Write_File_Result_Impl *implementation);
    // Constructor. This implementation will not be deleted.

    virtual ~Result (void);
    // Destructor.

    ACE_Asynch_Write_File_Result_Impl *implementation_;
    // The implementation class.
  };
};

// Forward declarations
class ACE_Asynch_Accept_Result_Impl;
class ACE_Asynch_Accept_Impl;

class ACE_Export ACE_Asynch_Accept : public ACE_Asynch_Operation
{
  // = TITLE
  //
  //     This class is a factory for starting off asynchronous accepts
  //     on a listen handle. This class forwards all methods to its
  //     implementation class.
  //
  // = DESCRIPTION
  //
  //     Once <open> is called, multiple asynchronous <accept>s can
  //     started using this class.  A ACE_Asynch_Accept::Result will
  //     be passed back to the <handler> when the asynchronous accept
  //     completes through the <ACE_Handler::handle_accept>
  //     callback.

public:
  ACE_Asynch_Accept (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Accept (void);
  // Destructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.
  
  int accept (ACE_Message_Block &message_block,
              u_long bytes_to_read,
              ACE_HANDLE accept_handle = ACE_INVALID_HANDLE,
              const void *act = 0,
              int priority = 0);
  // This starts off an asynchronous accept.  The asynchronous accept
  // call also allows any initial data to be returned to the
  // <handler>.  Upto <bytes_to_read> will be read and stored in the
  // <message_block>.  The <accept_handle> will be used for the
  // <accept> call.  If (<accept_handle> == INVALID_HANDLE), a new
  // handle will be created. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.
  //
  // <message_block> must be specified. This is because the address of
  // the new connection is placed at the end of this buffer.

  ACE_Asynch_Accept_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Accept_Impl *implementation);
  // Set the implementation class.

  ACE_Asynch_Accept_Impl *implementation_;
  // Delegation/implementation class that all methods will be
  // forwarded to.  

public:
  class ACE_Export Result : public ACE_Asynch_Result
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

    friend class ACE_POSIX_Asynch_Accept_Result;
    friend class ACE_WIN32_Asynch_Accept_Result;
    // The concrete implementation result classes only construct this
    // class.
    
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

    ACE_Asynch_Accept_Result_Impl *implementation (void) const;
    // Get the implementation.
    
  protected:
    Result (ACE_Asynch_Accept_Result_Impl *implementation);
    // Contructor. Implementation will not be deleted.
    
    virtual ~Result (void);
    // Destructor.

    ACE_Asynch_Accept_Result_Impl *implementation_;
    // Impelmentation class.
  };
};

// Forward declarations
class ACE_Asynch_Transmit_File_Result_Impl;
class ACE_Asynch_Transmit_File_Impl;

class ACE_Export ACE_Asynch_Transmit_File : public ACE_Asynch_Operation
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
  // Forward declarations
  class Header_And_Trailer;
  
  ACE_Asynch_Transmit_File (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Transmit_File (void);
  // Destructor.

  int open (ACE_Handler &handler,
            ACE_HANDLE handle = ACE_INVALID_HANDLE,
            const void *completion_key = 0,
            ACE_Proactor *proactor = 0);
  // Initializes the factory with information which will be used with
  // each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
  // <ACE_Handler::handle> will be called on the <handler> to get the
  // correct handle.
  
  int transmit_file (ACE_HANDLE file,
                     Header_And_Trailer *header_and_trailer = 0,
                     u_long bytes_to_write = 0,
                     u_long offset = 0,
                     u_long offset_high = 0,
                     u_long bytes_per_send = 0,
                     u_long flags = 0,
                     const void *act = 0,
                     int priority = 0);
  // This starts off an asynchronous transmit file.  The <file> is a
  // handle to an open file.  <header_and_trailer> is a pointer to a
  // data structure that contains pointers to data to send before and
  // after the file data is sent.  Set this parameter to 0 if you only
  // want to transmit the file data.  Upto <bytes_to_write> will be
  // written to the <socket>.  If you want to send the entire file,
  // let <bytes_to_write> = 0.  <bytes_per_send> is the size of each
  // block of data sent per send operation.  Please read the Win32
  // documentation on what the flags should be. Priority of the
  // operation is specified by <priority>. On POSIX4-Unix, this is
  // supported. Works like <nice> in Unix. Negative values are not
  // allowed. 0 means priority of the operation same as the process
  // priority. 1 means priority of the operation is one less than
  // process. And so forth. On Win32, this is a no-op.

  ACE_Asynch_Transmit_File_Impl *implementation (void) const;
  // Return the underlying implementation class.

protected:
  void implementation (ACE_Asynch_Transmit_File_Impl *);
  // Set the implementation.

  ACE_Asynch_Transmit_File_Impl *implementation_;
  // The implementation class.

public:
  class ACE_Export Result : public ACE_Asynch_Result
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

    friend class ACE_POSIX_Asynch_Transmit_File_Result;
    friend class ACE_WIN32_Asynch_Transmit_File_Result;
    // The concrete implementation result classes only construct this
    // class.
    
  public:
    ACE_HANDLE socket (void) const;
    // Socket used for transmitting the file.

    ACE_HANDLE file (void) const;
    // File from which the data is read.

    Header_And_Trailer *header_and_trailer (void) const;
    // Header and trailer data associated with this transmit file.
    
    u_long bytes_to_write (void) const;
    // The number of bytes which were requested at the start of the
    // asynchronous transmit file.
    
    u_long bytes_per_send (void) const;
    // Number of bytes per send requested at the start of the transmit
    // file.
    
    u_long flags (void) const;
    // Flags which were passed into transmit file.

    ACE_Asynch_Transmit_File_Result_Impl *implementation (void) const;
    // Get the implementation class.
    
  protected:
    Result (ACE_Asynch_Transmit_File_Result_Impl *implementation);
    // Constructor.

    virtual ~Result (void);
    // Destructor.
    
    ACE_Asynch_Transmit_File_Result_Impl *implementation_;
    // The implementation class.
  };

  class ACE_Export Header_And_Trailer
  {
    // = TITLE
    //
    //     The class defines a data structure that contains pointers
    //     to data to send before and after the file data is sent.
    //
    // = DESCRIPTION
    //
    //     This class provides a wrapper over TRANSMIT_FILE_BUFFERS
    //     and provided a consistent use of ACE_Message_Blocks.

  public:
    Header_And_Trailer (ACE_Message_Block *header = 0,
                        u_long header_bytes = 0,
                        ACE_Message_Block *trailer = 0,
                        u_long trailer_bytes = 0);
    // Constructor.
    
    virtual ~Header_And_Trailer (void);
    // Destructor

    void header_and_trailer (ACE_Message_Block *header = 0,
                             u_long header_bytes = 0,
                             ACE_Message_Block *trailer = 0,
                             u_long trailer_bytes = 0);
    // This method allows all the member to be set in one fell swoop.

    ACE_Message_Block *header (void) const;
    void header (ACE_Message_Block *message_block);
    // Header which goes before the file data.

    u_long header_bytes (void) const;
    void header_bytes (u_long bytes);
    // Size of the header data.

    ACE_Message_Block *trailer (void) const;
    void trailer (ACE_Message_Block *message_block);
    // Trailer which goes after the file data.

    u_long trailer_bytes (void) const;
    void trailer_bytes (u_long bytes);
    // Size of the trailer data.

    ACE_LPTRANSMIT_FILE_BUFFERS transmit_buffers (void);
    // Conversion routine.

  protected:
    ACE_Message_Block *header_;
    // Header data.

    u_long header_bytes_;
    // Size of header data.

    ACE_Message_Block *trailer_;
    // Trailer data.

    u_long trailer_bytes_;
    // Size of trailer data.

    ACE_TRANSMIT_FILE_BUFFERS transmit_buffers_;
    // Target data structure.
  };
};

class ACE_Export ACE_Handler
{
  // = TITLE
  //
  //     This base class defines the interface for receiving the
  //     results of asynchronous operations.
  //
  // = DESCRIPTION
  //
  //     Subclasses of this class will fill in appropriate methods.

public:
  ACE_Handler (void);
  // A do nothing constructor.

  ACE_Handler (ACE_Proactor *p);
  // A do nothing constructor which allows proactor to be set to <d>.

  virtual ~ACE_Handler (void);
  // Virtual destruction.

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
   // This method will be called when an asynchronous read completes on
  // a stream.

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This method will be called when an asynchronous write completes
  // on a strea_m.

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This method will be called when an asynchronous read completes on
  // a file.

  virtual void handle_write_file (const ACE_Asynch_Write_File::Result &result);
  // This method will be called when an asynchronous write completes
  // on a file.

  virtual void handle_accept (const ACE_Asynch_Accept::Result &result);
  // This method will be called when an asynchronous accept completes.

  virtual void handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result);
  // This method will be called when an asynchronous transmit file
  // completes.

  virtual void handle_time_out (const ACE_Time_Value &tv,
                                const void *act = 0);
  // Called when timer expires.  <tv> was the requested time value and
  // <act> is the ACT passed when scheduling the timer

  ACE_Proactor *proactor (void);
  // Get the proactor associated with this handler.

  void proactor (ACE_Proactor *p);
  // Set the proactor.

  virtual ACE_HANDLE handle (void) const;
  // Get the I/O handle used by this <handler>. This method will be
  // called by the ACE_Asynch_* classes when an ACE_INVALID_HANDLE is
  // passed to <open>.

protected:
  ACE_Proactor *proactor_;
  // The proactor associated with this handler.
};

// Forward declarations
class ACE_INET_Addr;

// Forward declarations
template <class HANDLER>
class ACE_Asynch_Acceptor;

class ACE_Export ACE_Service_Handler : public ACE_Handler
{
  // = TITLE
  //
  //     This base class defines the interface for the
  //     ACE_Asynch_Acceptor to call into when new connection are
  //     accepted.
  //
  // = DESCRIPTION
  //
  //     Subclasses of this class will fill in appropriate methods to
  //     define application specific behavior.
  
  friend class ACE_Asynch_Acceptor<ACE_Service_Handler>;
  // The Acceptor is the factory and therefore should have special
  // privileges.
  
public:
  ACE_Service_Handler (void);
  // A do nothing constructor.

  virtual ~ACE_Service_Handler (void);
  // Virtual destruction.

  virtual void open (ACE_HANDLE new_handle,
                     ACE_Message_Block &message_block);
  // <open> is called by ACE_Asynch_Acceptor to initialize a new
  // instance of ACE_Service_Handler that has been created after the a
  // new connection is accepted. The handle for the new connection is
  // passed along with an initial data that may have shown up.

  // protected:
  // This should be corrected after the correct semantics of the
  // friend has been figured out.

  virtual void addresses (const ACE_INET_Addr &remote_address,
                          const ACE_INET_Addr &local_address);
  // Called by ACE_Asynch_Acceptor to pass the addresses of the new
  // connections.

  virtual void act (const void *);
  // Called by ACE_Asynch_Acceptor to pass the act.
};

#if defined (__ACE_INLINE__)
#include "ace/Asynch_IO.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS*/
#endif /* ACE_ASYNCH_IO_H */
