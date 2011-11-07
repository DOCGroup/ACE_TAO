// $Id$

#include "Invocation.h"
#include "PeerProcess.h"
#include "PeerObject.h"
#include "Session.h"
#include "HostProcess.h"
#include "Thread.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include <stdio.h>

static const char *size_leadin_1_5 = "GIOP v1."; //"x msg, ";
static size_t leadin_len_1_5 = 15;
static const char *size_leadin_1_6 = "GIOP message v1."; //2, ";
static size_t leadin_len_1_6 = 19;


static const char *size_leadin = 0;
static size_t leadin_len = 0;
static const size_t giop_header_len = 12;

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



static const size_t target_offset_12 = giop_header_len + 12;

// 12 = req_id + flags + RESVD + addr disp.

Invocation::GIOP_Buffer::GIOP_Buffer(const char *text,
                                     size_t offset,
                                     Thread *thread,
                                     Invocation *owner)
  : preamble_(text),
    log_offset_(offset),
    thr_(thread),
    time_(0),
    expected_req_id_(0),
    expected_size_(0),
    expected_type_(0),
    size_(0),
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
    num_contexts_ (0),
    header_parsed_ (false)

{
  const char *size_str = ACE_OS::strstr(text, size_leadin) + leadin_len;
  const char *id = size_str == 0 ? 0 : ACE_OS::strchr(size_str, '[') + 1;
  this->sending_ = ACE_OS::strstr(text,"send") ? 0 : 1;
  this->expected_type_ = ACE_OS::strstr(text,"Request") ? 0 : 1;
  this->expected_size_ = ACE_OS::strtol(size_str, 0,10) + giop_header_len;
  this->expected_req_id_ = ACE_OS::strtol(id, 0, 10);
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
Invocation::GIOP_Buffer::owner (Invocation *owner)
{
  this->owner_ = owner;
}

Invocation *
Invocation::GIOP_Buffer::owner (void)
{
  return this->owner_;
}

void
Invocation::GIOP_Buffer::init_buf (const char *text)
{
  // GIOP message -  HEXDUMP
  const char * size_str = ACE_OS::strstr(text,"HEXDUMP ") + 8;
  this->size_ = ACE_OS::strtol(size_str, 0,10);
  size_str =  ACE_OS::strstr(text,"showing first ");
  if (size_str != 0)
    {
      size_str += 14;
      this->size_ = ACE_OS::strtol(size_str, 0, 10);
    }
  this->octets_ = new char [this->size_];
  ACE_OS::memset(this->octets_,0,this->size_);
  this->wr_pos_ = this->octets_;
}

Invocation::GIOP_Buffer::~GIOP_Buffer(void)
{
  delete [] this->octets_;
}

int
Invocation::GIOP_Buffer::add_octets(const char *text)
{
  if (this->octets_ == 0)
    {
      this->init_buf(text);
      return 0;
    }

  const char *c = text;
  char *err;
  for (int count = 0; count < 16 && this->cur_size() < this->size_; count++)
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
  if (cs == this->size_)
    return -1;
  else
    if (!this->header_parsed_)
      {
        this->header_parsed_ = this->parse_header();
        if (this->header_parsed_ )
          return 1;
      }
  return 0;
}

bool
Invocation::GIOP_Buffer::sending (void) const
{
  return this->sending_;
}

bool
Invocation::GIOP_Buffer::is_full(void) const
{
  return this->size_ > 0 && this->cur_size() == this->size_;
}

char
Invocation::GIOP_Buffer::type (void) const
{
  if (this->octets_ == 0)
    return 127;
  return this->octets_[7];
}

char
Invocation::GIOP_Buffer::expected_type (void) const
{
  return this->expected_type_;
}

char
Invocation::GIOP_Buffer::minor_version (void) const
{
  return this->ver_minor_;
}

char
Invocation::GIOP_Buffer::reply_status (void) const
{
  return this->reply_status_;
}

size_t
Invocation::GIOP_Buffer::num_contexts (void) const
{
  return this->num_contexts_;
}

bool
Invocation::GIOP_Buffer::is_oneway(void)
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
Invocation::GIOP_Buffer::log_posn (void) const
{
  return this->log_offset_;
}

Thread *
Invocation::GIOP_Buffer::thread (void)
{
  return this->thr_;
}

time_t
Invocation::GIOP_Buffer::time (void) const
{
  return this->time_;
}

const ACE_CString &
Invocation::GIOP_Buffer::preamble(void) const
{
  return this->preamble_;
}

size_t
Invocation::GIOP_Buffer::expected_size (void) const
{
  return this->expected_size_;
}

size_t
Invocation::GIOP_Buffer::size(void) const
{
  return this->size_;
}

size_t
Invocation::GIOP_Buffer::expected_req_id(void) const
{
  return this->expected_req_id_;
}

size_t
Invocation::GIOP_Buffer::actual_req_id(void)
{
  if (this->octets_ == 0)
    return 0;

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  return this->req_id_;
}

size_t
Invocation::GIOP_Buffer::cur_size(void) const
{
  return this->wr_pos_ - this->octets_;
}

bool
Invocation::GIOP_Buffer::parse_svc_contexts (ACE_InputCDR &cdr)
{
  ACE_CDR::ULong temp;
  ACE_CDR::ULong num_svc_cont;
  if ( !(cdr >> num_svc_cont))
    return false;
  this->num_contexts_ = static_cast<size_t>(num_svc_cont);
  while (num_svc_cont > 0)
    {
      if (!(cdr >> temp))  // tag really
        return false;
      if (!(cdr >> temp))
        return false;
      if (!cdr.skip_bytes(temp))
        return false;
      --num_svc_cont;
    }
  return true;
}

bool
Invocation::GIOP_Buffer::parse_header (void)
{
  if (this->octets_ == 0)
    {
      return false;
    }

  char vmaj = this->octets_[4];
  this->ver_minor_ = this->octets_[5];
  char bo = this->octets_[6];

  char mtype = this->octets_[7];
  if (mtype > 1) // not a request or reply
    return false;

  ACE_InputCDR cdr(this->octets_ + giop_header_len,
                   this->cur_size() - giop_header_len,
                   bo, vmaj, this->ver_minor_);

  ACE_CDR::ULong len_ulong;

  if (this->ver_minor_ < 2)
    {
      if (!this->parse_svc_contexts(cdr))
        return false;
    }

  if (!(cdr >> len_ulong))
    return false;
  this->req_id_ = static_cast<size_t>(len_ulong);

  switch (mtype) {
  case 0: //Request
    if (!(cdr >> this->resp_exp_))
      return false;
    if (this->ver_minor_ > 1 &&
        !(cdr >> len_ulong)) // address disposition
      return false;
    if (!(cdr >> len_ulong))
      return false;
    this->oid_len_ = static_cast<size_t>(len_ulong);
    this->oid_ = cdr.rd_ptr();
    if (!cdr.skip_bytes(len_ulong))
      return false;

    if (!(cdr >> len_ulong))
      return false;
    this->opname_ = cdr.rd_ptr();
    if (!cdr.skip_bytes(len_ulong))
      return false;
    break;
  case 1: //Reply
    if (!(cdr >> len_ulong))
      return false;
    this->reply_status_ = static_cast<size_t>(len_ulong);
    break;
  default:
    return false;
  }
  if (this->ver_minor_ > 1)
    {
      if (!this->parse_svc_contexts(cdr))
        return false;
    }

  return true;
}

const char *
Invocation::GIOP_Buffer::target_oid(size_t &len)
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
Invocation::GIOP_Buffer::operation(void)
{
  if (octets_ == 0)
    return 0;

  if (!this->header_parsed_)
    this->header_parsed_ = this->parse_header();

  return this->opname_;
}

bool
Invocation::GIOP_Buffer::validate (void)
{
  return this->expected_req_id_ == this->actual_req_id() &&
    this->expected_type_ == this->type();
}

bool
Invocation::GIOP_Buffer::matches (Invocation::GIOP_Buffer *other) const
{
  return this->expected_req_id_ == other->actual_req_id() &&
    this->expected_type_ == other->type() &&
    this->expected_size_ == other->size();
}

void
Invocation::GIOP_Buffer::reset (void)
{
  this->octets_ = 0;
  this->wr_pos_ = 0;
  this->size_ = 0;
  this->buffer_lost_ = true;
}

void
Invocation::GIOP_Buffer::transfer_from (GIOP_Buffer *other)
{
  delete this->octets_;
  this->octets_ = other->octets_;
  this->wr_pos_ = other->wr_pos_;
  this->size_ = other->size_;
  other->reset();
}


//----------------------------------------------------------------------------


Invocation::Invocation (PeerProcess *peer, long handle, size_t rid)
  :req_octets_(0),
   repl_octets_(0),
   peer_(peer),
   req_id_(rid),
   target_(0),
   handle_(handle)
{
  if (size_leadin == 0)
    {
      if (Session::tao_version() == 150)
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

Invocation::~Invocation (void)
{
  delete this->req_octets_;
  delete this->repl_octets_;
}

bool
Invocation::init (const char * text, size_t offset, Thread *thread)
{
  const char *size_str = ACE_OS::strstr(text, size_leadin);
  const char *id = size_str == 0 ? 0 : ACE_OS::strchr(size_str, '[');
  if (size_str == 0 || id == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Not a request preamble line %d:\n '%s'\n",offset,
                  text));
      return false;
  }


  if( ACE_OS::strstr(text,"Request") == 0)
    this->repl_octets_ = new GIOP_Buffer(text, offset, thread, this);
  else
    this->req_octets_  = new GIOP_Buffer(text, offset, thread, this);
  return true;
}

bool
Invocation::is_oneway(void) const
{
  return this->req_octets_ == 0 ? false : this->req_octets_->is_oneway();
}

void
Invocation::set_target (const char *oid, size_t oid_len)
{
  PeerObject *tgt = this->peer_->object_for (oid, oid_len);
  if (tgt == 0)
    return;

  if (this->target_ == 0)
    {
      this->target_ = tgt;
      this->target_->add_invocation (this);
    }
}

bool
Invocation::message_complete (void)
{
  if (this->is_oneway())
    {
      if (this->req_octets_ == 0 || !this->req_octets_->is_full())
        return false;
    }
  else
    if (this->repl_octets_ == 0 || !this->repl_octets_->is_full())
      return false;

  return true;
}

Invocation::GIOP_Buffer *
Invocation::octets (bool request)
{
  return request ? this->req_octets_ : this->repl_octets_ ;
}

Invocation::GIOP_Buffer *
Invocation::give_octets (bool request)
{
  GIOP_Buffer *result = request ? this->req_octets_ : this->repl_octets_ ;

  if (request)
    this->req_octets_ = 0;
  else
    this->repl_octets_ = 0;
  return result;
}

void
Invocation::set_octets (bool request, GIOP_Buffer *octets)
{
  if (request)
    {
      if (this->req_octets_ != 0)
        {
          return;
        }
      this->req_octets_ = octets;
    }
  else
    {
      if (this->repl_octets_ != 0)
        {
          return;
        }
      this->repl_octets_ = octets;
    }
  octets->owner(this);
}

size_t
Invocation::request_id (void) const
{
  return this->req_octets_ == 0 ? this->req_id_ : this->req_octets_->expected_req_id();
}

size_t
Invocation::expected_size (void) const
{
  if (repl_octets_ != 0)
    return repl_octets_->expected_size();
  return req_octets_->expected_size();
}

long
Invocation::handle (void) const
{
  return this->handle_;
}

bool
Invocation::contains (size_t line)
{
  if (this->req_octets_ == 0 || this->repl_octets_ == 0)
    return false;
  return
    line > this->req_octets_->log_posn() &&
    line < this->repl_octets_->log_posn();
}

size_t
Invocation::req_line (void)
{
  return this->req_octets_ == 0 ? 0 : this->req_octets_->log_posn();
}

void
Invocation::dump_detail (ostream &strm, int indent, Dump_Mode mode, bool show_handle)
{
  const char *opname = "";
  const char *dir_1 = "to ";
  const char *dir_2 = " in ";

  if (this->req_octets_ != 0)
    {
      opname = this->req_octets_->operation();
      if (this->req_octets_->sending())
        {
          dir_1 = "for ";
          dir_2 = " from ";
        }
    }

  while (indent-- > 0)
    strm << "  ";

  if (opname == 0 || opname[0] == 0)
    opname = "<no operation>";

  strm << dir_1;
  if (this->target_ == 0)
    strm << "<unknown object>" ;
  else
    strm << this->target_->name();
  if (mode == Dump_Proc || mode == Dump_Both)
    strm << dir_2 << this->peer_->id() << ",";

  strm << " req " << this->req_id_;
  if (show_handle)
    strm << "(h=" << this->handle_ << ")";
  strm << " [" << opname << "]\t";
  time_t req_time = 0;
  time_t rep_time = 0;
  long delta = 0;
  if (!this->is_oneway() && this->req_octets_ != 0)
    {
      req_time = this->req_octets_->time();
      if (this->repl_octets_ != 0)
        {
          rep_time = this->repl_octets_->time();
          delta = this->repl_octets_->log_posn() - this->req_octets_->log_posn();
        }
    }
  if (req_time != 0 && rep_time != 0)
    strm << " took " << (rep_time - req_time) << " ms";

  if (this->req_octets_ != 0)
    {
      strm << " Request, ";
      if (this->req_octets_->num_contexts() > 0)
        strm << "with " << this->req_octets_->num_contexts() << " contexts, ";
      strm << "line " << this->req_octets_->log_posn();
      if (mode == Dump_Thread || mode == Dump_Both)
        strm << " " << this->req_octets_->thread()->alias();
    }
  else
    strm << " <no request found>";
  if (this->is_oneway())
    strm << " oneway";
  else
    {
      if (this->repl_octets_ != 0)
        {
          strm << " Reply, ";
          if (this->repl_octets_->num_contexts() > 0)
            strm << "with " << this->repl_octets_->num_contexts() << " contexts, ";
          strm << "line " << this->repl_octets_->log_posn();
#if defined (SHOW_THREAD_ID)
          strm << " " << this->repl_octets_->thread()->alias();
#endif
          char rstat = this->repl_octets_->reply_status();
          switch (rstat)
            {
            case 0:
              break;
            case 1:
              strm << " User Exception";
              break;
            case 2:
              strm << " System Exception";
              break;
            case 3:
              strm << " Location Forward";
              break;
            default:
              strm << " status = " << static_cast<short>(rstat);
            }
        }
      else
        strm << " <no reply found>";
    }
  if (delta > 0)
    strm << " log span = " << delta;
  strm << endl;
}
