// $Id$

template<class EC,class P,class I> ACE_INLINE void
TAO_ESF_Proxy_Admin<EC,P,I>::
    for_each (TAO_ESF_Worker<P> *worker
              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->collection_->for_each (worker ACE_ENV_ARG_PARAMETER);
}
