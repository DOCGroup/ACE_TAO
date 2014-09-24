// $Id$

#ifndef TAO_ESF_COPY_ON_READ_CPP
#define TAO_ESF_COPY_ON_READ_CPP

#include "orbsvcs/ESF/ESF_Copy_On_Read.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "tao/Exception.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_ESF_Copy_On_Read (void)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_ESF_Copy_On_Read (const COLLECTION &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    for_each (TAO_ESF_Worker<PROXY> *worker)
{
  // @@ Use an allocator for this memory...
  PROXY **proxies = 0;
  size_t size = 0;
  try
    {
      {
        ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

        size = this->collection_.size ();
        ACE_NEW (proxies, PROXY*[size]);
        PROXY **j = proxies;

        for (; j != proxies + size; ++j)
          *j = 0;

        j = proxies;

        ITERATOR end = this->collection_.end ();
        for (ITERATOR i = this->collection_.begin (); i != end; ++i)
          {
            *j = *i;
            (*j)->_incr_refcnt ();
            ++j;
          }
      }
      worker->set_size(size);
      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          worker->work (*j);
          (*j)->_decr_refcnt ();
        }
      delete[] proxies;
    }
  catch (const CORBA::Exception&)
    {
      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          if (*j != 0)
            (*j)->_decr_refcnt ();
        }
      delete[] proxies;

      throw;
    }
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    connected (PROXY *proxy)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    reconnected (PROXY *proxy)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    disconnected (PROXY *proxy)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.disconnected (proxy);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::shutdown (void)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.shutdown ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_COPY_ON_READ_CPP */
