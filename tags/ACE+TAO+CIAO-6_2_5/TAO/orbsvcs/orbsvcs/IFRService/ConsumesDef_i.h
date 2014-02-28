// -*- C++ -*-


//=============================================================================
/**
 *  @file    ConsumesDef_i.h
 *
 *  $Id$
 *
 *  ConsumesDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONSUMESDEF_I_H
#define TAO_CONSUMESDEF_I_H

#include "orbsvcs/IFRService/EventPortDef_i.h"
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
 * Represents the definition of an event that is consumed
 * by a component.
 */
class TAO_IFRService_Export TAO_ConsumesDef_i
  : public virtual TAO_EventPortDef_i
{
public:
  /// Constructor
    TAO_ConsumesDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ConsumesDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONSUMESDEF_I_H */
