// $Id$

#include "Invocation.h"
#include "GIOP_Buffer.h"
#include "PeerProcess.h"
#include "PeerObject.h"
#include "Session.h"
#include "HostProcess.h"
#include "Thread.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"

Invocation::Invocation (PeerProcess *peer, Thread *thr, size_t rid)
  :req_octets_(0),
   repl_octets_(0),
   waiter_ (thr),
   notify_incidents_ (),
   peer_(peer),
   req_id_(rid),
   target_(0),
   handle_(thr->active_handle())
{
}

Invocation::~Invocation (void)
{
  delete this->req_octets_;
  delete this->repl_octets_;
}

bool
Invocation::init (const char * text, size_t offset, Thread *thread)
{
  if (GIOP_Buffer::size_leadin == 0)
    GIOP_Buffer::init_leadin (Session::tao_version());
  const char *size_str = ACE_OS::strstr(text, GIOP_Buffer::size_leadin);
  const char *id = size_str == 0 ? 0 : ACE_OS::strchr(size_str, '[');
  if (size_str == 0 || id == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Not a request preamble line %d:\n '%s'\n",offset,
                  text));
      return false;
  }


  if( ACE_OS::strstr(text,"Request") == 0)
    this->repl_octets_ = new GIOP_Buffer(text, offset, thread, this);
  else
    this->req_octets_  = new GIOP_Buffer(text, offset, thread, this);
  return true;
}

void
Invocation::add_notify_incident (const ACE_CString &text, size_t /* offset */)
{
  this->notify_incidents_.enqueue_tail (text);
}

bool
Invocation::is_oneway(void) const
{
  return this->req_octets_ == 0 ? false : this->req_octets_->is_oneway();
}

void
Invocation::set_target (const char *oid, size_t oid_len)
{
  PeerObject *tgt = this->peer_->object_for (oid, oid_len);
  if (tgt == 0)
    return;

  if (this->target_ == 0)
    {
      this->target_ = tgt;
      this->target_->add_invocation (this);
    }
}

bool
Invocation::message_complete (void)
{
  if (this->is_oneway())
    {
      if (this->req_octets_ == 0 || !this->req_octets_->is_full())
        return false;
    }
  else
    if (this->repl_octets_ == 0 || !this->repl_octets_->is_full())
      return false;

  return true;
}

GIOP_Buffer *
Invocation::octets (bool request)
{
  return request ? this->req_octets_ : this->repl_octets_ ;
}

GIOP_Buffer *
Invocation::give_octets (bool request)
{
  GIOP_Buffer *result = request ? this->req_octets_ : this->repl_octets_ ;

  if (request)
    this->req_octets_ = 0;
  else
    this->repl_octets_ = 0;
  return result;
}

void
Invocation::set_octets (bool request, GIOP_Buffer *octets)
{
  if (request)
    {
      if (this->req_octets_ != 0)
        {
          return;
        }
      this->req_octets_ = octets;
    }
  else
    {
      if (this->repl_octets_ != 0)
        {
          return;
        }
      this->repl_octets_ = octets;
    }
  octets->owner(this);
}

bool
Invocation::sent_request (void) const
{
  if (this->req_octets_)
    return this->req_octets_->sending();
  if (this->repl_octets_)
    return !this->repl_octets_->sending();
  return false;
}

size_t
Invocation::request_id (void) const
{
  return this->req_octets_ == 0 ? this->req_id_ : this->req_octets_->expected_req_id();
}

size_t
Invocation::expected_size (void) const
{
  if (repl_octets_ != 0)
    return repl_octets_->expected_size();
  return req_octets_->expected_size();
}

size_t
Invocation::request_bytes (void) const
{
  return req_octets_ != 0 ? req_octets_->expected_size() : 0;
}

Thread *
Invocation::waiter (void) const
{
  return this->waiter_;
}

long
Invocation::handle (void) const
{
  return this->handle_;
}

bool
Invocation::contains (size_t line)
{
  if (this->req_octets_ == 0 || this->repl_octets_ == 0)
    return false;
  return
    line > this->req_octets_->log_posn() &&
    line < this->repl_octets_->log_posn();
}

size_t
Invocation::req_line (void)
{
  return this->req_octets_ == 0 ? 0 : this->req_octets_->log_posn();
}

void
Invocation::new_line (ostream &strm, int indent, int offset, bool add_nl, bool show_indent)
{
  if (add_nl)
    {
      strm << "\n";
    }

  if (indent > 9)
    {
      if (show_indent)
        strm << "[indent " << indent << "] ---> ";
      else
        strm << "                   ";
    }
  else
    {
      indent += offset;
      while (indent-- > 0)
        strm << "  ";
    }
}

void
Invocation::dump_detail (ostream &strm,
                         int indent,
                         Dump_Mode mode,
                         bool show_handle)
{
  const char *opname = "";
  const char *dir_1 = "sent to ";
  const char *dir_2 = " in ";

  if (this->req_octets_ != 0)
    {
      opname = this->req_octets_->operation();
      if (this->req_octets_->sending())
        {
          dir_1 = "recv for ";
          dir_2 = " from ";
        }
    }

  this->new_line (strm, indent, 0, false, true);

  if (opname == 0 || opname[0] == 0)
    opname = "<no operation>";

  strm << dir_1;
  if (this->target_ == 0)
    strm << "<unknown object>" ;
  else
    strm << this->target_->name();
  if (mode == Dump_Proc || mode == Dump_Both)
    strm << dir_2 << this->peer_->id() << ",";

  strm << " req " << this->req_id_;
  if (show_handle)
    strm << "(h=" << this->handle_ << ")";
  strm << " [" << opname << "]\t";
  time_t req_time = 0;
  time_t rep_time = 0;
  size_t delta = 0;
  if (!this->is_oneway() && this->req_octets_ != 0)
    {
      req_time = this->req_octets_->time();
      if (this->repl_octets_ != 0)
        {
          rep_time = this->repl_octets_->time();
          delta = this->repl_octets_->log_posn() -
            this->req_octets_->log_posn();
        }
    }
  if (req_time != 0 && rep_time != 0)
    strm << " took " << (rep_time - req_time) << " ms";

  if (this->req_octets_ != 0)
    {
      strm << " Request, ";
      if (this->req_octets_->num_contexts() > 0)
        strm << "with " << this->req_octets_->num_contexts() << " contexts, ";
      strm << "size " << this->req_octets_->expected_size() << " ";
      strm << "line " << this->req_octets_->log_posn();
      if (mode == Dump_Thread || mode == Dump_Both)
        strm << " " << this->req_octets_->thread()->alias();
    }
  else
    strm << " <no request found>";
  if (this->is_oneway())
    strm << " oneway";
  else
    {
      if (this->repl_octets_ != 0)
        {
          strm << " Reply, ";
          if (this->repl_octets_->num_contexts() > 0)
            strm << "with " << this->repl_octets_->num_contexts()
                 << " contexts, ";
          strm << "size " << this->repl_octets_->expected_size() << " ";
          strm << "line " << this->repl_octets_->log_posn();
#if defined (SHOW_THREAD_ID)
          strm << " " << this->repl_octets_->thread()->alias();
#endif
          char rstat = this->repl_octets_->reply_status();
          if (rstat == 1 || rstat == 2)
            {
              strm << (rstat == 1 ? " User" : " System") << " Exception";
            }
          else if (rstat == 3)
            {
              strm << " Location Forward";
            }
        }
      else
        strm << " <no reply found>";
    }
  if (delta > 0)
    strm << " log span = " << delta;
  if (this->req_octets_ != 0 && this->req_octets_->has_octets())
    this->dump_special_details (strm, indent, opname);
  strm << endl;
  if (this->notify_incidents_.size() > 0)
    {
      for (NotifyIncidents::ITERATOR i = this->notify_incidents_.begin();
           !(i.done()); i.advance())
        {
          ACE_CString *note;
          i.next(note);
          strm << "            " << *note << endl;
        }
    }
}

void
Invocation::dump_special_details (ostream &strm, int indent, const char *opname)
{
  char rstat = 0;
  if (this->repl_octets_ != 0 && this->repl_octets_->has_octets())
    rstat = this->repl_octets_->reply_status();
  int opid = 0;
  if (ACE_OS::strcmp (opname, "_is_a") == 0)
    {
      opid = 1;
      ACE_InputCDR &giop_cdr = this->req_octets_->payload();
      ACE_InputCDR cdr (giop_cdr.rd_ptr(),
                        giop_cdr.length(),
                        giop_cdr.byte_order());
      this->new_line (strm, indent, 8, true, false);
      ACE_CDR::ULong len;
      if (cdr >> len)
        {
          strm << "expected type ( len = " << len << ") " << cdr.rd_ptr();
        }
      else
        {
          strm << "expected type parse failed";
        }
    }
  else if (ACE_OS::strcmp (opname, "_get_MyID") == 0)
    {
      opid = 2;
    }
  else if (ACE_OS::strcmp (opname, "resolve_str") == 0)
    {
      opid = 3;
      ACE_InputCDR &giop_cdr = this->req_octets_->payload();
      ACE_InputCDR cdr (giop_cdr.rd_ptr(),
                        giop_cdr.length(),
                        giop_cdr.byte_order());
      ACE_CDR::ULong len;
      if (cdr >> len)
        {
          this->new_line (strm, indent, 8, true, false);
          strm << "name (len = " << len << ") " << cdr.rd_ptr();
        }
    }
  else if (ACE_OS::strcmp (opname, "resolve") == 0 ||
           ACE_OS::strcmp (opname, "bind") == 0 ||
           ACE_OS::strcmp (opname, "rebind") == 0 ||
           ACE_OS::strcmp (opname, "bind_new_context") == 0
           )
    {
      opid = 3;
      ACE_InputCDR &giop_cdr = this->req_octets_->payload();
      ACE_InputCDR cdr (giop_cdr.rd_ptr(),
                        giop_cdr.length(),
                        giop_cdr.byte_order());
      ACE_CDR::ULong count;
      if (cdr >> count)
        {
          this->new_line (strm, indent, 3, true, false);
          strm << "name_seq.lengh = " << count << " ";
          while (count-- > 0)
            {
              ACE_CDR::ULong len;
              if (!(cdr >> len))
                break;
              strm << cdr.rd_ptr();
              if (!cdr.skip_bytes (len))
                break;
              if (!(cdr >> len))
                break;
              if (len > 1)
                {
                  strm << "." << cdr.rd_ptr();
                }
              if (!cdr.skip_bytes (len))
                break;
              if (count > 0)
                {
                  strm << "/";
                }
            }
          if (static_cast<ACE_CDR::Long>(count) > 0)
            {
              strm << " [name truncated]";
            }
        }
    }
  else if (ACE_OS::strcmp (opname, "register_activator") == 0 ||
           ACE_OS::strcmp (opname, "unregister_activator") == 0 ||
           ACE_OS::strcmp (opname, "notify_child_death") == 0 ||
           ACE_OS::strcmp (opname, "start_server") == 0 ||
           ACE_OS::strcmp (opname, "wait_for_startup") == 0 ||
           ACE_OS::strcmp (opname, "activate_server") == 0 ||
           ACE_OS::strcmp (opname, "add_or_update_server") == 0 ||
           ACE_OS::strcmp (opname, "shutdown_server") == 0 ||
           ACE_OS::strcmp (opname, "server_is_running") == 0 ||
           ACE_OS::strcmp (opname, "server_is_shutting_down") == 0 ||
           ACE_OS::strcmp (opname, "find") == 0)
    {
      ACE_InputCDR &giop_cdr = this->req_octets_->payload();
      ACE_InputCDR cdr (giop_cdr.rd_ptr(),
                        giop_cdr.length(),
                        giop_cdr.byte_order());
      ACE_CDR::ULong len;
      if (cdr >> len)
        {
          this->new_line (strm, indent, 8, true, false);
          if (this->repl_octets_ == 0)
            strm << "<nrf> ";
          else if (ACE_OS::strcmp (opname, "server_is_running") == 0)
            strm << "<sir> ";
          else if (ACE_OS::strcmp (opname, "server_is_shutting_down") == 0)
            strm << "<ssd> ";
          strm << "name ( len = " << len << ") " << cdr.rd_ptr();
        }
    }

  if (this->repl_octets_ == 0 || !this->repl_octets_->has_octets())
    return;

  ACE_InputCDR &giop_cdr = this->repl_octets_->payload();
  ACE_InputCDR cdr (giop_cdr.rd_ptr(),
                    giop_cdr.length(),
                    giop_cdr.byte_order());

  if (rstat == 0)
    {
      switch (opid)
        {
        case 1:
          {
            ACE_CDR::Boolean x;
            if (cdr >> ACE_InputCDR::to_boolean (x))
              strm << " reply: " << (x ? "yes" : "no");
            break;
          }
        case 2:
          {
            this->new_line (strm, indent, 8, true, false);
            ACE_CDR::Long x;
            if (cdr >> x)
              strm << "MyID reply: " << x;
            break;
          }
        default:;
        }
    }
  else
    {
      this->new_line (strm, indent, 8, true, false);
      ACE_CDR::ULong len;
      if (rstat == 1 || rstat == 2)
        {
          strm << "Exception ";
        }
      else
        {
          strm << "Redirect to ";
        }
      if (cdr >> len)
        {
          strm << "(len = " << len << ") " << cdr.rd_ptr();
        }
      else
        {
          strm << "unparsable";
        }
    }

}
