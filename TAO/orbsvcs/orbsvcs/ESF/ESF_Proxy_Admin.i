// $Id$

template<class EC,class P> ACE_INLINE void
TAO_ESF_Proxy_Admin<EC,P>::
    for_each (TAO_ESF_Worker<P> *worker,
              CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (())
{
  this->collection_->for_each (worker, ACE_TRY_ENV);
}

