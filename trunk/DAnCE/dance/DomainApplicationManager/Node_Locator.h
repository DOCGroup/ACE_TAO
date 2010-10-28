// -*- C++ -*-

//=============================================================================
/**
 *  @file    Node_Locator.h
 *
 *  $Id$
 *
 * @brief  Locates NodeManager instances.
 *
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODE_LOCATOR_H_
#define NODE_LOCATOR_H_

#include "ace/Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"
#include "tao/ORB.h"
#include "dance/Deployment/Deployment_NodeManagerC.h"
#include "dance/DomainApplicationManager/DomainApplicationManager_Export.h"

namespace DAnCE
{
  class DomainApplicationManager_Export Node_Locator
  {
  public:
    Node_Locator (CORBA::ORB_ptr orb,
                  CosNaming::NamingContext_ptr nc);

    ::Deployment::NodeManager_ptr locate_node (const char *name);

    bool process_node_map (const ACE_TCHAR *file);
    bool process_cdd (const ACE_TCHAR *file);

    void store_ior (const char *name, const char *ior);

  private:
    ::Deployment::NodeManager_ptr resolve_ior (const char *name,
                                               const char *ior);

    ::Deployment::NodeManager_ptr ns_lookup (const char *name);

    typedef ACE_Map_Manager<ACE_CString,
                            ACE_CString,
                            ACE_Null_Mutex> NODEMAP;

    NODEMAP nodes_;
    CORBA::ORB_var orb_;
    CosNaming::NamingContext_var nc_;
  };

}

#endif /* NODE_LOCATOR_H_ */
