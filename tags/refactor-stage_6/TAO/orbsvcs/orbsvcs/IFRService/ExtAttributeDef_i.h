/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ExtAttributeDef_i.h
//
// = DESCRIPTION
//    ExtAttributeDef_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_EXTATTRIBUTEDEF_I_H
#define TAO_EXTATTRIBUTEDEF_I_H

#include "AttributeDef_i.h"
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

class TAO_IFRService_Export TAO_ExtAttributeDef_i
  : public virtual TAO_AttributeDef_i
{
  // = TITLE
  //    TAO_ExtAttributeDef_i
  //
  // = DESCRIPTION
  //    Adds IDL3 extensions to AttributeDef.
  //
public:
  TAO_ExtAttributeDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_ExtAttributeDef_i (void);
  // Destructor

  virtual CORBA::ExcDescriptionSeq *get_exceptions (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ExcDescriptionSeq *get_exceptions_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void get_exceptions (
      const CORBA::ExcDescriptionSeq &get_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  void get_exceptions_i (
      const CORBA::ExcDescriptionSeq &get_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ExcDescriptionSeq *set_exceptions (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ExcDescriptionSeq *set_exceptions_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_exceptions (
      const CORBA::ExcDescriptionSeq &set_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  void set_exceptions_i (
      const CORBA::ExcDescriptionSeq &set_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ExtAttributeDescription *describe_attribute (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ExtAttributeDescription *describe_attribute_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Utility function that fills sequence elements for a
  // ComponentDescription.
  void fill_description (
      CORBA::ExtAttributeDescription &desc
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Common code for the set and get varieties.

  void fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                        const char *sub_section
                        ACE_ENV_ARG_DECL);

  void exceptions (const char *sub_section,
                   const CORBA::ExcDescriptionSeq &exceptions);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTATTRIBUTEDEF_I_H */

