// $Id$

#include "ace/FILE_Addr.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"
#include "Options.h"
#include "Mem_Map_Stream.h"



ACE_SOCK_Stream &
Mem_Map_Stream::stream (void)
{
  return svc_handler_->peer ();
}

ssize_t
Mem_Map_Stream::send_n (const void *buf, size_t size, ACE_Time_Value *tv)
{
  return svc_handler_->peer ().send_n (buf, size, 0, tv);
}

int
Mem_Map_Stream::eof (void) const
{
  return this->get_pos_ >= this->end_of_mapping_plus1_;
}

int
Mem_Map_Stream::get_char (void)
{
  if (this->eof () && this->grow_file_and_remap () == -1)
    return EOF;

  return *this->get_pos_++;
}

int
Mem_Map_Stream::rewind (void)
{
  this->recv_pos_ =
    reinterpret_cast<char *> (this->mem_map_.addr ());
  this->get_pos_ = this->recv_pos_;
  this->end_of_mapping_plus1_ =
    this->recv_pos_ + this->mem_map_.size ();
  return 0;
}

int
Mem_Map_Stream::peek_char (size_t offset)
{
  // We may need to iterate if the size of <n> is large.
  while (this->get_pos_ + offset >= this->end_of_mapping_plus1_)
    if (this->grow_file_and_remap () == -1)
      return EOF;

  return this->get_pos_[offset];
}

const char *
Mem_Map_Stream::recv (void) const
{
  return this->recv_pos_;
}

const char *
Mem_Map_Stream::recv (size_t &len)
{
  if (this->eof () && this->grow_file_and_remap () == -1)
    return 0;


  const char *s = this->recv_pos_;
  ACE_OFF_T olen = static_cast <ACE_OFF_T> (len);
  this->seek (olen, SEEK_CUR);
  len = this->get_pos_ - s;
  return s;
}

size_t
Mem_Map_Stream::recv_len (void) const
{
  return this->get_pos_ - this->recv_pos_;
}

const char *
Mem_Map_Stream::peek_str (size_t offset,
                          size_t size)
{
  // We will iterate if the size of <offset> is large.
  while (this->get_pos_ + (offset + size) > this->end_of_mapping_plus1_)
    if (this->grow_file_and_remap () == -1)
      return 0;

  return &this->get_pos_[offset];
}

ACE_OFF_T
Mem_Map_Stream::seek (ACE_OFF_T offset, int whence)
{
  switch (whence)
    {
    case SEEK_SET:
      this->get_pos_ =
        reinterpret_cast<char *> (this->mem_map_.addr ())
        + offset;
      break;

    case SEEK_CUR:
      this->get_pos_ += offset;
      break;

    case SEEK_END:
      this->get_pos_ =
        this->end_of_mapping_plus1_ + offset;
      // @@ Not sure how to implement this (yet).
      ACE_NOTSUP_RETURN (-1);
    }

  // Make sure that the backing store will cover this.
  while (this->get_pos_ > this->end_of_mapping_plus1_)
    if (this->grow_file_and_remap () == -1)
      return (ACE_OFF_T) -1;

  this->recv_pos_ = this->get_pos_;
  return
    ACE_Utils::truncate_cast<ACE_OFF_T> (
      this->recv_pos_ - reinterpret_cast<char *> (this->mem_map_.addr ()));
}

Mem_Map_Stream::Svc_Handler *
Mem_Map_Stream::svc_handler (void)
{
  return this->svc_handler_;
}


int
Mem_Map_Stream::open (STRAT_CONNECTOR *connector,
                      const ACE_INET_Addr &addr)
{
  svc_handler_ = 0;

  // Connect to the server at <addr>. If the handler has to be
  // connected to the server again, the Caching strategy takes care
  // and uses the same connection.
  if (connector->connect (svc_handler_,
                          addr) == -1)
    {

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p %s %d\n",
                         "Connect failed",
                         addr.get_host_name (),
                         addr.get_port_number ()),
                        -1);
    }
  // Create a temporary filename.
  ACE_FILE_Addr file (ACE_sap_any_cast (ACE_FILE_Addr &));

  // Create the temporary file via the <ACE_Mem_Map> class API.
  if (this->mem_map_.open (file.get_path_name (),
                          O_RDWR | O_CREAT | O_APPEND,
                          ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  // Make sure to unlink this right away so that if this process
  // crashes these files will be removed automatically.
#if 0
  else if (ACE_OS::unlink (file.get_path_name ()) == -1)
  ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "unlink"),
                      -1);
  else
#endif
    // Initialize all the position pointers to 0.
    this->rewind ();

  return 0;
}

int
Mem_Map_Stream::grow_file_and_remap (void)
{
  char buf[BUFSIZ + 1];

  // Copy the next chunk of bytes from the socket into the temporary
  // file.
  ACE_Time_Value tv (*OPTIONS::instance ()->timeout ());

  ssize_t n = this->svc_handler_->peer ().recv_n (buf,
                                                  sizeof buf,
                                                  0,
                                                  &tv);
  if (n == -1)
    {
      if (OPTIONS::instance ()->debug ())
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "recv"));
      return -1;
    }
  else if (n == 0)
    return -1;
  else if (ACE::write_n (this->mem_map_.handle (), buf, n) != n)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "write_n"),
                      -1);

  // Grow the memory-mapping to encompass the entire temporary file.
  if (this->mem_map_.map (static_cast<size_t> (-1),
                          PROT_RDWR,
                          ACE_MAP_PRIVATE | ACE_MAP_FIXED,
                          ACE_DEFAULT_BASE_ADDR) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "map"),
                      -1);
  // MAP_FAILED is used as a "first time in" flag.
  if (this->recv_pos_ == MAP_FAILED)
    {
      this->recv_pos_ = reinterpret_cast<char *> (this->mem_map_.addr ());
      this->get_pos_ = this->recv_pos_;
    }

  this->end_of_mapping_plus1_ =
    reinterpret_cast<char *> (this->mem_map_.addr ())
    + this->mem_map_.size ();

  return 0;
}

Mem_Map_Stream::~Mem_Map_Stream (void)
{
  // Remove the mapping and the file.
  this->mem_map_.remove ();
}

