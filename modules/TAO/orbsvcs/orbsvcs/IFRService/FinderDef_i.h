// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    FinderDef_i.h
//
// = DESCRIPTION
//    FinderDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FINDERDEF_I_H
#define TAO_FINDERDEF_I_H

#include "orbsvcs/IFRService/OperationDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_FinderDef_i : public virtual TAO_OperationDef_i
{
  // = TITLE
  //    TAO_FinderDef_i
  //
  // = DESCRIPTION
  //    Represents the definition of a finder operation in a home.
  //
public:
  TAO_FinderDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_FinderDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_FINDERDEF_I_H */
