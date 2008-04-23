// $Id$

namespace CIAO
{
  ACE_INLINE const char*
  Port_Activator::oid (void) const
  {
    return this->oid_.in ();
  }

  ACE_INLINE void
  Port_Activator::oid (const char* oid)
  {
    this->oid_ = oid;
  }

  ACE_INLINE const char*
  Port_Activator::name (void) const
  {
    return this->name_.in ();
  }
}
