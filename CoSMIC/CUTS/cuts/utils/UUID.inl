// $Id$

CUTS_INLINE
bool CUTS_UUID::operator != (const CUTS_UUID & uuid) const
{
  return !(*this == uuid);
}
