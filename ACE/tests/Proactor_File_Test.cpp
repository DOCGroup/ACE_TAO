// $Id$

// ============================================================================
/**
 *  @file Proactor_File_Test.cpp
 *
 *  $Id$
 *
 *  This program illustrates how the ACE_Proactor can be used to
 *  implement an application that does asynchronous file IO
 *  operations.
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

//////////////////////////////////////////////////////////////////

// This sample application integrates asynch file
// read/write operations with the Proactor, using an ACE_FILE_Connector,
// ACE_FILE_IO, ACE_Asynch_Read_File, ACE_Asynch_Write_File in an ACE_Handler.
// The program sets up asynch read and write on a temporary file, and sends
// out 16 16-character bursts of data at timed intervals.

#include <stdio.h>
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"

#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

#include "ace/Proactor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Time_Value.h"


// How long are our fake serial I/O frames?
#define FILE_FRAME_SIZE 16

class FileIOHandler : public ACE_Handler
{
public:
  virtual ~FileIOHandler ();

  int
  Connect();

  // This method will be called when an asynchronous read
  // completes on a file.
  virtual void
  handle_read_file(const ACE_Asynch_Read_File::Result &result);

  // This method will be called when an asynchronous write
  // completes on a file.
  virtual void
  handle_write_file(const ACE_Asynch_Write_File::Result &result);

  // Callback hook invoked by the Proactor's Timer_Queue.
  virtual void
  handle_time_out(const ACE_Time_Value &tv, const void *arg);

  // Our I/O objects; they're public so others can access them
  ACE_Asynch_Read_File reader_;
  ACE_Asynch_Write_File writer_;
private:
  int block_count_;
  ACE_FILE_IO peer_;
  ACE_FILE_Connector connector_;
};

FileIOHandler::~FileIOHandler ()
{
  ACE_FILE_Addr tmp_addr;
  peer_.get_local_addr (tmp_addr);
  if (tmp_addr.get_path_name ())
  {
    peer_.remove ();
  }
}

//***************************************************************************
//
//    Method:          Connect
//
//    Description:   Establishes connection, primes read process
//
// Inputs:        port name, port parameter block
//
// Returns:       none
//
//***************************************************************************

int FileIOHandler::Connect()
{
  int result = 0;

  // create an empty temporary file for the test
  if(connector_.connect(peer_,
                        ACE_sap_any_cast (ACE_FILE_Addr &)) != 0)
  {
    ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),
            ACE_TEXT("FileIOHandler connect failed to create file")));
    result = -1;
  }

  // close opened file but leave it where it is
  if (peer_.close () != 0)
  {
    ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),
            ACE_TEXT("FileIOHandler connect failed to close file")));
    peer_.remove ();
    result = -1;
  }

  // get file address
  ACE_FILE_Addr tmp_addr;
  peer_.get_local_addr (tmp_addr);

  // reopen new file for asynch IO
  if(connector_.connect(peer_,
                        tmp_addr,
                        0, //timeout
                        ACE_Addr::sap_any,
                        0, //reuse
                        O_RDWR |FILE_FLAG_OVERLAPPED) != 0)
  {
    ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),
            ACE_TEXT("FileIOHandler connect failed to open file")));
    peer_.remove ();
    result = -1;
  }
  else // device connected successfully
  {
    // keep track of our writes for offset calculations (can't use O_APPEND since
    // this is not supported for the Win32_Asynch implementation) and data verifications
    this->block_count_ = 0; // start counting

    // Set our I/O handle to that of the peer_ object handling our connection
    handle(peer_.get_handle());

    if (writer_.open(*this) != 0 || reader_.open(*this) != 0)
    {
      ACE_ERROR(
          (LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("FileIOHandler reader or writer open failed")));
      result = -1;
    }
    else // reader and writer opened successfully
    {
      // Allocate a new message block and initiate a read operation on it
      // to prime the asynchronous read pipeline
      // The message block is sized for the largest message we expect
      ACE_Message_Block *mb;
      ACE_NEW_NORETURN(mb, ACE_Message_Block(FILE_FRAME_SIZE));
      if (reader_.read(*mb, mb->space()) != 0)
      {
        ACE_ERROR(
            (LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("FileIOHandler begin read failed")));
        mb->release();
        result = -1;
      }
      // If read worked, psMsg is now controlled by Proactor framework.
    }
  }
  return result;
}

//***************************************************************************
//
//    Method:          handle_read_file
//
//    Description:   Callback used when a read completes
//
// Inputs:        read file result structure containing message block
//
// Returns:       none
//
//***************************************************************************
void
FileIOHandler::handle_read_file(const ACE_Asynch_Read_File::Result &result)
{
  ACE_Message_Block &mb = result.message_block();
  // If the read failed, queue up another one using the same message block
  if (!result.success() || result.bytes_transferred() == 0)
  {
    //ACE_DEBUG((LM_INFO, ACE_TEXT("FileIOHandler receive timeout.\n")));
    reader_.read(mb,
                 mb.space(),
                 result.offset () + result.bytes_transferred ());
  }
  else
  {
    // We have a message block with some read data in it. Send it onward
    ACE_DEBUG((LM_INFO, ACE_TEXT("FileIOHandler received %d bytes of data at offset %d\n"),
                        result.bytes_transferred(), result.offset ()));

    // TODO: Process this data in some meaningful way
    if (result.offset () != (unsigned long)*reinterpret_cast<unsigned char*> (mb.rd_ptr ()))
    {
      ACE_DEBUG((LM_ERROR, ACE_TEXT("FileIOHandler received incorrect data: got [%u] expected [%u]\n"),
                           *reinterpret_cast<unsigned char*> (mb.rd_ptr ()), result.offset ()));
    }

    // Release the message block when we're done with it
    mb.release();

    if ((result.offset () + result.bytes_transferred ()) < 256)
    {
      // Our processing is done; prime the read process again
      ACE_Message_Block *new_mb;
      ACE_NEW_NORETURN(new_mb, ACE_Message_Block(FILE_FRAME_SIZE));
      reader_.read(*new_mb, new_mb->space(),
                   result.offset () + result.bytes_transferred ());
    }
    else
    {
      // we have it all; stop the proactor
      ACE_Proactor::instance ()->proactor_end_event_loop ();
    }
  }
}

//***************************************************************************
//
//    Method:          handle_write_file
//
//    Description:   Callback used when a write completes
//
// Inputs:        write file result structure containing message block
//
// Returns:       none
//
//***************************************************************************
void
FileIOHandler::handle_write_file(const ACE_Asynch_Write_File::Result &result)
{
  ACE_DEBUG((LM_INFO, ACE_TEXT("Finished write\n")));
  // When the write completes, we get the message block. It's been sent,
  // so we just deallocate it.
  result.message_block().release();
}

//***************************************************************************
//
//    Method:          handle_time_out
//
//    Description:   Hook method called when a timer expires
//
// Inputs:        time value, completion token passed to timer at scheduling
//                The token tells us which timer we're handling
//
// Returns:       none
//
//***************************************************************************
void
FileIOHandler::handle_time_out(const ACE_Time_Value &/*tv*/, const void */*act*/)
{
  // do not schedule more than 16 writes
  if (this->block_count_ < 16)
  {
    // In our example, we send a bunch of data every time the timer expires

    // setup the next payload
    char payload[FILE_FRAME_SIZE];
    for (int i=0; i<FILE_FRAME_SIZE ;++i)
    {
      payload[i] = (this->block_count_ * FILE_FRAME_SIZE) + i;
    }
    ACE_Message_Block *new_mb;
    ACE_NEW_NORETURN(new_mb, ACE_Message_Block(FILE_FRAME_SIZE));
    new_mb->copy(payload, FILE_FRAME_SIZE);

    // queue up a write (append to end of file) operation, give visual feedback on  success or failure.
    if (this->writer_.write(*new_mb, new_mb->length(),
                            this->block_count_ * FILE_FRAME_SIZE) == 0)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT("Successfully queued write of %d bytes\n"), new_mb->length ())); // success
      this->block_count_ ++; // next block
    }
    else
    {
      ACE_DEBUG((LM_ERROR, ACE_TEXT("FAILED to queue write operation\n"))); // failure
    };
  }
}


int
run_main(int /*argc*/, ACE_TCHAR */*argv*/[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_File_Test"));

  int rc = 0;
  FileIOHandler fileIOHandler;

  // Initialize the serial port handler
  if (0 != fileIOHandler.Connect())
  {
    rc = 1;
  }
  else
  {
    ACE_DEBUG((LM_INFO, ACE_TEXT(" File I/O Handler connected.\n")));

    // start the repeating timer for data transmission

    ACE_Time_Value repeatTime(0, 50000); // 0.05 second time interval
    ACE_Proactor::instance()->schedule_repeating_timer(fileIOHandler,
                                                       (void *) (100),
                                                       repeatTime);

    // Run the Proactor
    ACE_Proactor::instance()->proactor_run_event_loop();
  }

  ACE_END_TEST;

  return rc;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_File_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_File_Test will not be run.\n")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
