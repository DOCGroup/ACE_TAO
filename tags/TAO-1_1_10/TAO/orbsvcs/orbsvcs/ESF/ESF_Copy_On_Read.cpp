// $Id$

#ifndef TAO_ESF_COPY_ON_READ_CPP
#define TAO_ESF_COPY_ON_READ_CPP

#include "ESF_Copy_On_Read.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Copy_On_Read.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Worker.h"

ACE_RCSID(ESF, ESF_Copy_On_Read, "$Id$")

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
    for_each (TAO_ESF_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Use an allocator for this memory...
  PROXY **proxies = 0;
  size_t size = 0;
  ACE_TRY
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

      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          worker->work (*j, ACE_TRY_ENV);
          ACE_TRY_CHECK;
          (*j)->_decr_refcnt ();
        }
      delete[] proxies;
    }
  ACE_CATCHANY
    {
      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          if (*j != 0)
            (*j)->_decr_refcnt ();
        }
      delete[] proxies;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_TRY_ENV);
}

#endif /* TAO_ESF_COPY_ON_READ_CPP */
