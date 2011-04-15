// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Message_Version.h
 *
 *  $Id$
 *
 *   GIOP Version utility class definition
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author  Carlos O' Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_GIOP_MESSAGE_VERSION_H
#define TAO_GIOP_MESSAGE_VERSION_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * @class TAO_GIOP_Message_Version
 *
 * @brief Major and Minor version number of the Inter-ORB Protocol.
 */
class TAO_Export TAO_GIOP_Message_Version
{
public:

  /// Major version number
  CORBA::Octet major;

  /// Get major version number
  CORBA::Octet major_version (void) const;

  /// Minor version number
  CORBA::Octet minor;

  /// Get minor version number
  CORBA::Octet minor_version (void) const;

  /// Copy constructor
  TAO_GIOP_Message_Version (const TAO_GIOP_Message_Version &src);

  /// Default constructor.
  TAO_GIOP_Message_Version (CORBA::Octet maj = TAO_DEF_GIOP_MAJOR,
                            CORBA::Octet min = TAO_DEF_GIOP_MINOR);

  /// Explicitly set the major and minor version.
  void set_version (CORBA::Octet maj, CORBA::Octet min);

  /// Copy operator.
  TAO_GIOP_Message_Version &operator= (const TAO_GIOP_Message_Version &src);

  /// Equality operator
  bool operator== (const TAO_GIOP_Message_Version &src) const;
  bool operator!= (const TAO_GIOP_Message_Version &src) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Version.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_VERSION_H */
