// $Id$

#include "PeerProcess.h"
#include "PeerObject.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"
#include "Invocation.h"
#include "HostProcess.h"
#include "Session.h"

char *
PeerProcess::nextIdent(void)
{
  static int count = 0;
  char *ident = new char[15];
  ACE_OS::sprintf (ident,"proc_%d", count++);
  return ident;
}

PeerProcess::PeerProcess (const char *text,
                          size_t offset,
                          bool as_client,
                          const char *target,
                          long handle)
  : owner_ (0),
    remote_ (0),
    local_addr_(),
    remote_addr_(target),
    handle_(handle),
    client_(as_client),
    origin_(text),
    origin_offset_ (offset),
    closed_offset_ (0),
    objects_ ()
{
  this->ident_ = PeerProcess::nextIdent();
}

PeerProcess::~PeerProcess (void)
{
  delete [] ident_;
  while (this->invocations_.size())
    {
      Invocation *head = this->invocations_.delete_head();
      delete head;
    }
  for (PeerObjectTable::ITERATOR i = objects_.begin(); i != objects_.end(); i++)
    {
      PeerObjectTable::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      delete entry->item();
    }
}

void
PeerProcess::set_addr (const char *addr, bool local)
{
  if (local)
    this->local_addr_ = addr;
  else
    this->remote_addr_ = addr;
}

void
PeerProcess::match_hosts (Session *session)
{
  if (this->remote_addr_.length () > 0)
    this->remote_ = session->find_host(this->remote_addr_.c_str());
}

void
PeerProcess::handle (long h)
{
  this->handle_ = h;
}

long
PeerProcess::handle (void) const
{
  return this->handle_;
}

const char *
PeerProcess::id (void) const
{
  if (this->remote_ != 0)
    {
      const ACE_CString &pname = this->remote_->proc_name();
      if (pname.length() > 0)
        return pname.c_str();
    }

  return this->ident_;
}

void
PeerProcess::close (size_t offset)
{
  this->closed_offset_ = offset;
}

bool
PeerProcess::is_closed (void) const
{
  return this->closed_offset_ > 0;
}

bool
PeerProcess::is_client (void) const
{
  return this->client_;
}

PeerObject *
PeerProcess::object_for (const char *oid, size_t len)
{
  PeerObject *po = 0;
  u_long key = ACE::hash_pjw (oid,len);
  int result = objects_.find(key, po);
  if (result == -1)
    {
      long index = static_cast<long>(objects_.current_size());
      char alias[20];
      ACE_OS::sprintf (alias, "obj_%d", index);
      po = new PeerObject(index,alias, this);
      objects_.bind(key, po);
    }
  return po;
}

Invocation *
PeerProcess::new_invocation (size_t req_id)
{
  if (this->find_invocation (req_id) != 0)
    return 0;
  Invocation *inv = new Invocation (this, req_id);
  this->invocations_.insert_tail(inv);
  return inv;
}

Invocation *
PeerProcess::find_invocation (size_t req_id)
{
  Invocation *inv = 0;
  for (ACE_DLList_Reverse_Iterator<Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      i.next(inv);
      if (inv->request_id() == req_id)
        {
          return inv;
        }
    }
  return 0;
}

Invocation *
PeerProcess::find_invocation_size (size_t len)
{
  Invocation *inv = 0;
  for (ACE_DLList_Reverse_Iterator<Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      i.next(inv);
      if (!inv->message_complete() && inv->expected_size() == len)
        {
          return inv;
        }
    }
  return 0;
}

void
PeerProcess::set_owner (HostProcess *hp)
{
  this->owner_ = hp;
}

HostProcess *
PeerProcess::owner (void)
{
  return this->owner_;
}

void
PeerProcess::dump_summary (ostream &strm)
{
  strm << "    peer process " << this->ident_ << " handle["
       << this->handle_ << "] as ";
  if (this->client_)
      strm << "client on "
           << this->local_addr_;
  else
    strm << " server";
  if (this->remote_)
    {
      strm << " to " << remote_->proc_name();
    }
  else
    strm << " to peer at " << this->remote_addr_;
  strm << " referenced " << this->objects_.current_size()
       << " objects in " << this->invocations_.size() << " invocations";
  if (this->is_closed())
    strm << " closed on line " << this->closed_offset_;
  strm << endl;
}

void
PeerProcess::dump_object_detail (ostream &strm)
{
  strm << this->objects_.current_size()
       << " Objects referenced in " << this->ident_ << ":" << endl;
  size_t count_inv = 0;
  for (PeerObjectTable::ITERATOR i = this->objects_.begin();
       i != this->objects_.end();
       i++)
    {
      PeerObjectTable::ENTRY *entry = 0;
      i.next (entry);
      PeerObject *obj = entry->item();
      obj->dump_detail (strm);
      count_inv += obj->num_invocations();
    }
  strm << "Total of  " << count_inv << " invocations "  << endl;
}

void
PeerProcess::dump_invocation_detail (ostream &strm)
{
  strm << "\n " << this->invocations_.size() << " Invocations with " << this->ident_ << ":" << endl;
  Invocation *inv = 0;
  for (ACE_DLList_Iterator<Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      i.next(inv);
      inv->dump_detail(strm, 0);
    }

}
