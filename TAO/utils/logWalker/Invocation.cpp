// $Id$

#include "Invocation.h"
#include "PeerProcess.h"
#include "PeerObject.h"
#include "Session.h"
#include "HostProcess.h"
#include "Thread.h"

#include "ace/OS_NS_string.h"
#include "ace/CDR_Stream.h"
#include "ace/Log_Msg.h"
#include <stdio.h>

static const char *size_leadin_1_5 = "GIOP v1.2 msg, ";
static const char *size_leadin_1_6 = "GIOP message v1.2, ";
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

static const size_t target_offset = giop_header_len + 12;
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
    sending_(false)
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
        hms = ACE_OS::strchr (hms + 1, ' ');
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
  ACE_OS::memset(this->octets_,this->size_,0);
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
  return cs == this->size_ ? -1 : cs / 16;
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

bool
Invocation::GIOP_Buffer::is_oneway(void) const
{
  if (this->octets_ == 0)
    {
      return false;
    }
  char mtype = this->octets_[7];
  if (mtype != 0) //GIOP_REQUEST
    return false;
  char flag = this->octets_[16]; // GIOP 1.2 request_header.response_flags
  return (flag & 1) == 0;
}
    
size_t
Invocation::GIOP_Buffer::log_posn (void) const
{
  return this->log_offset_;
}

const Thread *
Invocation::GIOP_Buffer::thread (void) const
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
Invocation::GIOP_Buffer::actual_req_id(void) const
{
  if (this->octets_ == 0)
    return 0;
  char vmaj = this->octets_[4];
  char vmin = this->octets_[5];
  char bo = this->octets_[6];
  
  ACE_InputCDR cdr(this->octets_+giop_header_len, 
                   this->cur_size() - giop_header_len,
                   bo, vmaj, vmin);
  size_t rid;
  cdr >> rid;
  return rid;
}

size_t
Invocation::GIOP_Buffer::cur_size(void) const
{
  return this->wr_pos_ - this->octets_;
}

const char *
Invocation::GIOP_Buffer::target_oid(size_t &len) const
{
  if (this->octets_ == 0)
    {
      return 0;
    }
  char mtype = this->octets_[7];
  if (mtype != 0) //GIOP_REQUEST
    return 0;

  char vmaj = this->octets_[4];
  char vmin = this->octets_[5];
  char bo = this->octets_[6];
  ACE_InputCDR cdr(this->octets_ + target_offset,
                   this->cur_size() - target_offset,
                   bo, vmaj, vmin);
  cdr >> len;
  if (target_offset + len > this->cur_size())
    {
      len = 0;
      return 0;
    }
  return this->octets_ + target_offset + 4;
}
 
const char *
Invocation::GIOP_Buffer::operation(void) const
{
  if (octets_ == 0)
    return 0;

  char mtype = this->octets_[7];
  if (mtype != 0) //GIOP_REQUEST
    return 0;

  char vmaj = this->octets_[4];
  char vmin = this->octets_[5];
  char bo = this->octets_[6];
  ACE_InputCDR cdr(this->octets_ + target_offset,
                   this->cur_size() - target_offset,
                   bo, vmaj, vmin);
  ACE_CDR::Long tgt_size;
  cdr.read_long(tgt_size);
  if (target_offset + tgt_size > this->cur_size())
    return 0;
  cdr.skip_bytes(tgt_size);
  cdr.read_long(tgt_size);
  size_t offset = cdr.rd_ptr() - this->octets_;
  if (offset + tgt_size > this->cur_size())
    return 0;
  return this->octets_ + offset;
}

bool
Invocation::GIOP_Buffer::validate (void) const
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


Invocation::Invocation (PeerProcess *peer, size_t rid)
  :req_octets_(0),
   repl_octets_(0),
   peer_(peer),
   req_id_(rid),
   target_(0),
   child_(0),
   sibling_(0),
   dumped_(false)
{
  if (size_leadin == 0)
    {
      size_leadin = Session::tao_version() == 150 ?
        size_leadin_1_5 : size_leadin_1_6;
      leadin_len = ACE_OS::strlen(size_leadin);
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
                  "Not a request preamble:\n '%s'\n",
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


void
Invocation::add_child(Invocation *child)
{
  if (this->child_ == 0)
    this->child_ = child;
  else
    this->child_->add_sibling(child);
}

void
Invocation::add_sibling(Invocation *sibling)
{
  if (this->sibling_ == 0)
    this->sibling_ = sibling;
  else
    this->sibling_->add_sibling(sibling);
}

Invocation *
Invocation::child(void)
{
  return this->child_;
}

Invocation *
Invocation::sibling(void)
{
  return this->sibling_;
}
  
void 
Invocation::dump_detail (ostream &strm, int indent)
{
  if (this->dumped_)
    return;
  this->dumped_ = true;
  const char *opname = "";
  const char *dir_1 = " to ";
  const char *dir_2 = " in ";
  
  if (this->req_octets_ != 0)
    {
      opname = this->req_octets_->operation();
      if (this->req_octets_->sending())
        {
          dir_1 = " for ";
          dir_2 = " from ";
        }
    }

  for (int ind = 0; ind < indent; ind++)
    strm << "  ";

  if (opname == 0)
    opname = "<no operation>";

  strm << " " << this->req_id_ << dir_1;
  if (this->target_ == 0)
    strm << "<unknown object>" ;
  else
    strm << this->target_->name();
  strm << dir_2 << this->peer_->id()
       << " [" << opname << "]\t";
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
      strm << " Request, line " << this->req_octets_->log_posn();
// #if defined (SHOW_THREAD_ID)
      strm << " " << this->req_octets_->thread()->alias();
// #endif
    }
  else
    strm << " <no request found>";
  if (this->is_oneway())
    strm << " oneway";
  else
    {
      if (this->repl_octets_ != 0)
        {
          strm << " Reply, line " << this->repl_octets_->log_posn();
#if defined (SHOW_THREAD_ID)
          strm << " " << this->repl_octets_->thread()->alias();
#endif
        }
      else
        strm << " <no reply found>";
    }
  if (delta > 0)
    strm << " log offset = " << delta;
  strm << endl;
  if (this->child_ != 0)
    child_->dump_detail(strm, indent+1);
  if (this->sibling_ != 0)
    sibling_->dump_detail (strm, indent);
}
