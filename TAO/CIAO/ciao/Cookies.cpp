// $Id$

#include "Cookies.h"

#if !defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */


/// Initialize a @c Cookie with an @c ACE_Active_Map_Manager_Key
CIAO::Map_Key_Cookie::Map_Key_Cookie (const ACE_Active_Map_Manager_Key &key)
{
}

CIAO::Map_Key_Cookie::Map_Key_Cookie (const ::OBV_Components::Cookie &ck)
{
  this->cookieValue (ACE_reinterpret_cast (CIAO::Map_Key_Cookie, ck).cookieValue ());
}

int
CIAO::Map_Key_Cookie::extract (::OBV_Components::Cookie &ck,
                               ACE_Active_Map_Manager_Key &key)
{
  return 0;
}


CORBA::ValueBase *
CIAO::Map_Key_Cookie_init::create_for_unmarshal (void)
{
  CORBA_ValueBase *ret_val = 0;
  ACE_NEW_RETURN (
      ret_val,
      CIAO::Map_Key_Cookie,
      0
    );
  return ret_val;
}
