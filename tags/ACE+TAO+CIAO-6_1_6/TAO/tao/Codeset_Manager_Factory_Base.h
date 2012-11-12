// -*- C++ -*-

//=============================================================================
/*
 *  @file Codeset_Manager_Factory_Base.h
 *
 *  $Id$
 *
 *  Interface for the TAO CodeSet Manager.
 *
 *  @author  Mahesh Vedantam <mahesh@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CODESET_MANAGER_FACTORY_BASE_H
#define TAO_CODESET_MANAGER_FACTORY_BASE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Codeset_Manager;

/**
 * @class TAO_Codeset_Manager_Factory_Base
 *
 * @brief Base class for creating instances of the codeset manager.
 *
 * The base also provides a default implementation which
 * instantiates nothing. This default impl is replaced by the actual
 * implementation if libTAO_Codeset is loaded.
 *
 */

class TAO_Export TAO_Codeset_Manager_Factory_Base : public ACE_Service_Object
{
public:
  virtual ~TAO_Codeset_Manager_Factory_Base ();

  /// Create makes a new instance of the codeset manager for every
  /// call. This allows multiple ORBs to have their own (or none).
  /// This default implementation returns a null pointer only.
  virtual TAO_Codeset_Manager *create(void);

  /// Is_default is called by the ORB Core to determine if it needs
  /// to reload the factory with a dynamically linked libTAO_Codeset.
  /// Statically linked applications get derive implementation by
  /// including "tao/Codeset/Codeset.h" somewhere in their source code.
  virtual bool is_default (void) const;

  /// Static initializer ensures the factory is loaded
  static int initialize (void);
};

static int
TAO_Requires_Base_Codeset_Initializer =
  TAO_Codeset_Manager_Factory_Base::initialize ();

ACE_STATIC_SVC_DECLARE (TAO_Codeset_Manager_Factory_Base)
ACE_FACTORY_DECLARE (TAO, TAO_Codeset_Manager_Factory_Base)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_MANAGER_FACTORY_BASE_H */
