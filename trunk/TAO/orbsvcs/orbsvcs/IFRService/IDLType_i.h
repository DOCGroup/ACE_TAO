// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
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

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Repository_i;

class TAO_IFRService_Export TAO_IDLType_i : public virtual TAO_IRObject_i
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
  TAO_IDLType_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_IDLType_i (void);
  // Destructor.

  virtual CORBA::TypeCode_ptr type ()

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Defined in concrete classes.

  virtual CORBA::TypeCode_ptr type_i ()

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Defined in concrete classes.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IDLTYPE_I_H */
