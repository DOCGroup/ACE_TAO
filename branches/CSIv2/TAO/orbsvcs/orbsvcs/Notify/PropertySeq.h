/* -*- C++ -*- */
/**
 *  @file PropertySeq.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROPERTYSEQ_H
#define TAO_Notify_PROPERTYSEQ_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"

/**
 * @class TAO_Notify_PropertySeq
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_Notify_PropertySeq
{
public:
  /// Constuctor
  TAO_Notify_PropertySeq (void);

  /// Destructor
  ~TAO_Notify_PropertySeq ();

  /// Return 0 on success, -1 on error.
  int init (const CosNotification::PropertySeq& prop_seq);

  /// Find the <value> for property <name>. Returns 0 on success.
  int find (const char* name, CosNotification::PropertyValue& value) const;

  /// Return -1 on error.
  int populate (CosNotification::PropertySeq_var& prop_seq);

protected:
  /// Property Map.
  typedef ACE_Hash_Map_Manager <ACE_CString, CosNotification::PropertyValue, ACE_SYNCH_NULL_MUTEX> PROPERTY_MAP;

  PROPERTY_MAP property_map_;
};

#if defined (__ACE_INLINE__)
#include "PropertySeq.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROPERTYSEQ_H */
