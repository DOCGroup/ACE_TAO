// $Id$

#include "Peer_i.h"

#if !defined(__ACE_INLINE__)
#include "Peer_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FL_Callback, Peer_i, "$Id$")

Peer_Handler_i::Peer_Handler_i (Peer_i *peer)
  : peer_ (peer)
{
}

void
Peer_Handler_i::request (CORBA::Long retval, 
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  static int i = 0;
  i++;
  if (i % 100 == 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %d replies received\n", i));
  this->peer_->reply (retval, ACE_TRY_ENV);
}


void 
Peer_Handler_i::request_excep (
    AMI_PeerExceptionHolder * excep_holder,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (excep_holder);
  ACE_UNUSED_ARG (ACE_TRY_ENV);
}


void
Peer_Handler_i::start (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Peer_Handler_i::shutdown (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))

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
              const ACE_Time_Value &delay,
              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->progress_ = Progress::_duplicate (progress);
  this->delay_ = delay;

  Peer_var peer = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Peer (%P|%t) - binding\n"));
  this->id_ = this->progress_->bind (peer.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
Peer_i::reply (CORBA::Long result,
               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->progress_->recv_reply (result, ACE_TRY_ENV);
}

CORBA::Long
Peer_i::request (CORBA::Long id,
                 CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Time_Value tv  = this->delay_;
  ACE_OS::sleep (tv);

  return id;
}

void
Peer_i::start (const PeerSet &the_peers,
               CORBA::Long iterations,
               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  AMI_PeerHandler_var handler =
    this->reply_handler_._this (ACE_TRY_ENV);
  ACE_CHECK;

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
Peer_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
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
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              this->the_peers_[j]->sendc_request (this->handler_.in (),
                                                  this->id_,
                                                  ACE_TRY_ENV);
              ACE_TRY_CHECK;

              this->progress_->sent_request (this->id_,
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
              // Ignore exceptions;
            }
          ACE_ENDTRY;
        }
      if (i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) %d requests sent\n", i));
    }
  return 0;
}
