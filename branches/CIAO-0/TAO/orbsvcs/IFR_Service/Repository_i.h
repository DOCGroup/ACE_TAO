/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    Repository_i.h
//
// = DESCRIPTION
//    Repository servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef REPOSITORY_I_H
#define REPOSITORY_I_H

#include "Container_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "IFR_macro.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/CORBA_String.h"

class IFR_Servant_Factory;

class TAO_Repository_i : public virtual TAO_Container_i
{
  // = TITLE
  //    TAO_Repository_i
  //
  // = DESCRIPTION
  //    Provides global access to the Interface Repository, but
  //    does not support access to information related to
  //    CORBA Components.
  //
public:
  TAO_Repository_i (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    ACE_Configuration *config);
  // Constructor.

  virtual ~TAO_Repository_i (void);
  // Destructor.

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Accessor for the readonly attribute.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // May not be called on a repository - raises BAD_INV_ORDER.

  virtual CORBA_Contained_ptr lookup_id (
      const char *search_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_Contained_ptr lookup_id_i (
      const char *search_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr get_canonical_typecode (
      CORBA::TypeCode_ptr tc
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr get_canonical_typecode_i (
      CORBA::TypeCode_ptr tc
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PrimitiveDef_ptr get_primitive (
      CORBA::PrimitiveKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC (( CORBA::SystemException));
  // No locking necessary because the database is not
  // accessed.

  virtual CORBA_StringDef_ptr create_string (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_StringDef_ptr create_string_i (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_WstringDef_ptr create_wstring (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_WstringDef_ptr create_wstring_i (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_SequenceDef_ptr create_sequence (
      CORBA::ULong bound,
      CORBA_IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC (( CORBA::SystemException));

  CORBA_SequenceDef_ptr create_sequence_i (
      CORBA::ULong bound,
      CORBA_IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC (( CORBA::SystemException));

  virtual CORBA_ArrayDef_ptr create_array (
      CORBA::ULong length,
      CORBA_IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ArrayDef_ptr create_array_i (
      CORBA::ULong length,
      CORBA_IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_FixedDef_ptr create_fixed (
      CORBA::UShort digits,
      CORBA::Short scale
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_FixedDef_ptr create_fixed_i (
      CORBA::UShort digits,
      CORBA::Short scale
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr ir_poa (void) const;
  // Accessor for the POA that creates the servants.

  ACE_Configuration *config (void) const;
  // Accessor for the ACE_Configuration database.

  IFR_Servant_Factory *servant_factory (void) const;
  // Accessor for the servant factory.

  CORBA::TypeCodeFactory_ptr tc_factory (void) const;
  // Accessor for the Typecode factory.

  CORBA_Repository_ptr repo_objref (void) const;
  void repo_objref (CORBA_Repository_ptr objref);
  // Accessor/mutator for our object reference.

  ACE_Configuration_Section_Key root_key (void) const;
  // Accessor for the root key for all IR objects.

  ACE_Configuration_Section_Key repo_ids_key (void) const;
  // Accessor for the repository ids root key.

  ACE_Configuration_Section_Key pkinds_key (void) const;
  // Accessor for the primitive kinds section.

  ACE_Configuration_Section_Key strings_key (void) const;
  // Accessor for the bounded strings section.

  ACE_Configuration_Section_Key wstrings_key (void) const;
  // Accessor for the bounded wstrings section.

  ACE_Configuration_Section_Key fixeds_key (void) const;
  // Accessor for the fixed types section.

  ACE_Configuration_Section_Key arrays_key (void) const;
  // Accessor for the anonymous arrays section.

  ACE_Configuration_Section_Key sequences_key (void) const;
  // Accessor for the anonymous sequences section.

  const char *extension (void) const;
  // Accessor for the name extension string.

  ACE_Lock &lock (void) const;
  // Repo lock.

  void shutdown (void);
  // Used ONLY with Purify, for memory leak checking.
  // A call to this can be temporariily appended to the
  // destroy() method of the last thing to be destroyed
  // by the test code.

protected:
  CORBA::ORB_ptr orb_;
  // Reference to our ORB.

  PortableServer::POA_ptr ir_poa_;
  // Reference to the POA that creates the servants.

  ACE_Configuration *config_;
  // Our ACE_Configuration database.

  IFR_Servant_Factory *servant_factory_;
  // Creates _i and _tie instances.

  CORBA::TypeCodeFactory_var tc_factory_;
  // Our Typecode factory.

  CORBA_Repository_ptr repo_objref_;
  // The object reference of this servant.

  ACE_Configuration_Section_Key root_key_;
  // Root of all IR objects.

  ACE_Configuration_Section_Key repo_ids_key_;
  // Flat section of Interface Repository ids.

  ACE_Configuration_Section_Key pkinds_key_;
  // Section holding the primitive kinds.

  ACE_Configuration_Section_Key strings_key_;
  // Section holding the bounded strings.

  ACE_Configuration_Section_Key wstrings_key_;
  // Section holding the bounded wstrings.

  ACE_Configuration_Section_Key fixeds_key_;
  // Section holding the fixed types.

  ACE_Configuration_Section_Key arrays_key_;
  // Section holding the anonymous arrays.

  ACE_Configuration_Section_Key sequences_key_;
  // Section holding the anonymous sequences.

  CORBA::String_var extension_;
  // Added to names temporarily to avoid name clashes.

  ACE_Lock *lock_;
  // Lock.

private:
  static const char *TAO_IFR_primitive_kinds[];
  // Set of strings corresponding to the CORBA_PrimitiveKind
  // enum values.

  const char *pkind_to_string (CORBA::PrimitiveKind pkind) const;
  // Convert the enum value to the equivalent string.

  u_int num_pkinds (void) const;
  // Return the number of entries in the CORBA_PrimitiveKind enum.

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_REPOSITORY_I_H */
