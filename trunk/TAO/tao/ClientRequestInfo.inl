// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_ClientRequestInfo::check_validity (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->info_ == 0)
    ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                     CORBA::COMPLETED_NO));
}

ACE_INLINE void
TAO_ClientRequestInfo::info (TAO_ClientRequestInfo_i *info)
{
  this->info_ = info;
}

ACE_INLINE TAO_ClientRequestInfo_i *
TAO_ClientRequestInfo::info (void) const
{
  return this->info_;
}


// -------------------------------------------------------------------


ACE_INLINE
TAO_ClientRequestInfo_Guard::TAO_ClientRequestInfo_Guard (
  TAO_ClientRequestInfo * info,
  TAO_ClientRequestInfo_i * ri)
  : info_ (info),
    previous_info_ (0)
{
  if (info != 0)
    {
      this->previous_info_ = info->info ();
      info->info (ri);
    }
}
