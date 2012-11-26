// $Id$

#include "Session.h"
#include "HostProcess.h"
#include "Log.h"
#include "ace/OS_NS_strings.h"
#include "ace/SString.h"
#include "ace/OS_NS_sys_stat.h"

long
Session::tao_version_ = 200;

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

bool
Session::set_tao_version (ACE_TCHAR *str)
{
  if (ACE_OS::strncmp(str, ACE_TEXT("1.5"), 3)== 0)
    tao_version_ = 150;
  else if (ACE_OS::strncmp (str, ACE_TEXT("1.6"), 3) == 0)
    tao_version_ = 160;
  else if (ACE_OS::strncmp (str, ACE_TEXT("1.7"), 3) == 0)
    tao_version_ = 170;
  else if (ACE_OS::strncmp (str, ACE_TEXT("1.8"), 3) == 0)
    tao_version_ = 180;
  else if (ACE_OS::strncmp (str, ACE_TEXT("2.0"), 3) == 0)
    tao_version_ = 200;
  else
    return false;
  return true;
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
  if (procs_by_name_.bind(proc->proc_name(), proc) != 0)
    ACE_ERROR ((LM_ERROR,
                "Session::add_process could not bind procname %s\n",
                proc->proc_name().c_str()));
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
  hp->add_listen_endpoint (endpoint);
  this->processes_.bind(next_def_pid,hp);
  this->procs_by_name_.bind(name,hp);
}

HostProcess *
Session::find_process (long pid)
{
  Processes::ENTRY *entry = 0;
  if (this->processes_.find(pid,entry) == 0)
    return entry->item();
  else
    return 0;
}

HostProcess *
Session::find_host (ACE_CString &endpoint, bool server)
{
  ACE_CString test(endpoint);
  ACE_CString alternate;
  size_t sep = test.find(':');
  if (this->alt_addrs_.find(test.substring (0,sep),alternate) == 0)
    {
      test = alternate + test.substring(sep);
    }
  for (Processes::ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        break;
      if (entry->item()->has_endpoint(test, server))
        return entry->item();
    }
  return 0;
}

void
Session::make_dir (const char *dirname)
{
  this->base_dir_ = dirname;
}

void
Session::outfile (const char *o)
{
  this->outfile_ = o;
}

bool
Session::has_dir (void)
{
  return this->base_dir_.length() > 0;
}

bool
Session::has_outfile (void)
{
  return this->outfile_.length() > 0;
}

ostream *
Session::stream_for ( ostream *oldstream, HostProcess *hp, const char *sub)
{
  if (this->has_dir())
    {
      ACE_CString outname = this->base_dir_;

      if (oldstream == 0)
        {
          ACE_OS::mkdir(this->base_dir_.c_str());
        }
      delete oldstream;
      outname += ACE_DIRECTORY_SEPARATOR_CHAR;
      if (hp != 0)
        {
          outname += hp->proc_name();
          ACE_OS::mkdir(outname.c_str());
          outname += ACE_DIRECTORY_SEPARATOR_CHAR;
        }
      outname += (sub == 0) ? "summary.txt" : sub;
      return new ofstream (outname.c_str());
    }

  if (oldstream != 0)
    return oldstream;
  if (this->has_outfile())
    return new ofstream(this->outfile_.c_str());
  else
    return &cout;
}

void
Session::dump ()
{
  bool single = !this->has_dir();
  ostream *strm = this->stream_for(0);

  // report session metrics

  if (single)
    *strm << "Session summary report: "
        << this->processes_.current_size() << " Processes detected." << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->dump_summary (*strm);
    }

  if (single)
    *strm << "\n\n\nSession detail threads report: " << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      strm = stream_for (strm,entry->item(),"threads.txt");
      entry->item()->dump_thread_detail (*strm);
    }

  if (single)
    *strm << "\n\n\nSession detail peer process report: " << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      strm = stream_for (strm,entry->item(),"peer_processes.txt");
      entry->item()->dump_peer_detail (*strm);
    }

  if (single)
    *strm << "\n\n\nSession detail object report: " << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      strm = stream_for (strm,entry->item(),"objects.txt");
      entry->item()->dump_object_detail (*strm);
    }

  if (single)
    *strm << "\n\n\nSession detail invocation by peer process report: " << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      strm = stream_for (strm,entry->item(),"invocation_by_peer.txt");
      entry->item()->dump_invocation_detail (*strm);
    }

  if (single)
    *strm << "\n\n\nSession detail invocation by thread report: " << endl;
  for (Procs_By_Name::ITERATOR i (this->procs_by_name_); !i.done(); i.advance())
    {
      Procs_By_Name::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      strm = stream_for (strm,entry->item(),"invocation_by_thread.txt");
//       entry->item()->dump_invocation_detail (*strm);
       entry->item()->dump_thread_invocations (*strm);
    }
  if (this->has_outfile() || this->has_dir())
    delete strm;
}


// iterate over the collection of host processes to associate peer
// processes via endpoints.
void
Session::reconcile (void)
{
  for (Processes::ITERATOR i (this->processes_); !i.done(); i.advance())
    {
      Processes::ENTRY *entry;
      if (i.next(entry) == 0)
        continue;
      entry->item()->reconcile_peers(this);
    }

}
