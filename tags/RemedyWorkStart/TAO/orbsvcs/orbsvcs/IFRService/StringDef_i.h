// -*- C++ -*-


//=============================================================================
/**
 *  @file    StringDef_i.h
 *
 *  $Id$
 *
 *  StringDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_STRINGDEF_I_H
#define TAO_STRINGDEF_I_H

#include "orbsvcs/IFRService/IDLType_i.h"
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
 * @class TAO_StringDef_i
 *
 * @brief TAO_StringDef_i
 *
 * Represents a bounded string (unbounded strings
 * are included in PrimitiveDef).
 */
class TAO_IFRService_Export TAO_StringDef_i : public virtual TAO_IDLType_i
{
public:
  /// Constructor.
  TAO_StringDef_i (TAO_Repository_i *repoy);

  /// Destructor.
  virtual ~TAO_StringDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// Remove the repository entry.
  virtual void destroy ();

  virtual void destroy_i ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i ();

  virtual CORBA::ULong bound ();

  CORBA::ULong bound_i ();

  virtual void bound (CORBA::ULong bound);

  void bound_i (CORBA::ULong bound);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_STRINGDEF_I_H */
