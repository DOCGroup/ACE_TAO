// -*- C++ -*-

//=============================================================================
/**
 *  @file     AbstractBase.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons  <parsons@cs.wust.edu>
 */
//=============================================================================


#ifndef TAO_ABSTRACTBASE_H
#define TAO_ABSTRACTBASE_H

#include /**/ "ace/pre.h"

#include "valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Pseudo_VarOut_T.h"
#include "ace/CORBA_macros.h"
#include "ace/OS.h"
#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Objref_VarOut_T.h"

class TAO_Stub;
class TAO_Abstract_ServantBase;

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Valuetype_Export

namespace CORBA
{
  class ValueBase;

  class AbstractBase;
  typedef AbstractBase *AbstractBase_ptr;

  extern TAO_Valuetype_Export Boolean is_nil (AbstractBase_ptr);
  extern TAO_Valuetype_Export void release (AbstractBase_ptr);

  typedef TAO_Pseudo_Var_T<AbstractBase> AbstractBase_var;
  typedef TAO_Pseudo_Out_T<AbstractBase, AbstractBase_var> AbstractBase_out;

  /**
   * @class AbstractBase
   *
   * @brief Abstract base class for Interfaces and Valuetypes
   *
   * Allows the determination of whether an object has been passed by
   * reference or by value to be deferred until runtime.
   */
  class TAO_Valuetype_Export AbstractBase
  {
  public:

    /// Constructor.
    /**
     * This constructor is only meant to be called by the
     * corresponding CDR stream extraction operator.
     */
    AbstractBase (TAO_Stub *p,
                  CORBA::Boolean collocated,
                  TAO_Abstract_ServantBase *servant);

    typedef CORBA::AbstractBase_ptr _ptr_type;
    typedef CORBA::AbstractBase_var _var_type;

    static int _tao_class_id;

    static CORBA::AbstractBase_ptr _narrow (CORBA::AbstractBase_ptr obj
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    static CORBA::AbstractBase_ptr _unchecked_narrow (
      CORBA::AbstractBase_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

    static CORBA::AbstractBase_ptr _duplicate (CORBA::AbstractBase_ptr obj);
    static CORBA::AbstractBase_ptr _nil (void);

    /// Used in the implementation of CORBA::Any
    static void _tao_any_destructor (void*);

    /// Spec required conversion operations
    CORBA::Object_ptr _to_object (void);
    CORBA::ValueBase *_to_value (void);

    virtual CORBA::Boolean _is_a (const char *type_id
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    virtual const char* _interface_repository_id (void) const;

    /// TAO specific operation
    virtual void *_tao_QueryInterface (ptrdiff_t type);
    virtual const char* _tao_obv_repository_id (void) const;
    virtual void *_tao_obv_narrow (ptrdiff_t type_id);
    virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &strm);
    virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &strm);

    /// Memmory management operations
    virtual void _add_ref (void);
    virtual void _remove_ref (void);

    CORBA::Boolean _is_objref (void) const;

    /// Return the stub object
    TAO_Stub *_stubobj (void) const;

    /// Acessors
    CORBA::Boolean _is_collocated (void) const;
    TAO_Abstract_ServantBase *_servant (void) const;
    CORBA::Boolean _is_local (void) const;

    /// Return the equivalent object reference.
    /**
     * The object is not refcounted. The caler should not put this in
     * a var or some such thing. The memory is owned by <this>
     * object.
     */
    CORBA::Object_ptr equivalent_objref (void);

  protected:

    AbstractBase (void);
    AbstractBase (const AbstractBase &);

    virtual ~AbstractBase (void);

  protected:

    CORBA::Boolean is_objref_;


  private:

    AbstractBase & operator= (const AbstractBase &);

    virtual CORBA::ValueBase *_tao_to_value (void);

  private:

    TAO_Stub *concrete_stubobj_;
    CORBA::Boolean is_collocated_;
    TAO_Abstract_ServantBase *servant_;
    CORBA::Boolean is_local_;

    /// Our equivalent CORBA::Object version
    /// @@todo: We may at some point of time should probably cache a
    /// version of  CORBA::ValueBase
    CORBA::Object_var equivalent_obj_;
  };
}

TAO_Valuetype_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, const CORBA::AbstractBase_ptr);

TAO_Valuetype_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA::AbstractBase_ptr &);

/// Used in generated code if CORBA::AbstractBase is an argument or return type.
namespace TAO
{
  template<>
  class TAO_Valuetype_Export Arg_Traits<CORBA::AbstractBase>
    : public Object_Arg_Traits_T<CORBA::AbstractBase_ptr,
                                 CORBA::AbstractBase_var,
                                 CORBA::AbstractBase_out,
                                 TAO::Objref_Traits<CORBA::AbstractBase> >
  {
  };
};

#if defined (__ACE_INLINE__)
# include "AbstractBase.inl"
#endif /* __ACE_INLINE__) */

#include /**/ "ace/post.h"

#endif /* TAO_ABSTRACTBASE_H */
