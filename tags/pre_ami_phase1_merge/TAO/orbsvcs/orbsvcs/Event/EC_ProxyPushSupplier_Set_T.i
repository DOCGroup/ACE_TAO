// $Id$

template<class ACE_LOCK> ACE_INLINE
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::
       TAO_EC_ProxyPushSupplier_Set_Immediate (void)
{
}

// ****************************************************************

template<ACE_SYNCH_DECL> ACE_INLINE
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::
       TAO_EC_ProxyPushSupplier_Set_Delayed (void)
 :  busy_cond_ (lock_),
    busy_count_ (0),
    write_delay_ (0)
{
}

