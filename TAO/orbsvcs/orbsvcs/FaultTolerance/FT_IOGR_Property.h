// -*- C++ -*-

//=============================================================================
/**
 *  @file    FT_IOGR_Property.h
 *
 *  $Id$
 *
 *  This class implements the IOGR properties for the FT service
 *
 *
 *  @author  Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_IOGR_PROPERTY_H
#define TAO_FT_IOGR_PROPERTY_H
#include /**/ "ace/pre.h"

#ifndef TAO_FT_SERVICE_SAFE_INCLUDE
#error "You should not include FT_IOGR_Property.h use FT_Service_Activate.h"
#endif /* !TAO_FT_SERVICE_SAFE_INCLUDE */


#include "tao/LocalObject.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Profile;

/**
 * @class TAO_FT_IOGR_Property
 *
 * @brief TAO_FT_IOGR_Property
 *
 * This class serves as the callback class for setting the
 * TaggedComponent properties in the profiles of the IOGR. The
 * user can make an instance of this class with the right
 * properties and use the helper methods in the IORManipulation
 *
 * NOTE: This class is NOT thread safe. The range of services that
 * use this class (like the RM) should use them in a thread safe
 * manner.
 */
class TAO_FT_Export TAO_FT_IOGR_Property
  : public virtual TAO_IOP::TAO_IOR_Property,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Our Constructor
  TAO_FT_IOGR_Property (FT::TagFTGroupTaggedComponent &ft_group);

  /// Destructor
  ~TAO_FT_IOGR_Property (void);

  /// Set the property for the IOGR
  virtual CORBA::Boolean set_property (
      CORBA::Object_ptr ior
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      TAO_IOP::Invalid_IOR
    ));

  /// Set <ior1> as  primary which is a part of <ior2>
  virtual CORBA::Boolean set_primary (
      CORBA::Object_ptr ior1,
      CORBA::Object_ptr ior2
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      TAO_IOP::NotFound,
      TAO_IOP::Duplicate
    ));

  /// Get the primary member from the IOGR <ior>
  virtual CORBA::Object_ptr get_primary (
      CORBA::Object_ptr ior
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        TAO_IOP::NotFound
      ));

  /// Is there a primary available for <ior>
  virtual CORBA::Boolean is_primary_set (
      CORBA::Object_ptr ior
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual CORBA::Boolean remove_primary_tag (
      CORBA::Object_ptr iogr
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reset the underlying tagged components held by the class
  CORBA::Boolean reset_tagged_components (
      FT::TagFTGroupTaggedComponent &ft_group);

private:

  /// An helper function that gets the primary profile from the <ior>
  TAO_Profile *get_primary_profile (CORBA::Object_ptr ior);

  /// An helper method for encoding the proeprties
  CORBA::Boolean encode_properties (
      TAO_OutputCDR &cdr,
      IOP::TaggedComponent &tagged_components);

  /// Hold the reference to the Ft group tagged component
  FT::TagFTGroupTaggedComponent &ft_group_tagged_component_;
};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "FT_IOGR_Property.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_FT_IOGR_PROPERTY_H */
