// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Utils.h
 *
 *  $Id$
 *
 *   GIOP utility definitions
 *
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author  Carlos O' Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_GIOP_UTILS_H
#define TAO_GIOP_UTILS_H

#include /**/ "ace/pre.h"

#include "tao/Object.h"
#include "tao/GIOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_GIOP_Locate_Status_Msg
 *
 * @brief Hold the relevant information for every type of Locate msg.
 *
 * This class is there to hold the relevant info for different
 * types of locate status messages. As on date we dont know much
 * about other mesg types other than OBJECT_FORWARD. This class can
 * be clearly defined as time progresses.
 */
class TAO_Export TAO_GIOP_Locate_Status_Msg
{
public:
  /// The value will need to be used when the Message type is
  /// TAO_GIOP_OBJECT_FORWARD
  CORBA::Object_var forward_location_var;

  /// Type of Locate status message
  GIOP::LocateStatusType status;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_UTILS_H */
