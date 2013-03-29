// -*- C++ -*-
//=============================================================================
/**
 *  @file   ImR_ResponseHandler.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef IMR_RESPONSE_HANDLER_H
#define IMR_RESPONSE_HANDLER_H


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


//----------------------------------------------------------------------------
/**
 * @class ImR_ResponseHandler
 *
 * @brief interface serving as the basis for different strategies of activating
 * servers on demand
 *
 */

class ImR_ResponseHandler
{
public:
  ImR_ResponseHandler (void);
  virtual ~ImR_ResponseHandler (void);

  // dummy implementations used for internal operations
  virtual void send_ior (const char *pior);
  virtual void send_exception (CORBA::Exception *ex);
};

#endif /* IMR_RESPONSE_HANDLER_H */
