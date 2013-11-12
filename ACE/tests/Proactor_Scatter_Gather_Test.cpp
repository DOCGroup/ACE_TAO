// $Id$

// ============================================================================
/**
 *  @file Proactor_Scatter_Gather_Test.cpp
 *
 *  The test runs on a single thread, and involves a single Sender,
 *  two Receivers and a single Writer.  The Sender async-reads
 *  (scattered) from a file into chunks of <page size>.  It
 *  async-sends (gathered) the odd chunks to the first receiver over a
 *  stream, and the even chunks to the second receiver over a
 *  different stream.  The receivers async-read (scattered) from the
 *  socket streams into chunks in size of <page size>, and convey the
 *  data to the Writer.  The Writer reconstructs the file using
 *  async-write (gathered).  Then, the reconstructed file is compared
 *  to the original file to determine test success. So, It covers both
 *  async scatter/gather stream I/O and async scatter/gather file I/O.
 *  The wire transfer protocol is very naive (and totally non
 *  reliable...) - when both connections are closed, EOF is assumed.
 *  The test can be also run in a seperated sender and receiver mode,
 *  to test real network influences.
 *
 *  This test is based upon some building blocks taken from the
 *  Proactor_Test.cpp.
 *
 *  @author Edan Ayal <edanayal@yahoo.com> */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)
  // This currently only works on Win32 platforms (NT SP2 and above).
  // Support for Unix platforms supporting POSIX aio calls should be added in future.

#include "ace/Get_Opt.h"

#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Mem_Map.h"
#include "ace/Min_Max.h"
#include "ace/OS_NS_math.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

#include "ace/SOCK_Connector.h"

// For the Acceptor/Connector handlers maintenance lists
static const  int SENDERS   = 1;
static const  int RECEIVERS = 2;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

static const ACE_TCHAR *host = ACE_LOCALHOST;

// File that we're sending.
static const ACE_TCHAR *input_file = ACE_TEXT("Proactor_Scatter_Gather_Test.cpp");

// Name of the output file.
static const ACE_TCHAR *output_file = ACE_TEXT("output");

static int client_only = 0;
static int server_only = 0;
static size_t chunk_size = 0;

enum
{
  ODD = 0,
  EVEN
};

// *************************************************************
//  Some chunks chain helper routines
// *************************************************************
static int allocate_chunks_chain (ACE_Message_Block *&head_mb,
                                  size_t number_of_chunks)
{
  ACE_Message_Block *pre_mb = 0;

  for (size_t index = 0; index < number_of_chunks; ++index)
    {
#if defined (ACE_WIN32)
      void *addr =  ::VirtualAlloc (0,
                                    chunk_size,
                                    MEM_COMMIT,
                                    PAGE_READWRITE);
#else
      void *addr =  new char[chunk_size];
#endif /* ACE_WIN32 */
      if (addr)
        {
          ACE_Message_Block *mb = new ACE_Message_Block (static_cast<char *> (addr),
                                                         chunk_size);
          if (!head_mb)
            head_mb = mb;

          // chain them together
          if (pre_mb)
            pre_mb->cont (mb);
          pre_mb = mb;
        }
      else
        {
          ACE_TEST_ASSERT (0);
          return -1;
        }
    }

  return 0;
}

static void
free_chunks_chain (ACE_Message_Block *&mb)
{
  for (const ACE_Message_Block* msg = mb;
       msg != 0;
       msg = msg->cont ())
    {
#if defined (ACE_WIN32)
      ::VirtualFree (msg->base (),
                     msg->size (),
                     MEM_DECOMMIT);
#else
      delete [] msg->base ();
#endif /* ACE_WIN32 */
    }

  mb->release ();
  mb = 0;
}

static int
last_chunk (ACE_Message_Block *chain,
            ACE_Message_Block *&last)
{
  if (!chain)
    return 0;

  int index = 1;
  last = chain;
  while (0 != last->cont ())
  {
    last = last->cont ();
    ++index;
  }

  return index;
}

static void
merge_odd_even_chains (ACE_Message_Block *odd_mb,
                       ACE_Message_Block *even_mb)
{
  ACE_Message_Block *pre_pre_mb = odd_mb;
  ACE_Message_Block *pre_mb = even_mb;
  ACE_Message_Block *curr_mb = odd_mb->cont ();

  if (even_mb)
    {
      for (; curr_mb != 0; curr_mb = pre_pre_mb->cont ())
        {
          pre_pre_mb->cont (pre_mb);

          // increment history pointers
          pre_pre_mb = pre_mb;
          pre_mb = curr_mb;
        }

      pre_pre_mb->cont (pre_mb);
      pre_mb->cont (0);
    }
}

static void
split_odd_even_chains (ACE_Message_Block *odd_mb,
                       ACE_Message_Block *even_mb)
{
  ACE_Message_Block *pre_pre_mb = odd_mb;
  ACE_Message_Block *pre_mb     = even_mb;
  ACE_Message_Block *curr_mb    = (even_mb ? even_mb->cont () : 0);

  for (; curr_mb != 0; curr_mb = curr_mb->cont ())
    {
      pre_pre_mb->cont (curr_mb);

      // increment history pointers
      pre_pre_mb = pre_mb;
      pre_mb = curr_mb;
    }

  pre_pre_mb->cont (0);
  if (pre_mb)
    pre_mb->cont (0);
}

static void
add_to_chunks_chain (ACE_Message_Block *&chunks_chain,
                     ACE_Message_Block *additional_chunks_chain)
{
  if (0 == chunks_chain)
    chunks_chain = additional_chunks_chain;
  else
    {
      ACE_Message_Block *last = 0;
      last_chunk (chunks_chain, last);
      if (last)
        last->cont (additional_chunks_chain);
    }
}

static void
remove_empty_chunks (ACE_Message_Block *&chunks_chain)
{
  if (0 == chunks_chain)
    return;

  ACE_Message_Block *first_empty = chunks_chain;
  ACE_Message_Block *pre_mb = 0;

  while (first_empty->length () > 0 &&
         0 != first_empty->cont ())
    {
      pre_mb = first_empty;
      first_empty = first_empty->cont ();
    }

  // break the chain there, and release the empty end (might be everything)
  if (0 == first_empty->length ())
    {
      if (pre_mb) // might be 0, in case it's the entire chain
        pre_mb->cont (0);

      if (first_empty == chunks_chain)
        chunks_chain = 0;

      free_chunks_chain (first_empty);
    }
}

// *************************************************************
//   Acceptor, Receiver and Writer
// *************************************************************
class Receiver;

class Acceptor : public ACE_Asynch_Acceptor<Receiver>
{
  friend class Receiver;

public:
  Acceptor (void);
  virtual ~Acceptor (void);

  void stop (void);

  // Virtual from ACE_Asynch_Acceptor
  virtual Receiver *make_handler (void);

  int get_number_sessions (void) { return this->sessions_; }

private:
  void on_new_receiver (Receiver &rcvr);
  void on_delete_receiver (Receiver &rcvr);

  int sessions_;
  Receiver *list_receivers_[RECEIVERS];
};

class Writer;

// The first instantiated take the role of the odd receiver
class Receiver : public ACE_Service_Handler
{
  friend class Acceptor;
  friend class Writer;

public:
  Receiver  (Acceptor *acceptor = 0, int index = -1);
  virtual ~Receiver (void);

  //FUZZ: disable check_for_lack_ACE_OS
  /// This is called after the new connection has been accepted.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  //FUZZ: enable check_for_lack_ACE_OS

protected:
  /// This is called by the framework when asynchronous <read> operation from the
  /// socket completes.
  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);

private:
  int initiate_read_stream (void);

  void check_destroy (void);

  Acceptor *acceptor_;
  int index_;

  // Socket input
  ACE_Asynch_Read_Stream rs_;
  ACE_HANDLE socket_handle_;

  // Writer
  static Writer* writer_;

  long io_count_;

  char odd_;

  // if we get non-page-size reminder, we will not send it to the writer
  // until it is full (unless at end)
  ACE_Message_Block *partial_chunk_;
};

class Writer : public ACE_Handler
{
  friend class Receiver;

public:
  Writer (void);
  virtual ~Writer (void);

  //FUZZ: disable check_for_lack_ACE_OS
  void open (void);
  //FUZZ: enable check_for_lack_ACE_OS

  // this is *not* a callback from the framework
  int handle_read_chunks_chain (ACE_Message_Block *mb,
                                int type);

  // for determining when last receiver dies
  void on_new_receiver ();
  void on_delete_receiver ();

protected:
  /// This is called by the framework when an asynchronous <write> to the file
  /// completes.
  virtual void handle_write_file (const ACE_Asynch_Write_File::Result &result);

private:
  int initiate_write_file (void);

private:
   // Output file
  ACE_Asynch_Write_File wf_;
  ACE_HANDLE output_file_handle_;
  u_long      writing_file_offset_;
  u_long      reported_file_offset_;
  ACE_Message_Block *odd_chain_;
  ACE_Message_Block *even_chain_;
  long io_count_;
  char receiver_count_;
};

// *************************************************************
//   Receiver Impl
// *************************************************************

Writer  *Receiver::writer_ = 0;

Receiver::Receiver (Acceptor * acceptor, int index)
  : acceptor_ (acceptor),
    index_    (index),
    socket_handle_      (ACE_INVALID_HANDLE),
    io_count_ (0),
    partial_chunk_ (0)
{
  // the first one is the odd one
  this->odd_ = ((0 == index) ? 1 : 0);

  if (this->odd_)
    {
      Receiver::writer_ = new Writer;
      if (!Receiver::writer_)
        {
          ACE_TEST_ASSERT (0);
          return;
        }
    }

  Receiver::writer_->on_new_receiver ();

  if (this->acceptor_ != 0)
    this->acceptor_->on_new_receiver (*this);
}

Receiver::~Receiver (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::~Receiver\n")));

  if (this->acceptor_ != 0)
    this->acceptor_->on_delete_receiver (*this);

  if (this->socket_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->socket_handle_);

  Receiver::writer_->on_delete_receiver ();

  if (this->partial_chunk_)
    {
      ACE_TEST_ASSERT (0); // should not be getting here
      this->partial_chunk_->release ();
    }
}

void
Receiver::check_destroy (void)
{
  if (this->io_count_ <= 0)
    delete this;
}

void
Receiver::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  this->socket_handle_ = handle;

  // Open the ACE_Asynch_Read_Stream
  if (this->rs_.open (*this, this->socket_handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Receiver::ACE_Asynch_Read_Stream::open")));
  else
    {
      if (this->odd_)
        Receiver::writer_->open ();

      this->initiate_read_stream ();
    }

  this->check_destroy ();
}

int
Receiver::initiate_read_stream (void)
{
  if (!Receiver::writer_)
    return -1;

  // how many chunks to allocate?
  size_t number_of_new_chunks = (this->partial_chunk_ ?
                                 (ACE_IOV_MAX / RECEIVERS) - 1
                                 : ACE_IOV_MAX / RECEIVERS);

  // allocate chunks chain
  ACE_Message_Block *head_mb = 0;
  if (-1 == allocate_chunks_chain (head_mb, number_of_new_chunks))
    {
      ACE_TEST_ASSERT (0);
      return -1;
    }

  // calculate how many bytes to read

  // head_mb could be 0 (no new chunks allocated)
  size_t bytes_to_read = head_mb ? head_mb->total_size () : 0;

  // add the partial chunk at the front if appropriate, and update
  // the number of bytes to read
  if (this->partial_chunk_)
    {
      bytes_to_read += this->partial_chunk_->space ();
      this->partial_chunk_->cont (head_mb);
      head_mb = this->partial_chunk_;
      this->partial_chunk_ = 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::initiate_read_stream - (%s) readv %d\n"),
              this->odd_ ? ACE_TEXT ("ODD ") : ACE_TEXT ("EVEN"),
              bytes_to_read));

  // perform the actual scattered read
  if (this->rs_.readv (*head_mb,
                       bytes_to_read) == -1)
    {
      free_chunks_chain (head_mb);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Receiver::ACE_Asynch_Stream::read")),
                        -1);
    }

  ++this->io_count_;
  return 0;
}

void
Receiver::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_Message_Block *mb = &result.message_block ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::handle_read_stream - (%s) read %d\n"),
              this->odd_ ? ACE_TEXT ("ODD ") : ACE_TEXT ("EVEN"),
              result.bytes_transferred ()));

  // Transfer only complete chunks to the writer.
  // Save last partial chunk for the next call.
  // On disconnect (error or 0 transferred), transfer whatever we have.

  // at this stage there should not be anything there
  ACE_TEST_ASSERT (!this->partial_chunk_);

  // first, remove the empty chunks
  remove_empty_chunks (mb);

  if (mb && Receiver::writer_)
    { // there's something to write, and who to write to

      // write everything or only complete chunks?

      // write everything - when no new bytes were transferred
      int write_everything = 0;
      if (!result.bytes_transferred ())
        write_everything = 1;
      if (write_everything)
        Receiver::writer_->handle_read_chunks_chain (mb,
                                                     this->odd_ ? ODD : EVEN);
      else
        { // filter out the partial chunk at the end (if present)
          // and save it for later before writing the full chunks

          // have this->partial_chunk_ point to the last chunk in the chain
          size_t last_index = last_chunk (mb, this->partial_chunk_);
          if (this->partial_chunk_ &&
              this->partial_chunk_->length () < chunk_size)
            { // found partial chunk at end of chain
              // detach it from the chain
              if (last_index > 1) // chain bigger than 1
                {
                  ACE_Message_Block *pre_last = mb;
                  for (size_t index = 1; index < last_index - 1; ++index)
                    pre_last = pre_last->cont ();

                  // detach partial chunk from chain
                  pre_last->cont (0);
                }
              else
                // chain in length of 1 - so we need to zero mb
                mb = 0;
            }
          else // last is a full chunk, so hand it over with the rest
            this->partial_chunk_ = 0;

          // transfer (if there's anything left)
          if (mb && mb->total_length ())
            Receiver::writer_->handle_read_chunks_chain (
                                                         mb,
                                                         this->odd_ ? ODD : EVEN);

          // initiate more reads only if no error
          if (!result.error ())
            this->initiate_read_stream ();
          else
            ACE_TEST_ASSERT (0);
        }
    }
  else if (mb && !Receiver::writer_)
    // no one to write to
    free_chunks_chain (mb);

  --this->io_count_;

  this->check_destroy ();
}

// *************************************************************
//   Acceptor Impl
// *************************************************************

Acceptor::Acceptor (void)
  : sessions_ (0)
{
  for (int i = 0; i < RECEIVERS; ++i)
     this->list_receivers_[i] = 0;
}

Acceptor::~Acceptor (void)
{
  this->stop ();
}


void
Acceptor::stop (void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.
  for (int i = 0; i < RECEIVERS; ++i)
    {
      delete this->list_receivers_[i];
      this->list_receivers_[i] = 0;
    }
}

void
Acceptor::on_new_receiver (Receiver & rcvr)
{
  ++this->sessions_;
  this->list_receivers_[rcvr.index_] = &rcvr;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::CTOR sessions_ = %d\n"),
              this->sessions_));
}

void
Acceptor::on_delete_receiver (Receiver & rcvr)
{
  --this->sessions_;
  if (rcvr.index_ >= 0
      && rcvr.index_ < RECEIVERS
      && this->list_receivers_[rcvr.index_] == &rcvr)
    this->list_receivers_[rcvr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Receiver::~DTOR sessions_ = %d\n"),
              this->sessions_));
}

Receiver *
Acceptor::make_handler (void)
{
  if (this->sessions_ >= RECEIVERS)
    return 0;

  for (int i = 0; i < RECEIVERS; ++i)
    {
      if (this->list_receivers_[i] == 0)
        {
          ACE_NEW_RETURN (this->list_receivers_[i],
                          Receiver (this, i),
                          0);
          return this->list_receivers_[i];
        }
    }

  return 0;
}

// *************************************************************
//   Writer Impl
// *************************************************************

Writer::Writer (void)
: output_file_handle_ (ACE_INVALID_HANDLE),
  writing_file_offset_ (0),
  reported_file_offset_ (0),
  odd_chain_ (0),
  even_chain_ (0),
  io_count_ (0),
  receiver_count_ (0)
{
}

Writer::~Writer (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::~Writer\n")));

  if (this->output_file_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->output_file_handle_);

  Receiver::writer_ = 0;
}

void
Writer::on_new_receiver ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::on_new_receiver\n")));

  ++this->receiver_count_;
}

void
Writer::on_delete_receiver ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::on_delete_receiver\n")));

  --this->receiver_count_;

  if (0 == this->receiver_count_)
    {
      if (this->io_count_ <= 0)
        // no pending io, so do the work oursleves
        // (if pending io, they'll see the zero receiver count)
        this->initiate_write_file ();
    }
}

void
Writer::open (void)
{
  // Open the file for output
  if (ACE_INVALID_HANDLE == (this->output_file_handle_ = ACE_OS::open (output_file,
                                                                       O_CREAT | _O_TRUNC | _O_WRONLY |\
                                                                       FILE_FLAG_OVERLAPPED |\
                                                                       FILE_FLAG_NO_BUFFERING,
                                                                       ACE_DEFAULT_FILE_PERMS)))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Writer::open::ACE_OS::open")));
  // Open the ACE_Asynch_Write_File
  else if (this->wf_.open (*this, this->output_file_handle_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Writer::open::ACE_Asynch_Write_File::open")));
}

int
Writer::handle_read_chunks_chain (ACE_Message_Block *mb,
                                  int type)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::handle_read_chunks_chain - (%s) %d bytes\n"),
              (type == ODD) ? ACE_TEXT ("ODD ") : ACE_TEXT ("EVEN"),
              mb->total_length ()));

  add_to_chunks_chain (ODD == type ? this->odd_chain_ : this->even_chain_, mb);

  this->initiate_write_file ();

  return 0;
}

int
Writer::initiate_write_file (void)
{
  // find out how much can we merge
  ACE_Message_Block *dummy_last = 0;
  size_t odd_count  = last_chunk (this->odd_chain_, dummy_last);
  size_t even_count = last_chunk (this->even_chain_, dummy_last);

  size_t merge_size = ACE_MIN (ACE_MIN (odd_count, even_count),
                               (size_t) ACE_IOV_MAX);

  // the options here are as follows:
  // io_count_ can be zero or greater.
  // merge_size can be zero or not.
  // if non zero merge, write the merge. ASSERT receiver_count_ is non zero too.
  // if zero merge:
  //     if receiver_count_ is non zero, NOOP.
  //     if zero receiver_count_, we should write whatever is left,
  //       and terminate the writer at completion.
  //       if nothing to write, and io_count_ is zero too, terminate here.

  if (0 == merge_size &&
      0 != this->receiver_count_)
    return 0;

  if (0 == merge_size &&
      0 == this->receiver_count_ &&
      0 == odd_count &&
      0 == even_count &&
      0 == this->io_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Writer::initiate_write_file")
                  ACE_TEXT (" - ending proactor event loop\n")));

      ACE_Proactor::instance ()->end_event_loop ();

      delete this;

      return 0;
    }

  // if we reached nere and merge_size is zero, we should write whatever is
  // in the queues (1 to 2 chunks together), so let's force the merge size to 1.
  if (0 == merge_size)
    {
      ACE_TEST_ASSERT (1 == odd_count && 1 >= even_count);
      merge_size = 1;
    }

  // Now that we found out what we want to do, prepare the chain
  // that will be written, and update the remainders
  ACE_Message_Block *new_odd_chain_head  = this->odd_chain_;
  ACE_Message_Block *new_even_chain_head = this->even_chain_;

  // locate the place for detachment in the chains
  ACE_Message_Block *pre_odd  = 0;
  ACE_Message_Block *pre_even = 0;
  for (size_t index = 0; index < merge_size; ++index)
    {
      pre_odd = new_odd_chain_head;
      if (new_odd_chain_head)
        new_odd_chain_head  = new_odd_chain_head->cont ();
      pre_even = new_even_chain_head;
      if (new_even_chain_head)
        new_even_chain_head = new_even_chain_head->cont ();
    }
  // now detach the chain
  if (pre_odd)
    pre_odd->cont (0);
  if (pre_even)
    pre_even->cont (0);

  // perform merge between the two chains
  merge_odd_even_chains (this->odd_chain_, this->even_chain_);

  // and now finally perform the write
  ACE_Message_Block *united_mb = this->odd_chain_;
  // update the remainders of the chains
  this->odd_chain_  = new_odd_chain_head;
  this->even_chain_ = new_even_chain_head;
  size_t increment_writing_file_offset = united_mb->total_length ();

  // Reconstruct the file
  // Write the size, not the length, because we must write in chunks
  // of <page size>
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::initiate_write_file: write %d bytes at %d\n"),
              united_mb->total_size (),
              this->writing_file_offset_));
  if (this->wf_.writev (*united_mb,
                        united_mb->total_size (),
                        this->writing_file_offset_) == -1)
    {
      free_chunks_chain (united_mb);

      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Writer::initiate_write_file::ACE_Asynch_Write_Stream::writev")),
                       -1);
    }

  // we update now because otherwise, we'd have error when performing
  // pipelined writing (that is, mulitple calls to write before the callbacks
  // to handle_x)
  this->writing_file_offset_ +=
    static_cast<u_long> (increment_writing_file_offset);
  ++this->io_count_;
  return 0;
}

void
Writer::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
  ACE_Message_Block *mb = &result.message_block ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Writer::handle_write_file at offset %d wrote %d\n"),
              this->reported_file_offset_,
              result.bytes_transferred ()));

  this->reported_file_offset_ +=
    static_cast<u_long> (result.bytes_transferred ());

  // Always truncate as required,
  // because partial will always be the last write to a file
  ACE_Message_Block *last_mb = mb;
  last_chunk (mb, last_mb);

  if (last_mb->space ())
    ACE_OS::truncate (output_file,
                      this->reported_file_offset_ -
                        static_cast<u_long> (last_mb->space ()));

  free_chunks_chain (mb);

  --this->io_count_;

  // end of process?
  if (0 == this->receiver_count_ &&
      0 == this->io_count_)
    {
      ACE_TEST_ASSERT (0 == this->odd_chain_ && 0 == this->even_chain_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Writer::handle_write_file")
                  ACE_TEXT (" - ending proactor event loop\n")));

      ACE_Proactor::instance ()->end_event_loop ();

      delete this;
    }
}

// *************************************************************
//   Connector and Sender
// *************************************************************
class Sender;

class Connector : public ACE_Asynch_Connector<Sender>
{
  friend class Sender;

public:
  Connector (void);
  virtual ~Connector (void);

  // Address to pass to Sender for secondary connect.
  void set_address (const ACE_INET_Addr &addr);
  const ACE_INET_Addr &get_address (void);

  void stop (void);

  // Virtual from ACE_Asynch_Connector
  virtual Sender *make_handler (void);

private:
  void on_new_sender  (Sender &rcvr);
  void on_delete_sender (Sender &rcvr);

  int sessions_;
  ACE_INET_Addr addr_;
  Sender *list_senders_[SENDERS];
};

class Sender : public ACE_Service_Handler
{
  friend class Connector;
public:

  Sender  (Connector *connector = 0, int index = -1);

  virtual ~Sender (void);

  //FUZZ: disable check_for_lack_ACE_OS
  /// This is called after the new connection has been established.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  //FUZZ: enable check_for_lack_ACE_OS

  // This is called by the framework when asynchronous reads from the
  // file complete.
  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);

  // This is called by the framework when asynchronous writes from the
  // socket complete.
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

private:
  void check_destroy (void);

  int initiate_read_file (void);

  int initiate_write_stream (ACE_Message_Block &mb);

  int  index_;
  Connector * connector_;

  // File to read from
  ACE_Asynch_Read_File rf_;
  ACE_HANDLE  input_file_handle_;
  u_long      file_offset_;

  // Sockets to send to
  // odd and even socket output streams
  ACE_Asynch_Write_Stream ws_[RECEIVERS];
  ACE_HANDLE socket_handle_[RECEIVERS];

  long io_count_;
};

// *************************************************************
//   Connector Impl
// *************************************************************

Connector::Connector (void)
  : sessions_ (0)
{
  for (int i = 0; i < SENDERS; ++i)
     this->list_senders_[i] = 0;
}

Connector::~Connector (void)
{
  this->stop ();
}

// Address to pass to Sender for secondary connect.
void
Connector::set_address (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

const ACE_INET_Addr &
Connector::get_address (void)
{
  return this->addr_;
}

void
Connector::stop (void)
{
  // This method can be called only after proactor event loop is done
  // in all threads.

  for (int i = 0; i < SENDERS; ++i)
    {
      delete this->list_senders_[i];
      this->list_senders_[i] = 0;
    }
}

void
Connector::on_new_sender (Sender &sndr)
{
  ++this->sessions_;
  this->list_senders_[sndr.index_] = &sndr;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::CTOR sessions_ = %d\n"),
              this->sessions_));
}

void
Connector::on_delete_sender (Sender &sndr)
{
  --this->sessions_;
  if (sndr.index_ >= 0
      && sndr.index_ < SENDERS
      && this->list_senders_[sndr.index_] == &sndr)
    this->list_senders_[sndr.index_] = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::~DTOR sessions_ = %d\n"),
              this->sessions_));
}

Sender *
Connector::make_handler (void)
{
  if (this->sessions_ >= SENDERS)
    return 0;

  for (int i = 0; i < SENDERS; ++i)
    {
      if (this->list_senders_ [i] == 0)
        {
          ACE_NEW_RETURN (this->list_senders_[i],
                          Sender (this, i),
                          0);
          return this->list_senders_[i];
        }
    }

  return 0;
}

// *************************************************************
//   Sender Impl
// *************************************************************

Sender::Sender (Connector * connector, int index)
  : index_     (index),
    connector_ (connector),
    input_file_handle_ (ACE_INVALID_HANDLE),
    file_offset_ (0),
    io_count_  (0)
{
  socket_handle_[ODD] = socket_handle_[EVEN] = ACE_INVALID_HANDLE;

  if (this->connector_ != 0)
    this->connector_->on_new_sender (*this);
}

Sender::~Sender (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::~Sender\n")));

  if (this->connector_ != 0)
    this->connector_->on_delete_sender (*this);

  if (this->socket_handle_[ODD] != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->socket_handle_[ODD]);

  if (this->socket_handle_[EVEN] != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->socket_handle_[EVEN]);

  if (this->input_file_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->input_file_handle_);

  if (client_only)
    ACE_Proactor::instance ()->end_event_loop ();
}

//  return true if we alive, false  we commited suicide
void
Sender::check_destroy (void)
{
  if (this->io_count_ <= 0)
    delete this;
}

void
Sender::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  this->socket_handle_[ODD] = handle;

  // Open the input file
  if (ACE_INVALID_HANDLE == (this->input_file_handle_ =
                               ACE_OS::open (input_file,
                                             _O_RDONLY |\
                                             FILE_FLAG_OVERLAPPED |\
                                             FILE_FLAG_NO_BUFFERING,
                                             ACE_DEFAULT_FILE_PERMS)))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Sender::open::ACE_OS::open")));
    }
  else
    {
      // Now connect (w/o the connector factory) to the even (=second)
      // receiver. We don't connect thru the factory in order not to
      // instantiate another Sender.
      ACE_SOCK_Connector sock_connector;
      ACE_SOCK_Stream    sock_stream;
      if (-1 == sock_connector.connect (sock_stream,
                                        this->connector_->get_address ()))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("Sender::open::ACE_SOCK_Connector::connect")));

      else
        {
          this->socket_handle_[EVEN] = sock_stream.get_handle ();

          // Open odd ACE_Asynch_Write_Stream
          if (this->ws_[ODD].open (*this, this->socket_handle_[ODD]) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::open::ACE_Asynch_Write_Stream::open")));

          // Open even ACE_Asynch_Write_Stream
          else if (this->ws_[EVEN].open (*this, this->socket_handle_[EVEN]) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::open::ACE_Asynch_Write_Stream::open")));

          // Open ACE_Asynch_Read_File
          else if (this->rf_.open (*this, this->input_file_handle_) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::open::ACE_Asynch_Read_File::open")));
          else
            // Start an asynchronous read
            this->initiate_read_file ();
        }
    }

  this->check_destroy ();
}

int
Sender::initiate_read_file (void)
{
  ACE_TEST_ASSERT (0 == this->file_offset_ % chunk_size);

  static const size_t file_size = ACE_OS::filesize (input_file);

  static const size_t number_of_chunks_needed_for_file =
    static_cast<size_t> (ACE_OS::ceil ((double) file_size / chunk_size));

  size_t relevant_number_of_chunks =
    ACE_MIN ((size_t)ACE_IOV_MAX,
             number_of_chunks_needed_for_file
             - (size_t)(this->file_offset_ / chunk_size));

  if (!relevant_number_of_chunks)
    {
      ACE_TEST_ASSERT (0); // Just 2 C it coming
      return 0;
    }

  ACE_Message_Block *head_mb = 0;
  if (-1 == allocate_chunks_chain (head_mb, relevant_number_of_chunks))
    {
      ACE_TEST_ASSERT (0);
      return -1;
    }

  // Inititiate read
  if (this->rf_.readv (*head_mb,
                       head_mb->total_size (),
                       this->file_offset_) == -1)
    {
      free_chunks_chain (head_mb);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Sender::initiate_read_file::")
                         ACE_TEXT ("ACE_Asynch_Read_Stream::readv")),
                        -1);
    }

  ++this->io_count_;
  return 0;
}

int
Sender::initiate_write_stream (ACE_Message_Block &mb)
{
  // send the odd to the first connection, and the even to the second
  // connection.

  ACE_Message_Block *odd_mb  = &mb;
  ACE_Message_Block *even_mb = mb.cont ();

  split_odd_even_chains (odd_mb, even_mb);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::initiate_write_stream - (ODD ) writev %d\n"),
              odd_mb->total_length ()));

  if (this->ws_[ODD].writev (*odd_mb, odd_mb->total_length ()) == -1)
    {
      free_chunks_chain (odd_mb);

      if (even_mb)
        free_chunks_chain (even_mb);

      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("Sender::ACE_Asynch_Stream::writev")),
                       -1);
    }

  ++this->io_count_;

  if (even_mb)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Sender::initiate_write_stream - (EVEN) writev %d\n"),
                  even_mb->total_length ()));

      if (this->ws_[EVEN].writev (*even_mb, even_mb->total_length ()) == -1)
        {
          free_chunks_chain (even_mb);

          ACE_ERROR_RETURN((LM_ERROR,
                            ACE_TEXT ("%p\n"),
                            ACE_TEXT ("Sender::ACE_Asynch_Stream::writev")),
                           -1);
        }

      ++this->io_count_;
    }

  return 0;
}

void
Sender::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  ACE_Message_Block *mb = &result.message_block ();

  if (result.error () == 0 && result.bytes_transferred () != 0)
    {
      size_t bytes_transferred = result.bytes_transferred ();
      size_t chunks_chain_size = mb->total_size ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Sender::handle_read_file, read %d, ")
                  ACE_TEXT ("chain total %d\n"),
                  bytes_transferred,
                  chunks_chain_size));

      this->file_offset_ += static_cast<u_long> (bytes_transferred);

      this->initiate_write_stream (*mb);

      // and read more if required
      if (bytes_transferred == chunks_chain_size)
        this->initiate_read_file ();
    }
  else
    free_chunks_chain (mb);

  --this->io_count_;

  this->check_destroy ();
}

void
Sender::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_Message_Block *mb = &result.message_block ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sender::handle_write_stream - wrote %d bytes\n"),
              result.bytes_transferred ()));

  if (result.error () == 0 && result.bytes_transferred () != 0)
    // verify sent all
    ACE_TEST_ASSERT (0 == mb->total_length ());
  else
    ACE_TEST_ASSERT (0);

  free_chunks_chain (mb);

  --this->io_count_;

  this->check_destroy ();
}

// *************************************************************
//   Configuration helpers
// *************************************************************
int
print_usage (int /* argc */, ACE_TCHAR *argv[])
{
  ACE_ERROR
    ((LM_ERROR,
      ACE_TEXT ("\nusage: %s")
      ACE_TEXT ("\n-f <input file>\n")
      ACE_TEXT ("\n-c client only (reader-sender)")
      ACE_TEXT ("\n-s server only (receiver-writer)")
      ACE_TEXT ("\n-h host to connect to")
      ACE_TEXT ("\n-p port")
      ACE_TEXT ("\n-u show this message")
      ACE_TEXT ("\n"),
      argv[0]
   ));
  return -1;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1) // no arguments , so one button test
    return 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:csh:p:u"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'f':
          input_file = get_opt.opt_arg ();
          break;
        case 'c':
          client_only = 1;
          server_only = 0;
          break;
        case 's':
          server_only = 1;
          client_only = 0;
          break;
        case 'h':
          host = get_opt.opt_arg ();
          break;
        case 'p':
          port = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          return print_usage (argc, argv);
        } // switch
    } // while

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Scatter_Gather_Test"));

  if (::parse_args (argc, argv) == -1)
    return -1;

  chunk_size = ACE_OS::getpagesize ();

  if (client_only)
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Running as client only, page size %d\n"),
                chunk_size));
  else if (server_only)
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Running as server only, page size %d\n"),
                chunk_size));
  else
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Running as server and client, page size %d\n"),
                chunk_size));

  Acceptor  acceptor;
  Connector connector;
  ACE_INET_Addr addr (port);

  if (!client_only)
    {
      // Simplify, initial read with zero size
      if (-1 == acceptor.open (addr, 0, 1))
        {
          ACE_TEST_ASSERT (0);
          return -1;
        }
    }

  if (!server_only)
    {
      if (-1 == connector.open (1, ACE_Proactor::instance ()))
        {
          ACE_TEST_ASSERT (0);
          return -1;
        }

      // connect to first destination
      if (addr.set (port, host, 1, addr.get_type ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), host), -1);
      connector.set_address (addr);
      if (-1 == connector.connect (addr))
        {
          ACE_TEST_ASSERT (0);
          return -1;
        }
    }

  ACE_Proactor::instance ()->run_event_loop ();

  // As Proactor event loop now is inactive it is safe to destroy all
  // senders

  connector.stop ();
  acceptor.stop ();

  ACE_Proactor::instance()->close_singleton ();

  // now compare the files - available only when on same machine

  int success = 0;
  if (!client_only && !server_only)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Comparing the input file and the output file...\n")));

      success = -1;
      // map the two files, then perform memcmp
      {
        ACE_Mem_Map original_file (input_file);
        ACE_Mem_Map reconstructed_file (output_file);

        if (original_file.addr () &&
            original_file.addr () != MAP_FAILED &&
            reconstructed_file.addr () &&
            reconstructed_file.addr () != MAP_FAILED)
          {
            // compare lengths
            if ((original_file.size () == reconstructed_file.size ()) &&
                // and if same size, compare file data
                (0 == ACE_OS::memcmp (original_file.addr (),
                                      reconstructed_file.addr (),
                                      original_file.size ())))
              success = 0;
          }
      }

      if (0 == success)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("input file and the output file identical!\n")));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("input file and the output file are different!\n")));
    }

  if (!client_only)
    ACE_OS::unlink (output_file);

  ACE_END_TEST;

  return success;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Scatter_Gather_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous Scatter/Gather IO is unsupported.\n")
              ACE_TEXT ("Proactor_Scatter_Gather_Test will not be run.\n")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_HAS_WIN32_OVERLAPPED_IO */
