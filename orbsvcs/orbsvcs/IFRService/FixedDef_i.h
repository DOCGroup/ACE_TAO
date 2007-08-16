// -*- C++ -*-


//=============================================================================
/**
 *  @file    FixedDef_i.h
 *
 *  $Id$
 *
 *  FixedDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FIXEDDEF_I_H
#define TAO_FIXEDDEF_I_H

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
 * @class TAO_FixedDef_i
 *
 * @brief TAO_FixedDef_i
 *
 * Represents an IDL fixed type.
 */
class TAO_IFRService_Export TAO_FixedDef_i : public virtual TAO_IDLType_i
{
public:
  /// Constructor
  TAO_FixedDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_FixedDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );

  /// Remove the repository entry.
  virtual void destroy (
    );

  virtual void destroy_i (
    );

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type (
    );

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i (
    );

  virtual CORBA::UShort digits (
    );

  CORBA::UShort digits_i (
    );

  virtual void digits (
      CORBA::UShort digits
    );

  void digits_i (
      CORBA::UShort digits
    );

  virtual CORBA::Short scale (
    );

  CORBA::Short scale_i (
    );

  virtual void scale (
      CORBA::Short scale
    );

  void scale_i (
      CORBA::Short scale
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_FIXEDDEF_I_H */
