// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Message_Version.h
 *
 *  $Id$
 *
 *   GIOP Version utility class definition
 *
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author  Carlos O' Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_GIOP_MESSAGE_VERSION_H
#define TAO_GIOP_MESSAGE_VERSION_H
#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



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

  /// Minor version number
  CORBA::Octet minor;

  /// Copy constructor
  TAO_GIOP_Message_Version (const TAO_GIOP_Message_Version &src);

  /// Default constructor.
  TAO_GIOP_Message_Version (CORBA::Octet maj = TAO_DEF_GIOP_MAJOR,
			    CORBA::Octet min = TAO_DEF_GIOP_MINOR);

  /// Destructor.
  ~TAO_GIOP_Message_Version (void);

  /// Explicitly set the major and minor version.
  void set_version (CORBA::Octet maj, CORBA::Octet min);

  /// Copy operator.
  TAO_GIOP_Message_Version &operator= (const TAO_GIOP_Message_Version &src);

  /// Equality operator
  int operator== (const TAO_GIOP_Message_Version &src);
  int operator!= (const TAO_GIOP_Message_Version &src);
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Version.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_VERSION_H */
