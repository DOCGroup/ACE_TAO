/* -*- C++ -*- */
/**
 *  @file Proxy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXY_H
#define TAO_NS_PROXY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Object_T.h"
#include "EventTypeSeq.h"

class TAO_NS_Admin;

/**
 * @class TAO_NS_Proxy
 *
 * @brief Base class proxy for all proxys in NS.
 *
 */
class TAO_Notify_Export TAO_NS_Proxy : public TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin>
{
public:
  /// Constuctor
  TAO_NS_Proxy (void);

  /// Destructor
  ~TAO_NS_Proxy ();  

  /// Init method.
  // virtual void init (ACE_ENV_SINGLE_ARG_DECL);

protected:
  typedef TAO_NS_Object_T <TAO_NS_Proxy, TAO_NS_Admin> inherited;

  /// Preprocess the types added and removed. 
  void preprocess (TAO_NS_EventTypeSeq& added, TAO_NS_EventTypeSeq& removed); 

  /// The types that we're subscribed with the event manager.
  TAO_NS_EventTypeSeq subscribed_types_;
};

#if defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXY_H */
