// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Current.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_CURRENT_H
#define TAO_SSLIOP_CURRENT_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOPC.h"

/**
 * @class TAO_SSLIOP_Current
 *
 * @brief Implementation of the TAO SSLIOP::Current extension.
 *
 * This object can be used to obtain SSL session related information
 * about the current execution context.  For example, SSL peer
 * certificate chains for the current request can be obtained from
 * this object.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Current
  : public SSLIOP::Current,
    public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_SSLIOP_Current (void);

  /// Return the peer certificate chain associated with the current
  /// upcall and SSL session.
  virtual SSLIOP::SSL_Cert * get_peer_certificate_chain (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Destructor
  ~TAO_SSLIOP_Current (void);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_SSLIOP_Current (const TAO_SSLIOP_Current &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_SSLIOP_Current &))
  //@}

};

#include "ace/post.h"

#endif  /* TAO_SSLIOP_CURRENT_H */
