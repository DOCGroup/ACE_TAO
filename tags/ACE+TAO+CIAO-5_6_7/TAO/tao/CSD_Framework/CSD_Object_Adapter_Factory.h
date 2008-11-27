// -*- C++ -*-

//=============================================================================
/**
 *  @file CSD_Object_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================


#ifndef TAO_CSD_OBJECT_ADAPTER_FACTORY_H
#define TAO_CSD_OBJECT_ADAPTER_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/Adapter_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CSD_FW_Export TAO_CSD_Object_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  /// Constructor
  TAO_CSD_Object_Adapter_Factory (void);

  // = The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  // details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);

  virtual int init (int argc,
                    ACE_TCHAR* argv[]);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_CSD_FW, TAO_CSD_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_CSD_FW, TAO_CSD_Object_Adapter_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_CSD_OBJECT_ADAPTER_FACTORY_H */
