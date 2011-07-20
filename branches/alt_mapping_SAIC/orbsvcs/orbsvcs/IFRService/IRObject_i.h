// -*- C++ -*-


//=============================================================================
/**
 *  @file    IRObject_i.h
 *
 *  $Id$
 *
 *  IRObject servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IROBJECT_I_H
#define TAO_IROBJECT_I_H

#include "ace/Configuration.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IFR_Client/IFR_ExtendedS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Repository_i;
class TAO_IDLType_i;
class TAO_Contained_i;
class TAO_Container_i;

/**
 * @class TAO_IRObject_i
 *
 * @brief TAO_IRObject_i
 *
 * Abstract base class for all IR object types.
 */
class TAO_IFRService_Export TAO_IRObject_i : public POA_CORBA::IRObject
{
public:
  /// Constructor.
  TAO_IRObject_i (TAO_Repository_i *repo);

  /// Destructor.
  virtual ~TAO_IRObject_i (void);

  virtual CORBA::DefinitionKind def_kind (
    );

  /// Pure virtual.
  virtual void destroy (
    )
    = 0;

  /// Pure virtual - the unlocked version called by the destroy() wrapper.
  virtual void destroy_i (
    )
    = 0;

  /// Plug in the section key.
  void section_key (ACE_Configuration_Section_Key &key);

protected:
  /// Set our key from the object ID via POACurrent.
  void update_key (void);

protected:
  /// Pointer to the repository we were constructed from.
  TAO_Repository_i *repo_;

  /// Our ACE_Configuration section key.
  ACE_Configuration_Section_Key section_key_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IROBJECT_I_H */
