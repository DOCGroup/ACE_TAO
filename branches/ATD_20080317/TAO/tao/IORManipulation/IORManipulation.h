// -*- C++ -*-

//=============================================================================
/**
 *  @file    IORManipulation.h
 *
 *  $Id$
 *
 *  This class implements IOR interface to the ORB
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IOR_MANIPULATION_H
#define TAO_IOR_MANIPULATION_H

#include /**/ "ace/pre.h"

#include "tao/LocalObject.h"
#include "tao/IORManipulation/IORManip_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORManipulation/IORC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IOR_Manipulation_impl
  : public TAO_IOP::TAO_IOR_Manipulation,
    public TAO_Local_RefCounted_Object
{
  // = TITLE
  //     IOR Manipulation class
  //
  // = DESCRIPTION
  //
  //
public:

  /// constructor
  TAO_IOR_Manipulation_impl (void);

  virtual CORBA::Object_ptr merge_iors (
    const TAO_IOP::TAO_IOR_Manipulation::IORList & iors
    );

  virtual CORBA::Object_ptr add_profiles (
      CORBA::Object_ptr ior1,
      CORBA::Object_ptr ior2
      );

  virtual CORBA::Object_ptr remove_profiles (
    CORBA::Object_ptr group,
    CORBA::Object_ptr ior2
    );

  virtual CORBA::Boolean set_property (
      TAO_IOP::TAO_IOR_Property_ptr prop,
      CORBA::Object_ptr group
      )
;

  // @@ note awkward argument order
  virtual CORBA::Boolean set_primary (
      TAO_IOP::TAO_IOR_Property_ptr prop,
      CORBA::Object_ptr new_primary,
      CORBA::Object_ptr group
      )
;

  virtual CORBA::Object_ptr get_primary (
      TAO_IOP::TAO_IOR_Property_ptr prop,
      CORBA::Object_ptr group
      )
;

  virtual CORBA::Boolean is_primary_set (
      TAO_IOP::TAO_IOR_Property_ptr prop,
      CORBA::Object_ptr group
      )
;

   virtual CORBA::Boolean remove_primary_tag (
       TAO_IOP::TAO_IOR_Property_ptr prop,
       CORBA::Object_ptr iogr
       );

  virtual CORBA::ULong is_in_ior (
    CORBA::Object_ptr ior1,
    CORBA::Object_ptr ior2
    );

  virtual CORBA::ULong get_profile_count (
    CORBA::Object_ptr group
    );

protected:

  /// destructor
  ~TAO_IOR_Manipulation_impl (void);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_IOR_MANIPULATION_H */
