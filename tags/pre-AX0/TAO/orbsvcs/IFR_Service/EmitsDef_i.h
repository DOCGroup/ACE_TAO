/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
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

#include "EventDef_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_EmitsDef_i : public virtual TAO_EventDef_i
{
  // = TITLE
  //    TAO_EmitsDef_i
  //
  // = DESCRIPTION
  //    Represents the definition of an event that is emitted
  //    by a component.
  //
public:
    TAO_EmitsDef_i (TAO_Repository_i *repo,
                    ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_EmitsDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA_Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EMITSDEF_I_H */


