/**
 * @file Auto_Disconnect.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Client> ACE_INLINE void
Disconnect<Client>::operator () (Client *client
                                 ACE_ENV_ARG_DECL)
{
  client->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template<class Client> ACE_INLINE
Auto_Disconnect<Client>::Auto_Disconnect (Client *client)
  :  Auto_Functor<Client,Disconnect<Client> > (client)
{
}
