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
#include "ace/pre.h"

#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_IOR_Manipulation_impl
  : public TAO_IOP::TAO_IOR_Manipulation,
    public CORBA::LocalObject
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

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual CORBA::Object_ptr merge_iors (
    const TAO_IOP::TAO_IOR_Manipulation::IORList & iors,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ())
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
        TAO_IOP::TAO_IOR_Manipulation::Duplicate,
        TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR
      ));

  virtual CORBA::Object_ptr add_profiles (
      CORBA::Object_ptr ior1,
      CORBA::Object_ptr ior2,
      CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ())
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
        TAO_IOP::TAO_IOR_Manipulation::Duplicate,
        TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR
      ));

  virtual CORBA::Object_ptr remove_profiles (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ())
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList,
        TAO_IOP::TAO_IOR_Manipulation::NotFound
      ));

  virtual CORBA::ULong is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ())
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::NotFound
      ));

  virtual CORBA::ULong get_profile_count (
    CORBA::Object_ptr ior,
    CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ())
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList
      ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_IOR_MANIPULATION_H */
