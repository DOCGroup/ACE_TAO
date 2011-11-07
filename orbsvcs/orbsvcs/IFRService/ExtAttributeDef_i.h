// -*- C++ -*-


//=============================================================================
/**
 *  @file    ExtAttributeDef_i.h
 *
 *  $Id$
 *
 *  ExtAttributeDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_EXTATTRIBUTEDEF_I_H
#define TAO_EXTATTRIBUTEDEF_I_H

#include "orbsvcs/IFRService/AttributeDef_i.h"
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
 * Adds IDL3 extensions to AttributeDef.
 */
class TAO_IFRService_Export TAO_ExtAttributeDef_i
  : public virtual TAO_AttributeDef_i
{
public:
  /// Constructor
  TAO_ExtAttributeDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ExtAttributeDef_i (void);

  virtual CORBA::ExcDescriptionSeq *get_exceptions (void);

  CORBA::ExcDescriptionSeq *get_exceptions_i (void);

  virtual void get_exceptions (const CORBA::ExcDescriptionSeq &get_exceptions);

  void get_exceptions_i (const CORBA::ExcDescriptionSeq &get_exceptions);

  virtual CORBA::ExcDescriptionSeq *set_exceptions (void);

  CORBA::ExcDescriptionSeq *set_exceptions_i (void);

  virtual void set_exceptions (
      const CORBA::ExcDescriptionSeq &set_exceptions);

  void set_exceptions_i (
      const CORBA::ExcDescriptionSeq &set_exceptions);

  virtual CORBA::ExtAttributeDescription *describe_attribute (void);

  CORBA::ExtAttributeDescription *describe_attribute_i (void);

  // Utility function that fills sequence elements for a
  // ComponentDescription.
  void fill_description (CORBA::ExtAttributeDescription &desc);

private:
  /// Common code for the set and get varieties.

  void fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                        const char *sub_section);

  void exceptions (const char *sub_section,
                   const CORBA::ExcDescriptionSeq &exceptions);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTATTRIBUTEDEF_I_H */
