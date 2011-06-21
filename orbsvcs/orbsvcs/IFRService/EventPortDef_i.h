// -*- C++ -*-


//=============================================================================
/**
 *  @file    EventPortDef_i.h
 *
 *  $Id$
 *
 *  EventPortDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_EVENTPORTDEF_I_H
#define TAO_EVENTPORTDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/IFR_ComponentsS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * Represents an event port definition. EmitsDef_i, PublishesDef_i,
 * and ConsumesDef_i inherit from this.
 */
class TAO_IFRService_Export TAO_EventPortDef_i
  : public virtual TAO_Contained_i
{
public:
  //// Constructor
  TAO_EventPortDef_i (TAO_Repository_i *repo);

  //// Destructor
  virtual ~TAO_EventPortDef_i (void);

  virtual CORBA::ComponentIR::EventDef_ptr event (void);

  CORBA::ComponentIR::EventDef_ptr event_i (void);

  virtual void event (CORBA::ComponentIR::EventDef_ptr event);

  void event_i (CORBA::ComponentIR::EventDef_ptr event);

  virtual CORBA::Boolean is_a (const char *event_id);

  CORBA::Boolean is_a_i (const char *event_id);

  /// This can be the same for the three derived classes, so it is
  /// implemented here.

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (void);

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i (void);

private:
  /// Dummy method so we can consolidate derived class methods
  /// in this 'abstract' base class.
  virtual CORBA::DefinitionKind def_kind (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EVENTPORTDEF_I_H */


