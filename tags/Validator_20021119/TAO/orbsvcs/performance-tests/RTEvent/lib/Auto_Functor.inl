/**
 * @file Auto_Functor.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class Client, class Functor> ACE_INLINE
Auto_Functor<Client,Functor>::Auto_Functor (Client *client)
  :  client_ (client)
{
}

template<class Client, class Functor> ACE_INLINE void
Auto_Functor<Client,Functor>::release (void)
{
  this->client_ = 0;
}
