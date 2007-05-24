// -*- C++ -*-

//=============================================================================
/**
 *  @file    Upcall_Command.h
 *
 *  $Id$
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_UPCALL_COMMAND_H
#define TAO_UPCALL_COMMAND_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#ifndef ACE_LACKS_PRAGMA_ONCE
# pragma once
#endif /* !ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Upcall_Command
   *
   * @brief "ABC" that all operation-specific command objects must
   *        subclass.
   *
   * The actual upcall into a servant is executed through a concrete
   * @c Upcall_Command object.  This is necessary to simplify upcall
   * interactions in the presence of interceptors, such as when the
   * @c send_exception() interception point must be invoked when the
   * servant operation throws an exception.
   */
  class TAO_PortableServer_Export Upcall_Command
  {
  public:
    /// Destructor.
    virtual ~Upcall_Command (void);

    /// Execute the upcall.
    virtual void execute (void) = 0;
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_UPCALL_COMMAND_H */
