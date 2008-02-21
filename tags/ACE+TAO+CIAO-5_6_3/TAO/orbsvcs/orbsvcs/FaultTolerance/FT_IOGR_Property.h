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

#include "tao/LocalObject.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORManipulation/IORManip_Loader.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IOPC.h"
#include "orbsvcs/FaultTolerance/FT_ORB_Utils_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_FT_ORB_Utils_Export TAO_FT_IOGR_Property
  : public virtual TAO_IOP::TAO_IOR_Property,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Our Constructor
  TAO_FT_IOGR_Property (FT::TagFTGroupTaggedComponent &ft_group);

  TAO_FT_IOGR_Property (void);

  /// Destructor
  ~TAO_FT_IOGR_Property (void);

  /// Set the property for the IOGR
  virtual CORBA::Boolean set_property (
      CORBA::Object_ptr &ior);

  /// Set <ior1> as  primary which is a part of <ior2>
  virtual CORBA::Boolean set_primary (
      CORBA::Object_ptr &ior1,
      CORBA::Object_ptr ior2);

  /// Get the primary member from the IOGR <ior>
  virtual CORBA::Object_ptr get_primary (
      CORBA::Object_ptr ior);

  /// Is there a primary available for <ior>
  virtual CORBA::Boolean is_primary_set (
      CORBA::Object_ptr ior);

  virtual CORBA::Boolean remove_primary_tag (
      CORBA::Object_ptr &iogr);

  /// Reset the underlying tagged components held by the class
  CORBA::Boolean reset_tagged_components (
      FT::TagFTGroupTaggedComponent &ft_group);

  /// Extract the TagFTGroupTaggedComponent inside the <ior>
  CORBA::Boolean get_tagged_component (
      const CORBA::Object_ptr iogr,
      FT::TagFTGroupTaggedComponent &ft_group) const;

private:

  /// An helper function that gets the primary profile from the <ior>
  TAO_Profile *get_primary_profile (CORBA::Object_ptr ior);

  /// An helper method for encoding the proeprties
  CORBA::Boolean encode_properties (
      TAO_OutputCDR &cdr,
      IOP::TaggedComponent &tagged_components);

private:
  /// Hold the reference to the FT group tagged component
  FT::TagFTGroupTaggedComponent *ft_group_tagged_component_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "orbsvcs/FaultTolerance/FT_IOGR_Property.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_FT_IOGR_PROPERTY_H */
