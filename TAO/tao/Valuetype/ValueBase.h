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

#include "tao/Valuetype/valuetype_export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Valuetype/Value_CORBA_methods.h"
#include "tao/Valuetype/Value_VarOut_T.h"

#include "tao/Object_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

#include "ace/Basic_Types.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"
#include "ace/Null_Mutex.h"
#include "ace/Vector_T.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Valuetype_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#define DEFAULT_INDIRECTION_MAP_SIZE 10
#define VERIFY_MAP(CDR, MAPNAME, MAPCLASSNAME)                                               \
  if (strm.get_##MAPNAME ().is_nil ())                                                       \
    {                                                                                        \
      CDR::MAPCLASSNAME##_Handle handle (                                                    \
          new CDR::RC_##MAPCLASSNAME (new CDR::MAPCLASSNAME (DEFAULT_INDIRECTION_MAP_SIZE)));\
      strm.set_##MAPNAME (handle);                                                           \
    } else do {} while (0)

///////////////

#undef TAO_local_COPY_VALUE_IS_PURE
#if defined(TAO_VALUETYPE_COPYING_ANY_INSERTION_USES_COPY_VALUE) || !defined(TAO_VALUETYPE_COPY_VALUE_IS_NOT_PURE)
#define TAO_local_COPY_VALUE_IS_PURE =0
#else
#define TAO_local_COPY_VALUE_IS_PURE
#endif

class TAO_Valuetype_Export TAO_ChunkInfo
{
public:
  TAO_ChunkInfo (CORBA::Boolean do_chunking = false,
                 CORBA::Long init_level = false);

  /// Methods to support chunking.
  /// Note: These methods are called for both chunking and non-chunking
  ///       valuetype. These methods checks the chunking_ flag. If it's
  ///       set to be false the methods return true rightaway.

  /// Methods for marshalling a valuetype.

  /// This is called in the _tao_marshal_state (). This method reserves
  /// space for the chunk size of the next chunk and also increments the
  /// nesting level. The reservasion actually occurs the first time that
  /// the start_chunk is called if there are multiple continuous start_chunk()
  /// calls without the close_chunk() called in between.
  CORBA::Boolean start_chunk (TAO_OutputCDR &strm);

  /// This is called in the _tao_marshal_state (). This method writes the
  /// actual chunk size to the reserved chunk size space and writes an end
  /// tag with the negation value of current nesting level. A start_chunk()
  /// needs an end_chunk() to close the current chunk. It's also needed for
  /// writing the outmost endtag to the stream.
  CORBA::Boolean end_chunk (TAO_OutputCDR &strm);

  /// Methods for unmarshalling a valuetype.

  /// This is called in the _tao_unmarshal_state () to read the chunk
  /// size or an end tag.
  CORBA::Boolean handle_chunking (TAO_InputCDR &strm);
  /// This is called in the _tao_unmarshal_state () to skip the rest
  /// chunks until the outmost endtag (-1) if the value is truncated
  /// to its truncatable parent.
  CORBA::Boolean skip_chunks (TAO_InputCDR &strm);
  /// This is called in end_chunk(). It writes the actual chunk size to the
  /// reserved chunk size space.
  CORBA::Boolean write_previous_chunk_size (TAO_OutputCDR &strm);
  /// Reserve space for chunk size. The memory in the stream will be
  /// overwritten after all the chunk data is written. This method
  /// only allows the reservasion being made once if the reserved
  /// space has not been overwritten.
  CORBA::Boolean reserve_chunk_size (TAO_OutputCDR &strm);

  /// A flag to indicate that this instance is actually involved in a chunked
  /// or truncatable valuetype.
  CORBA::Boolean chunking_;

  /// The level of nesting valuetypes.
  CORBA::Long value_nesting_level_;
  /// The starting position of the size of current chunk.
  char* chunk_size_pos_;
  /// The length of CDR stream from the begining to the current
  /// chunk data starting position.  Used to calculate the chunk size
  /// across multiple chained ACE_Message_Blocks.
  size_t length_to_chunk_octets_pos_;
  /// The end position of current chunk.
  char* chunk_octets_end_pos_;
};

namespace CORBA
{
  class ValueFactoryBase;
  typedef ValueFactoryBase *ValueFactory;

  class ValueBase;

  typedef TAO_Value_Var_T<ValueBase> ValueBase_var;
  typedef TAO_Value_Out_T<ValueBase> ValueBase_out;

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
    friend class TAO_ChunkInfo;

    typedef ValueBase* _ptr_type;
    typedef ValueBase_var _var_type;
    typedef ValueBase_out _out_type;

    typedef ACE_Vector < ACE_CString > Repository_Id_List;

    virtual CORBA::ValueBase* _copy_value (void) TAO_local_COPY_VALUE_IS_PURE;

    // Reference counting.
    virtual void _add_ref (void) = 0;
    virtual void _remove_ref (void) = 0;
    virtual CORBA::ULong _refcount_value (void) = 0;

    // dynamic casting
    static CORBA::ValueBase* _downcast (CORBA::ValueBase *);

    /// TAO extension

    /// Used in the implementation of CORBA::Any
    static void _tao_any_destructor (void *);
    virtual CORBA::TypeCode_ptr _tao_type (void) const = 0;

    /// Return the repository id of this valuetype.
    virtual const char * _tao_obv_repository_id (void) const = 0;

    /// Give the list of the RepositoryIds in the valuetype "truncatable"
    /// inheritance hierarchy. List the id of this valuetype as first
    /// RepositoryID and go up the "truncatable" derivation hierarchy.
    /// Note the truncatable repo ids only list the truncatable base types
    /// to which this type is safe to truncate, not all its parents.
    virtual void _tao_obv_truncatable_repo_ids (Repository_Id_List &) const = 0;

    // TAO internal --------------------------

    /// Marshal a valuetype (see operator<< in tao_idl generated file
    /// how it is called)
    static CORBA::Boolean _tao_marshal (TAO_OutputCDR &strm,
                                        const ValueBase *_this,
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
                                              CORBA::ValueBase *&valuetype,
                                              const char *const repo_id,
                                              CORBA::Boolean &is_null_object,
                                              CORBA::Boolean &is_indirected);

    static CORBA::Boolean _tao_unmarshal_header (
      TAO_InputCDR &strm,
      const char *const fallback_repo_id,
      Repository_Id_List &ids,
      CORBA::Boolean &is_null_object,
      CORBA::Boolean &is_indirected,
      CORBA::Boolean &is_chunked);

    CORBA::Boolean _tao_unmarshal_post (TAO_InputCDR &strm);

    /// Check repository id for value box type against what is
    /// in the CDR stream.
    static CORBA::Boolean _tao_validate_box_type (
                                          TAO_InputCDR &strm,
                                          TAO_InputCDR &indirected_strm,
                                          const char * const repo_id_expected,
                                          CORBA::Boolean & null_object,
                                          CORBA::Boolean & is_indirected);
#if defined (GEN_OSTREAM_OPS)
    /// Used by optionally generated ostream operators for valuetypes
    /// to output the state of the actual type for debugging.
    static std::ostream& _tao_stream (std::ostream &strm, const ValueBase *value);
    virtual std::ostream& _tao_stream_v (std::ostream &strm) const;
#endif /* GEN_OSTREAM_OPS */

  public:  // otherwise these cannot be called from a static function
    /// During marshal jump to the most derived part
    virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) const = 0;

    /// Called after obtaining the fresh object from create_for_unmarshal ()
    virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) = 0;

    /// Notify the truncated parent valuetype to skip the rest of the chunks
    /// when unmarshalling a value from its derived valuetype.
    /// This is called when the factory for the most derived valuetype (in
    /// the repository id list) does not exist and a truncated parent factory
    /// is registered.
    virtual void truncation_hook ();

  protected:
    ValueBase (void);
    ValueBase (const ValueBase&);
    virtual ~ValueBase (void);

    /// This flag is set to be true when the valuetype defined
    /// in the idl has the truncatable parent.
    CORBA::Boolean         is_truncatable_;

    /// This flag is set to be true when marshalling uses chunking.
    /// According to spec, the truncatable valuetype should use chunking
    /// and it can be used for marshalling large valuetype. In current
    /// implementation, we just use chunking for the truncatable valuetype.
    CORBA::Boolean chunking_;

    /// Compare the supplied formal type identifier with our actual type.
    /// This is used during marshaling of valuetypes to detect when it is
    /// appropriate to not explicitly marshal the typecode for the value.
    virtual CORBA::Boolean _tao_match_formal_type (ptrdiff_t ) const = 0;

  private:
    static void _tao_unmarshal_find_factory (
      TAO_InputCDR &strm,
      void *start_of_valuetype,
      CORBA::ValueBase *&valuetype,
      Repository_Id_List &ids,
      CORBA::Boolean &is_chunked);

    static CORBA::Boolean _tao_unmarshal_value_indirection_pre (TAO_InputCDR &strm,
                                                                TAO_InputCDR &indirected_strm);

    static CORBA::Boolean _tao_unmarshal_value_indirection (TAO_InputCDR &strm,
                                                            CORBA::ValueBase *&value);

    static CORBA::Boolean _tao_unmarshal_repo_id_indirection (TAO_InputCDR &strm,
                                                              ACE_CString& repo_id);

    static CORBA::Boolean _tao_unmarshal_codebase_url_indirection (TAO_InputCDR &strm,
                                                                   ACE_CString& codebase_url);
  public:
    static CORBA::Boolean _tao_write_repository_id (TAO_OutputCDR &strm,
                                                    ACE_CString& id);

    /// Write some special values such as null value or indirection value.
    static CORBA::Boolean _tao_write_special_value(TAO_OutputCDR &strm,
                                              const CORBA::ValueBase * value);
  private:
    /// Write whole value.
    static CORBA::Boolean _tao_write_value(TAO_OutputCDR &strm,
                                      const CORBA::ValueBase * value,
                                      ptrdiff_t formal_type_id);

    /// Write the header of the value which includes the valuetag, number of
    /// repository ids and list of repository ids.
    CORBA::Boolean _tao_write_value_header(TAO_OutputCDR &strm,
                                      ptrdiff_t formal_type_id) const;

    /// Read a single repository id from the CDR input stream,
    /// accounting for indirection.
    static CORBA::Boolean _tao_read_repository_id (TAO_InputCDR& strm,
                                                   ACE_CString& id);

    ///  Read a list of repository ids from the CDR input stream,
    /// accounting for indirection
    static CORBA::Boolean _tao_read_repository_id_list (TAO_InputCDR& strm,
                                                    Repository_Id_List& ids);

    /// Read a codebase url from the CDR input stream,
    /// accounting for indirection.
    static CORBA::Boolean _tao_read_codebase_url (TAO_InputCDR& strm,
                                                  ACE_CString& codebase_url);
  private:
    ValueBase & operator= (const ValueBase &);
  }; // ValueBase

  /// Valuetype-related type codes.
  extern TAO_Valuetype_Export TypeCode_ptr const _tc_ValueBase;
  extern TAO_Valuetype_Export TypeCode_ptr const _tc_EventBase;

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
    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;
  }; // DefaultValueRefCountBase

  //  which lock has the lowest memory overhead ?
  // %! todo refcountbase w/o locking (now memory overhead)
  // $! todo: debug aids for refcounts

}  // End CORBA namespace

/**
 * @namespace TAO_OBV_GIOP_Flags
 *
 * @brief TAO_OBV_GIOP_Flags
 *
 * @see CORBA 3.0.3 -- Section 15.3.4
 */
namespace TAO_OBV_GIOP_Flags
{
  const CORBA::Long Value_tag_base    = 0x7fffff00L;
  const CORBA::Long Value_tag_sigbits = 0x7fffff00L;
  const CORBA::Long Codebase_url      = 1;
  const CORBA::Long Type_info_sigbits = 0x00000006L;
  const CORBA::Long Type_info_none    = 0;
  const CORBA::Long Type_info_single  = 2;
  const CORBA::Long Type_info_list    = 6;
  const CORBA::Long Chunking_tag_sigbits = 0x00000008L;
  const CORBA::Long Indirection_tag   = 0xFFFFFFFF;
  const CORBA::Long Null_tag          = 0x00000000L;

  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_null_ref          (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_value_tag         (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean has_codebase_url     (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean has_no_type_info     (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean has_single_type_info (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean has_list_type_info   (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_chunked           (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_indirection_tag   (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_indirection       (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_block_size        (CORBA::Long tag);
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Boolean is_end_tag           (CORBA::Long tag);
}

TAO_Valuetype_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const CORBA::ValueBase *);

TAO_Valuetype_Export CORBA::Boolean
operator>> (TAO_InputCDR&, CORBA::ValueBase *&);

#if defined (GEN_OSTREAM_OPS)

/// Outputs the state for debugging.
TAO_Valuetype_Export std::ostream&
operator<< (std::ostream&, CORBA::ValueBase *);

#endif /* GEN_OSTREAM_OPS */

/// Used in generated code if CORBA::ValueBase is an argument or return type.
namespace TAO
{
  template<>
  class TAO_Valuetype_Export Arg_Traits<CORBA::ValueBase>
    : public Object_Arg_Traits_T<CORBA::ValueBase *,
                                 CORBA::ValueBase_var,
                                 CORBA::ValueBase_out,
                                 TAO::Value_Traits<CORBA::ValueBase>,
                                 TAO::Any_Insert_Policy_Stream>
  {
  };

  template<>
  struct TAO_Valuetype_Export Value_Traits<CORBA::ValueBase>
  {
    static void add_ref (CORBA::ValueBase *);
    static void remove_ref (CORBA::ValueBase *);

    // For INOUT value type arguments, so they can use the same set
    // of arg classes as interfaces.
    static void release (CORBA::ValueBase *);
  };
}

#undef TAO_local_COPY_VALUE_IS_PURE
TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Valuetype/ValueBase.inl"
#endif /* __ACE_INLINE__*/

#include /**/ "ace/post.h"

#endif /* TAO_VALUEBASE_H */
