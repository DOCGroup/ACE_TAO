// -*- C++ -*-

//=============================================================================
/**
 * @file Node_Manager_Module.h
 *
 * $Id$
 *
 * @Brief  To start NodeManager by starter
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 */
//=============================================================================

#ifndef NODE_MANAGER_MODULE_H
#define NODE_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "DAnCE_NodeManager_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/Object_Loader.h"

namespace DAnCE
  {
  class NodeManager_Impl;
  class RedirectionService;
}

/**
 * @class Node_Manager_Module
 *
 * @brief The shared object that is instantiated when the node manager
 *        module/library is dynamically loaded.
 *
 * This class runs the node manager instance
 */
class DAnCE_NodeManager_Module_Export DAnCE_NodeManager_Module
      : public TAO_Object_Loader
  {
  public:
    /// Constructor.
    DAnCE_NodeManager_Module (void);

    /// Destructor.
    ~DAnCE_NodeManager_Module (void);

    /// Overload the base class method to create a new instance
    /// of a DAnCE_NodeManager_Module object.
    virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                             int argc,
                                             ACE_TCHAR *argv []);

  private:
    /// Here we store the servants.
    typedef ACE_Map_Manager < ACE_CString,
    DAnCE::NodeManager_Impl *,
    ACE_Null_Mutex > Servant_Map;
    Servant_Map nm_map_;
    CORBA::ORB_var orb_;
    CosNaming::NamingContext_var domain_nc_;

    /// Single redirection service for all NMs.
    DAnCE::RedirectionService * redirection_;
  };

ACE_FACTORY_DECLARE (DAnCE_NodeManager_Module, DAnCE_NodeManager_Module)

#include /**/ "ace/post.h"

#endif  /* NODE_MANAGER_MODULE_H */
