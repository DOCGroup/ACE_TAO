// -*- C++ -*-


//=============================================================================
/**
 *  @file    SequenceDef_i.h
 *
 *  $Id$
 *
 *  SequenceDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef SEQUENCEDEF_I_H
#define SEQUENCEDEF_I_H

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
 * @class TAO_SequenceDef_i
 *
 * @brief TAO_SequenceDef_i
 *
 * Represents an IDL sequence type.
 */
class TAO_IFRService_Export TAO_SequenceDef_i : public virtual TAO_IDLType_i
{
public:
  /// Constructor
  TAO_SequenceDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_SequenceDef_i (void);

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

  virtual CORBA::TypeCode_ptr element_type ();

  CORBA::TypeCode_ptr element_type_i ();

  virtual CORBA::IDLType_ptr element_type_def ();

  CORBA::IDLType_ptr element_type_def_i ();

  virtual void element_type_def (CORBA::IDLType_ptr element_type_def);

  void element_type_def_i (CORBA::IDLType_ptr element_type_def);

private:
  /// Destroys an anonymous non-primitive element type.
  void destroy_element_type ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SEQUENCEDEF_I_H */
