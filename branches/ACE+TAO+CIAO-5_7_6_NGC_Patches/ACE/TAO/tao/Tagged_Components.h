// -*- C++ -*-

//=============================================================================
/**
 *  @file   Tagged_Components.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_TAGGED_COMPONENTS_H
#define TAO_TAGGED_COMPONENTS_H

#include /**/ "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;

/// Tag for storing multiple endpoints within a single profile.
/**
 * This is TAO-specific, and is mostly used by TAO's RTCORBA support.
 * The standard TAG_ALTERNATE_IIOP_ADDRESSES tagged component is the
 * portable alternative.
 */
static const CORBA::ULong TAO_TAG_ENDPOINTS =  0x54414f02U;

/**
 * @class TAO_Tagged_Components
 *
 * @brief The policy manager implementation.
 *
 * This class is used to manipulate and access the <components>
 * field of an IIOP profile (and other GIOP profiles).
 * The definition for that field is simply a sequence of the
 * following structures:
 * typedef unsigned long ComponentId;
 * struct TaggedComponent {
 * ComponentId     tag;
 * sequence<octet> component_data;
 * };
 * the real motivation is to minimize the marshaling and
 * demarshaling of the <component_data> field for certain critical
 * components.
 */
class TAO_Export TAO_Tagged_Components
{
public:
  /// Constructor
  TAO_Tagged_Components (void);

  // = Standard OMG that require fast access.
  /// The the IOP::TAG_ORB_TYPE component value
  void set_orb_type (CORBA::ULong orb_type);

  /// Gets the IOP::TAG_ORB_TYPE component value, return 0 if the
  /// component is not present.
  int get_orb_type (CORBA::ULong& orb_type) const;

  /**
   * The the IOP::TAG_CODE_SETS component value, the first version
   * copies the data, the second version steal all the sequence
   * buffers.
   */
  void set_code_sets (const CONV_FRAME::CodeSetComponentInfo &);
  void set_code_sets (CONV_FRAME::CodeSetComponentInfo &);

  /// Gets a reference of the IOP::TAG_CODE_SETS component value,
  /// return 0 if the component is not present.
  int get_code_sets (const CONV_FRAME::CodeSetComponentInfo* &) const;

  /// Gets a copy of the IOP::TAG_CODE_SETS component value, return 0
  /// if the component is not present.
  int get_code_sets (CONV_FRAME::CodeSetComponentInfo &) const;

  // = Generic components

  /// Insert the component into the list, making a copy of the octet
  /// sequence.
  void set_component (const IOP::TaggedComponent& component);

  /// Insert the component into the list, but efficiently stealing the
  /// contents of the octet sequence.
  void set_component (IOP::TaggedComponent& component);

  /// Get a copy of the component identified by <component.tag>, return
  /// 0 if the component is not present.
  int get_component (IOP::TaggedComponent& component) const;

  /// Remove a component, if present. Return number of components
  /// removed.
  int remove_component (IOP::ComponentId id);

  // = Marshaling and demarshaling

  /// Marshal the list.
  int encode (TAO_OutputCDR& cdr) const;

  /// Demarshal the list.
  int decode (TAO_InputCDR& cdr);

  /**
   * Read/Write access to the underlying
   * MutipleComponentProfile. Added by request from Chris Hafey
   * <chris@stentorsoft.com>
   */
  IOP::MultipleComponentProfile &components (void);

private:
  /// Helper method to implement set_code_sets()
  void set_code_sets_i (CONV_FRAME::CodeSetComponent &lhs,
                        CONV_FRAME::CodeSetComponent &rhs);

  /// Helper methods to implement set_component()
  void set_known_component_i (const IOP::TaggedComponent& component);
  void set_component_i (const IOP::TaggedComponent& component);
  void set_component_i (IOP::TaggedComponent& component);
  void add_component_i (const IOP::TaggedComponent& component);
  void add_component_i (IOP::TaggedComponent& component);
  void set_component_i (IOP::ComponentId tag, TAO_OutputCDR &cdr);

  /// Helper methods to implement remove_component()
  int remove_known_component_i (IOP::ComponentId tag);
  int remove_component_i (IOP::ComponentId tag);

  /// Helper methods to implement set_component()
  int get_known_component_i (IOP::TaggedComponent& component) const;
  int get_component_i (IOP::TaggedComponent& component) const;

  /// Is @a tag a well-known component?
  bool known_tag (IOP::ComponentId tag) const;

  /// Does @a tag show up only once?
  bool unique_tag (IOP::ComponentId tag) const;

private:
  /// The ORB_TYPE component value
  CORBA::ULong orb_type_;

  /// The ORB_TYPE component value
  CONV_FRAME::CodeSetComponentInfo code_sets_;

  /// The rest of the components, to be compliant we cannot drop a
  /// bunch of them.
  IOP::MultipleComponentProfile components_;

  /// A flag for each component...
  CORBA::Octet orb_type_set_;
  CORBA::Octet code_sets_set_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Tagged_Components.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_TAGGED_COMPONENTS_H */
