// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    EventDef_i.h
//
// = DESCRIPTION
//    EventDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_EVENTDEF_I_H
#define TAO_EVENTDEF_I_H

#include "orbsvcs/IFRService/ExtValueDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_EventDef_i : public virtual TAO_ExtValueDef_i
{
  // = TITLE
  //    TAO_EventDef_i
  //
  // = DESCRIPTION
  //    Represents an event definition. It refers to a ValueDef
  //    object that contains information about the event.
  //
public:
    TAO_EventDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_EventDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
    );
  // Return our definition kind.

  virtual void destroy (
    );
  // Remove the repository entry.

  virtual void destroy_i (
    );

  virtual CORBA::Contained::Description *describe (
    );
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i (
    );
  // From Contained_i's pure virtual function.

  virtual CORBA::Boolean is_a (
      const char *event_id
    );

  CORBA::Boolean is_a_i (
      const char *event_id
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EVENTDEF_I_H */
