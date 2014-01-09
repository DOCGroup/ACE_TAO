// -*- C++ -*-


//=============================================================================
/**
 *  @file    PrimitiveDef_i.h
 *
 *  $Id$
 *
 *  PrimitiveDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PRIMITIVEDEF_I_H
#define TAO_PRIMITIVEDEF_I_H

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
 * @class TAO_PrimitiveDef_i
 *
 * @brief TAO_PrimitiveDef_i
 *
 * Represents any of the OMG IDL primitive types
 */
class TAO_IFRService_Export TAO_PrimitiveDef_i : public virtual TAO_IDLType_i
{
public:
  /// Constructor.
  TAO_PrimitiveDef_i (TAO_Repository_i *repo);

  /// Destructor.
  virtual ~TAO_PrimitiveDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// May not be called on a primitive kind - raises BAD_INV_ORDER.
  virtual void destroy ();

  /// Must implement this here because it's pure virtual in the
  /// base class.
  virtual void destroy_i ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type ();

  virtual CORBA::TypeCode_ptr type_i ();

  virtual CORBA::PrimitiveKind kind ();

  CORBA::PrimitiveKind kind_i ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PRIMITIVEDEF_I_H */
