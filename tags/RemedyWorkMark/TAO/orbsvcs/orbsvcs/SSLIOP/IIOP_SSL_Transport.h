// -*- C++ -*-

//=============================================================================
/**
 *  @file   IIOP_SSL_Transport.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_TRANSPORT_H
#define TAO_IIOP_SSL_TRANSPORT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Current.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IIOP_Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Resume_Handle;

namespace TAO
{
  class IIOP_SSL_Connection_Handler;

  /**
   * @class IIOP_SSL_Transport
   *
   * @brief
   * IIOP Transport designed to be "SSL aware," i.e. it is
   * aware of the existence of the SSLIOP Transport.   It makes
   * sure that SSL session state from a previous connection is not
   * associated with the non-SSL connection handled by this handler.
   *
   * However, this class overrides the handle_input() method to
   * invalidate the current TSS SSL state during a standard IIOP
   * (insecure) upcall.  This prevents SSL session state from a
   * previous SSL connection from being associated with non-SSL
   * connections processed by this connection handler.  In particular,
   * this is very important for closing a security hole in nested
   * upcalls.  For example, an SSLIOP request is made.  During that
   * secure upcall, an insecure nested upcall is made.  A naive
   * implementation would associate the TSS SSL state from the secure
   * upcall with the insecure upcall.  This implementation closes that
   * security hole.
   */
  class IIOP_SSL_Transport : public TAO_IIOP_Transport
  {
  public:
    /// Constructor.
    IIOP_SSL_Transport (IIOP_SSL_Connection_Handler *handler,
                        TAO_ORB_Core *orb_core);

    /// Default destructor.
    ~IIOP_SSL_Transport (void);

    /** @name Overridden Template Methods
     *
     * Please check the documentation in "tao/Transport.h" for more
     * details.
     */
    virtual int handle_input (TAO_Resume_Handle &rh,
                              ACE_Time_Value *max_wait_time = 0);

  protected:

    /// Reference to the TAO::SSLIOP::Current object.
    TAO::SSLIOP::Current_var current_;

  };

}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IIOP_SSL_TRANSPORT_H */
