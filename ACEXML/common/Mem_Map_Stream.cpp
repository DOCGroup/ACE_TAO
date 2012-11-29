// $Id$

#include "ace/FILE_Addr.h"
#include "ace/OS_NS_unistd.h"
#include "ACEXML/common/Mem_Map_Stream.h"



ACEXML_Mem_Map_Stream::ACEXML_Mem_Map_Stream (void)
  : svc_handler_ (0)
{

}

ACE_SOCK_Stream &
ACEXML_Mem_Map_Stream::stream (void)
{
  return svc_handler_->peer ();
}

ssize_t
ACEXML_Mem_Map_Stream::send_n (const void *buf, size_t size,
                               ACE_Time_Value *tv)
{
  return svc_handler_->peer ().send_n (buf, size, 0, tv);
}

int
ACEXML_Mem_Map_Stream::eof (void) const
{
  return this->get_pos_ >= this->end_of_mapping_plus1_;
}

int
ACEXML_Mem_Map_Stream::get_char (void)
{
  if (this->eof () && this->grow_file_and_remap () == -1)
    return EOF;
  return *this->get_pos_++;
}

void
ACEXML_Mem_Map_Stream::rewind (void)
{
  this->recv_pos_ = reinterpret_cast<char *> (this->mem_map_.addr ());
  this->get_pos_ = this->recv_pos_;
  this->end_of_mapping_plus1_ = this->recv_pos_ + this->mem_map_.size ();
}

int
ACEXML_Mem_Map_Stream::peek_char (size_t offset)
{
  // We may need to iterate if the size of <n> is large.
  while (this->get_pos_ + offset >= this->end_of_mapping_plus1_)
    if (this->grow_file_and_remap () == -1)
      return EOF;

  return this->get_pos_[offset];
}

const char *
ACEXML_Mem_Map_Stream::recv (void) const
{
  return this->recv_pos_;
}

const char *
ACEXML_Mem_Map_Stream::recv (size_t &len)
{
  if (this->eof () && this->grow_file_and_remap () == -1)
    {
      len = 0;
      return 0;
    }
  const char *s = this->recv_pos_;
  this->seek (static_cast<ACE_OFF_T> (len), SEEK_CUR);
  len = this->get_pos_ - s;
  return s;
}

size_t
ACEXML_Mem_Map_Stream::recv_len (void) const
{
  return this->get_pos_ - this->recv_pos_;
}

const char *
ACEXML_Mem_Map_Stream::peek_str (size_t offset,
                                 size_t size)
{
  // We will iterate if the size of <offset> is large.
  while (this->get_pos_ + (offset + size) > this->end_of_mapping_plus1_)
    if (this->grow_file_and_remap () == -1)
      return 0;

  return &this->get_pos_[offset];
}

ACE_OFF_T
ACEXML_Mem_Map_Stream::seek (ACE_OFF_T offset, int whence)
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
      this->get_pos_ = this->end_of_mapping_plus1_;

  this->recv_pos_ = this->get_pos_;

  return
    ACE_Utils::truncate_cast<ACE_OFF_T> (
      this->recv_pos_ - reinterpret_cast<char *> (this->mem_map_.addr ()));
}

Svc_Handler *
ACEXML_Mem_Map_Stream::svc_handler (void)
{
  return this->svc_handler_;
}

size_t
ACEXML_Mem_Map_Stream::available (void) const
{
  return this->end_of_mapping_plus1_ - this->get_pos_;
}

int
ACEXML_Mem_Map_Stream::open (Connector *connector,
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
  else if (ACE_OS::unlink (file.get_path_name ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "unlink"),
                      -1);
  else
    // Initialize all the position pointers to 0.
    this->rewind ();

  return 0;
}

int
ACEXML_Mem_Map_Stream::grow_file_and_remap (void)
{
  char buf[8192];

  // Copy the next chunk of bytes from the socket into the temporary
  // file.
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ssize_t bytes = 0;
  ssize_t n = 0;
  while (1)
    {
      n = this->svc_handler_->peer ().recv (buf, sizeof buf, 0, &tv);
      if (n < 0)
        {
          if (errno != EWOULDBLOCK)
            {
              ACE_ERROR ((LM_ERROR, "%p\n", "recv"));
            }

          return -1;
        }
      bytes += n;
      if (n == 0 && !bytes)
        return -1;
      else if (n == 0)
        break;
      else if (ACE::write_n (this->mem_map_.handle (), buf, n) != n)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "write_n"),
                          -1);
    }

//   ssize_t n = this->svc_handler_->peer ().recv (buf, sizeof buf, 0, &tv);
//   if (n == -1)
//     {
//       ACE_ERROR ((LM_ERROR, "%p\n", "recv"));
//       return -1;
//     }
//   else if (n == 0)
//     return -1;
//   else if (ACE::write_n (this->mem_map_.handle (), buf, n) != n)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "%p\n",
//                            "write_n"),
//                           -1);

  // Grow the memory-mapping to encompass the entire temporary file.
  if (this->mem_map_.map (static_cast<size_t> (-1),
                          PROT_RDWR,
                          ACE_MAP_PRIVATE,
                          (void*)0) == -1)
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

ACEXML_Mem_Map_Stream::~ACEXML_Mem_Map_Stream (void)
{
  // Remove the mapping and the file.
  this->mem_map_.remove ();
  delete this->svc_handler_;
}

