/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ComponentRepository_i.h
//
// = DESCRIPTION
//    ComponentRepository servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbiltl.edu>
//
// ============================================================================

#ifndef TAO_COMPONENTREPOSITORY_I_H
#define TAO_COMPONENTREPOSITORY_I_H

#include "Repository_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ComponentContainer_i.h"

class TAO_ModuleDef_i;
class TAO_ComponentDef_i;
class TAO_HomeDef_i;
class TAO_FinderDef_i;
class TAO_FactoryDef_i;
class TAO_EventDef_i;
class TAO_EmitsDef_i;
class TAO_PublishesDef_i;
class TAO_ConsumesDef_i;
class TAO_ProvidesDef_i;
class TAO_UsesDef_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_ComponentRepository_i 
  : public virtual TAO_Repository_i,
    public virtual TAO_ComponentContainer_i
{
  // = TITLE
  //    TAO_ComponentRepository_i
  //
  // = DESCRIPTION
  //    Provides global access to the Interface Repository,
  //    including access to information related to
  //    CORBA Components.
  //
public:
  TAO_ComponentRepository_i (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             ACE_Configuration *config);
  // Constructor.

  virtual ~TAO_ComponentRepository_i (void);
  // Destructor.

  virtual int create_servants_and_poas (ACE_ENV_SINGLE_ARG_DECL);
  // We create a default servant servant for each IR Object
  // type and its corresponding POA.

  virtual TAO_IDLType_i *select_idltype (
      CORBA::DefinitionKind def_kind
    ) const;
  virtual TAO_Container_i *select_container (
      CORBA::DefinitionKind def_kind
    ) const;
  virtual TAO_Contained_i *select_contained (
      CORBA::DefinitionKind def_kind
    ) const;
  // Return one of our servants for internal use.

  virtual PortableServer::POA_ptr select_poa (
      CORBA::DefinitionKind def_kind
    ) const;
  // Select the right POA for object creation.

protected:

#ifdef CONCRETE_IR_OBJECT_TYPES
#undef CONCRETE_IR_OBJECT_TYPES
#endif

#define CONCRETE_IR_OBJECT_TYPES \
  GEN_IR_OBJECT (ModuleDef) \
  GEN_IR_OBJECT (ComponentDef) \
  GEN_IR_OBJECT (HomeDef) \
  GEN_IR_OBJECT (FinderDef) \
  GEN_IR_OBJECT (FactoryDef) \
  GEN_IR_OBJECT (EventDef) \
  GEN_IR_OBJECT (EmitsDef) \
  GEN_IR_OBJECT (PublishesDef) \
  GEN_IR_OBJECT (ConsumesDef) \
  GEN_IR_OBJECT (ProvidesDef) \
  GEN_IR_OBJECT (UsesDef)

#define GEN_IR_OBJECT(name) \
  POA_CORBA::ComponentIR:: name ## _tie<TAO_ ## name ## _i> * name ## _servant_; \
  PortableServer::POA_var name ## _poa_;

  CONCRETE_IR_OBJECT_TYPES

#undef GEN_IR_OBJECT
  // Servants for each IR Object type, created at startup.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_COMPONENTREPOSITORY_I_H */
