//-*- C++ -*-
//=============================================================================
/**
 *  @file    IOP_Defns.h
 *
 *  $Id$
 *
 *  Definitions for IOP related classes. IOP definitions are part of
 *  the Interoperability model specified by the CORBA spec.
 *
 *
 *  @author Taken from the old GIOP.h file. Not sure of the author
 */
//=============================================================================

#ifndef _TAO_IOP_DEFN_H_
#define _TAO_IOP_DEFN_H_
#include "ace/pre.h"

// These were all in the GIOP.h file. They shouldn't be combined with
// GIOP as they are not specific to GIOP. They are part of the CORBA
// Interoperable reference model.

// namespace TAO_IOP

// Assigned Protocol/Profile tag values.  ORB protcols may be uniquely
// identified by tags such as these.  This allows each ORB's own
// objref profiles to be interchanged using IORs.
//
// Email to tag-request@omg.org to allocate tags.
typedef CORBA::ULong TAO_IOP_Profile_ID;

/**
 * @class TAO_IOP_Tagged_Profile
 *
 * @brief One per protocol.
 */
class TAO_IOP_Tagged_Profile
{
public:
  TAO_IOP_Profile_ID tag;
  TAO_opaque profile_data;
};

typedef TAO_Unbounded_Sequence<TAO_IOP_Tagged_Profile>
        TAO_IOP_Tagged_Profile_Sequence;

/**
 * @class TAO_IOP_IOR
 *
 * @brief InteroperableObjectReference
 *
 * ... a set of protocol-specific protocol profiles, plus a type
 * ID.  Only one object is denoted by all of this information.
 * It's OK to delete all profiles except the one for the single
 * protocol actually being used.
 */
class TAO_IOP_IOR
{

public:
  char *type_id;
  TAO_IOP_Tagged_Profile_Sequence profiles;
};

// Some protocols can be factored into a set of optional components.
// Use of such components is defined by the protocol's specification.
//
// Email to tag-request@omg.org to allocate tags.

typedef CORBA::ULong TAO_IOP_ComponentId;

enum
{
  // = DESCRIPTION
  //   These are all defined by DCE-CIOP in OMG TC document 95-3-10.
  //   We don't use them in TAO, it is a left over from the SunSoft
  //   IIOP engine times!

  TAO_IOP_TAG_DCE_STRING_BINDING = 100,// string binding handle
  TAO_IOP_TAG_DCE_BINDING_NAME = 101, // CDS/GDS/... name
  TAO_IOP_TAG_DCE_NO_PIPES = 102, // no component data
  TAO_IOP_TAG_OBJECT_KEY = 10,  // opaque
  TAO_IOP_TAG_ENDPOINT_ID = 11, // uuid
  TAO_IOP_TAG_LOCATION_POLICY = 12 // octet/enum
};

/**
 * @class TAO_IOP_TaggedComponent
 *
 * One way to represent multicomponent profiles, e.g. as done by
 * the DCE-CIOP protocol.  One of these gets encapsulated in
 * Tagged_Profile::profile_data.  TAG_MULTIPLE_COMPONENTS may be
 * used to represent protocol profiles structured in that way, but
 * protocol-specific tags facilitate simpler scanning of IORs
 * since you can be assured that each profile only has data used
 * within a single ORB protocol.
 */
class TAO_IOP_TaggedComponent
{
public:

  TAO_IOP_ComponentId tag;
  TAO_opaque component_data;
};

// ****************************************************************

typedef TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
        TAO_IOP_MultipleComponentProfile;

#include "ace/post.h"
#endif /*_TAO_IOP_DEFN_H_*/
