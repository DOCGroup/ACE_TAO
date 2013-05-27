// -*- C++ -*-
//=============================================================================
/**
 *  @file   INS_Locator.h
 *
 *  $Id$
 *
 *  @brief  This class implements the ImR's INS Locator class
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_INS_LOCATOR_H
#define IMR_INS_LOCATOR_H
#include /**/ "ace/pre.h"

#include "tao/IORTable/Async_IORTable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ImR_ResponseHandler.h"
#include "tao/IORTable/Locate_ResponseHandler.h"

class ImR_Locator_i;

class INS_Loc_ResponseHandler : public ImR_ResponseHandler
{
public:
  INS_Loc_ResponseHandler (const char *key, ::IORTable::Locate_ResponseHandler rh);

  // dummy implementations used for internal operations
  virtual void send_ior (const char *pior);
  virtual void send_exception (CORBA::Exception *ex);

private:
  ACE_CString key_;
  TAO_AMH_Locate_ResponseHandler_var rh_;
};

/**
 * @class INS_Locator
 *
 * @brief Implementation Repository INS Locator class
 *
 * This class provides a callback for the IORTable to call when it needs
 * to dynamically receive a IOR to forward in response to an INS request.
 */
class INS_Locator
  : public virtual IORTable::AsyncLocator,
    public virtual ::CORBA::LocalObject
{
public:
  INS_Locator (ImR_Locator_i& loc);

  /// Locate the appropriate IOR.
  char* locate (const char *object_key);
  void async_locate (::IORTable::Locate_ResponseHandler handler,
                     const char *object_key);

private:
  ImR_Locator_i& imr_locator_;
};

#include /**/ "ace/post.h"
#endif
