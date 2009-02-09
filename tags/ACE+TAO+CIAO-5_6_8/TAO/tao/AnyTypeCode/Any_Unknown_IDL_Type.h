// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Unknown_IDL_Type.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_UNKNOWN_IDL_TYPE_H
#define TAO_ANY_UNKNOWN_IDL_TYPE_H

#include /**/ "ace/pre.h"

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Lock_Adapter_T.h"

#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Unknown_IDL_Type
   *
   * @brief CDR-based Any impl class.
   *
   * Used when demarshaling an Any, and with DII/DSI, and Dynamic Anys.
   */
  class TAO_AnyTypeCode_Export Unknown_IDL_Type : public Any_Impl
  {
  public:
    Unknown_IDL_Type (CORBA::TypeCode_ptr,
                      TAO_InputCDR &cdr);

    Unknown_IDL_Type (CORBA::TypeCode_ptr);

    virtual ~Unknown_IDL_Type (void);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    virtual const void *value (void) const;
    virtual void free_value (void);

    virtual TAO_InputCDR &_tao_get_cdr (void);
    virtual int _tao_byte_order (void) const;

    virtual void _tao_decode (TAO_InputCDR &
                             );

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

  private:
    // We make the lock global, so that it won't be deleted when shared.
    // For instance, see Any_Basic_Impl::extract() which copies the insides
    // from an Unknown_IDL_Type to an Any_Basic_Impl.

    // [Iliyan] However, having a global static makes this a subject
    // to the whim of the specific compiler implentation. It is the
    // one deciding the order in which our instance is initialized and
    // destroyed and that is boundto be a problem
    // somewhere. Typically, it becomes a problem when a code that
    // depends on that instance finds that the runtime has already
    // destroyed it. The scenario plays almost always in the process
    // shutdown code, after main() exits, having to debug which is a
    // lot of fun :) ...  Bottom line, use a static function, which
    // encapsulates a local static initializer, guaranteed to be
    // initialized at the first invocation. Using ACE_Refcounted_Auto_Ptr
    // ensures that the ref count will not drop to zero until the library
    // is unloaded *and* all Unknown_IDL_Type instances have been destroyed.
    typedef ACE_Refcounted_Auto_Ptr<ACE_Lock,
                                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >
      LOCK;
    static LOCK lock_i (void);
    LOCK const lock_;
    mutable TAO_InputCDR cdr_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_UNKNOWN_IDL_TYPE_H */
