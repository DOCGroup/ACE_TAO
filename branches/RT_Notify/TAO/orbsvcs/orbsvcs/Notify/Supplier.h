/* -*- C++ -*- */
/**
 *  @file Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SUPPLIER_H
#define TAO_NS_SUPPLIER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Refcountable.h"

/**
 * @class TAO_NS_Supplier
 *
 * @brief Base Wrappers for Suppliers that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_Supplier : public TAO_NS_Refcountable
{
public:
  /// Constuctor
  TAO_NS_Supplier (void);

  /// Destructor
  ~TAO_NS_Supplier ();  
};

#if defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIER_H */
