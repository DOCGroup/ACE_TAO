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
#include "tao/Dynamic_TP/Dynamic_TP_POA_Strategy.h"


#include "ace/Synch.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/RB_Tree.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

struct TAO_Dynamic_TP_Export TAO_DTP_POA_Config_Map
{
	ACE_CString tp_config_name;
	TAO_Dynamic_TP_POA_Strategy * dtp_strategy_config;

};

class TAO_Dynamic_TP_Export TAO_Dynamic_TP_POA_Config_Map_Registry_Installer
{
 public:
  TAO_Dynamic_TP_POA_Config_Map_Registry_Installer (void);
};

class TAO_Dynamic_TP_Export TAO_Dynamic_TP_POA_Config_Map_Registry : public ACE_Service_Object
{
public:
  TAO_Dynamic_TP_POA_Config_Map_Registry (void);
  virtual ~TAO_Dynamic_TP_POA_Config_Map_Registry (void);

  virtual int init (int argc, ACE_TCHAR* []);

  /// initializes the supplied set value with the configuration associated with the name, or returns false.
  bool find (const ACE_CString& name, TAO_DTP_POA_Config_Map &entry);

  int bind (const ACE_CString& name, TAO_DTP_POA_Config_Map &entry);
  int rebind (const ACE_CString& name, TAO_DTP_POA_Config_Map &entry);

  int get_count();

 private:
  typedef ACE_RB_Tree<ACE_CString, TAO_DTP_POA_Config_Map, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> Registry;
  Registry registry_;
};

class TAO_POA_Core;

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
  int load_poa_map (int &curarg, ACE_TCHAR *argv[], TAO_Dynamic_TP_Config_Registry* config_reg,TAO_Dynamic_TP_POA_Config_Map_Registry* reg);
  int parse_string (int &curarg, int argc, ACE_TCHAR* argv[], const ACE_TCHAR *match, ACE_TCHAR *&value);
  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);
};

static TAO_Dynamic_TP_POA_Config_Map_Registry_Installer poa_config_map_installer;


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Config_Map_Registry)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Config_Map_Registry)

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_POA_LOADER_H */
