// -*- C++ -*-

//=============================================================================
/**
 *  @file    ArrayDef_i.h
 *
 *  ArrayDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ARRAYDEF_I_H
#define TAO_ARRAYDEF_I_H

#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ArrayDef_i
 *
 * @brief TAO_ArrayDef_i
 *
 * Represents an IDL array type.
 */
class TAO_IFRService_Export TAO_ArrayDef_i : public virtual TAO_IDLType_i
{
public:
  /// Constructor
  TAO_ArrayDef_i (TAO_Repository_i *repo);

  /// Destructor
  ~TAO_ArrayDef_i (void) override;

  /// Return our definition kind.
  CORBA::DefinitionKind def_kind () override;

  /// Remove the repository entry.
  void destroy () override;

  void destroy_i () override;

  CORBA::TypeCode_ptr type () override;

  CORBA::TypeCode_ptr type_i () override;

  virtual CORBA::ULong length ();

  CORBA::ULong length_i ();

  virtual void length (CORBA::ULong length);

  void length_i (CORBA::ULong length);

  virtual CORBA::TypeCode_ptr element_type ();

  CORBA::TypeCode_ptr element_type_i ();

  virtual CORBA::IDLType_ptr element_type_def ();

  CORBA::IDLType_ptr element_type_def_i ();

  virtual void element_type_def (CORBA::IDLType_ptr element_type_def);

  void element_type_def_i (CORBA::IDLType_ptr element_type_def);

private:
  /// Destroys an anonymous non-primitive element type.
  void destroy_element_type ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ARRAYDEF_I_H */
