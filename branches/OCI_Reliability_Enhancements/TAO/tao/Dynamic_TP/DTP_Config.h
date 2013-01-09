// -*- C++ -*-

//=============================================================================
/**
 *  @file
 *
 *  $Id$
 *
 *   Header file for Loading DynamicTP Configurations.
 *
 *
 *  @author
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_CONFIG_H
#define TAO_DYNAMIC_TP_CONFIG_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/RB_Tree.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

struct TAO_Dynamic_TP_Export TAO_DTP_Definition
{
  int min_threads_;  // a default of -1 implies Lifespan of INFINITE, > 0 implies IDLE
  int init_threads_; // default to 5
  int max_threads_;  // a default of -1 implies no limit.
  size_t stack_size_;
  ACE_Time_Value timeout_;   // default to 60 seconds
  int queue_depth_;

  // Create explicit constructor to eliminate issues with non-initialized struct values.
  TAO_DTP_Definition() :
    min_threads_(-1),
    init_threads_(5),
    max_threads_(-1),
    stack_size_(ACE_DEFAULT_THREAD_STACKSIZE),
    timeout_(60,0),
    queue_depth_(0){}

};

class TAO_Dynamic_TP_Export TAO_DTP_Config_Registry_Installer
{
 public:
  TAO_DTP_Config_Registry_Installer (void);
};

class TAO_Dynamic_TP_Export TAO_DTP_Config_Registry : public ACE_Service_Object
{
public:
  TAO_DTP_Config_Registry (void);
  virtual ~TAO_DTP_Config_Registry (void);

  virtual int init (int argc, ACE_TCHAR* []);

  /// initializes the supplied set value with the configuration associated with the name, or returns false.
  bool find (const ACE_CString& name, TAO_DTP_Definition &entry);

  int bind (const ACE_CString& name, TAO_DTP_Definition &entry);
  int rebind (const ACE_CString& name, TAO_DTP_Definition &entry);

 private:
  typedef ACE_RB_Tree<ACE_CString, TAO_DTP_Definition, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> Registry;
  Registry registry_;
};

class TAO_Dynamic_TP_Export TAO_DTP_Config : public ACE_Service_Object
{
public:
  /// Constructor.
  TAO_DTP_Config (void);

  /// Destructor.
  virtual ~TAO_DTP_Config (void);

  /// Read a definition parameter set from the supplied args.
  /// There must be a -TPName argument, which, if replicated will cause the set to be ignored, unless -TPOverwrite is also set
  /// constriants: min threads <= initial threads <= max_threads.
  /// defaults: min threads = initial = max = 5
  /// default lifespan = infinite
  /// idle timeout is in secondes, default = 60
  /// default stack size = 0, system defined default used.
  /// queue depth is in number of messages, default is infinite
  /// Init can be called multiple times,
  virtual int init (int argc, ACE_TCHAR* []);

private:
  int parse_long (int &curarg, int argc, ACE_TCHAR* argv[], const ACE_TCHAR *match, long &value);
  int parse_bool (int &curarg, int argc, ACE_TCHAR* argv[], const ACE_TCHAR *match, bool &value);
  int parse_string (int &curarg, int argc, ACE_TCHAR* argv[], const ACE_TCHAR *match, ACE_TCHAR *&value);
  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);

};

static TAO_DTP_Config_Registry_Installer config_installer;


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_DTP_Config_Registry)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_DTP_Config_Registry)

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_DTP_Config)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_DTP_Config)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_CONFIG_H */
