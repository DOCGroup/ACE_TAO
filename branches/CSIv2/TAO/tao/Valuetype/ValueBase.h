// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "valuetype_export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "Value_VarOut_T.h"

#include "ace/Basic_Types.h"
#include "ace/Synch_T.h"
#include "ace/CORBA_macros.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Valuetype_Export

namespace CORBA
{
  class ValueBase;
  struct tao_ValueBase_life;

  extern TAO_Valuetype_Export void add_ref (ValueBase *);
  extern TAO_Valuetype_Export void remove_ref (ValueBase *);

  typedef TAO_Value_Var_T<ValueBase, tao_ValueBase_life> ValueBase_var;
  typedef TAO_Value_Out_T<ValueBase, tao_ValueBase_life> ValueBase_out;

  struct TAO_Valuetype_Export tao_ValueBase_life
  {
    static void tao_add_ref (ValueBase *);
    static void tao_remove_ref (ValueBase *);
  };

  /**
   * @class ValueBase
   *
   * @brief Abstract baseclass for Valuetypes
   *
   * @see CORBA 2.3 - Section 20.17.5
   */
  class TAO_Valuetype_Export ValueBase
  {
  public:
    typedef ValueBase* _ptr_type;
    typedef ValueBase_var _var_type;

    // reference counting
    /// %! virtual CORBA::ValueBase* _copy_value (void) = 0;
    virtual void _add_ref (void) = 0;
    virtual void _remove_ref (void) = 0;
    virtual CORBA::ULong _refcount_value (void) = 0;

    // dynamic casting
    static CORBA::ValueBase* _downcast (CORBA::ValueBase *);

    /// Used in the implementation of CORBA::Any
    static void _tao_any_destructor (void *);

    /// TAO extension
    virtual const char* _tao_obv_repository_id (void) const = 0;

    // TAO internal --------------------------

    /// Marshal a valuetype (see operator<< in tao_idl generated file
    /// how it is called)
    static CORBA::Boolean _tao_marshal (TAO_OutputCDR &strm,
                                        ValueBase *_this,
                                        ptrdiff_t formal_type_id = 0);

    /// Unmarshal a valuetype, if formal type is a pointer to
    /// ValueBase
    static CORBA::Boolean _tao_unmarshal (TAO_InputCDR &strm,
                                          ValueBase *&new_object);

    // static CORBA::Boolean
    // T::_tao_unmarshal (TAO_InputCDR &, ValueBase *&_this)
    // is typespecific for valuetype T and generated from tao_idl
    // Use this for unmarshaling.

    /// Both used internally and are called from T::_tao_unmarshal ()
    static CORBA::Boolean _tao_unmarshal_pre (TAO_InputCDR &strm,
                                              ValueFactory &,
                                              ValueBase *&,
                                              const char * const repo_id);
    CORBA::Boolean _tao_unmarshal_post (TAO_InputCDR &strm);

  public:  // otherwise these cannot be called from a static function

    virtual void *_tao_obv_narrow (ptrdiff_t) = 0;

    /// during marshal jump to the most derived part
    virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) = 0;

    /// called after obtaining the fresh object from create_for_unmarshal ()
    virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) = 0;

  protected:
    ValueBase (void);
    ValueBase (const ValueBase&);
    virtual ~ValueBase (void);

  private:
    ValueBase & operator= (const ValueBase &);

#ifdef SUN_CC_HAS_PVFC_BUG
    // Need ugly fix for sun cc "pure virtual function called" bug.
  private:
    unsigned long tao_sun_cc_pvfc_bug_fix_;

#endif /* SUN_CC_HAS_PVFC_BUG */

  }; // ValueBase

  /**
   * @class DefaultValueRefCountBase
   *
   * @brief Default mix-in for reference count of a valuetype.
   *
   * Default mix-in for reference count of a valuetype.
   */
  class TAO_Valuetype_Export DefaultValueRefCountBase
    : public virtual ValueBase
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
    DefaultValueRefCountBase (void);
    DefaultValueRefCountBase (const DefaultValueRefCountBase&);
    virtual ~DefaultValueRefCountBase ();

  private:
    void operator= (const DefaultValueRefCountBase &);

  private: // data
    CORBA::ULong _tao_reference_count_;
    TAO_SYNCH_MUTEX _tao_reference_count_lock_;
  }; // DefaultValueRefCountBase

  //  which lock has the lowest memory overhead ?
  // %! todo refcountbase w/o locking (now memory overhead)
  // $! todo: debug aids for refcounts

}  // End CORBA namespace

/**
 * @class TAO_OBV_GIOP_Flags
 *
 * @brief TAO_OBV_GIOP_Flags
 *
 * @see CORBA 2.3 -- Section 15.3.4
 */
class TAO_Valuetype_Export TAO_OBV_GIOP_Flags
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

TAO_Valuetype_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const CORBA::ValueBase *);

TAO_Valuetype_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA::ValueBase *&);

#if defined (__ACE_INLINE__)
# include "ValueBase.inl"
#endif /* __ACE_INLINE__*/

#include /**/ "ace/post.h"

#endif /* TAO_VALUEBASE_H */
