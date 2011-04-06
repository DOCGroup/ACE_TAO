//
// $Id$
//
#include "Coordinator.h"

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
                                  Test::Session_List &session_list)
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
                              this->peer_count_);
    }
}

void
Coordinator::shutdown_all_peers (void)
{
  for (Test::Peer_var *i = this->peers_;
       i != this->peers_ + this->peer_count_;
       ++i)
    {
      try
        {
          (*i)->shutdown ();
          (*i) = Test::Peer::_nil ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Coordinator::shutdown, ignored");
        }
    }
}

void
Coordinator::add_peer (Test::Peer_ptr peer)
{
  if (this->peer_count_ >= this->peer_max_)
    return;

  this->peers_[this->peer_count_++] =
    Test::Peer::_duplicate (peer);
}
