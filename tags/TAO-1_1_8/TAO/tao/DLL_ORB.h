// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     DLL_ORB.h
//
// = DESCRIPTION
//     Header file for the TAO_DLL_ORB helper class.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_DLL_ORB_H
#define TAO_DLL_ORB_H

#include "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

class TAO_Export TAO_DLL_ORB : public ACE_Task_Base
{
  // = TITLE
  //     Manager for TAO library services and singleton cleanup.
  //
  // = DESCRIPTION
  //     The <TAO_Singleton_Manager> is basically simplified version of
  //     the ACE_Object_Manager.  It is designed specifically to
  //     manage singletons created by TAO.  For example, Singleton
  //     instances created by TAO will be automatically registered
  //     with the Singleton instance of this Singleton Manager.
  //
  //     This class is necessary to ensure that TAO-specific
  //     Singletons are centrally isolated.  The idea is that
  //     destruction of the instance of the <TAO_Singleton_Manager>
  //     triggers destruction of all objects/services registered with
  //     it.

public:

  TAO_DLL_ORB (void);
  // Constructor

  ~TAO_DLL_ORB (void);
  // Destructor

  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initializes ORB when dynamic linking occurs.

  virtual int fini (void);
  // Terminates ORB when dynamic unlinking occurs.

  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing.

  CORBA::ORB_ptr orb (void);
  // Returns a duplicate reference to the ORB.

private:

  CORBA::ORB_var orb_;
  // Reference to the ORB.

};

#if defined (__ACE_INLINE__)
# include "tao/DLL_ORB.inl"
#endif  /* ! __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_DLL_ORB)
ACE_FACTORY_DECLARE (TAO, TAO_DLL_ORB)

#include "ace/post.h"

#endif  /* TAO_DLL_ORB_H */
