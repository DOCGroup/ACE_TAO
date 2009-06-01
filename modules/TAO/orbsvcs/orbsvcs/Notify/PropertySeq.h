// -*- C++ -*-

/**
 *  @file PropertySeq.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROPERTYSEQ_H
#define TAO_Notify_PROPERTYSEQ_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_PropertySeq
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_PropertySeq
{
public:
  /// Constructor
  TAO_Notify_PropertySeq (void);

  /// Destructor
  ~TAO_Notify_PropertySeq ();

  /// Return 0 on success, -1 on error.
  int init (const CosNotification::PropertySeq& prop_seq);

  /// Find the <value> for property <name>. Returns 0 on success.
  int find (const char* name, CosNotification::PropertyValue& value) const;

  /// Return -1 on error.
  int populate (CosNotification::PropertySeq_var& prop_seq) const;

  /// return the number of properties available as a sequence
  size_t size() const;

protected:
  /// Add the value. Used by subclasses to initialize the property map from their
  /// member variables.
  void add(const ACE_CString& name, const CORBA::Any& val);

protected:
  /// Property Map.
  typedef ACE_Hash_Map_Manager <ACE_CString, CosNotification::PropertyValue, ACE_SYNCH_NULL_MUTEX> PROPERTY_MAP;

  PROPERTY_MAP property_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/PropertySeq.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROPERTYSEQ_H */
