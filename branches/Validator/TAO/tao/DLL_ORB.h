// -*- C++ -*-

//=============================================================================
/**
 *  @file     DLL_ORB.h
 *
 *  $Id$
 *
 *  Header file for the TAO_DLL_ORB helper class.
 *
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_DLL_ORB_H
#define TAO_DLL_ORB_H

#include "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/ORB.h"

/**
 * @class TAO_DLL_ORB
 *
 * @brief Helper class that initializes an ORB when an instance of
 *        this class is dynamically loaded.
 *
 * This class is deprecated.  While it does demonstrate how to
 * initialize an ORB in a dynamically loaded object, the fact that it
 * is located in the TAO library forces the TAO library to be linked
 * to the application binary.  This prevents TAO from being completely
 * decoupled from an application when dynamic loading is used, hence
 * defeating the purpose of dynamically loading the object!
 * @par
 * For an example of how to dynamically load an ORB (actually initialize
 * an ORB inside a dynamically loaded object) that is completely
 * decoupled from an application binary see the TAO/tests/DLL_ORB
 * test.
 */
class TAO_Export TAO_DLL_ORB : public ACE_Task_Base
{

public:

  /// Constructor
  TAO_DLL_ORB (void);

  /// Destructor
  ~TAO_DLL_ORB (void);

  /// Initializes ORB when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates ORB when dynamic unlinking occurs.
  virtual int fini (void);

  /// Run by a daemon thread to handle deferred processing.
  virtual int svc (void);

  /// Returns a duplicate reference to the ORB.
  CORBA::ORB_ptr orb (void);

private:

  /// Reference to the ORB.
  CORBA::ORB_var orb_;

};

#if defined (__ACE_INLINE__)
# include "tao/DLL_ORB.inl"
#endif  /* ! __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_DLL_ORB)
ACE_FACTORY_DECLARE (TAO, TAO_DLL_ORB)

#include "ace/post.h"

#endif  /* TAO_DLL_ORB_H */
