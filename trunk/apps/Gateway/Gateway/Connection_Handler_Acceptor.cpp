// $Id$

#include "Event_Channel.h"
#include "Connection_Handler_Acceptor.h"

int
Connection_Handler_Acceptor::make_svc_handler (Connection_Handler *&ph)
{
  ACE_ALLOCATOR_RETURN (ph,
                        this->connection_handler_factory_.make_connection_handler (this->connection_config_info_),
                        -1);
  return 0;
}

Connection_Handler_Acceptor::Connection_Handler_Acceptor (Event_Channel &ec,
                                                char connection_role)
  : event_channel_ (ec)
{
  this->connection_config_info_.connection_id_ = 0;
  this->connection_config_info_.host_[0] = '\0';
  this->connection_config_info_.remote_port_ = 0;
  this->connection_config_info_.connection_role_ = connection_role;
  this->connection_config_info_.max_retry_timeout_ = Options::instance ()->max_timeout ();
  this->connection_config_info_.local_port_ = 0;
  this->connection_config_info_.priority_ = 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

