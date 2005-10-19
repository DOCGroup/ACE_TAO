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

#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"
#include "NodeApp_Configurator.h"

namespace CIAO
{
  class NodeApplication_Core;

  /**
   * @class NodeApplication_Options
   *
   * @brief An class for managing and extracting command line options
   * for NodeApplication.
   */
  class NodeApplication_Options
  {
  public:
    // default ctor.
    NodeApplication_Options () : use_callback_ (1), rt_support_ (0) {}

    // extracting commandline arguments 
    int parse_args (int argc, char *argv[]);

    friend class NodeApplication_Core;

  private:
    // The name of the file to write stringified IOR to.
    ACE_CString ior_output_filename_;

    // Stringified IOR of a CIAO's callback object.
    ACE_CString callback_ior_;

    // CIAO ComponentServer uses the callback object to pass it's
    // own object reference back to NodeApplicationManager.
    int use_callback_;

    // If we need to support RT-CORBA.  Currently, this is
    // mandatory, but we can probably allow some sort of
    // "best-effort" RT support.  I.e., if the platform/environment
    // doesn't support RT, then we will still deploy the NodeApp but
    // ignore the RT spec.  Perhaps something in the future.
    int rt_support_;
  };

  /**
   * @class NoOP_Configurator
   *
   * @brief The NoOp_Configurator doesn't do anything.
   */
  class NoOp_Configurator : public NodeApp_Configurator
  {
  public:
    /// Default constructor.
    NoOp_Configurator (CORBA::ORB_ptr o);

    /// Default destructor.
    virtual ~NoOp_Configurator (void);

    /**
     * @brief "initialize" is called after NodeApplication get a hold
     * at this object.
     */
    virtual int initialize (void);

    /**
     * @brief "init_resource_manager" is called by NodeApplication when
     * it receives an "install" commands.
     */
    virtual int
    init_resource_manager (const ::Deployment::Properties &properties);

    /**
     * @brief get a policyset by its name.
     */
    virtual CORBA::PolicyList *
    find_container_policies (const ::Deployment::Properties &properties);
  };

  /**
   * @class NodeApplication_Core
   *
   * @brief An ACE_Task subclass that allow the ORB thread to be run
   * as separate thread when RT behaviors are needed.
   */
  class NodeApplication_Core
    : public virtual ACE_Task_Base
  {
  public:
    NodeApplication_Core (CORBA::ORB_ptr orb,
                          NodeApplication_Options &opts);

    virtual int svc (void);

    int run_orb (void);

  protected:
    int configurator_factory (void);

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
