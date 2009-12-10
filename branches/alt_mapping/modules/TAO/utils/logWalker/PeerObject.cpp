// $Id$


#include "PeerObject.h"
#include "Invocation.h"

PeerObject::PeerObject (long id, const char *name, PeerProcess *parent)
  :ident_(id),
   name_(name),
   parent_(parent)
{
}

PeerObject::~PeerObject (void)
{
}

ACE_CString &
PeerObject::name (void)
{
  return this->name_;
}

long
PeerObject::ident (void)
{
  return this->ident_;
}

void
PeerObject::add_invocation (Invocation *inv)
{
  this->invocations_.enqueue_tail (inv);
}
size_t
PeerObject::num_invocations (void)
{
  return this->invocations_.size();
}

void
PeerObject::dump_detail (ostream &strm)
{
  strm  << this->ident_ << " has "
       << this->invocations_.size() << " invocations" << endl;

}
