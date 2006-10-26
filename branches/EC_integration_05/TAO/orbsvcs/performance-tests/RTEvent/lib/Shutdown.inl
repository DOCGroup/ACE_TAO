/**
 * @file Shutdown.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Client> ACE_INLINE void
Shutdown<Client>::operator () (Client *client)
{
  ACE_DECLARE_NEW_ENV;
  ACE_TRY
    {
      client->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY { };
  ACE_ENDTRY;
}
