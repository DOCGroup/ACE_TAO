// -*- C++ -*-
//=============================================================================
/**
 *  @file   ImR_ResponseHandler.h
 */
//=============================================================================

#ifndef IMR_RESPONSE_HANDLER_H
#define IMR_RESPONSE_HANDLER_H

#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


//----------------------------------------------------------------------------
/**
 * @class ImR_ResponseHandler
 *
 * @brief interface serving as the basis for different strategies of activating
 * servers on demand
 */

class ImR_ResponseHandler
{
public:
  ImR_ResponseHandler ();
  virtual ~ImR_ResponseHandler ();

  // Dummy implementations used for internal operations
  virtual void send_ior (const char *pior);
  virtual void send_exception (CORBA::Exception *ex);
};

#endif /* IMR_RESPONSE_HANDLER_H */
