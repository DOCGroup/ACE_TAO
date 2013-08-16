// $Id$

#include "orbsvcs/Notify/EventType.h"

#include "ace/ACE.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/OS_NS_string.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/EventType.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Topology_Saver.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      this->event_type_.domain_name = CORBA::string_dup ("*");
      this->event_type_.type_name = CORBA::string_dup ("%ALL");
    }

  this->recompute_hash ();
}

TAO_Notify_EventType::TAO_Notify_EventType (const char* domain_name,
                                            const char* type_name)
{
  this->init_i (domain_name, type_name);
}

TAO_Notify_EventType::TAO_Notify_EventType (
    const CosNotification::EventType& event_type
  )
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

bool
TAO_Notify_EventType::operator==(const TAO_Notify_EventType& event_type) const
{
  return (ACE_OS::strcmp (this->event_type_.domain_name,
                          event_type.event_type_.domain_name) == 0 ||
          this->domain_is_wildcard (this->event_type_.domain_name) ||
          this->domain_is_wildcard (event_type.event_type_.type_name)) &&
         (ACE_OS::strcmp (this->event_type_.type_name,
                          event_type.event_type_.type_name) == 0 ||
          this->type_is_wildcard (this->event_type_.type_name)   ||
          this->type_is_wildcard (event_type.event_type_.type_name));
}

bool
TAO_Notify_EventType::operator!=(const TAO_Notify_EventType& event_type) const
{
  return !(*this == event_type);
}

CORBA::Boolean
TAO_Notify_EventType::is_special (void) const
{
  return (this->domain_is_wildcard (this->event_type_.domain_name) &&
          this->type_is_wildcard (this->event_type_.type_name));
}

void
TAO_Notify_EventType::dump (void) const
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "(%s,%s)",
              this->event_type_.domain_name.in (),
              this->event_type_.type_name.in ()));
}

/// Initialize from an NVPList, return false on failure
bool TAO_Notify_EventType::init(const TAO_Notify::NVPList& attrs)
{
  bool result = false;

  ACE_CString domain;
  ACE_CString type;
  if (attrs.load("Domain", domain) && attrs.load("Type", type))
  {
    this->init_i(domain.c_str(), type.c_str());
    result = true;
  }
  return result;

}

  // TAO_Notify::Topology_Object

void
TAO_Notify_EventType::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  TAO_Notify::NVPList attrs;
  bool changed = true;

  attrs.push_back(TAO_Notify::NVP("Domain", this->event_type_.domain_name.in()));
  attrs.push_back(TAO_Notify::NVP("Type", this->event_type_.type_name.in()));
  saver.begin_object(0, "subscription", attrs, changed);

  saver.end_object(0, "subscription");
}

TAO_END_VERSIONED_NAMESPACE_DECL
