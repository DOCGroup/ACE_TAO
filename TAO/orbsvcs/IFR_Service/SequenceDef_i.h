/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    SequenceDef_i.h
//
// = DESCRIPTION
//    SequenceDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef SEQUENCEDEF_I_H
#define SEQUENCEDEF_I_H

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

class TAO_SequenceDef_i : public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_SequenceDef_i
  //
  // = DESCRIPTION
  //    Represents an IDL sequence type.
  //
public:
  TAO_SequenceDef_i (TAO_Repository_i *repo,
                     ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_SequenceDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::ULong bound (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong bound_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void bound (
      CORBA::ULong bound
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void bound_i (
      CORBA::ULong bound
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr element_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr element_type_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_IDLType_ptr element_type_def (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_IDLType_ptr element_type_def_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void element_type_def (
      CORBA_IDLType_ptr element_type_def
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void element_type_def_i (
      CORBA_IDLType_ptr element_type_def
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  void destroy_element_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroys an anonymous non-primitive element type.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SEQUENCEDEF_I_H */


