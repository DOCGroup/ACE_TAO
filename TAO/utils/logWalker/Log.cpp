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
  : session_ (session),
    dump_target_ (0),
    history_ (),
    timestamp_ (),
    line_ (0),
    info_ (0),
    offset_ (0),
    hostproc_ (0),
    thr_ (0)
{
}

Log::~Log (void)
{
}

bool
Log::process_file (const ACE_TCHAR *filename, const char *alias)
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
  char *text;
  size_t maxline = 1000;
  char *buffer = new char[maxline];
  this->offset_ = 1;
  while ((text = ACE_OS::strchr(base,'\n')) != 0)
    {
      linelen = text - base;
      if (linelen >= maxline)
        {
          delete [] buffer;
          buffer = 0;
          maxline = linelen + 100;
          buffer = new char[maxline];
        }
      ACE_OS::strncpy(buffer,base,linelen);
      buffer[linelen] = 0;
      this->line_ = buffer;
      if (linelen > 0)
        this->parse_line ();
      base = text+1;
      ++this->offset_;
      remainder -= linelen;
      if (remainder < 1)
        break;
    }

  delete [] buffer;

  mapped_file.close();

  return true;
}

void
Log::get_preamble ()
{
  char * p = ACE_OS::strstr (this->line_, "(");
  char * t = 0;

  this->info_ = this->line_;

  if (p == 0)
    return;

  if (p != this->line_)
    {
      char * x = ACE_OS::strstr (this->line_, "TAO (");
      if (x+4 != p)
        {
          x = ACE_OS::strstr (this->line_, "@(");
          if (x + 1 != p)
            return;
        }
    }


  long pid = ACE_OS::strtol(p + 1, &t, 10);
  if (pid == 0)
    return;

  long tid = 0;
  if ( *t == '|' )
    tid = ACE_OS::strtol(t + 1, 0, 10);
  else if ( *t != ')')
    return; // not either (pid) or (pid|tid)

  this->info_ = ACE_OS::strstr (p, ")") + 1;
  this->hostproc_ = 0;
  for (ACE_DLList_Iterator<HostProcess> i (this->procs_);
       !i.done();
       i.advance())
    {
      i.next(this->hostproc_);
      if (this->hostproc_->pid() == pid)
        {
          break;
        }
      this->hostproc_ = 0;
    }

  if (this->hostproc_ == 0)
    this->hostproc_ = this->session_.find_process(pid);

  if (this->hostproc_ == 0)
    {
      size_t numprocs = this->procs_.size();
      this->hostproc_ = new HostProcess (this->origin_,pid);
      this->procs_.insert_tail(this->hostproc_);
      ACE_CString &procname = this->alias_.length() > 0 ?
        this->alias_ : this->origin_;
      switch (numprocs)
        {
        case 0:
          this->hostproc_->proc_name(procname);
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
            this->hostproc_->proc_name(a2);
          }
        }

      this->session_.add_process (this->hostproc_);
    }
  this->thr_ = this->hostproc_->find_thread (tid, this->offset_);
  return;
}

void
Log::handle_msg_octets ()
{
  int pos = this->dump_target_->add_octets(this->line_, this->offset_);
  if (this->dump_target_ == &this->unknown_msg_)
    {
      for (ACE_DLList_Iterator<Thread> t_iter(this->giop_waiters_);
           !t_iter.done();
           t_iter.advance())
        {
          Thread *th = 0;
          t_iter.next(th);
          GIOP_Buffer *waiter = th->giop_target();
          if (waiter == 0)
            continue;
          if (waiter->matches (this->dump_target_))
            {
              waiter->transfer_from (this->dump_target_);
              this->dump_target_ = waiter;
              t_iter.remove();
              break;
            }
        }
    }
  if (pos == -1) // done
    {
      Invocation *inv = this->dump_target_->owner();
      if (inv != 0)
        {
          size_t len = 0;
          const char *oid = this->dump_target_->target_oid(len);
          if (oid != 0)
            inv->set_target (oid, len);
        }
      else
        {
          if (this->dump_target_ == &this->unknown_msg_)
            ACE_ERROR ((LM_ERROR, "%d dump ended with no target owner\n", this->offset_));
        }
      this->dump_target_ = 0;
      this->unknown_msg_.reset();
    }
}

void
Log::parse_HEXDUMP_i (void)
{
  char *pos = ACE_OS::strstr (this->line_,"HEXDUMP");
  pos += 8;

  if (this->dump_target_ != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Parse HEXDUMP in the middle of an existing dump\n",
                  this->offset_));
    }
  else
    {
      this->unknown_msg_.init_buf (this->line_, this->offset_);
      this->dump_target_ = &this->unknown_msg_;
    }
}

void
Log::parse_dump_giop_msg_i (void)
{
  int sending = ACE_OS::strstr (this->info_,"send") != 0 ? 0 : 1;
  int type =  ACE_OS::strstr (this->info_,"Request") != 0 ? 0 : 1;
  int mode = sending + type * 2;

  char *pos = strrchr (this->info_,'[');
  long rid = ACE_OS::strtol(pos+1, 0, 10);
  PeerProcess *pp = this->thr_->incoming();
  if (pp == 0)
    {
      ACE_ERROR((LM_ERROR,
                 "%d: dump_msg, could not find pp for incoming, text = %s\n",
                 this->offset_, this->info_));
      return;
    }

  GIOP_Buffer *target = 0;
  switch (mode)
    {
    case 1: { // receiving request
      this->thr_->handle_request();
      Invocation *inv = pp->new_invocation (rid, this->thr_);
      if (inv == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: process %s already has invocation %d\n",
                      this->offset_, pp->id(), rid));
          break;
        }
      inv->init (this->line_, this->offset_, this->thr_);
      this->thr_->push_invocation (inv);
      target = inv->octets(true);
      if (target == 0)
        {
          ACE_ERROR ((LM_ERROR, "%d: no target octets for new recv reqeust, id = %d\n",
                      this->offset_, rid));
          return;
        }
      break;
    }
    case 0: // sending request
      this->thr_->enter_wait(pp);
      this->thr_->push_invocation (0);
      // fall through.
    case 3: { // receiving reply
      Invocation *inv = pp->find_invocation(rid, this->thr_->active_handle());
      if (inv == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: could not find existing invocation for req_id %d\n",
                      this->offset_, rid));
          inv = pp->new_invocation (rid,this->thr_);
        }
      inv->init (this->line_, this->offset_, this->thr_);
      target = inv->octets(mode == 0);
      if (target == 0 && mode == 3)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: could not map invocation to target for req_id %d\n",
                      this->offset_, rid));
          return;
        }
//       if (mode == 3)
//         this->thr_->exit_wait(pp, this->offset_);
      break;
    }
    case 2: { // sending reply
      target = new GIOP_Buffer(this->line_, this->offset_, this->thr_);
      this->thr_->pop_invocation ();
      break;
    }
    default:;
    }

  this->thr_->set_giop_target (target);
  if (this->giop_waiters_.size() > 0)
    {
      Thread *other_thr = 0;
      for (ACE_DLList_Iterator<Thread> t_iter(this->giop_waiters_);
           !t_iter.done();
           t_iter.advance())
        {
          t_iter.next(other_thr);
          GIOP_Buffer *tgt = other_thr->giop_target();
          if (target == 0)
            {
              ACE_ERROR ((LM_ERROR, "%d: dump_giop_msg_i, target is null, mode = %d, reqid = %d\n",
                          this->offset_, mode, rid));
              return;
            }
          if (tgt != 0 && this->thr_ != other_thr && target->matches (tgt))
            {
              this->thr_->set_dup (other_thr, true);
            }
        }
    }
  this->giop_waiters_.insert_tail(this->thr_);
}

void
Log::parse_open_listener_i (void)
{
  char *addr = ACE_OS::strchr(this->info_,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  Endpoint server_addr(addr);
  this->hostproc_->add_listen_endpoint(server_addr);
}

void
Log::parse_got_existing_i (void)
{
  char *hpos = ACE_OS::strchr(this->info_,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);

  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  this->offset_, this->origin_.c_str(), handle, this->info_));
      return;
    }
  this->thr_->active_handle (handle);
}

void
Log::parse_muxed_tms_i (void)
{
  char *hpos = ACE_OS::strchr(this->info_,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  hpos = ACE_OS::strchr(hpos, '<');
  long req_id = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  this->offset_, this->origin_.c_str(), handle, this->info_));
      return;
    }
  this->thr_->active_handle (handle);

  Invocation *inv = pp->new_invocation(req_id, this->thr_);
  if (inv == 0)
    ACE_ERROR ((LM_ERROR,"%d: peer %s already has invocation id %d\n",
                this->offset_, pp->id(), req_id));
  this->thr_->incoming_from (pp);
}

void
Log::parse_exclusive_tms_i (void)
{
  long handle = this->thr_->active_handle();
  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  this->offset_, this->origin_.c_str(), handle, this->info_));
      return;
    }
  char *rpos = ACE_OS::strchr(this->info_, '<');
  long req_id = ACE_OS::strtol(rpos+1,0,10);

  Invocation *inv = pp->new_invocation(req_id, this->thr_);
  if (inv == 0)
    ACE_ERROR ((LM_ERROR,"%d: peer %s already has invocation id %d\n",
                this->offset_, pp->id(), req_id));
  this->thr_->incoming_from (pp);
}

void
Log::parse_process_parsed_msgs_i (void)
{
  char *hpos = ACE_OS::strchr(this->info_, '[');
  long handle = ACE_OS::strtol(hpos+1, 0, 10);

  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: Error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  this->offset_, this->origin_.c_str(), handle, this->info_));
      pp = new PeerProcess (this->offset_, true);
      Transport *t = new Transport ("<unknown>", false, this->offset_);
      t->handle_ = handle;
      pp->add_transport(t);
      this->hostproc_->add_peer (handle,pp);
      return;
    }
  this->thr_->active_handle(handle);
  this->thr_->incoming_from (pp);
}

void
Log::parse_wait_for_event_i (void)
{
//   char *pos = ACE_OS::strchr (this->info_,'[');
//   long rid = ACE_OS::strtol(pos+1, 0, 10);

  bool done = (ACE_OS::strstr (this->info_,"done (follower)") != 0) ||
    (ACE_OS::strstr(this->info_,"(leader) exit") != 0);

  PeerProcess *pp = this->thr_->incoming();
  if (pp == 0)
    pp = this->thr_->peek_new_connection();
  if (pp != 0 && done)
    {
      this->thr_->exit_wait(pp, this->offset_);
    }
}

void
Log::parse_wait_on_read_i (void)
{
  PeerProcess *pp = this->thr_->incoming();
  this->thr_->exit_wait (pp, this->offset_);
}

void
Log::parse_cleanup_queue_i (void)
{
  char *hpos = ACE_OS::strchr(this->info_,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%d: cleanup queue, error parsing %C, can't find peer "
                  "for handle %d, text = %s\n",
                  this->offset_, this->origin_.c_str(), handle, this->info_));
      return;
    }

  Thread *original_thr = this->thr_;
  GIOP_Buffer *target = original_thr->giop_target();
  if (target == 0 || target->owner() != 0)
    {
      original_thr = this->hostproc_->find_thread_for_handle (handle);
      if (original_thr == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "%d: cleanup queue, no original "
                      "thread found, handle %d\n",
                      this->offset_, handle));
          return;
        }
      target = original_thr->giop_target();
    }
  if (target != 0 && target->cur_size() == 0 && original_thr->has_dup())
    {
      ACE_ERROR ((LM_ERROR, "%d: cleanup queue, swapping targets for thread %d\n",
                  this->offset_, original_thr->id()));
      original_thr->swap_target();
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
                      "could not find invocation for rid = %d on thread %d\n",
                      this->offset_, rid, original_thr->id()));
          rid = target->expected_req_id();
          inv = pp->find_invocation (rid, handle);
          if (inv == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "%d, Cleanup queue still failed to find rid %d, on thread %d\n",
                          this->offset_, rid, original_thr->id()));
              return;
            }
          original_thr->exit_wait(pp, this->offset_);
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
Log::parse_complete_connection_i (void)
{
  if (ACE_OS::strstr (this->info_, "failed") == 0)
    return;
  char *addr = ACE_OS::strrchr(this->info_,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  //  ACE_DEBUG ((LM_DEBUG, "%d, complete_connection, failed for addr %s\n", this->offset_, addr));

  if (this->conn_waiters_.size() > 0)
    {
      // ACE_DEBUG ((LM_DEBUG,"%d: complete_connection: conn_waiters_.size() = %d, addr = %s\n",
      //             this->offset_, this->conn_waiters_.size(), addr));
      for (ACE_DLList_Iterator<PeerProcess> c_iter (this->conn_waiters_);
           !c_iter.done();
           c_iter.advance())
        {
          PeerProcess *waiter = 0;
          c_iter.next(waiter);
          if (waiter != 0 && waiter->match_server_addr (addr))
            {
              c_iter.remove();
              // ACE_DEBUG ((LM_DEBUG,"%d: complete_connection: purging waiter\n",this->offset_));
              // delete waiter;
              break;
            }
        }
    }
}

void
Log::parse_close_connection_i (void)
{
  char *hpos = ACE_OS::strchr(this->info_,'[');
  long handle = ACE_OS::strtol(hpos+1,0,10);
  PeerProcess *pp = this->hostproc_->find_peer(handle);
  if (pp != 0)
    {
      Transport *t = pp->find_transport (handle);
      if (t != 0)
        t->close_offset_ = this->offset_;
    }

  this->hostproc_->remove_peer(handle);
}

void
Log::parse_handler_open_i (bool is_ssl)
{
  char *addr = ACE_OS::strrchr(this->info_,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  c = ACE_OS::strstr(c+1,"on ");
  c += 3;
  if (*c == '[')
    c++;
  long handle = ACE_OS::strtol(c,0,10);
  PeerProcess *pp = 0;
  if (this->conn_waiters_.size() > 0)
    {
      // ACE_DEBUG ((LM_DEBUG,"%d: handler_open: conn_waiters_.size() = %d, addr = %s\n",
      //             this->offset_, this->conn_waiters_.size(), addr));
      for (ACE_DLList_Iterator<PeerProcess> c_iter (this->conn_waiters_);
           !c_iter.done();
           c_iter.advance())
        {
          PeerProcess *waiter = 0;
          c_iter.next(waiter);
          if (waiter != 0 && waiter->match_server_addr (addr))
            {
              pp = waiter;
              c_iter.remove();
              break;
            }
          // else
          //   ACE_DEBUG ((LM_DEBUG,"%d: handler_open: no match waiter addr = %s\n",
          //               this->offset_, (waiter == 0 ? "<null>" :  waiter->server_addr().c_str()) ));
        }
    }

  if (pp == 0)
    {
      // ACE_DEBUG ((LM_DEBUG,"%d: handler_open: calling pop_new_connection, addr = %s\n",
      //             this->offset_, addr));
      pp = this->thr_->pop_new_connection();
    }
  if (pp == 0)
  {
    ACE_ERROR ((LM_ERROR,"%d: handler_open: no pending peer for addr %s\n",
                this->offset_, addr));
    return;
  }

  pp->ssl (is_ssl);

  const ACE_CString &local_addr = this->thr_->pending_local_addr();
  if (local_addr.length() > 0 )
    {
      if (pp->is_server())
        {
          Transport *t = new Transport (local_addr.c_str(), true, this->offset_);
          pp->add_transport (t);
          this->hostproc_->add_client_endpoint (t->client_endpoint_);
        }
      else
        {
          pp->set_server_addr (local_addr);
        }
      this->thr_->pending_local_addr ("");
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
                      this->offset_, this->origin_.c_str()));
          return;
        }
      //      trans->client_endpoint_ = addr;
    }
  else
    {
      trans = new Transport (addr, false, this->offset_);
      pp->add_transport(trans);
    }
  trans->handle_ = handle;
  this->hostproc_->add_peer (handle,pp);
}

void
Log::parse_begin_connection_i (void)
{
  char *addr = ACE_OS::strchr(this->info_,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';
  PeerProcess *pp = this->hostproc_->find_peer(addr);
  if (pp == 0)
    {
      pp = new PeerProcess(this->offset_, true);
      pp->set_server_addr (addr);
    }
  this->conn_waiters_.insert_tail (pp);
  this->thr_->push_new_connection (pp);
  // ACE_DEBUG ((LM_DEBUG,"%d: begin_connection: pushing pp for addr %s\n", offset_,addr));
}

void
Log::parse_connection_handler_ctor_i (void)
{
  // char *c = ACE_OS::strchr (this->info_, '[') + 1;
  // size_t handle = ACE_OS::strtol (c, 0, 10);
  // ACE_DEBUG ((LM_DEBUG,"%d: constructed new handler for %d\n", offset_, handle));

}

void
Log::parse_local_addr_i (void)
{
  char *addr = ACE_OS::strchr(this->info_,'<') +1;
  char *c = ACE_OS::strchr(addr,'>');
  *c = '\0';

  PeerProcess *peer = this->thr_->peek_new_connection();
  if (peer == 0)
    {
      this->thr_->pending_local_addr (addr);
      return;
    }

  if (peer->is_server())
    {
      Transport *t = new Transport (addr, true, this->offset_);
      peer->add_transport (t);
      this->hostproc_->add_client_endpoint (t->client_endpoint_);
    }
  else
    {
      peer->set_server_addr (addr);
    }
}

void
Log::parse_connection_not_complete_i (void)
{
  PeerProcess *pp = this->thr_->pop_new_connection ();
  if (pp != 0)
    {
      // ACE_DEBUG ((LM_DEBUG,"%d: connection_not_complete: popping pp from %d\n",
      //             offset_, pp->offset()));
    }
  else
    ACE_DEBUG ((LM_DEBUG,"%d: connection_not_complete: no pending peer\n", offset_));
}

void
Log::parse_open_as_server_i (void)
{
  // ACE_DEBUG ((LM_DEBUG,"%d: open_as_server: adding peer process\n", offset_));

  this->thr_->push_new_connection (new PeerProcess(this->offset_, false));
}

void
Log::parse_iiop_connection_handler_ctor_i (void)
{
  PeerProcess *pp = this->thr_->peek_new_connection();
  if (pp == 0)
    {
      ACE_ERROR ((LM_ERROR, "%d: iiop_connection_handler_ctor_i: no pending peer on thread\n", this->offset_));
      return;
    }

#if 0
  char *pos = ACE_OS::strchr (this->info_,'[') + 1;
  long tmp_handle = ACE_OS::strtol (pos, 0, 10);
  //  pp->set_handle (tmp_handle);
#endif
}

void
Log::parse_wait_for_connection_i (void)
{
  //  ACE_ERROR ((LM_ERROR,"%d: wait_for_connection, line = %s\n", this->offset_, this->info_));
  if (ACE_OS::strstr (this->info_,"Connection not complete") == 0)
    {
      return;
    }
  else if (ACE_OS::strstr (this->info_,"wait done result =") == 0)
    {
#if 0
      char *pos = ACE_OS::strchr (this->info_, '=') + 2;
      int result = ACE_OS::strtol (pos, 0, 10);
      if (result == 1)
        return;
      pos = ACE_OS::strchr (this->info_, '[') + 1;
      long handle = ACE_OS::strtol (pos, 0, 10);
      PeerProcess *pp = 0;

      ACE_DEBUG ((LM_DEBUG, "%d: wait_for_connection: wait done, result = %d, purging handle = %d\n", this->offset_, result, handle));

      if (this->conn_waiters_.size() > 0)
        {
          for (ACE_DLList_Iterator<PeerProcess> c_iter (this->conn_waiters_);
               !c_iter.done();
               c_iter.advance())
            {
              PeerProcess *waiter = 0;
              c_iter.next(waiter);
              if (waiter != 0)
                {
                  Transport *t = waiter->find_transport (handle);
                  if (t != 0)
                    {
                      pp = waiter;
                      c_iter.remove();
                      break;
                    }
                }
            }
        }
      else
        {
          pp = this->thr_->pop_new_connection ();
          Transport *t = pp->find_transport (handle);
          if (t == 0)
            {
              this->thr_->push_new_connection (pp);
              pp = 0;
            }
        }
      if (pp == 0)
        {
          ACE_ERROR ((LM_ERROR,"%d: wait_for_connection: no pending peer for handle %s\n",
                      this->offset_, handle));
          return;
        }
      delete pp;
#endif
    }
}

void
Log::parse_post_open_i (void)
{
  //  ACE_ERROR ((LM_ERROR,"%d: post_open, line = %s\n", this->offset_, this->line_));
}

void
Log::parse_notify_poa_helper_i (void)
{
  Invocation *inv = this->thr_->current_invocation ();
  if (inv == 0)
    {
      ACE_ERROR ((LM_ERROR,"%d: notify_poa_helper line = %s, no current invocation on thread\n", this->offset_, this->info_));
      return;
    }
  bool activate = ACE_OS::strstr (this->info_, "Activating") != 0;
  char *idpos = ACE_OS::strstr (this->info_, "id = ");
  long objid = ACE_OS::strtol (idpos + 5, 0, 10);
  idpos = ACE_OS::strstr (idpos + 5, "in  POA : ");
  long poaid = ACE_OS::strtol (idpos + 10, 0, 10);

  char buffer[100];
  ACE_OS::sprintf (buffer,"Notify object %s, object id %ld, POA %ld on line %ld",
                   (activate ? "activation" : "deactivation"), objid, poaid,
                   (unsigned long)this->offset_);
  ACE_CString text (buffer);

  inv->add_notify_incident (text, this->offset_);
}

void
Log::parse_notify_object_i (void)
{
  Invocation *inv = this->thr_->current_invocation ();
  if (inv == 0)
    {
      // ACE_ERROR ((LM_ERROR,"%d: notify_object line = %s, no current invocation on thread\n", this->offset_, this->info_));
    }

  char *ptr = ACE_OS::strstr (this->info_, "object:") + 7;
  u_long objid = ACE_OS::strtol (ptr, &ptr, 16);
  char note[100];
  note[0] = 0;
  if (ACE_OS::strstr (ptr, "created") != 0)
    {
      ::sprintf (note, "Created notify object %lx",objid);
#if 0
      NotifyObject notobj = new NotifyObject (objid, this->offset_, this->timestamp_);
      this->hostproc_->add_notify_obj (notobj);
#endif
    }
  else if (ACE_OS::strstr (ptr, "destroyed") != 0)
    {
      ::sprintf (note, "Destroyed notify object %lx",objid);
#if 0
      NotifyObject notobj = this->hostproc_->find_notify_obj (objid);
      if (notobj == 0)
        {
          ACE_ERROR ((LM_ERROR, "%d: could not find notify object %lx\n", this->offset_, objid));
        }
      else
        {
          notobj->destroyed (this->offset_, this->timestamp_);
        }
#endif
    }
  else if (ACE_OS::strstr (ptr, "incr ") != 0)
    {
      ptr = ACE_OS::strchr (ptr, '=');
      int count = ACE_OS::strtol (ptr + 2, 0, 10);
      ::sprintf (note, "increment reference notify object %lx, count now %d",objid, count);
#if 0
      NotifyObject notobj = this->hostproc_->find_notify_obj (objid);
      if (notobj == 0)
        {
          notobj = new NotifyObject (objid, this->offset_, this->timestamp_);
          this->hostproc_->add_notify_obj (notobj);
        }
      notobj->incr  (this->offset_, this->timestamp_);
#endif
    }
  else if (ACE_OS::strstr (ptr, "decr ") != 0)
    {
      ptr = ACE_OS::strchr (ptr, '=');
      int count = ACE_OS::strtol (ptr + 2, 0, 10);
      ::sprintf (note, "decrement reference notify object %lx, count now %d",objid, count);
#if 0
      NotifyObject notobj = this->hostproc_->find_notify_obj (objid);
      if (notobj == 0)
        {
          ACE_ERROR ((LM_ERROR, "%d: could not find notify object %x\n", this->offset_, objid));
        }
      else
        {
          notobj->decr (this->offset_, this->timestamp_);
        }
#endif
    }

  ACE_CString text (note);
  if (inv)
    inv->add_notify_incident (text, this->offset_);
}

void
Log::get_timestamp (void)
{
  const char *time_tok = ACE_OS::strchr (this->line_,'@');
  size_t len = (size_t)(time_tok - this->line_);
  if (time_tok != 0 && len < 30)
    {
      this->timestamp_ = ACE_CString (this->line_, len);
#if 0
      int year, mon, day;
      int hr, min, sec, msec;
      ::sscanf (hms+1,"%d-%d-%d %d:%d:%d.%d", &year, &mon, &day, &hr, &min, &sec, &msec);
      time = (hr * 3600 + min *60 + sec) * 1000 + msec;
      if (this->time_ > time)
        time += 24 * 3600 * 1000;
#endif
    }
}

void
Log::parse_line (void)
{
  if (this->dump_target_ != 0)
    {
      this->handle_msg_octets ();
      return;
    }

  this->get_preamble();
  this->get_timestamp();

  if (ACE_OS::strstr (this->info_, "Handler::open, IIOP connection to peer") != 0)
    {
      this->parse_handler_open_i(false);
    }
  else if (ACE_OS::strstr (this->info_, "GIOP_Message_Base::dump_msg,") != 0)
    {
      this->parse_dump_giop_msg_i();
    }
  else if (ACE_OS::strstr (this->info_, "GIOP message - HEXDUMP") != 0)
    {
      this->parse_HEXDUMP_i();
    }
  else if (ACE_OS::strstr (this->info_, "open_i, listening on:") != 0)
    {
      this->parse_open_listener_i();
    }
  else if (ACE_OS::strstr (this->info_, "Muxed_TMS[") != 0)
    {
      this->parse_muxed_tms_i();
    }
  else if (ACE_OS::strstr (this->info_, "Exclusive_TMS::request_id") != 0)
    {
      this->parse_exclusive_tms_i();
    }
  else if (ACE_OS::strstr (this->info_, "process_parsed_messages") != 0)
    {
      this->parse_process_parsed_msgs_i();
    }
  else if (ACE_OS::strstr (this->info_, "wait_for_event") != 0)
    {
      this->parse_wait_for_event_i();
    }
  else if (ACE_OS::strstr (this->info_, "Wait_On_Read") != 0)
    {
      this->parse_wait_on_read_i();
    }
  else if (ACE_OS::strstr (this->info_, "::cleanup_queue, byte_count") != 0)
    {
      this->parse_cleanup_queue_i();
    }
  else if (ACE_OS::strstr (this->info_, "close_connection_eh") != 0)
    {
      this->parse_close_connection_i();
    }
  else if (ACE_OS::strstr (this->info_, "complete_connection, connection to") != 0)
    {
      this->parse_complete_connection_i();
    }
  else if (ACE_OS::strstr (this->info_, "IIOP_Connector::begin_connection, to ") != 0)
    {
      this->parse_begin_connection_i();
    }
  else if (ACE_OS::strstr (this->info_, "::IIOP_Connection_Handler ") != 0)
    {
      this->parse_connection_handler_ctor_i();
    }
  else if (ACE_OS::strstr (this->info_, "IIOP_Connection_Handler::open, The local addr is") != 0)
    {
      this->parse_local_addr_i();
    }
  else if (ACE_OS::strstr (this->info_, "Connection not complete.") != 0)
    {
      this->parse_connection_not_complete_i();
    }
  else if (ACE_OS::strstr (this->info_, "opened as TAO_SERVER_ROLE") != 0)
    {
      this->parse_open_as_server_i();
    }
  else if (ACE_OS::strstr (this->info_, "Transport_Connector::connect, got an existing connected") != 0)
    {
      this->parse_got_existing_i();
    }
  else if (ACE_OS::strstr (this->info_, "Transport_Connector::wait_for_connection_completion") != 0)
    {
      this->parse_wait_for_connection_i();
    }
  else if (ACE_OS::strstr (this->info_, "Transport::post_open, tport") != 0)
    {
      this->parse_post_open_i();
    }
  else if (ACE_OS::strstr (this->info_, "SSLIOP connection from client") != 0)
    {
      this->parse_handler_open_i(true);
    }
  else if (ACE_OS::strstr (this->info_, "SSLIOP connection accepted from server") != 0)
    {
      this->parse_local_addr_i();
    }
  else if (ACE_OS::strstr (this->info_, "POA_Helper") != 0)
    {
      this->parse_notify_poa_helper_i();
    }
  else if (ACE_OS::strstr (this->info_, "object:") != 0)
    {
      this->parse_notify_object_i ();
    }
  return;
}
