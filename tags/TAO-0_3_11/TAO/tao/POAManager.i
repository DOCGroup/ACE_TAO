// $Id$

ACE_INLINE ACE_Lock &
TAO_POA_Manager::lock (void)
{
  return *this->lock_;
}

ACE_INLINE TAO_POA_Manager *
TAO_POA_Manager::clone (void)
{
  return new TAO_POA_Manager;
}
