// $Id$

#include "Session.h"
#include "HostProcess.h"
#include "Log.h"
#include "ace/OS_NS_strings.h"
#include "ace/SString.h"

long
Session::tao_version_ = 160;

Session::Session (void)
{
  ACE_CString n ("localhost");
  ACE_CString v ("127.0.0.1");
  this->alt_addrs_.bind (n,v);
}

Session::~Session (void)
{
  for (Processes::iterator i = processes_.begin();
       i != processes_.end();
       i++)
    {
      delete i->item();
    }
}

void
Session::set_tao_version (ACE_TCHAR *str)
{
  if (ACE_OS::strncmp(str, ACE_TEXT("1.5"), 3)== 0)
    tao_version_ = 150;
  else if (ACE_OS::strncmp (str, ACE_TEXT("1.6"), 3) == 0)
    tao_version_ = 160;
}

long
Session::tao_version (void)
{
  return tao_version_;
}

void
Session::add_process (HostProcess *proc)
{
  if (processes_.bind(proc->pid(),proc) != 0)
    ACE_ERROR ((LM_ERROR,
                "Session::add_process could not bind pid %d\n",
                proc->pid()));
}

void
Session::alternate_address (const char *addrspec)
{
  const char *equal = ACE_OS::strchr(addrspec,'=');
  if (equal == 0)
   return;
  ACE_CString name (addrspec,(equal - addrspec));
  ACE_CString value (equal+1);
  this->alt_addrs_.bind(name,value);
}

void
Session::default_service (const char *addrspec)
{
   const char *equal = ACE_OS::strchr(addrspec,'=');
  if (equal == 0)
    return;
  ACE_CString name (addrspec,(equal - addrspec));
  ACE_CString endpoint (equal+1);

  static long next_def_pid = 0;
  --next_def_pid;
  HostProcess *hp = new HostProcess ("defaulted",next_def_pid);
  hp->proc_name(name);
  hp->add_endpoint (endpoint);
  this->processes_.bind(next_def_pid,hp);
}

HostProcess *
Session::find_process (long pid)
{
  Processes::ENTRY *entry;
  if (this->processes_.find(pid,entry) == 0)
    return entry->item();
  else 
    return 0;
}

HostProcess *
Session::find_host (const char *endpoint)
{
  ACE_CString test(endpoint);
  ACE_CString alternate;
  if (this->alt_addrs_.find(test,alternate) == 0)
    test = alternate;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      if (entry->item()->has_endpoint(test))
        return entry->item();
    }
  return 0;
}

void 
Session::dump (ostream &strm) const
{
  // report session metrics 

  strm << "Session summary report: "
       << this->processes_.current_size() << " Processes detected." << endl;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_summary (strm);
    }

  strm << "\n\n\nSession detail threads report: " << endl;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_thread_detail (strm);
    }

  strm << "\n\n\nSession detail peer process report: " << endl;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_peer_detail (strm);
    }

  strm << "\n\n\nSession detail object report: " << endl;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_object_detail (strm);
    }

  strm << "\n\n\nSession detail invocation report: " << endl;
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_invocation_detail (strm);
    }
}


// iterate over the collection of host processes to associate peer
// processes via endpoints. 
void
Session::reconcile (void)
{
  for (Processes::CONST_ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->reconcile_peers(this);
    }

}
