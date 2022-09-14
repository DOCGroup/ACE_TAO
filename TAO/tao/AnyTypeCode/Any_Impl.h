// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Impl.h
 *
 *  @author Carlos O'Ryan
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_IMPL_H
#define TAO_ANY_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"
#include "tao/Basic_Types.h"
#include <atomic>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;
class TAO_InputCDR;

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
  class TAO_AnyTypeCode_Export Any_Impl
  {
  public:
    /// Generated data types define a 'destructor' function that
    /// correctly destroys an object stored in an Any.
    typedef void (*_tao_destructor)(void *);

    CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &) = 0;

    virtual void free_value ();

    CORBA::TypeCode_ptr type () const;
    CORBA::TypeCode_ptr _tao_get_typecode () const;
    void type (CORBA::TypeCode_ptr);

    virtual int _tao_byte_order () const;

    virtual void _add_ref ();
    virtual void _remove_ref ();

    /// Used to release these CORBA basic types.
    static void _tao_any_string_destructor (void *);
    static void _tao_any_wstring_destructor (void *);

    virtual void _tao_decode (TAO_InputCDR &);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

    bool encoded () const;

  protected:
    Any_Impl (_tao_destructor,
              CORBA::TypeCode_ptr,
              bool encoded = false);
    virtual ~Any_Impl ();

    TAO::Any_Impl::_tao_destructor value_destructor_;
    CORBA::TypeCode_ptr type_;
    bool encoded_;

  private:
    /// Reference counter.
    std::atomic<uint32_t> refcount_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_IMPL_H */
