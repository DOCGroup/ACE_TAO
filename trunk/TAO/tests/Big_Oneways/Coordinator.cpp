//
// $Id$
//
#include "Coordinator.h"

ACE_RCSID(Big_Oneways, Coordinator, "$Id$")

Coordinator::Coordinator (CORBA::ULong peer_count)
  : peers_ (0)
  , peer_count_ (0)
  , peer_max_ (peer_count)
{
  ACE_NEW (this->peers_, Test::Peer_var[this->peer_max_]);
}

Coordinator::~Coordinator (void)
{
  delete[] this->peers_;
}

int
Coordinator::has_all_peers (void) const
{
  return this->peer_count_ == this->peer_max_;
}

void
Coordinator::create_session_list (Test::Session_Control_ptr session_control,
                                  CORBA::ULong payload_size,
                                  CORBA::ULong thread_count,
                                  CORBA::ULong message_count,
                                  Test::Session_List &session_list
                                  TAO_ENV_ARG_DECL)
{
  session_list.length (this->peer_count_);
  CORBA::ULong count = 0;
  for (Test::Peer_var *i = this->peers_;
       i != this->peers_ + this->peer_count_;
       ++i)
    {
      session_list[count++] =
        (*i)->create_session (session_control,
                              payload_size,
                              thread_count,
                              message_count,
                              this->peer_count_
                              TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Coordinator::shutdown_all_peers (TAO_ENV_SINGLE_ARG_DECL)
{
  for (Test::Peer_var *i = this->peers_;
       i != this->peers_ + this->peer_count_;
       ++i)
    {
      ACE_TRY
        {
          (*i)->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Coordinator::shutdown, ignored");
        }
      ACE_ENDTRY;
    }
}

void
Coordinator::add_peer (Test::Peer_ptr peer
                       TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->peer_count_ >= this->peer_max_)
    return;

  this->peers_[this->peer_count_++] =
    Test::Peer::_duplicate (peer);
}
