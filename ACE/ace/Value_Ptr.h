// -*- C++ -*-

//==========================================================================
/**
 *  @file Value_Ptr.h
 *
 *  Value_Ptr implementation based on code in Herb Sutter's book "More
 *  Exceptional C++".
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef ACE_VALUE_PTR_H
#define ACE_VALUE_PTR_H

#include /**/ "ace/config-lite.h"

#include <algorithm>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  /**
   * @struct VP_traits
   *
   * @brief @c Value_Ptr traits template structure.
   *
   * The @c Value_Ptr template class delegates some operations to this
   * template traits structure.
   *
   * Specialize this trait template if cloning through copy
   * construction is not sufficient.  For example, to avoid slicing
   * when copying an object through a base class pointer, one can
   * implement a virtual "clone" method that can be used to
   * polymorphically invoke the appropriate cloning operation(s).
   * That virtual method would then be invoked by the @c VP_traits\<\>
   * specialization.
   */
  template <typename T>
  struct VP_traits
  {
    /// Copy the given object.
    static T * clone (T const * p) { return new T (*p); }
  };

  /**
   * @class Value_Ptr
   *
   * @brief Smart pointer implementation designed for use as a class
   *        member.
   *
   * Using a @c std::unique_ptr\<\> as a class member is sometimes
   * problematic when the containing class needs copy semantics. This
   * @c Value_Ptr class performs copies of the underlying object rather
   * than transferring ownership. This allows it to be used as a member
   * in copyable classes and standard containers.
   *
   * @see Item 31 in "More Exceptional C++" by Herb Sutter.
   */
  template <typename T>
  class Value_Ptr
  {
  public:
    explicit Value_Ptr (T * p = nullptr) : p_ (p) {}

    ~Value_Ptr () { delete this->p_; }

    /// Deference operator.
    T & operator* () const { return *this->p_; }

    /// Pointer operator.
    T * operator-> () const { return this->p_; }

    /// Non-throwing swap operation used to make assignment strongly
    /// exception-safe.
    /**
     * @note Swapping exchanges the owned pointers. Each @c Value_Ptr
     *       remains responsible for deleting the pointer it holds.
     */
    void swap (Value_Ptr & other) { std::swap (this->p_, other.p_); }

    /// Copy constructor.
    Value_Ptr (Value_Ptr const & other)
      : p_ (create_from (other.p_)) { }

    /// Assignment operator.
    Value_Ptr & operator= (Value_Ptr const & other)
    {
      // Strongly exception-safe.
      Value_Ptr temp (other);
      this->swap (temp);
      return *this;
    }

    /// Converting copy constructor.
    template <typename U>
    Value_Ptr (Value_Ptr<U> const & other)
      : p_ (create_from (other.p_)) { }

    /// Converting assignment operator.
    template <typename U>
    Value_Ptr & operator= (Value_Ptr<U> const & other)
    {
      // Strongly exception-safe.
      Value_Ptr temp (other);
      this->swap (temp);
      return *this;
    }

  private:
    /// Copying method invoked when copy constructing.
    template <typename U>
    T * create_from (U const * p) const
    {
      return p ? VP_traits<U>::clone (p) : 0;
    }

  private:
    template <typename U> friend class Value_Ptr;

    /// Object owned by this @c Value_Ptr.
    T * p_;
  };
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_VALUE_PTR_H */
