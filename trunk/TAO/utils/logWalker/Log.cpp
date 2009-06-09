// $Id$

#include "Log.h"
#include "Invocation.h"
#include "PeerProcess.h"
#include "HostProcess.h"
#include "Session.h"
#include "Thread.h"

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
Log::init (ACE_TCHAR *filename, const char *alias)
{
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
  if (this->alias_.length() > 0)
    {
      switch (numprocs)
	{
	case 0:
	  hp->proc_name(alias_);
	  break;
	case 1:
	  {
	    ACE_CString a2 = alias_ + "_1";
	    HostProcess *first;
	    if (this->procs_.get(first) == 0)
	      first->proc_name(a2);
	  }
	  //fallthru
	default:
	  {
	    char ext[10];
	    ACE_OS::sprintf(ext,"_%d",numprocs+1);
	    ACE_CString a2 = alias_ + ext;
	    hp->proc_name(a2);
	  }
	}
    }
  this->session_.add_process(hp);
  return hp;
}

void 
Log::parse_line (char *line, size_t offset)
{
  // first, is it a client connection to a new peer?
  // next is it a server connection to a new peer?
  // is it a new request or reply?
  // a GIOP hex dump?

  static const char * exprs[] = {
    "make_connection, new connected",
    "Handler::open, IIOP connection to peer",
    "GIOP_Message_Base::dump_msg,",
    "GIOP message - HEXDUMP",
    "open_i, listening on:",
    "Muxed_TMS[", // initiating new request
    "process_parsed_messages", // server side new incoming
    "wait_for_event",
    "::cleanup_queue, byte_count",
    "close_connection_eh",
    "IIOP_Connector::begin_connection, to ",
    "IIOP_Connection_Handler::open, The local addr is",
    "opened as TAO_SERVER_ROLE",
    //connection to peer closed.
    0
  };

  if (this->dump_target_ != 0)
    {
      int pos = this->dump_target_->add_octets(line);
      if (pos == 1) // need to validate target
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
		      new_target->transfer_from (this->dump_target_);
		      this->dump_target_ = new_target;
		      th->exit_wait(th->incoming(), offset);
		      t_iter.remove();
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
		      th->exit_wait(th->incoming(), offset);
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
      return;
    }
     
  int i = 0;
  for (; exprs[i] != 0; i++)
    {
      if (ACE_OS::strstr(line, exprs[i]) != 0)
        break;
    }
  if (exprs[i] == 0)
    return;

  long pid = 0;
  long tid = 0;
  this->get_pid_tid(pid,tid,line);

  HostProcess *hp = this->get_host(pid);
  Thread *thr = hp == 0 ? 0 : hp->find_thread (tid);

  switch (i) 
    {
    case 0: //"make_connection, new connected" 
      {// no longer needed... at least for now.
        break;
      }
    case 1: // "Handler::open, IIOP connection to peer"
      {
        char *addr = ACE_OS::strchr(line,'<') +1; 
        char *c = ACE_OS::strchr(addr,'>');
        *c = '\0';
        c = ACE_OS::strstr(c+1,"on ");
        long handle = ACE_OS::strtol(c + 3,0,10);
        PeerProcess *pp = thr->pending_peer();
	if (pp == 0)
	  {
	    ACE_ERROR ((LM_ERROR,"no pending peer for file %s, line %d\n",
			this->origin_.c_str(), offset));
	    break;
	  }
	if (!pp->is_client())
	  {
	    pp->set_addr(addr,false);
	  }
	thr->pending_peer(0);
	pp->handle(handle);
	hp->add_peer(pp, pp->is_client());
        break;
      }
    case 2: //GIOP_Message_Base::dump_msg, 
      {
	int mode = ACE_OS::strstr (line,"send") != 0 ? 0 : 1;
	mode +=  ACE_OS::strstr (line,"Request") != 0 ? 0 : 2;
        char *pos = strrchr (line,'[');
        long rid = ACE_OS::strtol(pos+1, 0, 10);
        PeerProcess *pp = thr->incoming();
	if (pp == 0)
	  {
	    ACE_ERROR((LM_ERROR,"dump_msg, could not find pp for incoming\n"));
	    break;
	  }
	if (mode < 2)
	  thr->enter_wait(pp);
	Invocation::GIOP_Buffer *target = 0;
	switch (mode) 
	  {
	  case 1: { // receiving request
	    Invocation *inv = pp->new_invocation (rid);
	    if (inv == 0)
	      {
		ACE_ERROR ((LM_ERROR,
			    "process %d already has invocation "
			    "%d, at line %d\n",
			    pp->handle(), rid, offset));
		break;
	      }
	    inv->init (line, offset, thr);
	    target = inv->octets(true);
	    break;
	  }	    
	  case 0: // sending request
	  case 3: { // receiving reply
	    Invocation *inv = pp->find_invocation(rid);
	    if (inv == 0)
	      {
		ACE_ERROR ((LM_ERROR,"line %d, could not find existing invocation for req_id %d\n",
			    offset, rid));
		inv = pp->new_invocation (rid);
	      }
	    inv->init (line, offset, thr);
	    target = inv->octets(mode == 0);
	    if (mode == 3)
	      thr->exit_wait(pp, offset);
	    break;
	  }
	  case 2: { // sending reply
	    target = new Invocation::GIOP_Buffer(line,offset, thr);
	    break;
	  }
	  default:;
	  }
 
	thr->set_giop_target (target);
        this->giop_waiters_.insert_tail(thr);
	break;
      }
    case 3: // "GIOP message - HEXDUMP"
      {
        char *pos = ACE_OS::strstr (line,"HEXDUMP");
        pos += 8;
        size_t len = ACE_OS::strtol (pos, 0, 10);
        for (ACE_DLList_Reverse_Iterator<Thread> t_iter(this->giop_waiters_);
             !t_iter.done();
             t_iter.advance())
          {
            t_iter.next(thr);
	    Invocation::GIOP_Buffer *target = thr->giop_target();
	    if (target == 0 || target->expected_size() != len || target->size() > 0)
	      continue;
	    this->dump_target_ = target;
	    break;
	  }
        if (this->dump_target_ == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "Could not find a giop waiter for size %d, offset = %d\n",
                        len, offset));
          }
        else
          this->dump_target_->init_buf (line);
        break;
      }
    case 4: // "open_i, listening on:"
      {
        char *addr = ACE_OS::strchr(line,'<') +1; 
        char *c = ACE_OS::strchr(addr,'>');
        *c = '\0';
        ACE_CString s_addr(addr);
        hp->add_endpoint(s_addr);
        break;
      }
    case 5: // "Muxed_TMS"
      {
        char *hpos = ACE_OS::strchr(line,'[');
        long handle = ACE_OS::strtol(hpos+1,0,10);
        hpos = ACE_OS::strchr(hpos, '<');
        long req_id = ACE_OS::strtol(hpos+1,0,10);
        PeerProcess *pp = hp->find_peer(handle);
        if (pp == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "Error parsing %C, line %d, can't find peer "
                        "for handle %d, text = %s\n",
                        this->origin_.c_str(), offset, handle, line));
            break;
          }

        pp->new_invocation(req_id);
        thr->incoming_from (pp);
        break;
      }
    case 6: // "process_parsed_messages"
      {
        char *hpos = ACE_OS::strchr(line,'[');
        long handle = ACE_OS::strtol(hpos+1,0,10);

        PeerProcess *pp = hp->find_peer(handle);
        if (pp == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "Error parsing %C, line %d, can't find peer "
                        "for handle %d, text = %s\n",
                        this->origin_.c_str(), offset, handle, line));
            break;
          }
        thr->incoming_from (pp);
        break;
      }
    case 7: // "wait_for_event"
      {
	// thr->exit_wait()?
        break;
      }
    case 8: // cleanup_queue
      {
        char *hpos = ACE_OS::strchr(line,'[');
        long handle = ACE_OS::strtol(hpos+1,0,10);
	PeerProcess *pp = hp->find_peer(handle);
	Invocation::GIOP_Buffer *target = thr->giop_target();
	thr->set_giop_target(0);
	if (target != 0 && target->owner() == 0)
	  {
	    size_t rid = target->actual_req_id();
	    char mtype = target->type();
	    Invocation *inv = pp->find_invocation(rid);
	    if (inv == 0)
	      {
		ACE_ERROR ((LM_ERROR,
			    "Cleanup queue detected at line %d, "
			    "could not find invocation for rid = %d\n", 
			    offset, rid));
		rid = target->expected_req_id();
		inv = pp->find_invocation (rid);
		if (inv == 0)
		  {
		    ACE_ERROR ((LM_ERROR,
				"Cleanup queue still failed to find rid %d\n",
				rid));
		    break;
		  }
		thr->exit_wait(pp, offset);
		mtype = target->expected_type();
	      }
	    inv->set_octets (mtype == 0, target);
	    size_t len = 0;
	    const char *oid = target->target_oid(len);
	    if (mtype == 0 && len > 0)
	      inv->set_target (oid, len);
	  }
	break;
      }
    case 9: // close_connection_eh
      {
        char *hpos = ACE_OS::strchr(line,'[');
        long handle = ACE_OS::strtol(hpos+1,0,10);
	hp->close_peer (handle,offset);
	break;
      }
    case 10: // IIOP_Connector::begin_connection
      {
        char *addr = ACE_OS::strchr(line,'<') +1; 
        char *c = ACE_OS::strchr(addr,'>');
        *c = '\0';
	thr->pending_peer (new PeerProcess(line,offset,true,addr,0));
	break;
      }
    case 11: // IIOP_Connection_Handler::open, The local addr is
      {
	PeerProcess *peer = thr->pending_peer();
	if (peer == 0)
	  ACE_ERROR((LM_ERROR, "file %s, line %d, no pending client or server\n",
		     this->origin_.c_str(), offset));
	else
	  {
	    char *addr = ACE_OS::strchr(line,'<') +1; 
	    char *c = ACE_OS::strchr(addr,'>');
	    *c = '\0';
	    peer->set_addr(addr, true);
	    ACE_CString s_addr(addr);
	    hp->add_endpoint(s_addr);
	  }
	break;
      }
    case 12: // opened as TAO_SERVER_ROLE
      {
	thr->pending_peer (new PeerProcess(line,offset,false,"",0));
	break;
      }
    default: // future expansion
      {
        break;
      }
  }
}

void
Log::summarize (void)
{
  // todo
}
