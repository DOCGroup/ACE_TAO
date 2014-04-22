// -*- C++ -*-


//=============================================================================
/**
 *  @file    FactoryDef_i.h
 *
 *  $Id$
 *
 *  FactoryDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FACTORYDEF_I_H
#define TAO_FACTORYDEF_I_H

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
 * @class TAO_FactoryDef_i
 *
 * @brief TAO_FactoryDef_i
 *
 * Represents the definition of a factory operation in a home.
 */
class TAO_IFRService_Export TAO_FactoryDef_i : public virtual TAO_OperationDef_i
{
public:
  /// Constructor
  TAO_FactoryDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_FactoryDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_FACTORYDEF_I_H */
