// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplication_Core.h
 *
 *  $Id$
 *
 *  An ACE_Task subclass that manages the NodeApplication runtime.  We
 *  can run this class as a separate thread, as in the case of RT
 *  support, or as a event driven ORB.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_NODEAPPLICATION_CORE_H
#define CIAO_NODEAPPLICATION_CORE_H
#include /**/ "ace/pre.h"

#include "CIAO_NodeApplication_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "ace/Task.h"
#include "ace/Auto_Ptr.h"
#include "Configurator_Factory.h"
#include "NoOp_Configurator.h"

namespace CIAO
{
  /**
   * @class NodeApplication_Core
   *
   * @brief An ACE_Task subclass that allow the ORB thread to be run
   * as separate thread when RT behaviors are needed.
   */
  class NODEAPPLICATION_Export NodeApplication_Core
    : public virtual ACE_Task_Base
  {
  public:
    NodeApplication_Core (NodeApplication_Options &opts);

    virtual int svc (void);

    int startup (int argc, char *argv[]);

    int run_orb (void);

  protected:
    CORBA::ORB_var orb_;

    NodeApplication_Options &options_;

    auto_ptr<NodeApp_Configurator> configurator_;
  };

}

#if defined (__ACE_INLINE__)
# include "NodeApplication_Core.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEAPPLICAITON_CORE_H */
