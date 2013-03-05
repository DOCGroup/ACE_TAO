// $Id$

#include "PeerProcess.h"
#include "PeerObject.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/ACE.h"
#include "Invocation.h"
#include "HostProcess.h"
#include "Thread.h"
#include "Session.h"


Transport::Transport (const char *addr, bool is_client, size_t offset)
  : handle_ (0),
    client_endpoint_ (addr),
    local_is_client_ (is_client),
    open_offset_ (offset),
    close_offset_ (0)
{
}

char *
PeerProcess::nextIdent(bool is_server)
{
  static int count = 0;
  char *ident = new char[15];
  ACE_OS::sprintf (ident,"%s_%d", (is_server ? "server" : "client"), count++);
  return ident;
}

PeerProcess::PeerProcess (size_t offset, bool is_server)
  : owner_ (0),
    remote_ (0),
    server_port_(),
    server_host_(),
    server_(is_server),
    ssl_(false),
    localhost_(false),
    origin_offset_ (offset),
    objects_ (),
    object_by_index_ ()
{
  this->ident_ = PeerProcess::nextIdent(is_server);
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
PeerProcess::set_server_addr (const ACE_CString &addr)
{
  size_t p = addr.rfind (':');
  this->server_port_ = addr.substring(p);
  this->server_host_ = addr.substring(0,p);

  this->localhost_ = this->server_host_ == "localhost" ||
    this->server_host_ == "127.0.0.1" || this->server_host_ == "[::1]";
}

bool
PeerProcess::match_server_addr (const ACE_CString &addr, Session &session) const
{
  size_t p = addr.rfind (':');
  ACE_CString port = addr.substring (p);
  ACE_CString host = addr.substring (0,p);
  if (port != this->server_port_)
    return false;

  if (this->localhost_)
    {
      return host == "localhost" || host == "127.0.0.1" || host == "[::1]";
    }

  if (this->server_host_ == host)
    return true;

  return session.is_equivalent (this->server_host_, host);
}

ACE_CString
PeerProcess::server_addr (void) const
{
  return this->server_host_ + this->server_port_;
}

const ACE_CString&
PeerProcess::last_client_addr (void) const
{
  return this->last_transport_->client_endpoint_;
}

bool
PeerProcess::is_server (void) const
{
  return this->server_;
}

size_t
PeerProcess::offset (void) const
{
  return this->origin_offset_;
}

void
PeerProcess::ssl (bool is_ssl)
{
  this->ssl_ = is_ssl;
}

void
PeerProcess::add_transport (Transport *t)
{
  this->last_transport_ = t;
  this->transports_.insert_tail (t);
}

Transport *
PeerProcess::last_transport (void)
{
  return this->last_transport_;
}

Transport *
PeerProcess::find_transport (long handle)
{
  Transport *t = 0;
  for (ACE_DLList_Reverse_Iterator<Transport> i(this->transports_);
       !i.done();
       i.advance())
    {
      i.next(t);
      if (t->handle_ == handle)
        {
          return t;
        }
    }
  return 0;
}

void
PeerProcess::match_hosts (Session *session)
{
  // This method wants to find the host process that listens
  // on the server addr. But if the local side is the server
  // then this wants to find the remote based on the Transport
  // instance
  if (this->server_)
    this->remote_ = session->find_host(this->server_host_, true);
  else
    {
      Transport *t = 0;
      this->transports_.get(t,0);
      if (t != 0)
        this->remote_ = session->find_host(t->client_endpoint_, false);
    }
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
      ACE_OS::sprintf (alias, "obj_%ld", index);
      po = new PeerObject(index, alias, this);
      objects_.bind(key, po);
      object_by_index_.bind (index, po);
    }
  return po;
}

Invocation *
PeerProcess::new_invocation (size_t req_id, Thread *thr)
{
  if (this->find_invocation (req_id, thr->active_handle()) != 0)
    return 0;
  Invocation *inv = new Invocation (this, thr, req_id);
  this->invocations_.insert_tail(inv);
  thr->add_invocation (inv);
  return inv;
}

Invocation *
PeerProcess::find_invocation (size_t req_id, long handle)
{
  Invocation *inv = 0;
  for (ACE_DLList_Reverse_Iterator<Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      i.next(inv);
      if (inv->request_id() == req_id &&
          inv->handle() == handle)
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
  size_t num_transports = this->transports_.size();

  if (this->remote_)
    strm << "  " << remote_->proc_name();
  else
    strm << "  peer process " << this->ident_;
  strm << " is a ";
  if (this->ssl_)
    strm << "secure ";
  if (this->server_)
    strm << "server at ";
  else
    strm << "client to ";
  strm << this->server_host_ << this->server_port_;
  strm << " with " << num_transports << " connections, ";
  strm << " referenced " << this->objects_.current_size()
       << " objects in " << this->invocations_.size() << " invocations";
  strm << endl;
  for (ACE_DLList_Iterator<Transport> i(this->transports_);
       !i.done();
       i.advance())
    {
      Transport *tran = 0;
      i.next(tran);
      strm << "    connection[" << tran->handle_ << "] ";
      strm << (tran->local_is_client_ ? "to " : "from ");
      strm << tran->client_endpoint_;
      strm << " created line " << tran->open_offset_;
      if (tran->close_offset_)
        strm << " closed line " << tran->close_offset_;
      strm << endl;
    }
}

void
PeerProcess::dump_object_detail (ostream &strm)
{
  strm << this->objects_.current_size()
       << " Objects referenced";
  if (this->server_)
    strm << " in ";
  else
    strm << " by ";
  if (this->remote_)
    strm << remote_->proc_name();
  else
    strm << "peer process " << this->ident_;
  strm << ":" << endl;
  size_t count_inv = 0;
  for (ObjectByIndex::ITERATOR i = this->object_by_index_.begin();
       !i.done();
       i.advance())
    {
      ObjectByIndex::ENTRY *entry = 0;
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
  strm << "\n " << this->invocations_.size() << " Invocations ";
  strm << (this->server_ ? "to " : "from ");
  if (this->remote_)
    strm << remote_->proc_name();
  else
    strm << "peer process " << this->ident_;
  strm << ":" << endl;
  Invocation *inv = 0;
  bool show_handle = this->transports_.size() > 1;
  for (ACE_DLList_Iterator<Invocation> i(this->invocations_);
       !i.done();
       i.advance())
    {
      i.next(inv);
      inv->dump_detail(strm, 0, Invocation::Dump_Thread, show_handle);
    }

}
