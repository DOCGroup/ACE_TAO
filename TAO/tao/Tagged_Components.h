// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Tagged_Components.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_TAGGED_COMPONENTS_H
#define TAO_TAGGED_COMPONENTS_H

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"

class TAO_Export TAO_Tagged_Components
{
  //
  // = TITLE
  //   The policy manager implementation.
  //
  // = DESCRIPTION
  //   This class is used to manipulate and access the <components>
  //   field of an IIOP profile (and other GIOP profiles).
  //   The definition for that field is simply a sequence of the
  //   following structures:
  //
  //   typedef unsigned long ComponentId;
  //   struct TaggedComponent {
  //   ComponentId     tag;
  //   sequence<octet> component_data;
  //   };
  //
  //   the real motivation is to minimize the marshaling and
  //   demarshaling of the <component_data> field for certain critical
  //   components.
  //
public:
  TAO_Tagged_Components (void);
  // Constructor

  // = Standard OMG that require fast access.
  void set_orb_type (CORBA::ULong orb_type);
  // The the IOP::TAG_ORB_TYPE component value

  int get_orb_type (CORBA::ULong& orb_type) const;
  // Gets the IOP::TAG_ORB_TYPE component value, return 0 if the
  // component is not present.

  void set_code_sets (const CONV_FRAME::CodeSetComponentInfo &);
  void set_code_sets (CONV_FRAME::CodeSetComponentInfo &);
  // The the IOP::TAG_CODE_SETS component value, the first version
  // copies the data, the second version steal all the sequence
  // buffers.

  int get_code_sets (const CONV_FRAME::CodeSetComponentInfo* &) const;
  // Gets a reference of the IOP::TAG_CODE_SETS component value,
  // return 0 if the component is not present.

  int get_code_sets (CONV_FRAME::CodeSetComponentInfo &) const;
  // Gets a copy of the IOP::TAG_CODE_SETS component value, return 0
  // if the component is not present.

  // = TAO specific components
  void set_tao_priority_range (CORBA::Short min_priority,
                               CORBA::Short max_priority);
  // The the TAO_TAG_PRIORITY_RANGE component value.

  int get_tao_priority_range (CORBA::Short& min_priority,
                              CORBA::Short& max_priority) const;
  // Gets the TAO_TAG_PRIORITY_RANGE component value.

  // = Generic components

  void set_component (const IOP::TaggedComponent& component);
  // Insert the component into the list, making a copy of the octet
  // sequence.

  void set_component (IOP::TaggedComponent& component);
  // Insert the component into the list, but efficiently stealing the
  // contents of the octet sequence.

  int get_component (IOP::TaggedComponent& component) const;
  // Get a copy of the component identified by <component.tag>, return
  // 0 if the component is not present.

  // = Marshaling and demarshaling

  int encode (TAO_OutputCDR& cdr) const;
  int decode (TAO_InputCDR& cdr);
  // Marshal and demarshal the list.

private:
  void set_code_sets_i (CONV_FRAME::CodeSetComponent &lhs,
                        CONV_FRAME::CodeSetComponent &rhs);
  // Helper method to implement set_code_sets()

  void set_known_component_i (const IOP::TaggedComponent& component);
  void set_component_i (const IOP::TaggedComponent& component);
  void set_component_i (IOP::TaggedComponent& component);
  void add_component_i (const IOP::TaggedComponent& component);
  void add_component_i (IOP::TaggedComponent& component);
  void set_component_i (IOP::ComponentId tag, TAO_OutputCDR &cdr);
  // Helper methods to implement set_component()

  int get_known_component_i (IOP::TaggedComponent& component) const;
  int get_component_i (IOP::TaggedComponent& component) const;
  // Helper methods to implement set_component()

  int known_tag (IOP::ComponentId tag) const;
  // Is <tag> a well-known component?

  int unique_tag (IOP::ComponentId tag) const;
  // Does <tag> show up only once?

private:
  CORBA::ULong orb_type_;
  // The ORB_TYPE component value

  CONV_FRAME::CodeSetComponentInfo code_sets_;
  // The ORB_TYPE component value

  CORBA::Short tao_priority_min_;
  CORBA::Short tao_priority_max_;
  // The TAO_PRIORITY_RANGE priorities

  IOP::MultipleComponentProfile components_;
  // The rest of the components, to be compliant we cannot drop a
  // bunch of them.

  // A flag for each component...
  unsigned int orb_type_set_  : 1;
  unsigned int code_sets_set_ : 1;
  unsigned int tao_priority_range_set_ : 1;
};

#if defined (__ACE_INLINE__)
# include "tao/Tagged_Components.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_TAGGED_COMPONENTS_H */
