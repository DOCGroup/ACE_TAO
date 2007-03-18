// -*- C++ -*-


//=============================================================================
/**
 *  @file    FinderDef_i.h
 *
 *  $Id$
 *
 *  FinderDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class TAO_FinderDef_i
 *
 * @brief TAO_FinderDef_i
 *
 * Represents the definition of a finder operation in a home.
 */
class TAO_IFRService_Export TAO_FinderDef_i : public virtual TAO_OperationDef_i
{
public:
  /// Constructor
  TAO_FinderDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_FinderDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_FINDERDEF_I_H */
