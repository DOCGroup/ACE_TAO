// -*- C++ -*-

//=============================================================================
/**
 *  @file     DLL_ORB.h
 *
 *  $Id$
 *
 *   Header file for the TAO_DLL_ORB helper class.
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

#include "tao/ORB.h"

/**
 * @class TAO_DLL_ORB
 *
 * @brief Manager for TAO library services and singleton cleanup.
 *
 * The <TAO_Singleton_Manager> is basically simplified version of
 * the ACE_Object_Manager.  It is designed specifically to
 * manage singletons created by TAO.  For example, Singleton
 * instances created by TAO will be automatically registered
 * with the Singleton instance of this Singleton Manager.
 * This class is necessary to ensure that TAO-specific
 * Singletons are centrally isolated.  The idea is that
 * destruction of the instance of the <TAO_Singleton_Manager>
 * triggers destruction of all objects/services registered with
 * it.
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

ACE_STATIC_SVC_DECLARE (TAO_DLL_ORB)
ACE_FACTORY_DECLARE (TAO, TAO_DLL_ORB)

#include "ace/post.h"

#endif  /* TAO_DLL_ORB_H */
