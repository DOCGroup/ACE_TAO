// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IORManipulation.h
//
// = DESCRIPTION
//    This class implements IOR interface to the ORB
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IOR_MANIPULATION_H
#define TAO_IOR_MANIPULATION_H

#include "tao/Object.h"
#include "tao/IORS.h"

class TAO_IOR_Manipulation_impl : public POA_TAO_IOP::TAO_IOR_Manipulation
{
// = TITLE
  //     IOR Manipulation class
  //
  // = DESCRIPTION
  //
  //
public:

  TAO_IOR_Manipulation_impl (void);
  // constructor

  ~TAO_IOR_Manipulation_impl (void);
  // destructor

  virtual CORBA::Object_ptr merge_iors (
    const TAO_IOP::TAO_IOR_Manipulation::IORList & iors,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr add_profiles (
      CORBA::Object_ptr ior1,
      CORBA::Object_ptr ior2,
      CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::Object_ptr remove_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::ULong is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ());

  virtual CORBA::ULong get_profile_count (
    CORBA::Object_ptr ior,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ());
};

#endif /* TAO_IOR_MANIPULATION_H */
