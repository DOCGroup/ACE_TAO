// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ValueBase.h
//
// = AUTHOR
//     Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef TAO_VALUEBASE_H
#define TAO_VALUEBASE_H

#include "ace/OS.h"           /* for ptr_arith_t */
#include "tao/corbafwd.h"

#ifdef TAO_HAS_VALUETYPE


class TAO_Export CORBA_ValueBase
{
  // = TITLE
  //   Abstract baseclass for Valuetypes
  //   (see CORBA 2.3 20.17.5)

public:
  // reference counting
  virtual void _add_ref (void) = 0;
  virtual void _remove_ref (void) = 0;
  // %! virtual CORBA::ValueBase* _copy_value (void) = 0;
  virtual CORBA::ULong _refcount_value (void) = 0;

  // dynamic casting
  static CORBA::ValueBase* _downcast (CORBA::ValueBase*);

  virtual const char* _tao_obv_repository_id () const = 0;
  // TAO extension

  // TAO internal --------------------------

  static CORBA::Boolean _tao_marshal (TAO_OutputCDR &strm,
                                      CORBA_ValueBase *_this,
                               ptr_arith_t formal_type_id = 0);
  // Marshal a valuetype (see operator<< in tao_idl generated file
  // how it is called)

  static CORBA::Boolean _tao_unmarshal (TAO_InputCDR &strm,
                                        CORBA_ValueBase *&_this);
  // Unmarshal a valuetype, if formal type is a pointer to ValueBase

  // static CORBA::Boolean
  // T::_tao_unmarshal (TAO_InputCDR &, CORBA_ValueBase *&_this)
  // is typespecific for valuetype T and generated from tao_idl
  // Use this for unmarshaling.

  static CORBA::Boolean _tao_unmarshal_pre (TAO_InputCDR &strm,
                                            CORBA_ValueFactory_ptr &,
                                            CORBA_ValueBase *&,
                                            const char * const repo_id);
  CORBA::Boolean _tao_unmarshal_post (TAO_InputCDR &strm);
  // Both used internally and are called from T::_tao_unmarshal ()


public:  // otherwise these cannot be called from a static function
  virtual void *_tao_obv_narrow (ptr_arith_t) = 0;

  virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) = 0;
  // during marshal jump to the most derived part

  virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) = 0;
  // called after obtaining the fresh object from create_for_unmarshal ()

protected:
  CORBA_ValueBase (void);
  CORBA_ValueBase (const CORBA_ValueBase&);
  virtual ~CORBA_ValueBase ();

private:
  void operator= (const CORBA_ValueBase &);
}; // CORBA_ValueBase




class TAO_Export CORBA_DefaultValueRefCountBase : public virtual CORBA_ValueBase
{
  // = TITLE
  //   Default mix-in for reference count of a valuetype.

public:
  virtual void _add_ref (void);
  virtual void _remove_ref (void);
  virtual CORBA::ULong _refcount_value (void);

  void _tao_add_ref (void);
  void _tao_remove_ref (void);
  CORBA::ULong _tao_refcount_value (void);
  // The _tao variants are inline for fast access from T_var
  // (if valuetype T is compiled with optimization for that.) %! (todo)

protected:
  CORBA_DefaultValueRefCountBase (void);
  CORBA_DefaultValueRefCountBase (const CORBA_DefaultValueRefCountBase&);
  virtual ~CORBA_DefaultValueRefCountBase ();

private:
  void operator= (const CORBA_DefaultValueRefCountBase &);

private: // data
  CORBA::ULong _tao_reference_count_;
  ACE_SYNCH_MUTEX _tao_reference_count_lock_;
}; // CORBA_DefaultValueRefCountBase

// %! which lock has the lowest memory overhead ?
// %! todo refcountbase w/o locking (now memory overhead)
// $! todo: debug aids for refcounts


class TAO_OBV_GIOP_Flags
{
  // CORBA 2.3: 15.3.4
public:
  enum Tag                                   // test:
  {
    Null_ref              =  0x00000000L,    // equal
    Value_tag_base        =  0x7fffff00L,
    Value_tag_sigbits     =  0xffffff00L,
      Codebase_url        =  0x00000001L,    // and
      Type_info_no        =  0x00000000L,
      Type_info_single    =  0x00000002L,
      Type_info_list      =  0x00000006L,
      Type_info_sigbits   =  0x00000006L,
      Chunked_encoding    =  0x00000008L,    // and
    Indirection_tag       =  0xffffffffL,    // equal
    Block_size_tag_min    =  1,              // range
    Block_size_tag_max    =  0x7fffff00L -1,
    // @@ %! but how encode a stateless block ?

    // This constants define the ranges for end tags and
    // indirections.
    End_tag_min           = 0x80000000L - 1, // range
    End_tag_max           = 0xFFFFFFFFL,
    Indirection_min       = 0x80000000L - 1, // range
    Indirection_max       = 0xFFFFFFFFL - 4
  };
  // all flags without a comment are complete tested against
  // the analog _sigbits mask.



public:
  static CORBA::Boolean is_null_ref  (CORBA::ULong);
  static CORBA::Boolean is_value_tag (CORBA::ULong);
  static CORBA::Boolean has_codebase_url (CORBA::ULong);
  static CORBA::Boolean has_no_type_info (CORBA::ULong);
  static CORBA::Boolean has_single_type_info (CORBA::ULong);
  static CORBA::Boolean has_list_type_info (CORBA::ULong);
  static CORBA::Boolean is_chunked (CORBA::ULong);
  static CORBA::Boolean is_indirection_tag  (CORBA::ULong);
  static CORBA::Boolean block_size (CORBA::ULong, CORBA::ULong &size);
  static CORBA::Boolean end_tag_depth (CORBA::ULong, CORBA::ULong &d);
  static CORBA::Boolean indirection (CORBA::ULong, CORBA::Long &jump);
  //  static CORBA::Boolean (CORBA::ULong);
  //  static CORBA::Boolean (CORBA::ULong);

};

#if defined (__ACE_INLINE__)
# include "tao/ValueBase.i"
#endif /* __ACE_INLINE__) */

#endif /* TAO_HAS_VALUETYPE */
#endif /* TAO_VALUEBASE_H */
