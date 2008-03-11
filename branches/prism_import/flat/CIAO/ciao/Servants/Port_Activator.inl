// $Id$

namespace CIAO
{
  ACE_INLINE const char*
  Port_Activator_i::oid (void) const
  {
    return this->oid_.in ();
  }

  ACE_INLINE void
  Port_Activator_i::oid (const char* oid)
  {
    this->oid_ = oid;
  }

  ACE_INLINE const char*
  Port_Activator_i::name (void) const
  {
    return this->name_.in ();
  }
}
