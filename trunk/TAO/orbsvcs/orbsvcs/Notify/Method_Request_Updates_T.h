/* -*- C++ -*- */
/**
 *  @file Method_Request_Updates_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_UPDATES_T_H
#define TAO_Notify_METHOD_REQUEST_UPDATES_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Environment;
}

/**
 * @class TAO_Notify_Method_Request_Updates_T
 *
 * @brief
 *
 */
template <class SEQ, class PROXY, class SEQ_PARAM, class PROXY_PARAM>
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Updates_T
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Updates_T (SEQ_PARAM added,
                                       SEQ_PARAM removed,
                                       PROXY_PARAM proxy);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Updates_T ();

  /// Execute the Request
  int execute_i (void);

protected:
  /// Update Added
  SEQ added_;

  /// Update Removed
  SEQ removed_;

  /// The Proxy that will receive the updates.
  PROXY proxy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Method_Request_Updates_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Method_Request_Updates_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Method_Request_Updates_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_METHOD_REQUEST_UPDATES_T_H */
