// $Id$

#include "HostProcess.h"
#include "Invocation.h"
#include "PeerProcess.h"
#include "Thread.h"

#include "ace/OS_NS_stdio.h"

PeerNode::PeerNode (long h, PeerProcess *p)
  :handle_ (h),
   peer_ (p),
   closed_ (false)
{
}

HostProcess::HostProcess (const ACE_CString &src, long pid)
  : pid_(pid),
    logfile_name_(src)
{
}

HostProcess::~HostProcess (void)
{
  for (AddrList::ITERATOR i(this->listen_endpoints_);
       !i.done();
       i++)
    {
      delete reinterpret_cast<ACE_CString *>(i.next()->item_);
    }
  for (ThreadList::ITERATOR i(this->threads_);
       !i.done();
       i++)
    {
      delete reinterpret_cast<Thread *>(i.next()->item_);
    }
#if 0
  for (PeerProcs::ITERATOR i = by_addr_.begin(); i != servers_.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      delete entry->item();
    }
#endif
}

void
HostProcess::proc_name (const ACE_CString &name)
{
  this->proc_name_ = name;
}

const ACE_CString&
HostProcess::proc_name (void) const
{
  return this->proc_name_;
}

Thread *
HostProcess::find_thread (long tid)
{
  Thread *thr = 0;
  for (ACE_DLList_Iterator<Thread> i(threads_);
       !i.done();
       i.advance())
    {
      i.next(thr);
      if (thr->id() == tid)
        return thr;
    }
  char alias[20];
  ACE_OS::sprintf (alias,"Thread[" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII "]",
                   this->threads_.size() + 1);
  thr = new Thread (tid, alias);
  threads_.insert_tail (thr);
  return thr;
}

Thread *
HostProcess::find_thread_for_handle (long h)
{
  Thread *thr = 0;
  for (ACE_DLList_Iterator<Thread> i(threads_);
       !i.done();
       i.advance())
    {
      i.next(thr);
      if (thr->active_handle() == h && thr->giop_target() != 0)
        return thr;
    }
  return thr;
}

PeerProcess *
HostProcess::find_peer (const ACE_CString &addr)
{

  PeerProcess *pp = 0;
  (void)this->by_addr_.find(addr,pp);
  return pp;
}

PeerNode *
HostProcess::find_peer_i (long h)
{
  if (this->by_handle_.size() == 0)
    return 0;
  for (PeerArray::ITERATOR i(this->by_handle_);
       !i.done();
       i++)
    {
      PeerNode *node = reinterpret_cast<PeerNode *>(i.next()->item_);
      if (node->handle_ == h)
        return node;
    }

  return 0;
}


PeerProcess *
HostProcess::find_peer (long h, bool ignore_closed)
{
  PeerNode *node = this->find_peer_i (h);
  if (node != 0 && !(ignore_closed && node->closed_) )
    return node->peer_;
  return 0;
}


long
HostProcess::pid (void) const
{
  return this->pid_;
}

bool
HostProcess::has_endpoint (ACE_CString& addr, bool listen)
{
  ACE_CString *a = 0;
  AddrList &list = listen ? this->listen_endpoints_ : this->client_endpoints_;
  for (ACE_DLList_Iterator<ACE_CString> i(list);
       !i.done();
       i.advance())
    {
      i.next(a);
      if (*a == addr)
        return true;
    }
  return false;
}

void
HostProcess::add_client_endpoint(ACE_CString &addr)
{
  this->client_endpoints_.insert_tail(new ACE_CString (addr));
}

void
HostProcess::add_listen_endpoint(ACE_CString &addr)
{
  this->listen_endpoints_.insert_tail(new ACE_CString (addr));
}

void
HostProcess::add_peer(long handle, PeerProcess *peer)
{
  peer->set_owner (this);
  PeerNode *node = this->find_peer_i(handle);
  if (node != 0 && !node->closed_ )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "add_peer, found existing for %d\n",
                  handle));
    }
  const ACE_CString &addr = peer->is_server() ?
    peer->server_addr() : peer->last_client_addr();
  errno = 0;

  int result = this->by_addr_.bind (addr,peer);
  if (result == -1)
    {
      ACE_ERROR ((LM_ERROR,"add_peer, cannot bind handle %d to addr %s %p\n",
                  handle, addr.c_str(), "by_addr_.bind"));
    }

  if (node == 0)
    {
      node = new PeerNode (handle,peer);
      this->by_handle_.insert_tail(node);
    }
  else
    {
      node->closed_ = false;
      node->peer_ = peer;
    }
}

void
HostProcess::close_peer(long h)
{
  if (this->by_handle_.size() == 0)
    return;
  for (PeerArray::ITERATOR i(this->by_handle_);
       !i.done();
       i++)
    {
      PeerNode *node = reinterpret_cast<PeerNode *>(i.next()->item_);
      if (node->handle_ == h)
        {
          node->closed_ = true;
          return;
        }
    }
}

void
HostProcess::dump_summary (ostream &strm)
{
  strm << "Host process " << this->proc_name_ << " pid(" << this->pid_ << ") from logfile " << this->logfile_name_ <<  endl;
  size_t num = this->listen_endpoints_.size();
  if (num > 0)
    {
      strm << "  listening on ";
      size_t count = 0;
      for (ACE_DLList_Iterator <ACE_CString> t_iter (this->listen_endpoints_);
           !t_iter.done();
           t_iter.advance())
        {
          ACE_CString *ep = 0;
          t_iter.next(ep);
          strm << ep->c_str();
          if (++count < num)
            strm << ", ";
        }
      strm << endl;
    }

  strm << "  " << threads_.size() << " threads";
#if 0
  if (clients_.current_size() > 0)
    strm << ", client to " << clients_.current_size();
  if (servers_.current_size() > 0)
    strm << ", server to " << servers_.current_size();
#endif
  strm << endl;
}

void
HostProcess::dump_ident (ostream &strm, const char *message)
{
  strm <<"\nHost process ";
  if (this->proc_name_.length())
    strm << this->proc_name_;
  strm << " pid (" << this->pid_ << ") " << message << endl;
}

void
HostProcess::dump_thread_detail (ostream &strm)
{
  this->dump_ident (strm, " thread details:");
  for (ACE_DLList_Iterator <Thread> t_iter (this->threads_);
       !t_iter.done();
       t_iter.advance())
    {
      Thread *thr = 0;
      t_iter.next(thr);
      thr->dump_detail (strm);
    }
}

void
HostProcess::dump_thread_invocations (ostream &strm)
{
  this->dump_ident (strm, " invocations by thread:");
  for (ACE_DLList_Iterator <Thread> t_iter (this->threads_);
       !t_iter.done();
       t_iter.advance())
    {
      Thread *thr = 0;
      t_iter.next(thr);
      thr->dump_invocations (strm);
      strm << endl;
    }
}

void
HostProcess::iterate_peers (int group,
                            int operation,
                            ostream *strm,
                            Session *session)
{
  bool first = true;
  for (PeerProcs::ITERATOR i = this->by_addr_.begin(); i != by_addr_.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      PeerProcess *pp = entry->item();
      if (pp == 0)
        continue;
      if ((group != 3) &&
          ((pp->is_server() && group == 1)
           || (!pp->is_server() && group == 2)))
        continue;

      switch (operation)
        {
        case 0:
          entry->item()->dump_summary (*strm);
          break;
        case 1:
          entry->item()->dump_object_detail (*strm);
          *strm << endl;
          break;
        case 2:
          if (!first)
            this->dump_ident (*strm,"Invocations continued");
          entry->item()->dump_invocation_detail (*strm);
          *strm << endl;
          break;
        case 3:
          entry->item()->match_hosts (session);
          break;
        }
      first = false;
    }
}

void
HostProcess::dump_peer_detail (ostream &strm)
{
  this->dump_ident (strm, " peer processes:");
  size_t num_servers = 0;
  size_t num_clients = 0;
  strm << " total peers: " << this->by_addr_.current_size() << endl;
  for (PeerProcs::ITERATOR i = this->by_addr_.begin(); i != by_addr_.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      PeerProcess *pp = entry->item();
      if (pp == 0)
        break;
      if (pp->is_server())
        ++num_servers;
      else
        ++num_clients;
    }

  strm << " from " << num_clients << " clients" << endl;
  this->iterate_peers (1, 0, &strm);
  strm << " to " << num_servers << " servers" << endl;
  this->iterate_peers (2, 0, &strm);
}

void
HostProcess::dump_object_detail (ostream &strm)
{
  this->dump_ident (strm, " peer objects: ");
  this->iterate_peers (3, 1, &strm);
}

void
HostProcess::dump_invocation_detail(ostream &strm)
{
  this->dump_ident (strm, " invocations: ");
  this->iterate_peers (3, 2, &strm);
  this->dump_ident (strm, " end invocation report");
}

void
HostProcess::reconcile_peers (Session *session)
{
  this->iterate_peers (3, 3, 0, session);
}
