// $Id$

#include "Peer_i.h"
#include "ace/OS_NS_unistd.h"

Peer_Handler_i::Peer_Handler_i (Peer_i *peer)
  : peer_ (peer)
{
}

void
Peer_Handler_i::request (CORBA::Long retval)
{
  static int i = 0;
  i++;
  if (i % 100 == 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %d replies received\n", i));
  this->peer_->reply (retval);
}


void
Peer_Handler_i::request_excep (
    ::Messaging::ExceptionHolder *)
{
}

void
Peer_Handler_i::start (void)
{
}

void
Peer_Handler_i::shutdown (void)

{
}

Peer_i::Peer_i (void)
  :  reply_handler_ (this)
{
}

Peer_i::~Peer_i (void)
{
}

void
Peer_i::init (CORBA::ORB_ptr orb,
              Progress_ptr progress,
              const ACE_Time_Value &delay)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->progress_ = Progress::_duplicate (progress);
  this->delay_ = delay;

  Peer_var peer = this->_this ();

  ACE_DEBUG ((LM_DEBUG, "Peer (%P|%t) - binding\n"));
  this->id_ = this->progress_->bind (peer.in ());
}

void
Peer_i::reply (CORBA::Long result)
{
  this->progress_->recv_reply (result);
}

CORBA::Long
Peer_i::request (CORBA::Long id)
{
  ACE_Time_Value tv  = this->delay_;
  ACE_OS::sleep (tv);

  return id;
}

void
Peer_i::start (const PeerSet &the_peers,
               CORBA::Long iterations)
{
  AMI_PeerHandler_var handler =
    this->reply_handler_._this ();

  // @@ Report errors as exceptions...
  Peer_Task *task;
  ACE_NEW (task, Peer_Task (the_peers,
                            iterations,
                            this->progress_.in (),
                            handler.in (),
                            this->id_));
  task->activate ();
}

void
Peer_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

// ****************************************************************

Peer_Task::Peer_Task (const PeerSet& the_peers,
                      CORBA::Long iterations,
                      Progress_ptr progress,
                      AMI_PeerHandler_ptr handler,
                      CORBA::Long id)
  :  the_peers_ (the_peers),
     iterations_ (iterations),
     progress_ (Progress::_duplicate (progress)),
     handler_ (AMI_PeerHandler::_duplicate (handler)),
     id_ (id)
{
}

int
Peer_Task::svc (void)
{
  for (int i = 0; i != this->iterations_; ++i)
    {
      CORBA::ULong l = this->the_peers_.length ();
      for (CORBA::ULong j = 0; j != l; ++j)
        {
          try
            {
              this->the_peers_[j]->sendc_request (this->handler_.in (),
                                                  this->id_);

              this->progress_->sent_request (this->id_);
            }
          catch (const CORBA::Exception&)
            {
              // Ignore exceptions;
            }
        }
      if (i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) %d requests sent\n", i));
    }
  return 0;
}
