// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_proactor_with_aio.cpp
//
// = DESCRIPTION
//    This program illustrates how the ACE_Proactor can be used to
//    implement an application that does various asynchronous
//    operations.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

static char *file = "test_proactor.cpp";
static char *dump_file = "aio_out.log";
static int done = 0;

class Sender : public ACE_Handler
{
  // = TITLE
  //     Sender
  //
  // = DESCRIPTION
  //     The class will be created by main().  After connecting to the
  //     host, this class will then read data from a file and send it
  //     to the network connection.
public:
  Sender (void);
  // Constructor.

  ~Sender (void);
  // Destructor.

  int open (const char *host,
	    u_short port);
  // ACE_HANDLE handle (void) const;

protected:

#if 0
  // These methods are called by the freamwork
  virtual void handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result);
  // This is called when asynchronous transmit files complete.
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete.
#endif /* 0 */

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the file complete.

#if 0
  virtual void handle_write_file (const ACE_Asynch_Write_File::Result &result);
  // This is called when asynchronous write to a file complete.
#endif /* 0 */

private:
  //int transmit_file (void);

  int initiate_read_file (void);
  // Intiate read from the file.

  //ACE_SOCK_Stream stream_;
  // Network I/O handle

  //ACE_Asynch_Write_Stream ws_;
  // ws (write stream): for writing to the socket

  ACE_Asynch_Read_File rf_;
  // rf (read file): for reading from the file.

  ACE_HANDLE input_file_;
  // File to read from.

  u_long file_offset_;
  // Current file offset.

  u_long file_size_;
  // File size.

  //ACE_Message_Block welcome_message_;
  // Welcome message

  //ACE_Asynch_Transmit_File::Header_And_Trailer header_and_trailer_;
  // Header and trailer which goes with transmit_file

  //int stream_write_done_;
  //int transmit_file_done_;
  // These flags help to determine when to close down the event loop
};


Sender::Sender (void)
  : input_file_ (ACE_INVALID_HANDLE),
    file_offset_ (0),
    file_size_ (0)
  //stream_write_done_ (0),
  //transmit_file_done_ (0)
{
  // Moment of inspiration :-)
  //static char *data = "Welcome to Irfan World! Irfan RULES here !!";
  //this->welcome_message_.init (data, ACE_OS::strlen (data));
  //this->welcome_message_.wr_ptr (ACE_OS::strlen (data));
}

Sender::~Sender (void)
{
  //this->stream_.close ();
}

#if 0
ACE_HANDLE
Sender::handle (void) const
{
  return this->stream_.get_handle ();
}
#endif /* 0 */

int
Sender::open (const char *host,
              u_short port)
{
  // Initialize stuff.

  // Open input file (in OVERLAPPED mode)
  this->input_file_ = ACE_OS::open (file, GENERIC_READ | FILE_FLAG_OVERLAPPED);
  if (this->input_file_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::open"), -1);

  // Find file size
  this->file_size_ = ACE_OS::filesize (this->input_file_);
  ACE_DEBUG ((LM_DEBUG, "Input file size :%d\n", this->file_size_));
#if 0
  // Connect to remote host
  ACE_INET_Addr address (port, host);
  ACE_SOCK_Connector connector;
  if (connector.connect (this->stream_,
			 address) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_SOCK_Connector::connect"), -1);

  // Open ACE_Asynch_Write_Stream
  if (this->ws_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Write_Stream::open"), -1);
#endif /* 0 */

  // Open ACE_Asynch_Read_File
  if (this->rf_.open (*this, this->input_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Read_File::open"), -1);

  // Start an asynchronous transmit file
  // if (this->transmit_file () == -1)
  // return -1;

  // Start an asynchronous read file
  if (this->initiate_read_file () == -1)
    return -1;

  return 0;
}

#if 0
int
Sender::transmit_file (void)
{
  // Open file (in SEQUENTIAL_SCAN mode)
  ACE_HANDLE file_handle = ACE_OS::open (file, GENERIC_READ | FILE_FLAG_SEQUENTIAL_SCAN);
  if (file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::open"), -1);

  // Open ACE_Asynch_Transmit_File
  ACE_Asynch_Transmit_File tf;
  if (tf.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Transmit_File::open"), -1);

  // Header and trailer data for the file
  this->header_and_trailer_.header_and_trailer (&this->welcome_message_,
						this->welcome_message_.length (),
						&this->welcome_message_,
						this->welcome_message_.length ());

  // Starting position
  cerr << "Staring position: " << ACE_OS::lseek (file_handle, 0L, SEEK_CUR) << endl;

  // Send it
  if (tf.transmit_file (file_handle,
			&this->header_and_trailer_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Transmit_File::transmit_file"), -1);

  return 0;
}

void
Sender::handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_transmit_file called\n"));

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "socket", result.socket ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "file", result.file ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_per_send", result.bytes_per_send ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "flags", result.flags ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));

  // Ending position
  cerr << "Ending position: " << ACE_OS::lseek (result.file (), 0L, SEEK_CUR) << endl;

  // Done with file
  ACE_OS::close (result.file ());

  this->transmit_file_done_ = 1;
  if (this->stream_write_done_)
    done = 1;
}
#endif /* 0 */

int
Sender::initiate_read_file (void)
{
  // Create Message_Block
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (BUFSIZ + 1), -1);

  // Inititiate an asynchronous read from the file
  if (this->rf_.read (*mb,
		      mb->size () - 1,
		      this->file_offset_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Read_File::read"), -1);

  return 0;
}

void
Sender::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_read_file called\n"));

  result.message_block ().rd_ptr ()[result.bytes_transferred ()] = '\0';

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));


  if (result.success ())
    {
      // Read successful: increment offset and write data to network.
      this->file_offset_ += result.bytes_transferred ();

      // @@ Just print out the buffer.
      ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));

#if 0
      if (this->ws_.write (result.message_block (),
			   result.bytes_transferred ()) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Asynch_Write_Stream::write"));
	  return;
	}
#endif /* 0 */

      if (this->file_size_ > this->file_offset_)
	{
	  // Start an asynchronous read file
	  if (initiate_read_file () == -1)
	    return;
	}
      else
        done = 1;
    }
}

#if 0
void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_write_stream called\n"));

  // Reset pointers
  result.message_block ().rd_ptr (-result.bytes_transferred ());

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n", "message_block", result.message_block ().rd_ptr ()));

  if (result.success ())
    {
      // Partial write to socket
      int unsent_data = result.bytes_to_write () - result.bytes_transferred ();
      if (unsent_data != 0)
        {
          // Reset pointers
          result.message_block ().rd_ptr (result.bytes_transferred ());

          // Duplicate the message block and retry remaining data
          if (this->ws_.write (*result.message_block ().duplicate (),
                               unsent_data) == -1)
            {
              ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Asynch_Write_Stream::write"));
              return;
            }
        }
      else if (!(this->file_size_ > this->file_offset_))
        {
          this->stream_write_done_ = 1;
          if (this->transmit_file_done_)
            done = 1;
        }
    }

  // Release message block
  result.message_block ().release ();
}
#endif /* 0 */

int
main (int argc, char *argv [])
{
  ACE_DEBUG ((LM_DEBUG, "test_proactor_with_aio\n"));

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_UNUSED_ARG (dump_file);

  // Create a sender. It reads from a file.
  Sender sender;

  sender.open (0, 0);

  int error = 0;
  while (!error && !done)
    // dispatch events
    error = ACE_Proactor::instance ()->handle_events ();

  return 0;
}
