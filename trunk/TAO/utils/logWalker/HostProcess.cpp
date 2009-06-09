// $Id$

#include "HostProcess.h"
#include "Invocation.h"
#include "PeerProcess.h"
#include "Thread.h"

#include "ace/OS_NS_stdio.h"

HostProcess::HostProcess (const ACE_CString &src, long pid)
  : pid_(pid),
    logfile_name_(src),
    closed_handles_(0)
{
}

HostProcess::~HostProcess (void)
{
  for (AddrList::ITERATOR i(this->endpoints_);
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
  for (PeerProcs::ITERATOR i = clients_.begin(); i != clients_.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      delete entry->item();
    }
  for (PeerProcs::ITERATOR i = servers_.begin(); i != servers_.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      delete entry->item();
    }
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
  Thread *thr;
  for (ACE_DLList_Iterator<Thread> i(threads_);
       !i.done();
       i.advance())
    {
      i.next(thr);
      if (thr->id() == tid)
        return thr;
    }
  char alias[20];
  ACE_OS::sprintf (alias,"Thread[%d]",this->threads_.size() + 1);
  thr = new Thread (tid, alias);
  threads_.insert_tail (thr);
  return thr;
}

PeerProcess *
HostProcess::find_peer (long handle)
{
  PeerProcess *pp = 0;
  if (this->clients_.find(handle,pp) != 0)
    this->servers_.find(handle,pp);
  return pp;
}

long
HostProcess::pid (void) const
{
  return this->pid_;
}

bool
HostProcess::has_endpoint (ACE_CString& addr)
{
  ACE_CString *a;
  for (ACE_DLList_Iterator<ACE_CString> i(this->endpoints_);
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
HostProcess::add_endpoint(ACE_CString &addr)
{
  this->endpoints_.insert_tail(new ACE_CString (addr));
}

void
HostProcess::add_peer(PeerProcess *peer, bool as_client)
{
  peer->set_owner (this);
  PeerProcess *existing = this->find_peer(peer->handle());
  if (existing != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "add_peer, found existing for %d, is_closed = %d\n",
		  peer->handle(), existing->is_closed()));
    }
  PeerProcs *list = as_client ? &clients_ : &servers_;
  long handle = peer->handle();
  list->bind (handle, peer);
}

void
HostProcess::close_peer (long handle, size_t offset)
{
  PeerProcess *pp = 0;
  --this->closed_handles_;
  if (this->clients_.unbind(handle,pp) == 0)
    {
      pp->close (offset);
      this->clients_.bind (this->closed_handles_, pp);
    }
  else if (this->servers_.unbind(handle,pp) == 0)
    {
      pp->close (offset);
      this->servers_.bind (this->closed_handles_, pp);
    }
    return;
}

void 
HostProcess::dump_summary (ostream &strm)
{
  strm << "Host process " << this->proc_name_ << " pid(" << this->pid_ << ") from logfile " << this->logfile_name_ <<  endl;
  size_t num = this->endpoints_.size();
  if (num > 0)
    {
      strm << "  listening on ";
      size_t count = 0;
      for (ACE_DLList_Iterator <ACE_CString> t_iter (this->endpoints_);
	   !t_iter.done();
	   t_iter.advance())
	{
	  ACE_CString *ep;
	  t_iter.next(ep);
	  strm << ep->c_str();
	  if (++count < num)
	    strm << ", ";
	}
      strm << endl;
    }
  
  strm << "  " << threads_.size() << " threads";
  if (clients_.current_size() > 0)
    strm << ", client to " << clients_.current_size();
  if (servers_.current_size() > 0)
    strm << ", server to " << servers_.current_size();
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
      Thread *thr;
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
      Thread *thr;
      t_iter.next(thr);
      thr->dump_invocations (strm);
    }
}

void
HostProcess::dump_detail_list(ostream &strm, PeerProcs &list, int mode, Session *session)
{
  bool first = true;
  for (PeerProcs::ITERATOR i = list.begin(); i !=list.end(); i++)
    {
      PeerProcs::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      if (entry->item() == 0)
	break;
      switch (mode)
	{
	case 0:
	  entry->item()->dump_summary (strm);
	  break;
	case 1:
	  entry->item()->dump_object_detail (strm);
	  strm << endl;
	  break;
	case 2:
	  if (!first)
	    this->dump_ident (strm,"Invocations continued");
	  entry->item()->dump_invocation_detail (strm);
	  strm << endl;
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
  strm << this->clients_.current_size() << " clients" << endl;
  this->dump_detail_list(strm, this->clients_, 0);
  strm << this->servers_.current_size() << " servers" << endl;
  this->dump_detail_list(strm, this->servers_, 0);
}
  
void
HostProcess::dump_object_detail (ostream &strm)
{
  this->dump_ident (strm, " peer objects: ");
  this->dump_detail_list(strm, this->clients_, 1);
  this->dump_detail_list(strm, this->servers_, 1);
}

void
HostProcess::dump_invocation_detail(ostream &strm)
{
  this->dump_ident (strm, " invocations: ");
  this->dump_detail_list(strm, this->clients_, 2);
  this->dump_detail_list(strm, this->servers_, 2);
  this->dump_ident (strm, " end invocation report");
}
  
void
HostProcess::reconcile_peers (Session *session)
{
  this->dump_detail_list(cout, this->clients_, 3, session);
  this->dump_detail_list(cout, this->servers_, 3, session);
}
