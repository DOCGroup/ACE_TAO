/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    OperationDef_i.h
//
// = DESCRIPTION
//    OperationDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_OPERATIONDEF_I_H
#define TAO_OPERATIONDEF_I_H

#include "Contained_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_OperationDef_i : public virtual TAO_Contained_i
{
  // = TITLE
  //    TAO_OperationDef_i
  //
  // = DESCRIPTION
  //    Represents the information needed to describe an operation
  //    of an interface.
  //
public:
  TAO_OperationDef_i (TAO_Repository_i *repo,
                      ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_OperationDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA_Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA_Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr result (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr result_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_IDLType_ptr result_def (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_IDLType_ptr result_def_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void result_def (
      CORBA_IDLType_ptr result_def
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void result_def_i (
      CORBA_IDLType_ptr result_def
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ParDescriptionSeq *params (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ParDescriptionSeq *params_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void params (
      const CORBA_ParDescriptionSeq &params
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void params_i (
      const CORBA_ParDescriptionSeq &params
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::OperationMode mode (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::OperationMode mode_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void mode (
      CORBA::OperationMode mode
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void mode_i (
      CORBA::OperationMode mode
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ContextIdSeq *contexts (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ContextIdSeq *contexts_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void contexts (
      const CORBA_ContextIdSeq &contexts
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void contexts_i (
      const CORBA_ContextIdSeq &contexts
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ExceptionDefSeq *exceptions (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ExceptionDefSeq *exceptions_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void exceptions (
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void exceptions_i (
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_OperationDescription make_description (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Common code for InterfaceDef to call in
  // making its own description.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_OPERATIONDEF_I_H */



