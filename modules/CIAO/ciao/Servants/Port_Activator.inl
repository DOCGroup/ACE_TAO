// $Id$

namespace CIAO
{
  ACE_INLINE char*
  Port_Activator_i::oid (void) 
  {
    return CORBA::string_dup(this->oid_.in ());
  }

  ACE_INLINE void
  Port_Activator_i::oid (const char* oid)
  {
    this->oid_ = oid;
  }

  ACE_INLINE char*
  Port_Activator_i::name (void)
  {
    return CORBA::string_dup(this->name_.in ());
  }
}
