// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_TP_POA_Loader.h
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
#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "ace/Synch.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/RB_Tree.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA_Core; //TODO: Do we still need this?

class TAO_Dynamic_TP_Export TAO_Dynamic_TP_POA_Loader : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_Dynamic_TP_POA_Loader (void);

  /// Destructor.
  virtual ~TAO_Dynamic_TP_POA_Loader (void);

  /// Initialize the DynamicTP loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

private:
  /// Set to true after init is called.
  bool initialized_;
  int load_poa_map (int &curarg, ACE_TCHAR *argv[], TAO_Dynamic_TP_Config_Registry* config_reg,TAO_CSD_Strategy_Repository * reg);
  int parse_string (int &curarg, int argc, ACE_TCHAR* argv[], const ACE_TCHAR *match, ACE_TCHAR *&value);
  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_POA_LOADER_H */
