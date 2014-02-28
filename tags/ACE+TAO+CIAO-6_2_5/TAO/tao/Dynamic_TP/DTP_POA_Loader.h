// -*- C++ -*-

//=============================================================================
/**
 *  @file    DTP_POA_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading DynamicTP.
 *
 *
 *  @author  Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_POA_LOADER_H
#define TAO_DYNAMIC_TP_POA_LOADER_H
#include /**/ "ace/pre.h"
#include "tao/orbconf.h"
#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "ace/Synch.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/RB_Tree.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Dynamic_TP_Export TAO_DTP_POA_Loader : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_DTP_POA_Loader (void);

  /// Destructor.
  virtual ~TAO_DTP_POA_Loader (void);

  /// Initialize the DynamicTP loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

private:
  /// Set to true after init is called.
  int load_poa_map (ACE_TCHAR *map_str,
                    TAO_CSD_Strategy_Repository * reg);

  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_DTP_POA_Loader)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_DTP_POA_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_POA_LOADER_H */
