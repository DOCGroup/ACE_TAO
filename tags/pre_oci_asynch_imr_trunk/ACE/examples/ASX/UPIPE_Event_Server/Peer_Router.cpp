// $Id$

#if !defined (_PEER_ROUTER_C)

#define _PEER_ROUTER_C

#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"

#include "Peer_Router.h"
#include "Options.h"



#if defined (ACE_HAS_THREADS)

// Define some short-hand macros to deal with long templates
// names...

#define PH  PEER_HANDLER
#define PA  PEER_ACCEPTOR
#define PAD PEER_ADDR
#define PK  PEER_KEY
#define PM  PEER_MAP

template <class PH, class PK> int
Acceptor_Factory<PH, PK>::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("df:"), 0);
  ACE_UPIPE_Addr addr;

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'f':
        addr.set (get_opt.opt_arg ());
        break;
      case 'd':
        break;
      default:
        break;
      }

  if (this->open (addr, ACE_Reactor::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  return 0;
}

template <class PH, class PK>
Acceptor_Factory<PH, PK>::Acceptor_Factory (Peer_Router<PH, PK> *pr)
  : pr_ (pr)
{
}

template <class PH, class PK> Peer_Router<PH, PK> *
Acceptor_Factory<PH, PK>::router (void)
{
  return this->pr_;
}

template <class ROUTER, class KEY>
Peer_Handler<ROUTER, KEY>::Peer_Handler (ACE_Thread_Manager *tm)
  : ACE_Svc_Handler<ACE_UPIPE_STREAM, ACE_MT_SYNCH> (tm)
{
}

template <class ROUTER, class KEY> int
Peer_Handler<ROUTER, KEY>::svc (void)
{
  // Just a try !!  we're just reading from our ACE_Message_Queue.
  ACE_Message_Block *db, *hb;
  int n;
  // do an endless loop
  for (;;)
    {
      db = new ACE_Message_Block (BUFSIZ);
      hb = new ACE_Message_Block (sizeof (KEY), ACE_Message_Block::MB_PROTO, db);

      if ((n = this->peer ().recv (db->rd_ptr (), db->size ())) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("recv failed")), -1);
      else if (n == 0) // Client has closed down the connection.
        {
          if (this->router_task_->unbind_peer (this->get_handle ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                               ACE_TEXT ("unbind failed")), -1);
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) shutting down\n")));
          return -1; // We do not need to be deregistered by reactor
          // as we were not registered at all
        }
      else // Transform incoming buffer into a Message and pass downstream.
        {
          db->wr_ptr (n);
          *(ACE_HANDLE *) hb->rd_ptr () = this->get_handle (); // structure assignment.
          hb->wr_ptr (sizeof (long));
          if (this->router_task_->reply (hb) == -1)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Peer_Handler.svc : router_task->reply failed\n")));
              return -1;
            }

          // return this->router_task_->reply (hb) == -1 ? -1 : 0;
        }
    }
  ACE_NOTREACHED(return 0);
}

template <class ROUTER, class KEY> int
Peer_Handler<ROUTER, KEY>::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  return this->peer ().send_n (mb->rd_ptr (), mb->length ());
}

// Create a new handler and point its ROUTER_TASK_ data member to the
// corresponding router.  Note that this router is extracted out of
// the Acceptor_Factory * that is passed in via the
// ACE_Acceptor::handle_input() method.

template <class ROUTER, class KEY> int
Peer_Handler<ROUTER, KEY>::open (void *a)
{
  ACE_TCHAR buf[BUFSIZ], *p = buf;

  if (this->router_task_->info (&p, sizeof buf) != -1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) creating handler for %s, fd = %d, this = %@\n"),
                buf, this->get_handle (), a));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("info")), -1);

  if ( this->activate (options.t_flags ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activation of thread failed")), -1);
  else if (this->router_task_->bind_peer (this->get_handle (), this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("bind_peer")), -1);
  return 0;
}

// Receive a message from a supplier..

template <class ROUTER, class KEY> int
Peer_Handler<ROUTER, KEY>::handle_input (ACE_HANDLE h)
{

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) input arrived on sd %d\n"), h));
//  ACE_Reactor::instance ()->remove_handler(h,
//                                          ACE_Event_Handler::ALL_EVENTS_MASK
//                                          |ACE_Event_Handler::DONT_CALL);
// this method should be called only if the peer shuts down
// so we deactivate our ACE_Message_Queue to awake our svc thread

  return 0;

#if 0
  ACE_Message_Block *db = new ACE_Message_Block (BUFSIZ);
  ACE_Message_Block *hb = new ACE_Message_Block (sizeof (KEY), ACE_Message_Block::MB_PROTO, db);
  int           n;

  if ((n = this->peer ().recv (db->rd_ptr (), db->size ())) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("recv failed")), -1);
  else if (n == 0) // Client has closed down the connection.
    {
      if (this->router_task_->unbind_peer (this->get_handle ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("unbind failed")), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) shutting down %d\n", h));
      return -1; // Instruct the ACE_Reactor to deregister us by returning -1.
    }
  else // Transform incoming buffer into a Message and pass downstream.
    {
      db->wr_ptr (n);
      *(long *) hb->rd_ptr () = this->get_handle (); // structure assignment.
      hb->wr_ptr (sizeof (long));
      return this->router_task_->reply (hb) == -1 ? -1 : 0;
    }
#endif
}

template <class PH, class PK>
Peer_Router<PH, PK>::Peer_Router (ACE_Thread_Manager *tm)
  : ACE_Task<ACE_MT_SYNCH> (tm)
{
}

template <class PH, class PK> int
Peer_Router<PH, PK>::send_peers (ACE_Message_Block *mb)
{
  ACE_Map_Iterator<PK, PH *, ACE_RW_Mutex> map_iter = this->peer_map_;
  int bytes      = 0;
  int iterations = 0;
  ACE_Message_Block *data_block = mb->cont ();
  for (ACE_Map_Entry<PK, PH *> *ss = 0;
       map_iter.next (ss) != 0;
       map_iter.advance ())
    {
      if (options.debug ())
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) sending to peer via sd %d\n"),
                    ss->ext_id_));

      iterations++;
      bytes += ss->int_id_->put (data_block);
    }

  mb->release ();
  return bytes == 0 ? 0 : bytes / iterations;
}

template <class PH, class PK>
Peer_Router<PH, PK>::~Peer_Router (void)
{
}

template <class PH, class PK> int
Peer_Router<PH, PK>::fini (void)
{
  delete this->acceptor_;
  return 0;
}

template <class PH, class PK> int
Peer_Router<PH, PK>::control (ACE_Message_Block *mb)
{
  ACE_IO_Cntl_Msg *ioc = (ACE_IO_Cntl_Msg *) mb->rd_ptr ();
  ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds command;

  switch (command = ioc->cmd ())
    {
    case ACE_IO_Cntl_Msg::SET_LWM:
    case ACE_IO_Cntl_Msg::SET_HWM:
      this->water_marks (command, *(size_t *) mb->cont ()->rd_ptr ());
      break;
    default:
      return -1;
    }
  return 0;
}

template <class PH, class PK> int
Peer_Router<PH, PK>::unbind_peer (PK key)
{
  return this->peer_map_.unbind (key);
}

template <class PH, class PK> int
Peer_Router<PH, PK>::bind_peer (PK key, Peer_Handler<Peer_Router<PH, PK>, PK> *ph)
{
  PH *peer_handler = (PH *) ph;
  return this->peer_map_.bind (key, peer_handler);
}

template <class PH, class PK> int
Peer_Router<PH, PK>::init (int argc, ACE_TCHAR *argv[])
{
  this->acceptor_ = new Acceptor_Factory <PH, PK> (this);

  if (this->acceptor_->init (argc, argv) == -1
      || this->peer_map_.open () == -1)
    return -1;
  else
    {
      ACE_UPIPE_Addr addr;
      ACE_UPIPE_Acceptor &pa = this->acceptor_->acceptor ();

      if (pa.get_local_addr (addr) != -1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) initializing %s, file = %s, fd = %d, this = %@\n"),
                    this->name (), addr.get_path_name (), pa.get_handle (), this));
      else
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("get_local_addr")), -1);
    }
  return 0;
}

#undef PH
#undef PA
#undef PAD
#undef PK
#undef PM
#endif /* ACE_HAS_THREADS */
#endif /* _PEER_ROUTER_C */
