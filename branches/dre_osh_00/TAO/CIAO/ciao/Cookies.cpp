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
CIAO::Map_Key_Cookie::extract (::Components::Cookie *ck,
                               ACE_Active_Map_Manager_Key &key)
{
  CIAO::Cookie *c = CIAO::Cookie::_downcast (ck);

  if (c == 0)
    return -1;

  ::CORBA::OctetSeq *x = c->get_cookie ();

  if (x->length () != ACE_Active_Map_Manager_Key::size ())
    return -1;
  key.decode (x->get_buffer ());
  return 0;
}


CORBA::ValueBase *
CIAO::Map_Key_Cookie_init::create_for_unmarshal (void)
{
  CORBA::ValueBase *ret_val = 0;
  ACE_NEW_RETURN (
      ret_val,
      CIAO::Map_Key_Cookie,
      0
    );
  return ret_val;
}

::CORBA::OctetSeq *
CIAO::Map_Key_Cookie::get_cookie (ACE_ENV_SINGLE_ARG_DECL)
{
  return &this->cookieValue ();
}
