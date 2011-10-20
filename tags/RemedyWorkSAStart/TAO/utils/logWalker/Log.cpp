// $Id$

#include "Log.h"
#include "Invocation.h"
#include "PeerProcess.h"
#include "HostProcess.h"
#include "Session.h"
#include "Thread.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Mem_Map.h"

Log::Log (Session &session)
  : session_(session),
    dump_target_(0)
{
}

Log::~Log (void)
{
}

bool
Log::init (const ACE_TCHAR *filename, const char *alias)
{
  ACE_DEBUG ((LM_DEBUG,"Processing log file %C\n",
              filename));
  this->origin_ = ACE_TEXT_ALWAYS_CHAR (filename);
  this->alias_ = alias;

  ACE_Mem_Map mapped_file;
  int result = mapped_file.map(filename);
  if (result == -1)
    return false;

  size_t mapsize = mapped_file.size();
  char *base = reinterpret_cast<char *>(mapped_file.addr());
  if (mapsize == 0 || base == 0)
    {
      mapped_file.close();
      return false;
    }

  size_t remainder = mapsize;
  size_t linelen = 0;
  char *line;
  size_t maxline = 1000;
  char *buffer = new char[maxline];
  size_t line_pos = 1;
  while ((line = ACE_OS::strchr(base,'\n')) != 0)
    {
      linelen = line - base;
      if (linelen >= maxline)
        {
          delete [] buffer;
          buffer = 0;
          maxline = linelen + 100;
          buffer = new char[maxline];
        }
      ACE_OS::strncpy(buffer,base,linelen);
      buffer[linelen] = 0;
      this->parse_line (buffer, line_pos);
      base = line+1;
      ++line_pos;
      remainder -= linelen;
      if (remainder < 1)
        break;
    }

  delete [] buffer;

  mapped_file.close();

  return true;
}

bool
Log::get_pid_tid (long &pid, long &tid, char *line)
{
  const char * p = ACE_OS::strstr (line, "TAO (");
  char * t = 0;
  if (p == 0)
    return false;

  pid = ACE_OS::strtol(p+5,&t,10);
  tid = ACE_OS::strtol(t+1,0,10);
  return true;
}

HostProcess *
Log::get_host (long pid)
{
  HostProcess *hp = 0;
  if (pid == 0)
    return 0;
  for (ACE_DLList_Iterator<HostProcess> i (this->procs_);
       !i.done();
       i.advance())
    {
      i.next(hp);
      if (hp->pid() == pid)
        return hp;
    }
  hp = this->session_.find_process(pid);
  if (hp != 0)
    return hp;

  size_t numprocs = this->procs_.size();
  hp = new HostProcess (this->origin_,pid);
  this->procs_.insert_tail(hp);
  ACE_CString &procname = this->alias_.length() > 0 ?
    this->alias_ : this->origin_;
  switch (numprocs)
    {
    case 0:
      hp->proc_name(procname);
      break;
    case 1:
      {
        ACE_CString a2 = procname + "_1";
        HostProcess *first;
        if (this->procs_.get(first) == 0)
          first->proc_name(a2);
      }
      //fallthru
    default:
      {
        char ext[10];
        ACE_OS::sprintf(ext,"_" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII,numprocs+1);
        ACE_CString a2 = procname + ext;
        hp->proc_name(a2);
      }
    }

  this->session_.add_process(hp);
  return hp;
}

void
Log::handle_msg_dump (char *line, size_t offset)
{
  int pos = this->dump_target_->add_octets(line);
  if (pos > 0) // need to validate target
    {
      if (!this->dump_target_->validate())
        {
          for (ACE_DLList_Iterator<Thread> t_iter(this->giop_waiters_);
               !t_iter.done();
               t_iter.advance())
            {
              Thread *th = 0;
              t_iter.next(th);
              Invocation::GIOP_Buffer *new_target = th->giop_target();
              if (new_target == 0 || new_target == this->dump_target_)
                continue;
              if (new_target->matches (this->dump_target_))
                {
                  Thread *tmp_thr = this->dump_target_->thread();
                  new_target->transfer_from (this->dump_target_);
                  this->dump_target_ = new_target;
                  t_iter.remove();
                  this->giop_waiters_.insert_tail (tmp_thr);
                  break;
                }
            }
        }
      else
        {
          for (ACE_DLList_Iterator<Thread> t_iter(this->giop_waiters_);
               !t_iter.done();
               t_iter.advance())
            {
              Thread *th = 0;
              t_iter.next(th);
              if (this->dump_target_ == th->giop_target())
                {
                  PeerProcess *pp = th->incoming();
                  th->exit_wait(pp, offset);
                  t_iter.remove();
                  break;
                }
            }
        }
    }
  else if (pos == -1) // done
    {
      Invocation *inv = this->dump_target_->owner();
      if (inv != 0)
        {
          size_t len = 0;
          const char *oid = this->dump_target_->target_oid(len);
          if (oid != 0)
            inv->set_target (oid, len);
        }
      this->dump_target_ = 0;
    }
}

void
Log::parse_HEXDUMP (Log *this_, char *line, size_t offset)
{
  Thread *thr = 0;

  char *pos = ACE_OS::strstr (line,"HEXDUMP");
  pos += 8;
  size_t len = ACE_OS::strtol (pos, 0, 10);
  for (ACE_DLList_Reverse_Iterator<Thread> t_iter(this_->giop_waiters_);
       !t_iter.done();
       t_iter.advance())
    {
      t_iter.next(thr);
      Invocation::GIOP_Buffer *target = thr->giop_target();
      if (target == 0 || target->expected_size() != len || target->size() > 0)
        continue;
      this_->dump_target_ = target;
      t_iter.remove();
      break;
    }
  if (this_->dump_target_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Could not find a giop waiter for size %d\n",
                  offset, len));
    }
  else
    this_->dump_target_->init_buf (line);
}

void
Log::parse_dump_msg (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);
  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  int mode = ACE_OS::strstr (line,"send") != 0 ? 0 : 1;
  mode +=  ACE_OS::strstr (line,"Request") != 0 ? 0 : 2;
  char *pos = strrchr (line,'[');
  long rid = ACE_OS::strtol(pos+1, 0, 10);
  PeerProcess *pp = thr->incoming();
  if (pp == 0)
    {
      ACE_ERROR((LM_ERROR,
                 "%d: dump_msg, could not find pp for incoming, text = %s\n",
                 offset, line));
      return;
    }

  Invocation::GIOP_Buffer *target = 0;
  switch (mode)
    {
    case 1: { // receiving request
      thr->handle_request();
      Invocation *inv = pp->new_invocation (rid,thr);
      if (inv == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: process %s already has invocation %d\n",
                      offset, pp->id(), rid));
          break;
        }
      inv->init (line, offset, thr);
      target = inv->octets(true);
      break;
    }
    case 0: // sending request
      thr->enter_wait(pp);
      // fall through.
    case 3: { // receiving reply
      Invocation *inv = pp->find_invocation(rid, thr->active_handle());
      if (inv == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: could not find existing invocation for req_id %d\n",
                      offset, rid));
          inv = pp->new_invocation (rid,thr);
        }
      inv->init (line, offset, thr);
      target = inv->octets(mode == 0);
      if (target == 0 && mode == 3)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: could not map invocation to target for req_id %d\n",
                      offset, rid));
          return;
        }
//       if (mode == 3)
//         thr->exit_wait(pp, offset);
      break;
    }
    case 2: { // sending reply
      target = new Invocation::GIOP_Buffer(line,offset, thr);
      break;
    }
    default:;
    }

  thr->set_giop_target (target);
  this_->giop_waiters_.insert_tail(thr);
  if (this_->giop_waiters_.size() > 1 && (mode == 1 || mode == 3))
    {
      size_t prev_size = 0;
      for (ACE_DLList_Reverse_Iterator<Thread> t_iter(this_->giop_waiters_);
           !t_iter.done();
           t_iter.advance())
        {
          if (prev_size == 0)
            prev_size =  target->expected_size();
        }
    }
}

void
Log::parse_open_listener (Log *this_, char *line, size_t )
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  char *addr = ACE_OS::strchr(line,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  ACE_CString server_addr(addr);
  hp->add_listen_endpoint(server_addr);
}

void
Log::parse_got_existing (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *hpos = ACE_OS::strchr(line,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);

  PeerProcess *pp = hp->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  offset, this_->origin_.c_str(), handle, line));
      return;
    }
  thr->active_handle (handle);
}

void
Log::parse_muxed_tms (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *hpos = ACE_OS::strchr(line,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  hpos = ACE_OS::strchr(hpos, '<');
  long req_id = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = hp->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  offset, this_->origin_.c_str(), handle, line));
      return;
    }
  thr->active_handle (handle);

  Invocation *inv = pp->new_invocation(req_id, thr);
  if (inv == 0)
    ACE_DEBUG ((LM_DEBUG,"%d: peer %s already has invocation id %d\n",
                offset, pp->id(), req_id));
  thr->incoming_from (pp);
}

void
Log::parse_exclusive_tms (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  long handle = thr->active_handle();
  PeerProcess *pp = hp->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  offset, this_->origin_.c_str(), handle, line));
      return;
    }
  char *rpos = ACE_OS::strchr(line, '<');
  long req_id = ACE_OS::strtol(rpos+1,0,10);

  Invocation *inv = pp->new_invocation(req_id, thr);
  if (inv == 0)
    ACE_DEBUG ((LM_DEBUG,"%d: peer %s already has invocation id %d\n",
                offset, pp->id(), req_id));
  thr->incoming_from (pp);
}

void
Log::parse_process_parsed_msgs (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *hpos = ACE_OS::strchr(line,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);

  PeerProcess *pp = hp->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  offset, this_->origin_.c_str(), handle, line));
      pp = new PeerProcess (offset,true);
      Transport *t = new Transport ("<unknown>",false, offset);
      t->handle_ = handle;
      pp->add_transport(t);
      hp->add_peer (handle,pp);
      return;
    }
  thr->active_handle(handle);
  thr->incoming_from (pp);
}

void
Log::parse_wait_for_event (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);
//   char *pos = ACE_OS::strchr (line,'[');
//   long rid = ACE_OS::strtol(pos+1, 0, 10);
  PeerProcess *pp = thr->incoming();
  if (pp == 0)
    {
      ACE_ERROR((LM_ERROR,
                 "%d: wait_for_event, could not find pp for incoming, text = %s\n",
                 offset, line));
      return;
    }

  bool done = (ACE_OS::strstr (line,"done (follower)") != 0) ||
    (ACE_OS::strstr(line,"(leader) exit") != 0);


  if (done)
    thr->exit_wait(pp, offset);
//   else
//     thr->enter_wait(pp);
}

void
Log::parse_wait_on_read (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);
  PeerProcess *pp = thr->incoming();

  thr->exit_wait (pp, offset);
}

void
Log::parse_cleanup_queue (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *hpos = ACE_OS::strchr(line,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = hp->find_peer(handle);

  Thread *original_thr = thr;
  Invocation::GIOP_Buffer *target = original_thr->giop_target();
  if (target == 0 || target->owner() != 0)
    {
      original_thr = hp->find_thread_for_handle (handle);
      if (original_thr == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: cleanup queue, no original "
                      "thread found, handle %d\n",
                      offset, handle));
          return;
        }
      target = original_thr->giop_target();
    }
  original_thr->set_giop_target(0);
  original_thr->active_handle (0);

  if (target != 0 && target->owner() == 0)
    {
      size_t rid = target->actual_req_id();
      char mtype = target->type();
      Invocation *inv = pp->find_invocation(rid, handle);
      if (inv == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: Cleanup queue detected, "
                      "could not find invocation for rid = %d\n",
                      offset, rid));
          rid = target->expected_req_id();
          inv = pp->find_invocation (rid, handle);
          if (inv == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "%d, Cleanup queue still failed to find rid %d\n",
                          offset, rid));
              return;
            }
          original_thr->exit_wait(pp, offset);
          mtype = target->expected_type();
        }
      inv->set_octets (mtype == 0, target);
      size_t len = 0;
      const char *oid = target->target_oid(len);
      if (mtype == 0 && len > 0)
        inv->set_target (oid, len);
    }

}

void
Log::parse_close_connection (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);

  char *hpos = ACE_OS::strchr(line,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = hp->find_peer(handle);
  if (pp != 0)
    {
      Transport *t = pp->find_transport (handle);
      if (t != 0)
        t->close_offset_ = offset;
    }

  hp->remove_peer(handle);
}

void
Log::parse_handler_open (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *addr = ACE_OS::strchr(line,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  c = ACE_OS::strstr(c+1,"on ");
  long handle = ACE_OS::strtol(c + 3,0,10);
  PeerProcess *pp = thr->pending_peer();
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,"%d: no pending peer for file %s\n",
                  offset, this_->origin_.c_str()));
     return;
    }

  Transport *trans = 0;
  if (pp->is_server())
    {
      trans = pp->last_transport();
      if (trans == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: Pending peer exists, but no last transport "
                      "set, file %s\n",
                      offset, this_->origin_.c_str()));
          return;
        }
      //      trans->client_endpoint_ = addr;
    }
  else
    {
      trans = new Transport (addr,false,offset);
      pp->add_transport(trans);
    }
  trans->handle_ = handle;
  thr->pending_peer(0);
  hp->add_peer(handle,pp);
}

void
Log::parse_begin_connection (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  char *addr = ACE_OS::strchr(line,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  PeerProcess *pp = hp->find_peer(addr);
  if (pp == 0)
    {
      pp = new PeerProcess(offset,true);
      pp->set_server_addr (addr);
    }
  thr->pending_peer (pp);
}

void
Log::parse_local_addr (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  PeerProcess *peer = thr->pending_peer();
  if (peer == 0)
    {
      ACE_ERROR((LM_ERROR, "%d: file %s, no pending client or server\n",
                 offset, this_->origin_.c_str()));
      return;
    }

  char *addr = ACE_OS::strchr(line,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  if (peer->is_server())
    {
      Transport *t = new Transport (addr, true, offset);
      peer->add_transport (t);
      hp->add_client_endpoint (t->client_endpoint_);
    }
  else
    {
      peer->set_server_addr (addr);
    }
}

void
Log::parse_open_as_server (Log *this_, char *line, size_t offset)
{
  long pid = 0;
  long tid = 0;
  this_->get_pid_tid(pid,tid,line);

  HostProcess *hp = this_->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  thr->pending_peer (new PeerProcess(offset, false));

}

void
Log::parse_line (char *line, size_t offset)
{
  // first, is it a client connection to a new peer?
  // next is it a server connection to a new peer?
  // is it a new request or reply?
  // a GIOP hex dump?

  static parse_key exprs[] =
    {
      { "Handler::open, IIOP connection to peer", parse_handler_open },
      { "GIOP_Message_Base::dump_msg,", parse_dump_msg },
      { "GIOP message - HEXDUMP", parse_HEXDUMP },
      { "open_i, listening on:", parse_open_listener },
      { "Muxed_TMS[", parse_muxed_tms },
      { "Exclusive_TMS::request_id", parse_exclusive_tms },
      { "process_parsed_messages", parse_process_parsed_msgs },
      { "wait_for_event", parse_wait_for_event },
      { "Wait_On_Read", parse_wait_on_read },
      { "::cleanup_queue, byte_count", parse_cleanup_queue },
      { "close_connection_eh", parse_close_connection },
      { "IIOP_Connector::begin_connection, to ", parse_begin_connection },
      { "IIOP_Connection_Handler::open, The local addr is", parse_local_addr },
      { "opened as TAO_SERVER_ROLE", parse_open_as_server },
      { "Transport_Connector::connect, got an existing connected", parse_got_existing },
      { 0,0 }
    };

  if (this->dump_target_ != 0)
    {
      this->handle_msg_dump (line, offset);
      return;
    }

  for (int i = 0; exprs[i].text != 0; i++)
    {
      if (ACE_OS::strstr(line, exprs[i].text) != 0)
        {
          (*exprs[i].op)(this, line, offset);
          return;
        }
    }
  return;
}

void
Log::summarize (void)
{
  // todo
}
