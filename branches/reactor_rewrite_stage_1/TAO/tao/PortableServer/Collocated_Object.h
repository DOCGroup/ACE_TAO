// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Collocated_Object.h
//
// = DESCRIPTION
//
// ============================================================================

#ifndef TAO_COLLOCATED_OBJECT_H
#define TAO_COLLOCATED_OBJECT_H
#include "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Object.h"
#include "Servant_Base.h"


class TAO_PortableServer_Export TAO_Collocated_Object : public virtual CORBA_Object
{
public:
  TAO_Collocated_Object (TAO_Stub *p = 0,
                         CORBA::Boolean collocated = 0,
                         TAO_ServantBase *servant = 0);
  // constructor

  virtual ~TAO_Collocated_Object (void);
  // destructor
/*
  virtual TAO_ServantBase *_servant (void) const;
  // return the associated servant (if one exists)
*/

  static TAO_Collocated_Object *_narrow (CORBA::Object_ptr object
                                         TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // Downcast operation

  // = The CORBA_Object methods, check the tao/Object.h file for details
  virtual CORBA::Boolean _is_a (const CORBA::Char *logical_type_id
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual CORBA::Boolean _is_equivalent (CORBA_Object_ptr other_obj
                                         TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC (());
#if (TAO_HAS_MINIMUM_CORBA == 0)
  virtual CORBA::Boolean _non_existent (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
#endif /* TAO_HAS_MINIMUM_CORBA */
  virtual void *_tao_QueryInterface(ptr_arith_t type);

private:
  // = Unimplemented methods
  TAO_Collocated_Object (const TAO_Collocated_Object &);
  TAO_Collocated_Object &operator = (const TAO_Collocated_Object &);

  /*

private:
  TAO_ServantBase *servant_;
  // Servant pointer.  It is 0 except for collocated objects.

  */
};

#if defined (__ACE_INLINE__)
# include "Collocated_Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_COLLOCATED_OBJECT_H */
