// $Id$

#include "Thread.h"
#include "Invocation.h"
#include "PeerProcess.h"

#include "ace/OS_NS_stdio.h"

Thread::Thread (long tid, const char *alias)
  : id_(tid),
    alias_ (alias),
    max_depth_ (0),
    encounters_ (0),
    nested_ (0),
    pending_(),
    incoming_(0),
    new_connection_(0),
    giop_target_(0)
{
}

void
Thread::pending_peer (PeerProcess *pp)
{
  this->new_connection_ = pp;
}

PeerProcess *
Thread::pending_peer (void) const
{
  return this->new_connection_;
}

void 
Thread::enter_wait (PeerProcess *pp)
{
  this->pending_.push (pp);
  this->encounters_++;
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
  return this->max_depth_;
}

long 
Thread::encounters (void) const
{
  return this->encounters_;
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

Invocation::GIOP_Buffer *
Thread::giop_target (void)
{
  return this->giop_target_;
}

void 
Thread::set_giop_target (Invocation::GIOP_Buffer *buffer)
{
  this->giop_target_ = buffer;
}

void
Thread::dump_detail (ostream &strm)
{
  strm << "   " << this->alias_ << " tid = " << this->id_ 
       << "\t" << this->encounters_ << " encounters";
  if (nested_ > 0)
    strm <<", with " << this->nested_ << " nested upcalls, max depth " 
         << this->max_depth_;
  strm << endl; 
}

void
Thread::dump_invocations (ostream &strm)
{
  strm << "   " << this->alias_ << " handled " << this->invocations_.size() << " invocations" << endl;
  for (ACE_DLList_Iterator <Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      Invocation *inv;
      i.next(inv);
      inv->dump_detail (strm,0);
    }
}
