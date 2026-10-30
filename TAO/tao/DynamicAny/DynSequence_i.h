// -*- C++ -*-
//=============================================================================
/**
 *  @file    DynSequence_i.h
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNSEQUENCE_I_H
#define TAO_DYNSEQUENCE_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicAny/DynCommon.h"
#include "tao/LocalObject.h"

#include <vector>

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynSequence_i
 *
 * Implementation of Dynamic Any type for Sequences.
 */
class TAO_DynamicAny_Export TAO_DynSequence_i
  : public virtual DynamicAny::DynSequence,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  explicit TAO_DynSequence_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynSequence_i () = default;

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc);

  /// Initialize using an Any.
  void init (CORBA::Any const& any);

  // = LocalObject methods.
  static TAO_DynSequence_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynSequence.

  CORBA::ULong get_length () override;

  void set_length (CORBA::ULong len) override;

  DynamicAny::AnySeq * get_elements () override;

  void set_elements (DynamicAny::AnySeq const& value) override;

  DynamicAny::DynAnySeq * get_elements_as_dyn_any () override;

  void set_elements_as_dyn_any (DynamicAny::DynAnySeq const& value) override;


  // = DynAny common functions not implemented in class TAO_DynCommon.

  void from_any (CORBA::Any const& value) override;

  CORBA::Any * to_any () override;

  CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any) override;

  void destroy () override;

  DynamicAny::DynAny_ptr current_component () override;

private:
  // Utility, turns the type of elements contained in the sequence.
  CORBA::TypeCode_ptr get_element_type ();

  // Called by both versions of init().
  void init_common ();

  // = Use copy() or assign() instead of these
  TAO_DynSequence_i (TAO_DynSequence_i const &src) = delete;
  TAO_DynSequence_i &operator= (TAO_DynSequence_i const &src) = delete;
  TAO_DynSequence_i (TAO_DynSequence_i &&) = delete;
  TAO_DynSequence_i &operator= (TAO_DynSequence_i &&) = delete;

private:
  /// Each component is also a DynAny.
  std::vector<DynamicAny::DynAny_var> da_members_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNSEQUENCE_I_H */
