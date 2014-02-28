// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE u_long
TAO_Notify_EventType::hash (void) const
{
  return this->hash_value_;
}

ACE_INLINE const CosNotification::EventType&
TAO_Notify_EventType::native (void) const
{
  return event_type_;
}

ACE_INLINE bool
TAO_Notify_EventType::domain_is_wildcard (const char* domain) const
{
  return (domain == 0 || ACE_OS::strcmp (domain, "") == 0 ||
          ACE_OS::strcmp (domain, "*") == 0);
}

ACE_INLINE bool
TAO_Notify_EventType::type_is_wildcard (const char* type) const
{
  return (type == 0 || ACE_OS::strcmp (type, "") == 0 ||
          ACE_OS::strcmp (type, "*") == 0 ||
          ACE_OS::strcmp (type, "%ALL") == 0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
