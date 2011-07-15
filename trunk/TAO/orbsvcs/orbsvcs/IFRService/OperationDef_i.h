// -*- C++ -*-


//=============================================================================
/**
 *  @file    OperationDef_i.h
 *
 *  $Id$
 *
 *  OperationDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OPERATIONDEF_I_H
#define TAO_OPERATIONDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
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
 * @class TAO_OperationDef_i
 *
 * @brief TAO_OperationDef_i
 *
 * Represents the information needed to describe an operation
 * of an interface.
 */
class TAO_IFRService_Export TAO_OperationDef_i : public virtual TAO_Contained_i
{
public:
  /// Constructor
  TAO_OperationDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_OperationDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe ();

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i ();

  virtual CORBA::TypeCode_ptr result ();

  CORBA::TypeCode_ptr result_i ();

  virtual CORBA::IDLType_ptr result_def ();

  CORBA::IDLType_ptr result_def_i ();

  virtual void result_def (CORBA::IDLType_ptr result_def);

  void result_def_i (CORBA::IDLType_ptr result_def);

  virtual CORBA::ParDescriptionSeq *params ();

  CORBA::ParDescriptionSeq *params_i ();

  virtual void params (const CORBA::ParDescriptionSeq &params);

  void params_i (const CORBA::ParDescriptionSeq &params);

  virtual CORBA::OperationMode mode ();

  CORBA::OperationMode mode_i ();

  virtual void mode (CORBA::OperationMode mode);

  void mode_i (CORBA::OperationMode mode);

  virtual CORBA::ContextIdSeq *contexts ();

  CORBA::ContextIdSeq *contexts_i ();

  virtual void contexts (const CORBA::ContextIdSeq &contexts);

  void contexts_i (const CORBA::ContextIdSeq &contexts);

  virtual CORBA::ExceptionDefSeq *exceptions ();

  CORBA::ExceptionDefSeq *exceptions_i ();

  virtual void exceptions (const CORBA::ExceptionDefSeq &exceptions);

  void exceptions_i (const CORBA::ExceptionDefSeq &exceptions);

  // Common code for containers to call in
  // making descriptions.

  void make_description (CORBA::OperationDescription &desc);

  CORBA::TypeCode_ptr type_i (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_OPERATIONDEF_I_H */
