// -*- C++ -*-


//=============================================================================
/**
 *  @file    Repository_i.h
 *
 *  $Id$
 *
 *  Repository servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef REPOSITORY_I_H
#define REPOSITORY_I_H

#include "orbsvcs/IFRService/Container_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "orbsvcs/IFRService/IFR_macro.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/CORBA_String.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class IFR_Servant_Factory;

class TAO_IDLType_i;
class TAO_Container_i;
class TAO_Contained_i;
class TAO_ExtAbstractInterfaceDef_i;
class TAO_AliasDef_i;
class TAO_ArrayDef_i;
class TAO_AttributeDef_i;
class TAO_ConstantDef_i;
class TAO_EnumDef_i;
class TAO_ExceptionDef_i;
class TAO_FixedDef_i;
class TAO_ExtInterfaceDef_i;
class TAO_ExtLocalInterfaceDef_i;
class TAO_NativeDef_i;
class TAO_OperationDef_i;
class TAO_PrimitiveDef_i;
class TAO_SequenceDef_i;
class TAO_StringDef_i;
class TAO_StructDef_i;
class TAO_UnionDef_i;
class TAO_ValueBoxDef_i;
class TAO_ExtValueDef_i;
class TAO_ValueMemberDef_i;
class TAO_WstringDef_i;

/**
 * @class TAO_Repository_i
 *
 * @brief TAO_Repository_i
 *
 * Provides global access to the Interface Repository, but
 * does not support access to information related to
 * CORBA Components.
 */
class TAO_IFRService_Export TAO_Repository_i : public virtual TAO_Container_i
{
public:
  TAO_Repository_i (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    ACE_Configuration *config);

  virtual ~TAO_Repository_i (void);

  /// Accessor for the readonly attribute.
  virtual CORBA::DefinitionKind def_kind ();

  /// May not be called on a repository - raises BAD_INV_ORDER.
  virtual void destroy ();

  virtual CORBA::Contained_ptr lookup_id (const char *search_id);

  CORBA::Contained_ptr lookup_id_i (const char *search_id);

  virtual CORBA::TypeCode_ptr get_canonical_typecode (CORBA::TypeCode_ptr tc);

  CORBA::TypeCode_ptr get_canonical_typecode_i (CORBA::TypeCode_ptr tc);

  /// No locking necessary because the database is not
  /// accessed.
  virtual CORBA::PrimitiveDef_ptr get_primitive (CORBA::PrimitiveKind kind);

  virtual CORBA::StringDef_ptr create_string (CORBA::ULong bound);

  CORBA::StringDef_ptr create_string_i (CORBA::ULong bound);

  virtual CORBA::WstringDef_ptr create_wstring (CORBA::ULong bound);

  CORBA::WstringDef_ptr create_wstring_i (CORBA::ULong bound);

  virtual CORBA::SequenceDef_ptr create_sequence (
      CORBA::ULong bound,
      CORBA::IDLType_ptr element_type);

  CORBA::SequenceDef_ptr create_sequence_i (
      CORBA::ULong bound,
      CORBA::IDLType_ptr element_type);

  virtual CORBA::ArrayDef_ptr create_array (
      CORBA::ULong length,
      CORBA::IDLType_ptr element_type);

  CORBA::ArrayDef_ptr create_array_i (
      CORBA::ULong length,
      CORBA::IDLType_ptr element_type);

  virtual CORBA::FixedDef_ptr create_fixed (
      CORBA::UShort digits,
      CORBA::Short scale);

  CORBA::FixedDef_ptr create_fixed_i (
      CORBA::UShort digits,
      CORBA::Short scale);

  /// Called at startup to get everything initialized.
  int repo_init (CORBA::Repository_ptr repo_ref,
                 PortableServer::POA_ptr repo_poa);

  /// We create a default servant servant for each IR Object
  /// type and its corresponding POA.
  virtual int create_servants_and_poas (void);

  /// Create the top-level ACE_Configuration sections.
  int create_sections (void);

  /// Return one of our servants for internal use.
  virtual TAO_IDLType_i *select_idltype (
      CORBA::DefinitionKind def_kind) const;
  virtual TAO_Container_i *select_container (
      CORBA::DefinitionKind def_kind) const;
  virtual TAO_Contained_i *select_contained (
      CORBA::DefinitionKind def_kind) const;

  /// Select the right POA for object creation.
  virtual PortableServer::POA_ptr select_poa (
      CORBA::DefinitionKind def_kind) const;

  /// Accessor for the POA that is dispatching the current call.
  PortableServer::Current_ptr poa_current (void) const;

  /// Accessor for the ACE_Configuration database.
  ACE_Configuration *config (void) const;

  /// Accessor for the Typecode factory.
  CORBA::TypeCodeFactory_ptr tc_factory (void) const;

  /// Accessor/mutator for our object reference.
  CORBA::Repository_ptr repo_objref (void) const;
  void repo_objref (CORBA::Repository_ptr objref);

  /// Accessor for the root key for all IR objects.
  ACE_Configuration_Section_Key root_key (void) const;

  /// Accessor for the repository ids root key.
  ACE_Configuration_Section_Key repo_ids_key (void) const;

  /// Accessor for the primitive kinds section.
  ACE_Configuration_Section_Key pkinds_key (void) const;

  /// Accessor for the bounded strings section.
  ACE_Configuration_Section_Key strings_key (void) const;

  /// Accessor for the bounded wstrings section.
  ACE_Configuration_Section_Key wstrings_key (void) const;

  /// Accessor for the fixed types section.
  ACE_Configuration_Section_Key fixeds_key (void) const;

  /// Accessor for the anonymous arrays section.
  ACE_Configuration_Section_Key arrays_key (void) const;

  /// Accessor for the anonymous sequences section.
  ACE_Configuration_Section_Key sequences_key (void) const;

  /// Accessor for the name extension string.
  const char *extension (void) const;

  /// Repo lock.
  ACE_Lock &lock (void) const;

  /**
   * Used ONLY with Purify, for memory leak checking.
   * A call to this can be temporariily appended to the
   * destroy() method of the last thing to be destroyed
   * by the test code.
   */
  void shutdown (void);

protected:
  /// Reference to our ORB.
  CORBA::ORB_ptr orb_;

  /// Reference to the root POA.
  PortableServer::POA_var root_poa_;

  /// Reference to the POA handling calls to this servant.
  PortableServer::POA_var repo_poa_;

  /// Reference to the PortableServer::Current object
  PortableServer::Current_var poa_current_;

  /// Our ACE_Configuration database.
  ACE_Configuration *config_;

  /// Our Typecode factory.
  CORBA::TypeCodeFactory_var tc_factory_;

  /// The object reference of this servant.
  CORBA::Repository_var repo_objref_;

  /// Root of all IR objects.
  ACE_Configuration_Section_Key root_key_;

  /// Flat section of Interface Repository ids.
  ACE_Configuration_Section_Key repo_ids_key_;

  /// Section holding the primitive kinds.
  ACE_Configuration_Section_Key pkinds_key_;

  /// Section holding the bounded strings.
  ACE_Configuration_Section_Key strings_key_;

  /// Section holding the bounded wstrings.
  ACE_Configuration_Section_Key wstrings_key_;

  /// Section holding the fixed types.
  ACE_Configuration_Section_Key fixeds_key_;

  /// Section holding the anonymous arrays.
  ACE_Configuration_Section_Key arrays_key_;

  /// Section holding the anonymous sequences.
  ACE_Configuration_Section_Key sequences_key_;

  /// Added to names temporarily to avoid name clashes.
  CORBA::String_var extension_;

  /// Lock.
  ACE_Lock *lock_;

  // Define a POA for each IR Object type, and a corresponding
  // default servant.

#ifdef CONCRETE_IR_OBJECT_TYPES
#undef CONCRETE_IR_OBJECT_TYPES
#endif

#ifdef GEN_IR_OBJECT
#undef GEN_IR_OBJECT
#endif

#define CONCRETE_IR_OBJECT_TYPES \
  GEN_IR_OBJECT (ExtAbstractInterfaceDef) \
  GEN_IR_OBJECT (AliasDef) \
  GEN_IR_OBJECT (ArrayDef) \
  GEN_IR_OBJECT (AttributeDef) \
  GEN_IR_OBJECT (ConstantDef) \
  GEN_IR_OBJECT (EnumDef) \
  GEN_IR_OBJECT (ExceptionDef) \
  GEN_IR_OBJECT (FixedDef) \
  GEN_IR_OBJECT (ExtInterfaceDef) \
  GEN_IR_OBJECT (ExtLocalInterfaceDef) \
  GEN_IR_OBJECT (NativeDef) \
  GEN_IR_OBJECT (OperationDef) \
  GEN_IR_OBJECT (PrimitiveDef) \
  GEN_IR_OBJECT (SequenceDef) \
  GEN_IR_OBJECT (StringDef) \
  GEN_IR_OBJECT (StructDef) \
  GEN_IR_OBJECT (UnionDef) \
  GEN_IR_OBJECT (ValueBoxDef) \
  GEN_IR_OBJECT (ExtValueDef) \
  GEN_IR_OBJECT (ValueMemberDef) \
  GEN_IR_OBJECT (WstringDef)

#define GEN_IR_OBJECT(name) \
  POA_CORBA:: name ## _tie<TAO_ ## name ## _i> * name ## _servant_; \
  PortableServer::POA_var name ## _poa_;

  CONCRETE_IR_OBJECT_TYPES

#undef GEN_IR_OBJECT

private:
  /// Set of strings corresponding to the CORBA::PrimitiveKind
  /// enum values.
  static const char * TAO_IFR_primitive_kinds[];

  /// Convert the enum value to the equivalent string.
  const char *pkind_to_string (CORBA::PrimitiveKind pkind) const;

  /// Return the number of entries in the CORBA::PrimitiveKind enum.
  u_int num_pkinds (void) const;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_REPOSITORY_I_H */
