// -*- C++ -*-

// ===================================================================
/**
 *  @file   POA_Extension_Initializer.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 *  @author Angelo Corsaro <corsaro@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_POA_EXTENSION_INITIALIZER_H
#define TAO_POA_EXTENSION_INITIALIZER_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward reference.
class TAO_Object_Adapter;

class TAO_Export TAO_POA_Extension_Initializer
{
public:
  /// Constructor.
  TAO_POA_Extension_Initializer (void);

  /// Destructor.
  virtual ~TAO_POA_Extension_Initializer (void);

  /**
   * Called at POA initialization for libraries to register their
   * POA extensions with the POA.
   */
  void register_hooks (TAO_Object_Adapter &object_adapter,
                       CORBA::Environment &ACE_TRY_ENV);

  /**
   * Adds an initializer to the "chain of initializers". This is used
   * to reduce the footprint of the ORB core that would have been needed
   * to store a variable length list of POA extension initializers.
   */
  void add_initializer (TAO_POA_Extension_Initializer *initializer);

protected:

  virtual void register_hooks_impl (TAO_Object_Adapter &object_adapter,
                                    CORBA::Environment &ACE_TRY_ENV) = 0;

private:
  TAO_POA_Extension_Initializer *next_;
};

#include "ace/post.h"
#endif /* TAO_POA_EXTENSION_INITIALIZER_H */
