/**
 * @file Shutdown.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Client> ACE_INLINE void
Shutdown<Client>::operator () (Client *client
                               ACE_ENV_ARG_DECL)
{
  client->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}
