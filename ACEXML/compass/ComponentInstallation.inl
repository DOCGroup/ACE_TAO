// $Id$

ACE_INLINE const Location&
get_implementation (const UUID& implUUID)
  ACE_THROW_SPEC ((UnknownImplId, InstallationFailure));
{
  ACEXML_String loc;
  if (this->packages_.find (implUUID, loc) != 0)
    ACE_THROW (UnknownImplId());
  return this->loc;
}
