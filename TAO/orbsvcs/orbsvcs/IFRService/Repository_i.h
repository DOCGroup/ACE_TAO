/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
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
#include "ifr_service_export.h"

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

class TAO_IDLType_i;
class TAO_Container_i;
class TAO_Contained_i;
class TAO_AbstractInterfaceDef_i;
class TAO_AliasDef_i;
class TAO_ArrayDef_i;
class TAO_AttributeDef_i;
class TAO_ConstantDef_i;
class TAO_EnumDef_i;
class TAO_ExceptionDef_i;
class TAO_FixedDef_i;
class TAO_InterfaceDef_i;
class TAO_LocalInterfaceDef_i;
class TAO_NativeDef_i;
class TAO_OperationDef_i;
class TAO_PrimitiveDef_i;
class TAO_SequenceDef_i;
class TAO_StringDef_i;
class TAO_StructDef_i;
class TAO_UnionDef_i;
class TAO_ValueBoxDef_i;
class TAO_ValueDef_i;
class TAO_ValueMemberDef_i;
class TAO_WstringDef_i;

class TAO_IFRService_Export TAO_Repository_i : public virtual TAO_Container_i
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

  virtual ~TAO_Repository_i (void);

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Accessor for the readonly attribute.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // May not be called on a repository - raises BAD_INV_ORDER.

  virtual CORBA::Contained_ptr lookup_id (
      const char *search_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Contained_ptr lookup_id_i (
      const char *search_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr get_canonical_typecode (
      CORBA::TypeCode_ptr tc
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr get_canonical_typecode_i (
      CORBA::TypeCode_ptr tc
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PrimitiveDef_ptr get_primitive (
      CORBA::PrimitiveKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // No locking necessary because the database is not
  // accessed.

  virtual CORBA::StringDef_ptr create_string (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::StringDef_ptr create_string_i (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::WstringDef_ptr create_wstring (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::WstringDef_ptr create_wstring_i (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::SequenceDef_ptr create_sequence (
      CORBA::ULong bound,
      CORBA::IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC (( CORBA::SystemException));

  CORBA::SequenceDef_ptr create_sequence_i (
      CORBA::ULong bound,
      CORBA::IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC (( CORBA::SystemException));

  virtual CORBA::ArrayDef_ptr create_array (
      CORBA::ULong length,
      CORBA::IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ArrayDef_ptr create_array_i (
      CORBA::ULong length,
      CORBA::IDLType_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::FixedDef_ptr create_fixed (
      CORBA::UShort digits,
      CORBA::Short scale
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::FixedDef_ptr create_fixed_i (
      CORBA::UShort digits,
      CORBA::Short scale
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  int repo_init (CORBA::Repository_ptr repo_ref,
                 PortableServer::POA_ptr repo_poa
                 ACE_ENV_ARG_DECL);
  // Called at startup to get everything initialized.

  virtual int create_servants_and_poas (ACE_ENV_SINGLE_ARG_DECL);
  // We create a default servant servant for each IR Object
  // type and its corresponding POA.

  int create_sections (void);
  // Create the top-level ACE_Configuration sections.

  virtual TAO_IDLType_i *select_idltype (
      CORBA::DefinitionKind def_kind
    ) const;
  virtual TAO_Container_i *select_container (
      CORBA::DefinitionKind def_kind
    ) const;
  virtual TAO_Contained_i *select_contained (
      CORBA::DefinitionKind def_kind
    ) const;
  // Return one of our servants for internal use.

  virtual PortableServer::POA_ptr select_poa (
      CORBA::DefinitionKind def_kind
    ) const;
  // Select the right POA for object creation.

  PortableServer::Current_ptr poa_current (void) const;
  // Accessor for the POA that is dispatching the current call.

  ACE_Configuration *config (void) const;
  // Accessor for the ACE_Configuration database.

  CORBA::TypeCodeFactory_ptr tc_factory (void) const;
  // Accessor for the Typecode factory.

  CORBA::Repository_ptr repo_objref (void) const;
  void repo_objref (CORBA::Repository_ptr objref);
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

  PortableServer::POA_var root_poa_;
  // Reference to the root POA.

  PortableServer::POA_var repo_poa_;
  // Reference to the POA handling calls to this servant.

  PortableServer::Current_var poa_current_;
  // Reference to the PortableServer::Current object

  ACE_Configuration *config_;
  // Our ACE_Configuration database.

  CORBA::TypeCodeFactory_var tc_factory_;
  // Our Typecode factory.

  CORBA::Repository_var repo_objref_;
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

  // Define a POA for each IR Object type, and a corresponding
  // default servant.

#ifdef CONCRETE_IR_OBJECT_TYPES
#undef CONCRETE_IR_OBJECT_TYPES
#endif

#ifdef GEN_IR_OBJECT
#undef GEN_IR_OBJECT
#endif

#define CONCRETE_IR_OBJECT_TYPES \
  GEN_IR_OBJECT (AbstractInterfaceDef) \
  GEN_IR_OBJECT (AliasDef) \
  GEN_IR_OBJECT (ArrayDef) \
  GEN_IR_OBJECT (AttributeDef) \
  GEN_IR_OBJECT (ConstantDef) \
  GEN_IR_OBJECT (EnumDef) \
  GEN_IR_OBJECT (ExceptionDef) \
  GEN_IR_OBJECT (FixedDef) \
  GEN_IR_OBJECT (InterfaceDef) \
  GEN_IR_OBJECT (LocalInterfaceDef) \
  GEN_IR_OBJECT (NativeDef) \
  GEN_IR_OBJECT (OperationDef) \
  GEN_IR_OBJECT (PrimitiveDef) \
  GEN_IR_OBJECT (SequenceDef) \
  GEN_IR_OBJECT (StringDef) \
  GEN_IR_OBJECT (StructDef) \
  GEN_IR_OBJECT (UnionDef) \
  GEN_IR_OBJECT (ValueBoxDef) \
  GEN_IR_OBJECT (ValueDef) \
  GEN_IR_OBJECT (ValueMemberDef) \
  GEN_IR_OBJECT (WstringDef)

#define GEN_IR_OBJECT(name) \
  POA_CORBA:: name ## _tie<TAO_ ## name ## _i> * name ## _servant_; \
  PortableServer::POA_var name ## _poa_;

  CONCRETE_IR_OBJECT_TYPES

#undef GEN_IR_OBJECT

private:
  static const char *TAO_IFR_primitive_kinds[];
  // Set of strings corresponding to the CORBA::PrimitiveKind
  // enum values.

  const char *pkind_to_string (CORBA::PrimitiveKind pkind) const;
  // Convert the enum value to the equivalent string.

  u_int num_pkinds (void) const;
  // Return the number of entries in the CORBA::PrimitiveKind enum.

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_REPOSITORY_I_H */
