// $Id$

#include "EventType.h"
#include "ace/ACE.h"

#if ! defined (__ACE_INLINE__)
#include "EventType.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_EventType, "$Id$")

TAO_Notify_EventType
TAO_Notify_EventType::special (void)
{
  return TAO_Notify_EventType ("*", "%ALL");
}

TAO_Notify_EventType::TAO_Notify_EventType (void)
{
}

void
TAO_Notify_EventType::init_i (const char* domain_name, const char* type_name)
{
  this->event_type_.domain_name = domain_name;
  this->event_type_.type_name = type_name;

  if (this->is_special () == 1)
    {
      this->event_type_.domain_name = (const char* )"*";
      this->event_type_.type_name = (const char* )"%ALL";
    }

  this->recompute_hash ();
}

TAO_Notify_EventType::TAO_Notify_EventType (const char* domain_name, const char* type_name)
{
  this->init_i (domain_name, type_name);
}

TAO_Notify_EventType::TAO_Notify_EventType (const CosNotification::EventType& event_type)
{
  this->init_i (event_type.domain_name.in (), event_type.type_name.in ());
}

TAO_Notify_EventType::~TAO_Notify_EventType ()
{
}

void
TAO_Notify_EventType::recompute_hash (void)
{
  // @@ Pradeep: this code is bound to crash someday if the strings
  // are too long....  See if the hash_pjw () function can be modified
  // to take accumulate multiple strings, as in:
  //   hash = ACE::hash_pjw_accummulate (0, str1);
  //   hash = ACE::hash_pjw_accummulate (hash, str2);
  //
  // @@ Or use grow the buffer when needed, or just add the two hash
  // values or something, but fix this code!
  //
  char buffer[BUFSIZ];
  ACE_OS::strcpy (buffer, this->event_type_.domain_name.in ());
  ACE_OS::strcat (buffer, this->event_type_.type_name.in ());

  this->hash_value_ =  ACE::hash_pjw (buffer);
}

TAO_Notify_EventType&
TAO_Notify_EventType::operator=(const CosNotification::EventType& event_type)
{
  this->init_i (event_type.domain_name.in (),event_type.type_name.in ());

  return *this;
}

TAO_Notify_EventType&
TAO_Notify_EventType::operator=(const TAO_Notify_EventType& event_type)
{
  if (this == &event_type)
    return *this;

  this->init_i (event_type.event_type_.domain_name.in (),event_type.event_type_.type_name.in ());

  return *this;
}

int
TAO_Notify_EventType::operator==(const TAO_Notify_EventType& event_type) const
{
  if (this->hash () != event_type.hash ())
    return 0;
  else // compare the strings
    return (ACE_OS::strcmp (this->event_type_.type_name, event_type.event_type_.type_name) == 0  &&
            ACE_OS::strcmp (this->event_type_.domain_name, event_type.event_type_.domain_name) == 0
           );
}

int
TAO_Notify_EventType::operator!=(const TAO_Notify_EventType& event_type) const
{
  if (this->hash () != event_type.hash ())
    return 1;
  else // compare the strings
    return (ACE_OS::strcmp (this->event_type_.type_name, event_type.event_type_.type_name) != 0  ||
            ACE_OS::strcmp (this->event_type_.domain_name, event_type.event_type_.domain_name) != 0
           );
}

CORBA::Boolean
TAO_Notify_EventType::is_special (void) const
{
  if ((this->event_type_.domain_name == 0 ||
             ACE_OS::strcmp (this->event_type_.domain_name, "") == 0 ||
             ACE_OS::strcmp (this->event_type_.domain_name, "*") == 0) &&
      (this->event_type_.type_name == 0 ||
             ACE_OS::strcmp (this->event_type_.type_name, "") == 0 ||
             ACE_OS::strcmp (this->event_type_.type_name, "*") == 0 ||
             ACE_OS::strcmp (this->event_type_.type_name, "%ALL") == 0))
    return 1;
  else
    return 0;
}

void
TAO_Notify_EventType::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, "(%s,%s)", this->event_type_.domain_name.in (), this->event_type_.type_name.in ()));
}
