// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_FaultTolerance
//
// = FILENAME
//    FT_IOGR_Property.h
//
// = DESCRIPTION
//    This class implements the IOGR properties for the FT service
//
// = AUTHOR
//     Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FT_IOGR_PROPERTY_H
#define TAO_FT_IOGR_PROPERTY_H
#include "ace/pre.h"

#ifndef TAO_FT_SERVICE_SAFE_INCLUDE
#error "You should not include FT_IOGR_Property.h use FT_Service_Activate.h"
#endif /* !TAO_FT_SERVICE_SAFE_INCLUDE */


#include "tao/LocalObject.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FT_CORBAC.h"
#include "tao/IOPC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Profile;

class TAO_FT_Export TAO_FT_IOGR_Property  : public TAO_IOP::TAO_IOR_Property,
                                         public CORBA::LocalObject
{
  // = TITLE
  //   TAO_FT_IOGR_Property
  //
  // = DESCRIPTION
  //   This class serves as the callback class for setting the
  //   TaggedComponent properties in the profiles of the IOGR. The
  //   user can make an instance of this class with the right
  //   properties and use the helper methods in the IORManipulation
  //
  //   NOTE: This class is NOT thread safe. The range of services that
  //   use this class (like the RM) should use them in a thread safe
  //   manner.

public:

  TAO_FT_IOGR_Property (FT::TagFTGroupTaggedComponent &ft_group);
  // Our Constructor

  ~TAO_FT_IOGR_Property (void);
  // destructor

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual CORBA::Boolean set_property (
      CORBA::Object_ptr ior
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
      CORBA::SystemException,
      TAO_IOP::Invalid_IOR
    ));
  // Set the property for the IOGR

  virtual CORBA::Boolean set_primary (
      CORBA::Object_ptr ior1,
      CORBA::Object_ptr ior2
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
      CORBA::SystemException,
      TAO_IOP::NotFound,
      TAO_IOP::Duplicate
    ));
  // Set <ior1> as  primary which is a part of <ior2>

  virtual CORBA::Object_ptr get_primary (
      CORBA::Object_ptr ior
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::NotFound
      ));
  // Get the primary member from the IOGR <ior>

  virtual CORBA::Boolean is_primary_set (
      CORBA::Object_ptr ior
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  // Is there a primary available for <ior>

  CORBA::Boolean reset_tagged_components (
      FT::TagFTGroupTaggedComponent &ft_group);
  // Reset the underlying tagged components held by the class

private:

  TAO_Profile *get_primary_profile (CORBA::Object_ptr ior);
  // An helper function that gets the primary profile from the <ior>

  CORBA::Boolean encode_properties (
      TAO_OutputCDR &cdr,
      IOP::TaggedComponent &tagged_components);
  // An helper method for encoding the proeprties

  FT::TagFTGroupTaggedComponent &ft_group_tagged_component_;
  // Hold the reference to the Ft group tagged component
};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "FT_IOGR_Property.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_FT_IOGR_PROPERTY_H */
