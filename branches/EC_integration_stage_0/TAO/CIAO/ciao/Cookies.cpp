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

CIAO::ObjectId_Cookie::ObjectId_Cookie ()
{

}

CIAO::ObjectId_Cookie::~ObjectId_Cookie ()
{

}

CIAO::ObjectId_Cookie::ObjectId_Cookie (const PortableServer::ObjectId &oid)
{
  this->cookieValue ().length (oid.length ());
  this->cookieValue (oid);
}

int
CIAO::ObjectId_Cookie::insert (const PortableServer::ObjectId &oid)
{
  this->cookieValue ().length (oid.length ());
  this->cookieValue (oid);
  return 0;
}

int
CIAO::ObjectId_Cookie::extract (::Components::Cookie *ck,
                                PortableServer::ObjectId &oid)
{
  CIAO::Cookie *c = CIAO::Cookie::_downcast (ck);

  if (c == 0)
    return -1;

  ::CORBA::OctetSeq *x = c->get_cookie ();

  if (x == 0)
    return -1;

  oid = *x;

  return 0;
}


CORBA::ValueBase *
CIAO::ObjectId_Cookie_init::create_for_unmarshal (void)
{
  CORBA::ValueBase * return_value = 0;
  ACE_NEW_RETURN (return_value,
                  CIAO::ObjectId_Cookie,
                  0);
  return return_value;
}

::CORBA::OctetSeq *
CIAO::ObjectId_Cookie::get_cookie (ACE_ENV_SINGLE_ARG_DECL)
{
  return &this->cookieValue ();
}

CIAO::EventServiceInfo_Cookie::EventServiceInfo_Cookie ()
{

}

CIAO::EventServiceInfo_Cookie::~EventServiceInfo_Cookie ()
{

}

CIAO::EventServiceInfo_Cookie::EventServiceInfo_Cookie (const CIAO::EventServiceInfo &info)
{
  this->cookieValue ().length (sizeof (info));
  ACE_OS::memcpy (this->cookieValue ().get_buffer (0), (void *)&info, this->cookieValue ().length ());
}

int
CIAO::EventServiceInfo_Cookie::insert (const CIAO::EventServiceInfo &info)
{
  this->cookieValue ().length (sizeof (info));
  ACE_OS::memcpy (this->cookieValue ().get_buffer (0), (void *)&info, this->cookieValue ().length ());
  return 0;
}

int
CIAO::EventServiceInfo_Cookie::extract (::Components::Cookie *ck,
                                        CIAO::EventServiceInfo &info)
{
  CIAO::Cookie *c = CIAO::Cookie::_downcast (ck);

  if (c == 0)
    return -1;

  ::CORBA::OctetSeq *x = c->get_cookie ();

  if (x == 0)
    return -1;

  info = *((CIAO::EventServiceInfo *) x->get_buffer ());
  return 0;
}


CORBA::ValueBase *
CIAO::EventServiceInfo_Cookie_init::create_for_unmarshal (void)
{
  CORBA::ValueBase * return_value = 0;
  ACE_NEW_RETURN (return_value,
                  CIAO::EventServiceInfo_Cookie,
                  0);
  return return_value;
}

::CORBA::OctetSeq *
CIAO::EventServiceInfo_Cookie::get_cookie (ACE_ENV_SINGLE_ARG_DECL)
{
  return &this->cookieValue ();
}
