// -*- C++ -*-
//=============================================================================
/**
 *  @file    Any_Impl.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_IMPL_H
#define TAO_ANY_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/orbconf.h"

class TAO_OutputCDR;
class TAO_InputCDR;
class ACE_Message_Block;
class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  class Object;
  typedef Object *Object_ptr;

  class ValueBase;
  class AbstractBase;
  typedef AbstractBase *AbstractBase_ptr;
}

namespace TAO
{
  /**
   * @class Any_Impl
   *
   * @brief Base class for the Any template subclasses.
   *
   * Contains common functionality and some pure virtual methods.
   */
  class TAO_Export Any_Impl
  {
  public:
    /// Generated data types define a 'destructor' function that
    /// correctly destroys an object stored in an Any.
    typedef void (*_tao_destructor)(void *);

    CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &) = 0;
    virtual const void *value (void) const;

    virtual void free_value (void);

    CORBA::TypeCode_ptr type (void) const;
    CORBA::TypeCode_ptr _tao_get_typecode (void) const;
    void type (CORBA::TypeCode_ptr);

    virtual ACE_Message_Block *_tao_get_cdr (void) const;
    virtual int _tao_byte_order (void) const;

    virtual void _add_ref (void);
    virtual void _remove_ref (void);

    /// Used to release these CORBA basic types.
    static void _tao_any_string_destructor (void *);
    static void _tao_any_wstring_destructor (void *);

    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL);

    virtual void assign_translator (CORBA::TypeCode_ptr,
                                    TAO_InputCDR *
                                    ACE_ENV_ARG_DECL);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

  protected:
    Any_Impl (_tao_destructor,
              CORBA::TypeCode_ptr);
    virtual ~Any_Impl (void);

    TAO::Any_Impl::_tao_destructor value_destructor_;
    CORBA::TypeCode_ptr type_;

  private:
    /// Number of outstanding references to this object.
    CORBA::ULong refcount_;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ANY_IMPL_H */
