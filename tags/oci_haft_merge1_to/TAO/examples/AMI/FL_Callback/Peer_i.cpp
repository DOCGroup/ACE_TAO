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
Peer_Handler_i::request (CORBA::Long retval
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  static int i = 0;
  i++;
  if (i % 100 == 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %d replies received\n", i));
  this->peer_->reply (retval ACE_ENV_ARG_PARAMETER);
}


void
Peer_Handler_i::request_excep (
    AMI_PeerExceptionHolder * excep_holder
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (excep_holder);
  ACE_ENV_ARG_NOT_USED;
}


void
Peer_Handler_i::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Peer_Handler_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
              const ACE_Time_Value &delay
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->progress_ = Progress::_duplicate (progress);
  this->delay_ = delay;

  Peer_var peer = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Peer (%P|%t) - binding\n"));
  this->id_ = this->progress_->bind (peer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Peer_i::reply (CORBA::Long result
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->progress_->recv_reply (result ACE_ENV_ARG_PARAMETER);
}

CORBA::Long
Peer_i::request (CORBA::Long id
                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Time_Value tv  = this->delay_;
  ACE_OS::sleep (tv);

  return id;
}

void
Peer_i::start (const PeerSet &the_peers,
               CORBA::Long iterations
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  AMI_PeerHandler_var handler =
    this->reply_handler_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
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
Peer_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
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
          ACE_TRY_NEW_ENV
            {
              this->the_peers_[j]->sendc_request (this->handler_.in (),
                                                  this->id_
                                                  ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              this->progress_->sent_request (this->id_
                                             ACE_ENV_ARG_PARAMETER);
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
