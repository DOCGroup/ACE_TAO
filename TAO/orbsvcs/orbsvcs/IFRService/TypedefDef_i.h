// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    TypedefDef_i.h
//
// = DESCRIPTION
//    TypedefDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_TYPEDEFDEF_I_H
#define TAO_TYPEDEFDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
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

class TAO_IFRService_Export TAO_TypedefDef_i : public virtual TAO_Contained_i,
                                               public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_TypedefDef_i
  //
  // = DESCRIPTION
  //    Base interface inherited by all named non-object types:
  //    struct, union, enum, alias, native, and valuebox.
  //
public:
  TAO_TypedefDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_TypedefDef_i (void);
  // Destructor

  virtual CORBA::Contained::Description *describe ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_TYPEDEFDEF_I_H */
