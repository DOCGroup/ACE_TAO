// $Id$

#include "Cookies.h"

#if !defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */


/// Initialize a @c Cookie with an @c ACE_Active_Map_Manager_Key
CIAO::Map_Key_Cookie::Map_Key_Cookie (const ACE_Active_Map_Manager_Key &key)
{
  this->cookieValue ().length (ACE_Active_Map_Manager_Key::size ());
  key.encode (this->cookieValue ().get_buffer (0));
}

// CIAO::Map_Key_Cookie::Map_Key_Cookie (const ::OBV_Components::Cookie &ck)
// {
//   this->cookieValue (ACE_reinterpret_cast (CIAO::Map_Key_Cookie, ck).cookieValue ());
// }

int
CIAO::Map_Key_Cookie::insert (ACE_Active_Map_Manager_Key &key)
{
  this->cookieValue ().length (ACE_Active_Map_Manager_Key::size ());
  key.encode (this->cookieValue ().get_buffer (0));
  return 0;
}

int
CIAO::Map_Key_Cookie::extract (ACE_Active_Map_Manager_Key &key)
{
  if (this->cookieValue ().length () != ACE_Active_Map_Manager_Key::size ())
    return -1;
  key.decode (this->cookieValue ().get_buffer ());
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
