#include "Coordinator.h"

Coordinator::Coordinator (CORBA::ULong peer_count)
  : peer_max_ (peer_count)
{
  this->peers_.reserve (this->peer_max_);
}

bool
Coordinator::has_all_peers () const
{
  return this->peers_.size () == this->peer_max_;
}

void
Coordinator::create_session_list (Test::Session_Control_ptr session_control,
                                  CORBA::ULong payload_size,
                                  CORBA::ULong thread_count,
                                  CORBA::ULong message_count,
                                  Test::Session_List &session_list)
{
  CORBA::ULong const peer_count = static_cast<CORBA::ULong> (this->peers_.size ());
  session_list.length (peer_count);
  CORBA::ULong count = 0;
  for (Test::Peer_var const &peer : this->peers_)
    {
      session_list[count++] =
        peer->create_session (session_control,
                              payload_size,
                              thread_count,
                              message_count,
                              peer_count);
    }
}

void
Coordinator::shutdown_all_peers ()
{
  for (Test::Peer_var &peer : this->peers_)
    {
      try
        {
          peer->shutdown ();
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Coordinator::shutdown, ignored");
        }
    }
}

void
Coordinator::add_peer (Test::Peer_ptr peer)
{
  if (this->peers_.size () >= this->peer_max_)
    return;

  this->peers_.emplace_back (Test::Peer::_duplicate (peer));
}
