// -*- C++ -*-

//=============================================================================
/**
 *  @file    Configurator_Factory.h
 *
 *  $Id$
 *
 *  Configurator Factory contains the factory method for creating concret
 *  NodeApp_Configurator object.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_CONFIGURATOR_FACTORY_H
#define CIAO_CONFIGURATOR_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"
#include "ace/DLL.h"
#include "NodeApp_Configurator.h"

namespace CIAO
{


  /**
   * @class NodeApplication_Options
   *
   * @brief An class for managing and extracting command line options
   * for NodeApplication.
   *
   * @note We currently support loading one single external module for
   * RT support.  Perhaps how this should really be done is to allow
   * the NodeApplication_Core to load up a list of external modules
   * (in DLLs or otherwise) and call the corresponding init methods in
   * sequence.   This way, we open up the component server so system
   * developers can plug in their own system configuration needs into
   * the whole system.
   */
  class NodeApplication_Options
  {
  public:
    // default ctor.
    NodeApplication_Options () : use_callback_ (true), rt_support_ (false) {}

    /// extracting commandline arguments
    int parse_args (int &argc, char *argv[]);

    NodeApp_Configurator *create_nodeapp_configurator (void);

    bool use_callback () {return this->use_callback_; }

    bool rt_support () {return this->rt_support_; }

    int write_ior_file ()
    { return (this->ior_output_filename_.length () != 0); }

    const char *ior_output_filename ()
    { return this->ior_output_filename_.c_str (); }

    const char *callback_ior ()
    { return this->callback_ior_.c_str (); }

  private:
    /// The name of the file to write stringified IOR to.
    ACE_CString ior_output_filename_;

    /// Stringified IOR of a CIAO's callback object.
    ACE_CString callback_ior_;

    /// CIAO ComponentServer uses the callback object to pass it's
    /// own object reference back to NodeApplicationManager.
    bool use_callback_;

    /// If we need to support RT-CORBA.  Currently, this is
    /// mandatory, but we can probably allow some sort of
    /// "best-effort" RT support.  I.e., if the platform/environment
    /// doesn't support RT, then we will still deploy the NodeApp but
    /// ignore the RT spec.  Perhaps something in the future.
    bool rt_support_;

    // For managing dynamically loaded configurator library
    ACE_DLL config_dll_;
  };

}


#include /**/ "ace/post.h"
#endif /* CIAO_CONFIGURATOR_FACTORY_H */
