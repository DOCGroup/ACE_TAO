/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    PrimitiveDef_i.h
//
// = DESCRIPTION
//    IR::PrimitiveDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PRIMITIVEDEF_I_H
#define TAO_PRIMITIVEDEF_I_H

#include "IDLType_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_PrimitiveDef_i : public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_PrimitiveDef_i
  //
  // = DESCRIPTION
  //    Represents any of the OMG IDL primitive types
  //
public:
  TAO_PrimitiveDef_i (TAO_Repository_i *repo,
                      ACE_Configuration_Section_Key section_key);
  // Constructor.

  virtual ~TAO_PrimitiveDef_i (void);
  // Destructor.

  virtual IR::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // May not be called on a primitive kind - raises BAD_INV_ORDER.

  virtual void destroy_i (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Must implement this here because it's pure virtual in the
  // base class.

  virtual CORBA::TypeCode_ptr type (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::PrimitiveKind kind (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::PrimitiveKind kind_i (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PRIMITIVEDEF_I_H */


