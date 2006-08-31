#ifndef _CUTS_COMPONENT_REGISTRY_H_
#define _CUTS_COMPONENT_REGISTRY_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/RW_Thread_Mutex.h"

#include <map>
#include <string>

//=============================================================================
/**
 * @class CUTS_Component_Registry_Node
 */
//=============================================================================

class CUTS_Export CUTS_Component_Registry_Node
{
public:
  CUTS_Component_Registry_Node (void);

  virtual ~CUTS_Component_Registry_Node (void);
};

//=============================================================================
/**
 *
 */
//=============================================================================

typedef std::map <
  std::string, CUTS_Component_Registry_Node *> CUTS_Component_Registry_Map;


//=============================================================================
/**
 * @class CUTS_Component_Registry
 */
//=============================================================================

class CUTS_Export CUTS_Component_Registry
{
public:
  CUTS_Component_Registry (void);

  virtual ~CUTS_Component_Registry (void);

  virtual long register_component (const char * uuid);

  virtual void unregister_component (const char * uuid);

  virtual bool is_registered (const char * uuid);

  virtual long get_registration (const char * uuid,
                                 bool auto_register = true);

  size_t registry_size (void);

  ACE_RW_Thread_Mutex & lock (void);

  std::string get_component_by_id (long regid) const;

protected:
  CUTS_Component_Registry_Node * get_node (const char * uuid);

  CUTS_Component_Registry_Map registry_;

  ACE_RW_Thread_Mutex lock_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Component_Registry.inl"
#endif

#endif  // !defined _CUTS_COMPONENT_REGISTRY_H_
