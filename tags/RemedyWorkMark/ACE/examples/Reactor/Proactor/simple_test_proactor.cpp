// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    simple_test_proactor.cpp
//
// = DESCRIPTION
//    Very simple version of test_proactor.cpp.
//
// = AUTHOR
//    Alexander Babu Arulanthu (alex@cs.wustl.edu)
//
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_IO_Impl.h"
#include "ace/Message_Block.h"
#include "ace/Get_Opt.h"
#include "ace/OS_main.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

static ACE_TCHAR *file = ACE_TEXT("simple_test_proactor.cpp");
static ACE_TCHAR *dump_file = ACE_TEXT("simple_output");

class Simple_Tester : public ACE_Handler
{
  // = TITLE
  //
  //     Simple_Tester
  //
  // = DESCRIPTION
  //
  //     The class will be created by main(). This class reads a block
  // from the file and write that to the dump file.

public:
  Simple_Tester (void);
  // Constructor.

  ~Simple_Tester (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void);
  // Open the operations and initiate read from the file.
  //FUZZ: enble check_for_lack_ACE_OS

protected:
  // = These methods are called by the freamwork.

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the socket complete.

  virtual void handle_write_file (const ACE_Asynch_Write_File::Result &result);
  // This is called when asynchronous writes from the socket complete.

private:
  int initiate_read_file (void);

  ACE_Asynch_Read_File rf_;
  // rf (read file): for writing from the file.

  ACE_Asynch_Write_File wf_;
  // ws (write File): for writing to the file.

  ACE_HANDLE input_file_;
  // File to read from.

  ACE_HANDLE dump_file_;
  // File for dumping data.

  // u_long file_offset_;
  // Current file offset

  // u_long file_size_;
  // File size
};


Simple_Tester::Simple_Tester (void)
  : input_file_ (ACE_INVALID_HANDLE),
    dump_file_ (ACE_INVALID_HANDLE)
{
}

Simple_Tester::~Simple_Tester (void)
{
  ACE_OS::close (this->input_file_);
  ACE_OS::close (this->dump_file_);
}


int
Simple_Tester::open (void)
{
  // Initialize stuff

  // Open input file (in OVERLAPPED mode)
  this->input_file_ = ACE_OS::open (file,
                                    GENERIC_READ | FILE_FLAG_OVERLAPPED);
  if (this->input_file_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::open"), -1);

  // Open dump file (in OVERLAPPED mode)
  this->dump_file_ = ACE_OS::open (dump_file,
                                   O_CREAT | O_RDWR | O_TRUNC | FILE_FLAG_OVERLAPPED,
                                   0644);
  if (this->dump_file_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::open"), -1);

  // Open ACE_Asynch_Read_File
  if (this->rf_.open (*this, this->input_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Read_File::open"), -1);

  // Open ACE_Asynch_Write_File
  if (this->wf_.open (*this, this->dump_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Write_File::open"), -1);

  ACE_DEBUG ((LM_DEBUG,
              "Simple_Tester::open: Files and Asynch Operations opened successfully\n"));


  // Start an asynchronous read file
  if (this->initiate_read_file () == -1)
    return -1;

  return 0;
}


int
Simple_Tester::initiate_read_file (void)
{
  // Create Message_Block
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (BUFSIZ + 1), -1);

  // Inititiate an asynchronous read from the file
  if (this->rf_.read (*mb, mb->size () - 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Read_File::read"), -1);

  ACE_DEBUG ((LM_DEBUG,
              "Simple_Tester:initiate_read_file: Asynch Read File issued successfully\n"));

  return 0;
}

void
Simple_Tester::handle_read_file (const ACE_Asynch_Read_File::Result &result)
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
  // Watch out if you need to enable this... the ACE_Log_Record::MAXLOGMSGLEN
  // value controls to max length of a log record, and a large output
  // buffer may smash it.
#if 0
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n",
              "message_block",
              result.message_block ().rd_ptr ()));
#endif /* 0 */

  if (result.success ())
    {
      // Read successful: write this to the file.
      if (this->wf_.write (result.message_block (),
                           result.bytes_transferred ()) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Asynch_Write_File::write"));
          return;
        }
    }
}

void
Simple_Tester::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "handle_write_File called\n"));

  // Reset pointers
  result.message_block ().rd_ptr (result.message_block ().rd_ptr () - result.bytes_transferred ());

  result.message_block ().rd_ptr ()[result.bytes_transferred ()] = '\0';

  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key ()));
  ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error ()));
  ACE_DEBUG ((LM_DEBUG, "********************\n"));
  // Watch out if you need to enable this... the ACE_Log_Record::MAXLOGMSGLEN
  // value controls to max length of a log record, and a large output
  // buffer may smash it.
#if 0
  ACE_DEBUG ((LM_DEBUG, "%s = %s\n",
              "message_block",
              result.message_block ().rd_ptr ()));
#endif  /* 0 */
  ACE_Proactor::end_event_loop ();
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("f:d:"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'f':
        file = get_opt.opt_arg ();
        break;
      case 'd':
        dump_file = get_opt.opt_arg ();
        break;
      default:
        ACE_ERROR ((LM_ERROR, "%p.\n",
                    "usage :\n"
                    "-d <dumpfile>\n"
                    "-f <file>\n"));
        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  Simple_Tester Simple_Tester;

  if (Simple_Tester.open () == -1)
    return -1;

  int success = 1;

  // dispatch events
  success = !(ACE_Proactor::run_event_loop () == -1);

  return success ? 0 : 1;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
