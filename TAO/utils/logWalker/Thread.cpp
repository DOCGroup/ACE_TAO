// $Id$

#include "Thread.h"
#include "Invocation.h"
#include "PeerProcess.h"
#include "GIOP_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include <stack>

Thread::Thread (long tid, const char *alias, size_t offset)
  : id_(tid),
    alias_ (alias),
    max_depth_ (0),
    client_encounters_ (0),
    server_encounters_ (0),
    nested_ (0),
    pending_(),
    incoming_(0),
    new_connection_(),
    giop_target_(0),
    target_dup_(0),
    current_invocation_ (),
    active_handle_ (0),
    first_line_ (offset)
{
}

void
Thread::push_new_connection (PeerProcess *pp)
{
  this->new_connection_.push (pp);
}

PeerProcess *
Thread::pop_new_connection (void)
{
  PeerProcess *pp = 0;
  this->new_connection_.pop (pp);
  return pp;
}

PeerProcess *
Thread::peek_new_connection (void) const
{
  PeerProcess *pp = 0;
  this->new_connection_.top (pp);
  return pp;
}

void
Thread::pending_local_addr (const ACE_CString &addr)
{
  this->pending_local_addr_ = addr;
}

const ACE_CString &
Thread::pending_local_addr (void) const
{
  return this->pending_local_addr_;
}

void
Thread::handle_request (void)
{
  this->server_encounters_++;
  if (this->pending_.size() > 1)
    this->nested_++;
}

void
Thread::enter_wait (PeerProcess *pp)
{
  this->pending_.push (pp);
  this->client_encounters_++;
  if (this->pending_.size() > this->max_depth_)
    this->max_depth_ = this->pending_.size();
  if (this->pending_.size() > 1)
    this->nested_++;
}

void
Thread::exit_wait (PeerProcess *pp, size_t linenum)
{
  PeerProcess *old;
  if (this->pending_.pop(old) == -1)
    return;
  while (old != pp)
    {
      ACE_ERROR ((LM_ERROR,
                  "Line %d, Ending an invocation to peer %s, but most recent started"
                  " is to peer %s\n", linenum, pp->id(), old->id()));
      //      this->pending_.push(old);
  if (this->pending_.pop(old) == -1)
    return;
    }
}

long
Thread::max_depth (void) const
{
  return static_cast<long> (this->max_depth_);
}

long
Thread::id (void) const
{
  return this->id_;
}

const ACE_CString &
Thread::alias (void) const
{
  return this->alias_;
}

void
Thread::incoming_from (PeerProcess  *pp)
{
  this->incoming_ = pp;
}

PeerProcess *
Thread::incoming (void) const
{
  return this->incoming_;
}

void
Thread::active_handle (long handle)
{
  this->active_handle_ = handle;
}

long
Thread::active_handle (void) const
{
  return this->active_handle_;
}

void
Thread::set_dup (Thread *other, bool set_other)
{
  this->target_dup_ = other;
  if (set_other)
    {
      other->set_dup (this, false);
    }
}

void
Thread::clear_dup (void)
{
  this->target_dup_ = 0;
}

bool
Thread::has_dup (void)
{
  return this->target_dup_ != 0;
}

void
Thread::swap_target (void)
{
  if (target_dup_ != 0 && target_dup_->giop_target() != 0)
    {
      this->giop_target_->swap (target_dup_->giop_target());
      this->target_dup_->clear_dup ();
      this->target_dup_ = 0;
    }
  else
    {
      if (target_dup_ == 0)
        ACE_ERROR ((LM_ERROR, "Thread::swap_target, target_dup_ == 0\n"));
      else
        ACE_ERROR ((LM_ERROR, "Thread::swap_target, target_dup_.id = %d, giop_target == 0\n", target_dup_->id()));
    }
}

GIOP_Buffer *
Thread::giop_target (void)
{
  return this->giop_target_;
}

void
Thread::set_giop_target (GIOP_Buffer *buffer)
{
  this->giop_target_ = buffer;
  if (this->target_dup_ != 0)
    {
      this->target_dup_->clear_dup();
      this->target_dup_ = 0;
    }
}

void
Thread::add_invocation (Invocation *inv)
{
  this->invocations_.insert_tail (inv);
}

void
Thread::push_invocation (Invocation *inv)
{
  this->current_invocation_.push(inv);
}

void
Thread::pop_invocation (void)
{
  Invocation *inv;
  this->current_invocation_.pop (inv);
}

Invocation *
Thread::current_invocation (void) const
{
  Invocation *inv = 0;
  if (this->current_invocation_.size() > 0)
    this->current_invocation_.top(inv);
  return inv;
}

void
Thread::dump_detail (ostream &strm) const
{
  strm << "   " << this->alias_ << " tid = 0x" << hex << this->id_
       << "\tfirst line " << dec << this->first_line_ << "\t"
       << this->server_encounters_ << " requests sent "
       << this->client_encounters_ << " requests received";
  if (nested_ > 0)
    strm <<", with " << this->nested_ << " nested upcalls, max depth "
         << this->max_depth_;
  strm << endl;
}

void
Thread::get_summary (long &sent_reqs,
                     long &recv_reqs,
                     size_t &sent_size,
                     size_t &recv_size)
{
  for (ACE_DLList_Iterator <Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      Invocation *inv;
      i.next(inv);
      if (inv->sent_request())
        {
          ++sent_reqs;
          sent_size += inv->request_bytes();
        }
      else
        {
          ++recv_reqs;
          recv_size += inv->request_bytes();
        }
    }
}

void
Thread::dump_invocations (ostream &strm)
{
  size_t total_request_bytes = 0;
  strm << "   " << this->alias_ << " handled " << this->invocations_.size()
       << " invocations" << endl;

  std::stack<Invocation *> nested;
  for (ACE_DLList_Iterator <Invocation> i (this->invocations_);
       !i.done();
       i.advance())
    {
      Invocation *inv;
      i.next(inv);
      size_t level = 0;
      while (!nested.empty ())
        {
          if (nested.top()->contains (inv->req_line ()))
            {
              level = nested.size();
              break;
            }
          nested.pop();
        }
      nested.push(inv);

      inv->dump_detail (strm, level, Invocation::Dump_Proc, false);
      total_request_bytes += inv->request_bytes();
    }
  strm << "total request octet count: " << total_request_bytes;
}
