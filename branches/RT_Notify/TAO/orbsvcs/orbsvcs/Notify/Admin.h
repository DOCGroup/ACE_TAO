/* -*- C++ -*- */
/**
 *  @file Admin.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_ADMIN_H
#define TAO_NS_ADMIN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "Container_T.h"
//#include "Proxy.h"

class TAO_NS_Proxy;
class TAO_NS_EventChannel;

/**
 * @class TAO_NS_Admin
 *
 * @brief Base class for Admin interface implementations.
 *
 */
class TAO_Notify_Export TAO_NS_Admin : public TAO_NS_Container_T <TAO_NS_Proxy, TAO_NS_Admin, TAO_NS_EventChannel>
{
public:
  /// Constuctor
  TAO_NS_Admin (void);

  /// Destructor
  ~TAO_NS_Admin ();  

protected:
   typedef TAO_NS_Container_T <TAO_NS_Proxy, TAO_NS_Admin, TAO_NS_EventChannel> inherited;

  /// = Data Members

};

#if defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_ADMIN_H */
