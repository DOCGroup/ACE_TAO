/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    ValueBoxDef_i.h
//
// = DESCRIPTION
//    IR_ValueBoxDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_VALUEBOXDEF_I_H
#define TAO_VALUEBOXDEF_I_H

#include "TypedefDef_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ValueBoxDef_i : public virtual TAO_TypedefDef_i
{
public:
  // = TITLE
  //    TAO_ValueBoxDef_i
  //
  // = DESCRIPTION
  //    Represents an value box definition.
  //
public:
  TAO_ValueBoxDef_i (TAO_Repository_i *repo,
                     ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_ValueBoxDef_i (void);
  // Destructor

  virtual IR_DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

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
  // From IDLType_i's pure virtual function.

  virtual IR_IDLType_ptr original_type_def (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_IDLType_ptr original_type_def_i (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void original_type_def (
      IR_IDLType_ptr original_type_def,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  void original_type_def_i (
      IR_IDLType_ptr original_type_def,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_VALUEBOXDEF_I_H */

