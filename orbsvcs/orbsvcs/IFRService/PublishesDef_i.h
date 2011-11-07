/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PublishesDef_i.h
 *
 *  $Id$
 *
 *  PublishesDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PUBLISHESDEF_I_H
#define TAO_PUBLISHESDEF_I_H

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
 * Represents the definition of an event that is published
 * by a component.
 */
class TAO_IFRService_Export TAO_PublishesDef_i
  : public virtual TAO_EventPortDef_i
{
public:
  /// Constructor
    TAO_PublishesDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_PublishesDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PUBLISHESDEF_I_H */
