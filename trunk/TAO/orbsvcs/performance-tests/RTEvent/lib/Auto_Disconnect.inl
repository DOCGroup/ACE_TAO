/**
 * @file Auto_Disconnect.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#include "ace/CORBA_macros.h"

template<class Client> ACE_INLINE void
Disconnect<Client>::operator () (Client *client)
{
  ACE_DECLARE_NEW_ENV;
  ACE_TRY
    {
      client->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY { };
  ACE_ENDTRY;
}

template<class Client> ACE_INLINE
Auto_Disconnect<Client>::Auto_Disconnect (Client *client)
  :  ACE_Utils::Auto_Functor<Client,Disconnect<Client> > (client)
{
}
