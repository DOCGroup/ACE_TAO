/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    IDLType_i.h
//
// = DESCRIPTION
//    IDLType servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IDLTYPE_I_H
#define TAO_IDLTYPE_I_H

#include "IRObject_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Repository_i;

class TAO_IDLType_i : public virtual TAO_IRObject_i
{
  // = TITLE
  //    TAO_IDLType_i
  //
  // = DESCRIPTION
  //    Abstract base class for all IR objects that represent
  //    OMG IDL types. Provides access to the TypeCode describing
  //    the type.
  //
public:
  TAO_IDLType_i (TAO_Repository_i *repo,
                 ACE_Configuration_Section_Key section_key);
  // Constructor.

  virtual ~TAO_IDLType_i (void);
  // Destructor.

  virtual CORBA::TypeCode_ptr type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Defined in concrete classes.

  virtual CORBA::TypeCode_ptr type_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Defined in concrete classes.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IDLTYPE_I_H */

