// $Id$

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected_i (PROXY *proxy
               TAO_ENV_ARG_DECL)
{
  this->collection_.connected (proxy TAO_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected_i (PROXY *proxy
                 TAO_ENV_ARG_DECL)
{
  this->collection_.reconnected (proxy TAO_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected_i (PROXY *proxy
                  TAO_ENV_ARG_DECL)
{
  this->collection_.disconnected (proxy TAO_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown_i (TAO_ENV_SINGLE_ARG_DECL)
{
  this->collection_.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
}
