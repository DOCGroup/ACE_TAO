/**
 * @file Auto_Disconnect.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Client> ACE_INLINE
Auto_Disconnect<Client>::Auto_Disconnect (Client *client)
  :  client_ (client)
{
}

template<class Client> ACE_INLINE void
Auto_Disconnect<Client>::release (void)
{
  this->client_ = 0;
}
