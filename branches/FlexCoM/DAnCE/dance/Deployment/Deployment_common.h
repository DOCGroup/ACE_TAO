// $Id$

/**
 * @file Deployment_common.h
 *
 * This file collects common type definitions used across different
 * objects of DAnCE toolchain.
 *
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 */

#ifndef DANCE_COMMON_H
#define DANCE_COMMON_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"
#include "ace/Hash_Map_Manager_T.h"
#include "tao/AnyTypeCode/Any.h"
#include "dance/Deployment/Deployment_ConnectionC.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

/// A struct which captures the binding information about a component

namespace DAnCE 
{
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  CORBA::Any,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> PROPERTY_MAP;
  
  /// A struct which captures the compnent  binding information, which
  /// is useful for inter-assembly connection setup
  typedef struct _component_binding
  {
    _component_binding ()
    {
      providedReference_ = 0;
    }

    ACE_CString name_;
    ACE_CString plan_uuid_;
    ACE_CString node_;

    Deployment::Connections_var providedReference_;

    bool operator==(const struct _component_binding & comp)
    {
      if (this->name_ == comp.name_ &&
          this->plan_uuid_ == comp.plan_uuid_ &&
          this->node_ == comp.node_)
        return true;
      else
        return false;
    }
  } Component_Binding_Info;
}

#include /**/ "ace/post.h"
#endif /* DANCE_COMMON_H */
