/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    CC_LockSetFactory.h
//
// = DESCRIPTION
//      This class implements the lock set factory interface from the
//      concuurency service
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#if !defined (_CC_LOCKSETFACTORY_H)
#define _CC_LOCKSETFACTORY_H

// #include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlS.h"

class TAO_ORBSVCS_Export CC_LockSetFactory : public POA_CosConcurrencyControl::LockSetFactory
  // = TITLE
  //     CC_LockSetFactory
  //
  // = DESCRIPTION
  //     This class implements the LockSetFactory interface that is part
  //     of the CosConcurrency service. Please consult the idl file for
  //     detailed descriptions apart from the comments in this file
{
public:
  CC_LockSetFactory(void);
  // Default constructor

  ~CC_LockSetFactory(void);
  // Destructor

  virtual CosConcurrencyControl::LockSet_ptr create ( CORBA::Environment &env);

  virtual CosConcurrencyControl::LockSet_ptr
    create_related (CosConcurrencyControl::LockSet_ptr which,
		    CORBA::Environment &env);
private:
  ACE_Lock *lock_;
  // Lock to serialize the access to the factory.
}; // CC_LockSetFactory

//#if defined (__ACE_INLINE__)
//#include "CC_LockSetFactory.i"
//#endif // defined INLINE

typedef CC_LockSetFactory *CC_LockSetFactory_var ;

#endif /* _CC_LOCKSETFACTORY_H */
