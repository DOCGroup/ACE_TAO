// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocated_Object.h
 *
 *  $Id$
 *
 *  @brief
 *
 */
//=============================================================================


#ifndef TAO_COLLOCATED_OBJECT_H
#define TAO_COLLOCATED_OBJECT_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"

#include "tao/Object.h"


/**
 * @class TAO_Collocated_Object
 *
 * @brief TAO_Collocated_Object
 *
 * TAO_Collocated_Object
 */
class TAO_PortableServer_Export TAO_Collocated_Object
  : public virtual CORBA::Object
{
public:

  /// Constructor.
  TAO_Collocated_Object (TAO_Stub *p = 0,
                         CORBA::Boolean collocated = 0,
                         TAO_ServantBase *servant = 0);

  /// Destructor.
  virtual ~TAO_Collocated_Object (void);

/*
  /// return the associated servant (if one exists)
  virtual TAO_ServantBase *_servant (void) const;
*/

  /// Downcast operation
  static TAO_Collocated_Object *_narrow (CORBA::Object_ptr object
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  // = The CORBA::Object methods, check the tao/Object.h file for details
  virtual CORBA::Boolean _is_a (const CORBA::Char *logical_type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual CORBA::Boolean _is_equivalent (CORBA::Object_ptr other_obj
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC (());
#if (TAO_HAS_MINIMUM_CORBA == 0)
  virtual CORBA::Boolean _non_existent (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  virtual CORBA::Object_ptr _get_component
    (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual void *_tao_QueryInterface (ptrdiff_t type);

private:
  // = Unimplemented methods
  TAO_Collocated_Object (const TAO_Collocated_Object &);
  TAO_Collocated_Object &operator = (const TAO_Collocated_Object &);

  /*

private:
  /// Servant pointer.  It is 0 except for collocated objects.
  TAO_ServantBase *servant_;

  */
};


#if defined (__ACE_INLINE__)
# include "Collocated_Object.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_COLLOCATED_OBJECT_H */
