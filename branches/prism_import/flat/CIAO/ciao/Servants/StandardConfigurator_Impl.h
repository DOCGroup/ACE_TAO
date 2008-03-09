// -*- C++ -*-

//=============================================================================
/**
 *  @file    StandardConfigurator_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Components::StandardConfigurator interface.
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_STANDARDCONFIGURATOR_IMPL_H
#define CIAO_STANDARDCONFIGURATOR_IMPL_H

# include /**/ "ace/pre.h"

#include "CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_StandardConfiguratorS.h"

namespace CIAO
{
  class Servant_Impl_Base;
}

namespace CIAO
{
  /**
   * @class StandardConfigurator_Impl
   *
   * @brief Servant implementation for Components::StandardConfigurator
   *
   * This class implements the Components::StandardConfigurator
   * interface as defined by the CCM spcification. This class aids
   * in setting the initial values of component attributes that are
   * defined in the meta data.
   */
  class CIAO_SERVER_Export StandardConfigurator_Impl
    : public virtual POA_Components::StandardConfigurator
  {
  public:
    /// Constructor
    StandardConfigurator_Impl (Servant_Impl_Base* toconfigure);

    /// Destructor
    virtual ~StandardConfigurator_Impl (void);

    /// This method will call set_attributes on the component that is
    /// being configured, i.e, the template method from
    /// StandardConfigurator interface.
    virtual void set_configuration (const Components::ConfigValues & descr);

    void configure (Components::CCMObject_ptr comp);

  private:
    /// The component whose attributes are going to be configured via
    /// set_attributes
    Servant_Impl_Base* component_;
  };

}

#include /**/ "ace/post.h"

#endif /* CIAO_STANDARDCONFIGURATOR_IMPL_H */
