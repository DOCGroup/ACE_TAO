// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file FT_Invocation_Endpoint_Selectors.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_INVOCATION_ENDPOINT_SELECTOR_H
#define TAO_FT_INVOCATION_ENDPOINT_SELECTOR_H
#include /**/ "ace/pre.h"

#include "fault_tol_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Invocation_Endpoint_Selectors.h"

class TAO_GIOP_Invocation;
class TAO_Profile;

/**
 * @class TAO_FT_Invocation_Endpoint_Selector
 *
 * @brief Implementation specific for FT_Service.
 *
 * We inherit from the Default endpoint selector, as this would give
 * us a chance to use the default endpoint selector options if
 * needed.
 *
 */

class TAO_FT_Export TAO_FT_Invocation_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{

public:
  /// Constructor.
  TAO_FT_Invocation_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_FT_Invocation_Endpoint_Selector (void);

  /// Implementation for FT. Please refer to
  /// $TAO_ROOT/tao/Invocation_Endpoint_Selectors.h  for details of
  /// documentation.
  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                ACE_ENV_ARG_DECL);

  virtual void next (TAO_GIOP_Invocation *invocation
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void success (TAO_GIOP_Invocation *invocation);
  virtual void close_connection (TAO_GIOP_Invocation *invocation);

protected:

  int select_endpoint_i (TAO_GIOP_Invocation *invoc
                         ACE_ENV_ARG_DECL);

  int select_primary (TAO_GIOP_Invocation *invocation
                      ACE_ENV_ARG_DECL);

  int check_profile_for_primary (TAO_GIOP_Invocation *,
                                 TAO_Profile *
                                 ACE_ENV_ARG_DECL);

private:

  /// Is the primary alive? The default is yes.
  CORBA::Boolean is_primary_alive_;

  /// Has the profile list been rewound
  CORBA::Boolean is_rewound_;
};

#if defined (__ACE_INLINE__)
# include "FT_Invocation_Endpoint_Selectors.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* FT_TAO_INVOCATION_ENDPOINT_SELECTOR_H */
