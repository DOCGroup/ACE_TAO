// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     ValueBase.h
 *
 *  $Id$
 *
 *  @author  Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef TAO_VALUEBASE_H
#define TAO_VALUEBASE_H
#include "ace/pre.h"

#include "ace/OS.h"           /* for ptr_arith_t */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

#ifdef TAO_HAS_VALUETYPE


/**
 * @class CORBA_ValueBase
 *
 * @brief Abstract baseclass for Valuetypes
 * (see CORBA 2.3 20.17.5)
 */
class TAO_Export CORBA_ValueBase
{

public:
  // reference counting
  /// %! virtual CORBA::ValueBase* _copy_value (void) = 0;
  virtual void _add_ref (void) = 0;
  virtual void _remove_ref (void) = 0;
  virtual CORBA::ULong _refcount_value (void) = 0;

  // dynamic casting
  static CORBA::ValueBase* _downcast (CORBA::ValueBase*);

  /// TAO extension
  virtual const char* _tao_obv_repository_id () const = 0;

  // TAO internal --------------------------

  /// Marshal a valuetype (see operator<< in tao_idl generated file
  /// how it is called)
  static CORBA::Boolean _tao_marshal (TAO_OutputCDR &strm,
                                      CORBA_ValueBase *_this,
                               ptr_arith_t formal_type_id = 0);

  /// Unmarshal a valuetype, if formal type is a pointer to ValueBase
  static CORBA::Boolean _tao_unmarshal (TAO_InputCDR &strm,
                                        CORBA_ValueBase *&_this);

  // static CORBA::Boolean
  // T::_tao_unmarshal (TAO_InputCDR &, CORBA_ValueBase *&_this)
  // is typespecific for valuetype T and generated from tao_idl
  // Use this for unmarshaling.

  /// Both used internally and are called from T::_tao_unmarshal ()
  static CORBA::Boolean _tao_unmarshal_pre (TAO_InputCDR &strm,
                                            CORBA_ValueFactory_ptr &,
                                            CORBA_ValueBase *&,
                                            const char * const repo_id);
  CORBA::Boolean _tao_unmarshal_post (TAO_InputCDR &strm);


public:  // otherwise these cannot be called from a static function
  virtual void *_tao_obv_narrow (ptr_arith_t) = 0;

  /// during marshal jump to the most derived part
  virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) = 0;

  /// called after obtaining the fresh object from create_for_unmarshal ()
  virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) = 0;

protected:
  CORBA_ValueBase (void);
  CORBA_ValueBase (const CORBA_ValueBase&);
  virtual ~CORBA_ValueBase ();

private:
  void operator= (const CORBA_ValueBase &);

#ifdef SUN_CC_HAS_PVFC_BUG
  // Need ugly fix for sun cc "pure virtual function called" bug.
private:
  unsigned long tao_sun_cc_pvfc_bug_fix_;

#endif /* SUN_CC_HAS_PVFC_BUG */

}; // CORBA_ValueBase




/**
 * @class CORBA_DefaultValueRefCountBase
 *
 * @brief Default mix-in for reference count of a valuetype.
 */
class TAO_Export CORBA_DefaultValueRefCountBase : public virtual CORBA_ValueBase
{

public:
  virtual void _add_ref (void);
  virtual void _remove_ref (void);
  virtual CORBA::ULong _refcount_value (void);

  /// The _tao variants are inline for fast access from T_var
  /// (if valuetype T is compiled with optimization for that.) %! (todo)
  void _tao_add_ref (void);
  void _tao_remove_ref (void);
  CORBA::ULong _tao_refcount_value (void);

protected:
  CORBA_DefaultValueRefCountBase (void);
  CORBA_DefaultValueRefCountBase (const CORBA_DefaultValueRefCountBase&);
  virtual ~CORBA_DefaultValueRefCountBase ();

private:
  void operator= (const CORBA_DefaultValueRefCountBase &);

private: // data
  CORBA::ULong _tao_reference_count_;
  TAO_SYNCH_MUTEX _tao_reference_count_lock_;
}; // CORBA_DefaultValueRefCountBase

// %! which lock has the lowest memory overhead ?
// %! todo refcountbase w/o locking (now memory overhead)
// $! todo: debug aids for refcounts


/**
 * @class TAO_OBV_GIOP_Flags
 CORBA 2.3: 15.3.4
 */
class TAO_OBV_GIOP_Flags
{
public:
  static const CORBA::ULong Value_tag_base;
  static const CORBA::ULong Value_tag_sigbits;
  static const CORBA::ULong Codebase_url;
  static const CORBA::ULong Type_info_sigbits;
  static const CORBA::ULong Type_info_none;
  static const CORBA::ULong Type_info_single;
  static const CORBA::ULong Type_info_list;

  static CORBA::Boolean is_null_ref           (CORBA::ULong);
  static CORBA::Boolean is_value_tag          (CORBA::ULong);
  static CORBA::Boolean has_codebase_url      (CORBA::ULong);
  static CORBA::Boolean has_no_type_info      (CORBA::ULong);
  static CORBA::Boolean has_single_type_info  (CORBA::ULong);
  static CORBA::Boolean has_list_type_info    (CORBA::ULong);
  static CORBA::Boolean is_chunked            (CORBA::ULong);
  static CORBA::Boolean is_indirection_tag    (CORBA::ULong);
  static CORBA::Boolean is_indirection        (CORBA::ULong);
  static CORBA::Boolean is_block_size         (CORBA::ULong);
  static CORBA::Boolean is_end_tag            (CORBA::ULong);
};

#if defined (__ACE_INLINE__)
# include "tao/ValueBase.i"
#endif /* __ACE_INLINE__) */

#endif /* TAO_HAS_VALUETYPE */
#include "ace/post.h"
#endif /* TAO_VALUEBASE_H */
