/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
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

#include "ace/Configuration.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IFR_ExtendedS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Repository_i;
class TAO_IDLType_i;
class TAO_Contained_i;
class TAO_Container_i;

class TAO_IFRService_Export TAO_IRObject_i : public POA_CORBA::IRObject
{
  // = TITLE
  //    TAO_IRObject_i
  //
  // = DESCRIPTION
  //    Abstract base class for all IR object types.
  //
public:
  TAO_IRObject_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_IRObject_i (void);
  // Destructor.

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual - the unlocked version called by the destroy() wrapper.

  void section_key (ACE_Configuration_Section_Key &key);
  // Plug in the section key.

protected:
  void update_key (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set our key from the object ID via POACurrent.

protected:
  TAO_Repository_i *repo_;
  // Pointer to the repository we were constructed from.

  ACE_Configuration_Section_Key section_key_;
  // Our ACE_Configuration section key.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IROBJECT_I_H */
