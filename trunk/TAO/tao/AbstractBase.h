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

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/Object.h"
#include "tao/ValueBase.h"
#include "tao/ValueFactory.h"

#if defined (TAO_HAS_VALUETYPE)

/**
 * @class CORBA_AbstractBase
 *
 * @brief Abstract base class for Interfaces and Valuetypes
 *
 * Allows the determination of whether an object has been
 * passed by reference or by value to be deferred until runtime.
 */
class TAO_Export CORBA_AbstractBase
{
public:
  /// So the extraction operator can call the protectec constructor.
  friend TAO_Export CORBA::Boolean
         operator<< (TAO_OutputCDR &, const CORBA_AbstractBase_ptr);

  friend TAO_Export CORBA::Boolean
         operator>> (TAO_InputCDR &, CORBA_AbstractBase_ptr &);

  typedef CORBA_AbstractBase_ptr _ptr_type;
  typedef CORBA_AbstractBase_var _var_type;

  static int _tao_class_id;

  static CORBA::AbstractBase_ptr _narrow (CORBA::AbstractBase_ptr obj
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  static CORBA::AbstractBase_ptr _unchecked_narrow (
      CORBA::AbstractBase_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static CORBA::AbstractBase_ptr _duplicate (CORBA::AbstractBase_ptr obj);
  static CORBA::AbstractBase_ptr _nil (void);

  CORBA::Object_ptr _to_object (void);
  CORBA::ValueBase *_to_value (void);

  virtual CORBA::Boolean _is_a (const char *type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void *_tao_QueryInterface (ptr_arith_t type);
  virtual const char* _interface_repository_id (void) const;
  virtual const char* _tao_obv_repository_id (void) const;
  virtual void *_tao_obv_narrow (ptr_arith_t type_id);
  virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &strm);
  virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &strm);

  virtual void _add_ref (void);
  virtual void _remove_ref (void);

  CORBA::Boolean _is_objref (void) const;
  TAO_Stub *_stubobj (void) const;
  CORBA::Boolean _is_collocated (void) const;
  TAO_Abstract_ServantBase *_servant (void) const;
  CORBA::Boolean _is_local (void) const;

protected:
  CORBA_AbstractBase (void);
  CORBA_AbstractBase (const CORBA_AbstractBase &);
  CORBA_AbstractBase (TAO_Stub *p,
                      CORBA::Boolean collocated,
                      TAO_Abstract_ServantBase *servant);

  virtual ~CORBA_AbstractBase (void);

protected:
  CORBA::Boolean is_objref_;

private:
  CORBA_AbstractBase & operator= (const CORBA_AbstractBase &);

  virtual CORBA::ValueBase *_tao_to_value (void);

private:
  TAO_Stub *concrete_stubobj_;
  CORBA::Boolean is_collocated_;
  TAO_Abstract_ServantBase *servant_;
  CORBA::Boolean is_local_;

  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;

  /// Protect reference count manipulation from race conditions.
  /**
   * This lock is only instantiated for unconstrained objects.  The
   * reason for this is that locality-constrained objects that do not
   * require reference counting (the default) may be instantiated in
   * the critical path.
   *
   * @note This assumes that unconstrained objects will not be
   *       instantiated in the critical path.
   */
  TAO_SYNCH_MUTEX * refcount_lock_;
};

/**
 * @class CORBA_AbstractBase_var
 *
 * @brief _var class for AbstractBase
 */
class TAO_Export CORBA_AbstractBase_var
{
public:
  CORBA_AbstractBase_var (void);
  CORBA_AbstractBase_var (CORBA::AbstractBase_ptr);
  CORBA_AbstractBase_var (const CORBA_AbstractBase_var &);
  ~CORBA_AbstractBase_var (void);

  CORBA_AbstractBase_var &operator= (CORBA::AbstractBase_ptr);
  CORBA_AbstractBase_var &operator= (const CORBA_AbstractBase_var &);
  CORBA::AbstractBase_ptr operator-> (void) const;

  /// in, inout, out, _retn
  operator const CORBA::AbstractBase_ptr &() const;
  operator CORBA::AbstractBase_ptr &();
  CORBA::AbstractBase_ptr in (void) const;
  CORBA::AbstractBase_ptr &inout (void);
  CORBA::AbstractBase_ptr &out (void);
  CORBA::AbstractBase_ptr _retn (void);
  CORBA::AbstractBase_ptr ptr (void) const;

  static CORBA::AbstractBase_ptr tao_duplicate (CORBA::AbstractBase_ptr);
  static void tao_release (CORBA::AbstractBase_ptr);
  static CORBA::AbstractBase_ptr tao_nil (void);
  static CORBA::AbstractBase_ptr tao_narrow (CORBA::AbstractBase * 
                                             ACE_ENV_ARG_DECL_NOT_USED);
  static CORBA::AbstractBase * tao_upcast (void *);

private:
  CORBA::AbstractBase_ptr ptr_;
};

/**
 * @class CORBA_AbstractBase_var
 *
 * @brief _out class for AbstractBase
 */
class TAO_Export CORBA_AbstractBase_out
{
public:
  CORBA_AbstractBase_out (CORBA::AbstractBase_ptr &);
  CORBA_AbstractBase_out (CORBA_AbstractBase_var &);
  CORBA_AbstractBase_out (const CORBA_AbstractBase_out &);
  CORBA_AbstractBase_out &operator= (const CORBA_AbstractBase_out &);
  CORBA_AbstractBase_out &operator= (const CORBA_AbstractBase_var &);
  CORBA_AbstractBase_out &operator= (CORBA::AbstractBase_ptr);
  operator CORBA::AbstractBase_ptr &();
  CORBA::AbstractBase_ptr &ptr (void);
  CORBA::AbstractBase_ptr operator-> (void);

private:
  CORBA::AbstractBase_ptr &ptr_;
};

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, const CORBA_AbstractBase_ptr);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA_AbstractBase_ptr &);

#if defined (__ACE_INLINE__)
# include "tao/AbstractBase.inl"
#endif /* __ACE_INLINE__) */

#endif /* TAO_HAS_VALUETYPE */

#include "ace/post.h"

#endif /* TAO_ABSTRACTBASE_H */
