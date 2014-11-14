// $Id$

#include "GIOP_Buffer.h"
#include "ace/OS_NS_string.h"

static const char *size_leadin_1_5 = "GIOP v1."; //"x msg, ";
static size_t leadin_len_1_5 = 15;
static const char *size_leadin_1_6 = "GIOP message v1."; //2, ";
static size_t leadin_len_1_6 = 19;


const char *GIOP_Buffer::size_leadin = 0;
size_t GIOP_Buffer::leadin_len = 0;
const size_t GIOP_Buffer::giop_header_len = 12;

void
GIOP_Buffer::init_leadin (int version)
{
  if (size_leadin == 0)
    {
      if (version == 150)
        {
          size_leadin = size_leadin_1_5;
          leadin_len = leadin_len_1_5;
        }
      else
        {
          size_leadin = size_leadin_1_6;
          leadin_len = leadin_len_1_6;
        }
    }
}

// GIOP 1.2 header: 12 bytes
// Magic: 4
// ver:   2
// flags: 1 // bo
// type:  1 // req/repl/lf/excp
// len:   4
//
// Request 1.2 header:
// req_id: 4
// flags:  1
// RESVD:  3
// Address disposition: 4
// target: <4 len, + len> [0-3 pad]
// opname: <4 len, + len>

// GIOP 1.0 header: 12 bytes
// Magic: 4
// ver:   2
// byte_order: 1
// type:  1 // req/repl/lf/excp
// len:   4
//
// Request 1.0 header:
/*
struct RequestHeader_1_0 { // Renamed from RequestHeader
    IOP::ServiceContextList      service_context;
    unsigned long                request_id;
    boolean                      response_expected;
    sequence <octet>             object_key;
    string                       operation;
    CORBA::OctetSeq              requesting_principal;
};
*/
// service_context: 4(count)+contexts.
//   context id: 4
//   content blob: 4(len) + len octets + (0-3)pad
// request_id: 4
// respexp:  1
// RESVD:  3
// Address disposition: 4
// target: <4 len, + len> [0-3 pad]
// opname: <4 len, + len>

/*
struct ReplyHeader_1_0 { // Renamed from ReplyHeader
         IOP::ServiceContextList      service_context;
         unsigned long                request_id;
         ReplyStatusType_1_0          reply_status;
    };
    // GIOP 1.1
    typedef ReplyHeader_1_0 ReplyHeader_1_1;
    // Same Header contents for 1.0 and 1.1
#else
    // GIOP 1.2, 1.3
    enum ReplyStatusType_1_2 {
         NO_EXCEPTION,
         USER_EXCEPTION,
         SYSTEM_EXCEPTION,
         LOCATION_FORWARD,
         LOCATION_FORWARD_PERM,// new value for 1.2
         NEEDS_ADDRESSING_MODE // new value for 1.2
    };
    struct ReplyHeader_1_2 {
         unsigned long                request_id;
         ReplyStatusType_1_2          reply_status;
         IOP::ServiceContextList      service_context;
    };

 */



//static const size_t target_offset_12 = GIOP_Buffer::giop_header_len + 12;

// 12 = req_id + flags + RESVD + addr disp.

GIOP_Buffer::GIOP_Buffer(void)
  : cdr_ (0),
    preamble_(),
    log_offset_(0),
    thr_(0),
    time_(0),
    expected_req_id_(0),
    expected_size_(0),
    expected_type_(0),
    buffer_size_(0),
    wr_pos_ (0),
    octets_ (0),
    owner_ (0),
    buffer_lost_ (false),
    sending_(false),
    oid_ (0),
    oid_len_ (0),
    opname_ (0),
    req_id_ (0),
    resp_exp_ (0),
    reply_status_ (0),
    ver_minor_ (0),
    msg_size_ (0),
    num_contexts_ (0),
    header_parsed_ (false),
    payload_start_ (0)
{
}

GIOP_Buffer::GIOP_Buffer(const char *text,
                         size_t offset,
                         Thread *thread,
                         Invocation *owner)
  : cdr_ (0),
    preamble_(text),
    log_offset_(offset),
    thr_(thread),
    time_(0),
    expected_req_id_(0),
    expected_size_(0),
    expected_type_(0),
    buffer_size_(0),
    wr_pos_ (0),
    octets_ (0),
    owner_ (owner),
    buffer_lost_ (false),
    sending_(false),
    oid_ (0),
    oid_len_ (0),
    opname_ (0),
    req_id_ (0),
    resp_exp_ (0),
    reply_status_ (0),
    ver_minor_ (0),
    msg_size_ (0),
    num_contexts_ (0),
    header_parsed_ (false),
    payload_start_ (0)
{
  const char *size_str = ACE_OS::strstr(text, size_leadin);
  if (size_str != 0)
    {
      size_str += ACE_OS::strlen (size_leadin);
      size_str += 3;
      this->expected_size_ = ACE_OS::strtol(size_str, 0, 10);
      const char *id = ACE_OS::strchr(size_str, '[') + 1;
      this->expected_req_id_ = ACE_OS::strtol(id, 0, 10);
    }
  this->sending_ = ACE_OS::strstr(text,"send") ? 0 : 1;
  this->expected_type_ = ACE_OS::strstr(text,"Request") ? 0 : 1;
  const char *time_tok = ACE_OS::strchr (text,'@');
  if (time_tok != 0)
    {
      char timebuf[30];
      ACE_OS::strncpy(timebuf, text, (time_tok - text));
      timebuf[time_tok - text] = 0;
      char *hms = ACE_OS::strchr (timebuf,' ');
      if (hms != 0)
        {
          int hr, min, sec, msec;
          ::sscanf (hms+1,"%d:%d:%d.%d", &hr, &min, &sec, &msec);
          this->time_ = (hr * 3600 + min *60 + sec) * 1000 + msec;
        }
    }
}

void
GIOP_Buffer::owner (Invocation *owner)
{
  this->owner_ = owner;
}

Invocation *
GIOP_Buffer::owner (void)
{
  return this->owner_;
}

void
GIOP_Buffer::init_buf (const char *text, size_t offset)
{
  // GIOP message -  HEXDUMP
  this->log_offset_ = offset;
  const char * size_str = ACE_OS::strstr (text,"HEXDUMP ") + 8;
  this->buffer_size_ = ACE_OS::strtol (size_str, 0, 10);
  size_str =  ACE_OS::strstr (text,"showing first ");
  if (size_str != 0)
    {
      size_str += 14;
      this->buffer_size_ = ACE_OS::strtol (size_str, 0, 10);
    }
  if (this->octets_ != 0)
    {
      delete [] this->octets_;
    }
  this->octets_ = new char [this->buffer_size_];
  ACE_OS::memset (this->octets_, 0, this->buffer_size_);
  this->wr_pos_ = this->octets_;
}

GIOP_Buffer::~GIOP_Buffer(void)
{
  delete [] this->octets_;
}

int
GIOP_Buffer::add_octets (const char *text, size_t offset)
{
  if (this->octets_ == 0)
    {
      this->init_buf(text, offset);
      return 0;
    }

  const char *c = text;
  char *err;
  for (int count = 0;
       count < 16 && this->cur_size() < this->buffer_size_;
       count++)
    {
      if (count == 8)
        ++c;
      int o = ::strtol(c, &err, 16);
      if (err == c || *err == 0)
        return -1;
      *this->wr_pos_++ = o;
      c = err+1;
    }
  size_t cs = this->cur_size();
  int rtn = 0;
  if (!this->header_parsed_)
    {
      this->header_parsed_ = this->parse_header();
      if (this->header_parsed_ )
        rtn = 1;
    }
  if (cs == this->buffer_size_)
    {
      char vmaj = this->octets_[4];
      char order = this->octets_[6];

      delete this->cdr_;
      this->cdr_ = new ACE_InputCDR (this->octets_,
                                     this->cur_size(),
                                     order, vmaj, this->ver_minor_);
      this->cdr_->skip_bytes (this->payload_start_ - this->octets_);
      rtn = -1;
    }
  return rtn;
}

bool
GIOP_Buffer::sending (void) const
{
  return this->sending_;
}

bool
GIOP_Buffer::is_full (void) const
{
  return this->buffer_size_ > 0 && this->cur_size() == this->buffer_size_;
}

char
GIOP_Buffer::type (void) const
{
  if (this->octets_ == 0)
    return 127;
  return this->octets_[7];
}

char
GIOP_Buffer::expected_type (void) const
{
  return this->expected_type_;
}

char
GIOP_Buffer::minor_version (void) const
{
  return this->ver_minor_;
}

size_t
GIOP_Buffer::reply_status (void) const
{
  return this->reply_status_;
}

size_t
GIOP_Buffer::num_contexts (void) const
{
  return this->num_contexts_;
}

bool
GIOP_Buffer::is_oneway (void)
{
  if (this->octets_ == 0)
    {
      return false;
    }

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  return (resp_exp_ &1) == 0;
}

size_t
GIOP_Buffer::log_posn (void) const
{
  return this->log_offset_;
}

Thread *
GIOP_Buffer::thread (void)
{
  return this->thr_;
}

time_t
GIOP_Buffer::time (void) const
{
  return this->time_;
}

const ACE_CString &
GIOP_Buffer::preamble (void) const
{
  return this->preamble_;
}

size_t
GIOP_Buffer::expected_size (void) const
{
  return this->expected_size_;
}

size_t
GIOP_Buffer::msg_size (void)
{
  if (this->cur_size() < 12)
    return 0;
  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();
  return this->msg_size_;
}

size_t
GIOP_Buffer::expected_req_id (void) const
{
  return this->expected_req_id_;
}

size_t
GIOP_Buffer::actual_req_id (void)
{
  if (this->octets_ == 0)
    return 0;

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  return this->req_id_;
}

size_t
GIOP_Buffer::cur_size (void) const
{
  return this->wr_pos_ - this->octets_;
}

bool
GIOP_Buffer::parse_svc_contexts (void)
{
  ACE_CDR::ULong temp;
  ACE_CDR::ULong num_svc_cont;
  if ( !(*this->cdr_ >> num_svc_cont))
    return false;
  this->num_contexts_ = static_cast<size_t>(num_svc_cont);
  while (num_svc_cont > 0)
    {
      if (!(*this->cdr_ >> temp))  // tag really
        return false;
      if (!(*this->cdr_ >> temp))
        return false;
      if (!this->cdr_->skip_bytes(temp))
        return false;
      --num_svc_cont;
    }
  return true;
}

bool
GIOP_Buffer::parse_header (void)
{
  if (this->octets_ == 0 || this->cur_size() < 12)
    {
      return false;
    }

  char vmaj = this->octets_[4];
  this->ver_minor_ = this->octets_[5];
  char order = this->octets_[6];

  char mtype = this->octets_[7];
  if (mtype > 1) // not a request or reply
    return false;

  delete this->cdr_;
  this->cdr_ = new ACE_InputCDR (this->octets_,
                                 this->cur_size(),
                                 order, vmaj, this->ver_minor_);
  this->cdr_->skip_bytes (8);
  this->payload_start_ = this->cdr_->rd_ptr();
  ACE_CDR::ULong len_ulong;
  if (!(*this->cdr_ >> len_ulong))
      return false;
  this->msg_size_ = len_ulong;

  if (this->ver_minor_ < 2)
    {
      if (!this->parse_svc_contexts())
        return false;
    }

  if (!(*this->cdr_ >> len_ulong))
    return false;
  this->req_id_ = static_cast<size_t>(len_ulong);

  switch (mtype) {
  case 0: //Request
    if (!(*this->cdr_ >> this->resp_exp_))
      return false;
    if (this->ver_minor_ > 1 &&
        !(*this->cdr_ >> len_ulong)) // address disposition
      return false;
    if (!(*this->cdr_ >> len_ulong))
      return false;
    this->oid_len_ = static_cast<size_t>(len_ulong);
    this->oid_ = this->cdr_->rd_ptr();
    if (!this->cdr_->skip_bytes(len_ulong))
      return false;

    if (!(*this->cdr_ >> len_ulong))
      return false;
    this->opname_ = this->cdr_->rd_ptr();
    if (!this->cdr_->skip_bytes(len_ulong))
      return false;
    break;
  case 1: //Reply
    if (!(*this->cdr_ >> len_ulong))
      return false;
    this->reply_status_ = static_cast<size_t>(len_ulong);
    break;
  default:
    return true;
  }
  if (this->ver_minor_ > 1)
    {
      if (!this->parse_svc_contexts())
        return false;
    }
  this->cdr_->align_read_ptr (8);
  this->payload_start_ = this->cdr_->rd_ptr();
  return true;
}

const char *
GIOP_Buffer::target_oid (size_t &len)
{
  if (this->octets_ == 0)
    {
      return 0;
    }

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  if (this->oid_ == 0)
    return 0;

  len = this->oid_len_;
  return this->oid_;
}

const char *
GIOP_Buffer::operation (void)
{
  if (octets_ == 0)
    return 0;

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  return this->opname_;
}

ACE_InputCDR &
GIOP_Buffer::payload (void)
{
  if (octets_ != 0 && !this->header_parsed_)
    this->header_parsed_ = this->parse_header();
  return *this->cdr_;
}

bool
GIOP_Buffer::has_octets (void) const
{
  return (octets_ != 0);
}

bool
GIOP_Buffer::validate (void)
{
  return
    this->expected_req_id_ == this->actual_req_id() &&
    this->expected_type_ == this->type() &&
    this->expected_size_ == this->msg_size();
}

bool
GIOP_Buffer::matches (GIOP_Buffer *other) const
{
  if (other->header_parsed_)
    return this->expected_req_id_ == other->actual_req_id() &&
      this->expected_type_ == other->type() &&
      (this->expected_size_ == other->msg_size() ||
       this->expected_size_ == other->msg_size() + 4);
  else
    return this->expected_req_id_ == other->expected_req_id() &&
      this->expected_type_ == other->expected_type() &&
      this->sending_ == other->sending() &&
      this->expected_size_ == other->expected_size();
}

void
GIOP_Buffer::reset (void)
{
  this->octets_ = 0;
  this->wr_pos_ = 0;
  this->buffer_size_ = 0;
  this->buffer_lost_ = true;
  this->header_parsed_ = false;
  this->opname_ = 0;
}

void
GIOP_Buffer::transfer_from (GIOP_Buffer *other)
{
  delete this->octets_;
  this->octets_ = other->octets_;
  this->wr_pos_ = other->wr_pos_;
  this->buffer_size_ = other->buffer_size_;
  this->header_parsed_ = false;
  other->reset();
}

void
GIOP_Buffer::swap (GIOP_Buffer *other)
{
  char *tmp_octets = this->octets_;
  char *tmp_wr_pos = this->wr_pos_;
  size_t tmp_size = this->buffer_size_;

  this->octets_ = other->octets_;
  this->wr_pos_ = other->wr_pos_;
  this->buffer_size_ = other->buffer_size_;

  other->octets_ = tmp_octets;
  other->wr_pos_ = tmp_wr_pos;
  other->buffer_size_ = tmp_size;
}
