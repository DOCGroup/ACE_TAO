/**
 * @file Shutdown.inl
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
  catch (CORBA::Exception const&){ };
}
