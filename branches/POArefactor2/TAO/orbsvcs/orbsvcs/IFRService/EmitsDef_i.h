/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    EmitsDef_i.h
//
// = DESCRIPTION
//    EmitsDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_EMITSDEF_I_H
#define TAO_EMITSDEF_I_H

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

class TAO_IFRService_Export TAO_EmitsDef_i 
  : public virtual TAO_EventPortDef_i
{
  // = TITLE
  //    TAO_EmitsDef_i
  //
  // = DESCRIPTION
  //    Represents the definition of an event that is emitted
  //    by a component.
  //
public:
    TAO_EmitsDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_EmitsDef_i (void);
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

#endif /* TAO_EMITSDEF_I_H */
