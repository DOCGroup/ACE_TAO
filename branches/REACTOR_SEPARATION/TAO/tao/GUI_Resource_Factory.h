// -*- C++ -*-

//=============================================================================
/**
 *  @file   GUI_Resource_Factory.h
 *
 *  $Id$
 *
 *  @author Hamish Friedlander <ullexco@wave.co.nz>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_GUI_RESOURCE_FACTORY_H
#define TAO_GUI_RESOURCE_FACTORY_H
#include /**/ "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namepace TAO
{
  /**
   * @class GUI_Resource_Factory
   *
   * @brief Abstract Base Class for the GUI resource factory, which
   * will be held by the ORB Core
   *
   */
  class TAO_Export GUI_Resource_Factory
  {
  public:
    /// Create a reactor instance.
    /**
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual ACE_Reactor_Impl *create (void) const = 0;

  protected:
    ACE_Reactor_Impl *impl_;

  private:
    ACE_UNIMPLEMENTED_FUNC (GUI_Resource_Factory (void))
  };
}

#include /**/ "ace/post.h"
#endif /*TAO_GUI_RESOURCE_FACTORY_H*/
