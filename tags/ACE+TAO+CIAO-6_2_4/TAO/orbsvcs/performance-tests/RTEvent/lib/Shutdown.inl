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
  try
    {
      client->shutdown ();
    }
  catch (const CORBA::Exception&){ };
}
