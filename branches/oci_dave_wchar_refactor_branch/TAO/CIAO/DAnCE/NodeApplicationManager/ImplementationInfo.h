//$Id$
/* -*- C++ -*- */
/***************************************************************
 * @file   ImplementationInfo.h
 *
 * @brief  This file defines some helper functions for paring the
 *         Component ImplementationInfo and properties from
 *         the plan.
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 ***************************************************************/

#ifndef IMPLEMENTATIONIFO_H
#define IMPLEMENTATIONIFO_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/DeploymentC.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "Containers_Info_Map.h"
#include "CIAO_NAM_Export.h"

//@@ I wish we could have precompiled header support soon!!!

// This method is a helper function to parse the local deployment plan into the
// NodeImplementationInfo struct.

namespace CIAO
{
  class CIAO_NAM_Export NodeImplementationInfoHandler
  {
  public:
    NodeImplementationInfoHandler (
        const ::Deployment::DeploymentPlan & plan,
        const Deployment::ComponentPlans & shared_components);

    Deployment::NodeImplementationInfo *
      node_impl_info (void) const;

  private:
    Deployment::DeploymentPlan plan_;

    Deployment::NodeImplementationInfo_var node_info_;

    void populate_server_resource_def (void);

    void populate_container_impl_infos (void);

    /// Helper class that helps populate the container_impl_infos
    /// field.
    Containers_Info_Map containers_info_map_;
  };
}

#include /**/ "ace/post.h"
#endif /* IMPLEMENTATIONIFO_H */
