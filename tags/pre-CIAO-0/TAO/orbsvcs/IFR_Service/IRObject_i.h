/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    IRObject_i.h
//
// = DESCRIPTION
//    IRObject servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IROBJECT_I_H
#define TAO_IROBJECT_I_H

#include "IFR_ExtendedS.h"
#include "ace/Configuration.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Repository_i;

class TAO_IRObject_i : public POA_CORBA_IRObject
{
  // = TITLE
  //    TAO_IRObject_i
  //
  // = DESCRIPTION
  //    Abstract base class for all IR object types.
  //
public:
  TAO_IRObject_i (TAO_Repository_i *repo,
                  ACE_Configuration_Section_Key section_key);
  // Constructor.

  virtual ~TAO_IRObject_i (void);
  // Destructor.

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual- the unlocked version called by destroy() wrapper.

protected:
  char *int_to_string (CORBA::ULong number) const;
  // Convert an unsigned int to a string of its hex form.

  TAO_Repository_i *repo_;
  // Pointer to the repository we were constructed from.

  ACE_Configuration_Section_Key section_key_;
  // Our ACE_Configuration section key.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IROBJECT_I_H */
