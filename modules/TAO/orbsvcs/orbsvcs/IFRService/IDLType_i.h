// -*- C++ -*-

//=============================================================================
/**
 *  @file    IDLType_i.h
 *
 *  $Id$
 *
 *  IDLType servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IDLTYPE_I_H
#define TAO_IDLTYPE_I_H

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Repository_i;

/**
 * @class TAO_IDLType_i
 *
 * @brief TAO_IDLType_i
 *
 * Abstract base class for all IR objects that represent
 * OMG IDL types. Provides access to the TypeCode describing
 * the type.
 */
class TAO_IFRService_Export TAO_IDLType_i : public virtual TAO_IRObject_i
{
public:
  /// Constructor.
  TAO_IDLType_i (TAO_Repository_i *repo);

  /// Destructor.
  virtual ~TAO_IDLType_i (void);
  /// Defined in concrete classes.
  virtual CORBA::TypeCode_ptr type ()

    = 0;
  /// Defined in concrete classes.
  virtual CORBA::TypeCode_ptr type_i ()

    = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_IDLTYPE_I_H */
