// $Id$
#include "Cookies.h"

#if !defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */

ACE_RCSID (ciao,
           Cookies,
           "$Id$")

namespace CIAO
{
  Map_Key_Cookie::Map_Key_Cookie (const ACE_Active_Map_Manager_Key &key)
  {
    this->cookieValue ().length (ACE_Active_Map_Manager_Key::size ());
    key.encode (this->cookieValue ().get_buffer (0));
  }

  Map_Key_Cookie::~Map_Key_Cookie (void)
  {
  }

  bool
  Map_Key_Cookie::insert (ACE_Active_Map_Manager_Key &key)
  {
    this->cookieValue ().length (ACE_Active_Map_Manager_Key::size ());
    key.encode (this->cookieValue ().get_buffer (0));
    return true;
  }

  bool
  Map_Key_Cookie::extract (::Components::Cookie *ck,
                           ACE_Active_Map_Manager_Key &key)
  {
    Map_Key_Cookie *c =
      dynamic_cast <Map_Key_Cookie *> (ck);

    if (c == 0)
      return false;

    ::CORBA::OctetSeq *x = c->get_cookie ();

    if (x->length () != ACE_Active_Map_Manager_Key::size ())
      return false;

    key.decode (x->get_buffer ());

    return true;
  }

  ::CORBA::OctetSeq *
  Map_Key_Cookie::get_cookie (void)
  {
    return &this->cookieValue ();
  }

  //========================================================
  CORBA::ValueBase *
  Map_Key_Cookie_init::create_for_unmarshal ()
  {
    CORBA::ValueBase *ret_val = 0;

    ACE_NEW_THROW_EX (ret_val,
                      CIAO::Map_Key_Cookie,
                      CORBA::NO_MEMORY ());

    return ret_val;
  }

  Map_Key_Cookie_init::~Map_Key_Cookie_init ()
  {
  }
}
