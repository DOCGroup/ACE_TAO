/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    EventPortDef_i.h
//
// = DESCRIPTION
//    EventPortDef_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_EVENTPORTDEF_I_H
#define TAO_EVENTPORTDEF_I_H

#include "Contained_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IFR_ComponentsS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_EventPortDef_i 
  : public virtual TAO_Contained_i
{
  // = TITLE
  //    TAO_EventPortDef_i
  //
  // = DESCRIPTION
  //    Represents an event port definition. EmitsDef_i, PublishesDef_i,
  //    and ConsumesDef_i inherit from this.
  //
public:
  TAO_EventPortDef_i (TAO_Repository_i *repo);
  /// Constructor

  virtual ~TAO_EventPortDef_i (void);
  /// Destructor

  virtual CORBA::ComponentIR::EventDef_ptr event (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  CORBA::ComponentIR::EventDef_ptr event_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void event (
      CORBA::ComponentIR::EventDef_ptr event
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  void event_i (
      CORBA::ComponentIR::EventDef_ptr event
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Boolean is_a (
      const char *event_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  CORBA::Boolean is_a_i (
      const char *event_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This can be the same for the three derived classes, so it is
  /// implemented here.

  virtual CORBA::Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EVENTPORTDEF_I_H */


