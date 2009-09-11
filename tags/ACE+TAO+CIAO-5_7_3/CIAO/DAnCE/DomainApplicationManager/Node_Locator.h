// -*- C++ -*-

//=============================================================================
/**
 *  @file    Node_Locator.h
 *
 *  $Id$
 *
 * @Brief  Locates NodeManager instances.
 *
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODE_LOCATOR_H_
#define NODE_LOCATOR_H_

#include "ace/Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"
#include "tao/ORB.h"
#include "DAnCE/Deployment/Deployment_NodeManagerC.h"
#include "DomainApplicationManager/DomainApplicationManager_Export.h"

namespace DAnCE
{
  class DomainApplicationManager_Export Node_Locator
  {
  public:
    Node_Locator (CORBA::ORB_ptr orb,
                  CosNaming::NamingContext_ptr nc);

    ::Deployment::NodeManager_ptr locate_node (const ACE_TCHAR *name);

    bool process_node_map (const ACE_TCHAR *file);

    void store_ior (const ACE_TCHAR *name, const ACE_TCHAR *ior);

  private:
    ::Deployment::NodeManager_ptr resolve_ior (const ACE_TCHAR *name,
                                               const ACE_TCHAR *ior);

    ::Deployment::NodeManager_ptr ns_lookup (const ACE_TCHAR *name);

    typedef ACE_Map_Manager<ACE_TString,
                            ACE_TString,
                            ACE_Null_Mutex> NODEMAP;

    NODEMAP nodes_;
    CORBA::ORB_var orb_;
    CosNaming::NamingContext_var nc_;
  };

}

#endif /* NODE_LOCATOR_H_ */
