/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    PublishesDef_i.h
//
// = DESCRIPTION
//    PublishesDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PUBLISHESDEF_I_H
#define TAO_PUBLISHESDEF_I_H

#include "EventPortDef_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_PublishesDef_i 
  : public virtual TAO_EventPortDef_i
{
  // = TITLE
  //    TAO_PublishesDef_i
  //
  // = DESCRIPTION
  //    Represents the definition of an event that is published
  //    by a component.
  //
public:
    TAO_PublishesDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_PublishesDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PUBLISHESDEF_I_H */
