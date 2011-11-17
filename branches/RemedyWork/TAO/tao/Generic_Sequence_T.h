#ifndef guard_generic_sequence_hpp
#define guard_generic_sequence_hpp
/**
 * @file Generic_Sequence_T.h
 *
 * @brief Implement the generic version of CORBA sequences.
 *
 * All CORBA sequences are based on this class template.  The behavior
 * of this class is controlled by two sets of traits.  First, the
 * ALLOCATION_TRAITS control how buffers are allocated and
 * initialized.  Since this is where most of the variation between
 * unbounded and bounded sequences is found, the ALLOCATION_TRAITS can
 * be thought as the bounded aspect of the sequence.
 *
 * Second, the element traits control how are elements copied,
 * initialized and released.  Value-like types, such as integers and
 * structures, have trivial initialization and release requirements
 * (their constructor/destructors do the job!)   But reference-like
 * types, such as strings and object references, have more complicated
 * requirements.  This is yet another aspect of the sequences, we can
 * call it the "element copy semantics" or something.
 *
 * Oh, and let us not forget the type that the sequences encapsulates.
 *
 * The intent is not for sequences to simply derive or instantiate this
 * type.  Instead, different each sequence type is written using
 * composition.  They instantiate a generic sequence with the correct
 * traits, and implement the adapt the generic sequence interface to
 * whatever requirements the spec may impose.  For example, replace()
 * has different number of arguments in bounded vs. unbounded
 * sequences, and operator[] returns different types depending on the
 * underlying type of the sequence.
 *
 * This class offers the strong exception-safety guarantee, as long as
 * destructors and release operations do not throw.
 *
 * This class is not thread-safe.  Thread-safe collections are mostly
 * useless anyways.
 *
 * In general the performance characteristics of the class depends on
 * the traits.  Obviously, they can only be expressed on the number of
 * element constructor and destructor calls.  If the constructor takes
 * O(K) time that is not the sequence fault!
 *
 * All accessors are O(1), single-element modifiers are O(1), multiple
 * element modifiers are O(n + m) where n is the number of elements
 * originally in the sequence, and m is the number of elements left in
 * the sequence afterwards.
 *
 * Beware:
 * - get_buffer(true) may modify multiple elements
 * - length(CORBA::ULong) may modify multiple elements!
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Range_Checking_T.h"
#include "ace/checked_iterator.h"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

// Forward declare the iterators
template<typename T>
class Generic_Sequence_Iterator;

template<typename T>
class Const_Generic_Sequence_Iterator;

template<typename T>
class Generic_Sequence_Reverse_Iterator;

template<typename T>
class Const_Generic_Sequence_Reverse_Iterator;

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class generic_sequence
{

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

  friend class Generic_Sequence_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> >;
  friend class Const_Generic_Sequence_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> >;
  friend class Generic_Sequence_Reverse_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> >;
  friend class Const_Generic_Sequence_Reverse_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> >;

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

public:
  typedef T value_type;
  typedef ALLOCATION_TRAITS allocation_traits;
  typedef ELEMENT_TRAITS element_traits;
  typedef range_checking<value_type,true> range;

  /// Default constructor.
  generic_sequence(void)
    : maximum_(allocation_traits::default_maximum())
    , length_(0)
    , buffer_(allocation_traits::default_buffer_allocation())
    , release_(buffer_ != 0)
  {
  }

  /// Constructor with control of ownership.
  explicit generic_sequence(CORBA::ULong maximum)
    : maximum_(maximum)
    , length_(0)
    , buffer_(allocbuf(maximum_))
    , release_(true)
  {
  }

  generic_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release)
    : maximum_(maximum)
    , length_(length)
    , buffer_(data)
    , release_(release)
  {
  }

  /// Copy constructor
  generic_sequence(generic_sequence const & rhs)
    : maximum_(0)
    , length_(0)
    , buffer_(0)
    , release_(false)
  {
    if (rhs.maximum_ == 0 || rhs.buffer_ == 0)
    {
      maximum_ = rhs.maximum_;
      length_ = rhs.length_;
      return;
    }
    generic_sequence tmp(rhs.maximum_, rhs.length_,
                         allocation_traits::allocbuf_noinit(rhs.maximum_),
                         true);
    element_traits::initialize_range(
        tmp.buffer_ + tmp.length_, tmp.buffer_ + tmp.maximum_);
    element_traits::copy_range(
        rhs.buffer_,
        rhs.buffer_ + rhs.length_,
        ACE_make_checked_array_iterator (tmp.buffer_, tmp.length_));
    swap(tmp);
  }

  /// Assignment operator
  generic_sequence & operator=(generic_sequence const & rhs)
  {
    generic_sequence tmp(rhs);
    swap(tmp);
    return * this;
  }

  /// Destructor.
  ~generic_sequence()
  {
    if (release_)
    {
      freebuf(buffer_);
    }
  }

  /// Return the maximum length of the sequence
  inline CORBA::ULong maximum() const
  {
    return maximum_;
  }

  /// Returns the state of the sequence release flag.
  inline CORBA::Boolean release() const
  {
    return release_;
  }

  /// Returns the length of the sequence
  inline CORBA::ULong length() const
  {
    return length_;
  }

  /// Set a new length for the sequence
  void length(CORBA::ULong length)
  {
    if (length <= maximum_)
    {
      if (buffer_ == 0)
        {
          buffer_ = allocbuf(maximum_);
          release_ = true;
          length_ = length;
          // Since allocbuf returns completely initialized buffer
          // no further actions are required.
          return;
        }

      // When sequence doesn't own a buffer it's not allowed
      // to change it in any way.
      if (length < length_ && release_)
      {
        // TODO This code does not provide the strong-exception
        //      guarantee, but it does provide the weak-exception
        //      guarantee.  The problem would appear when
        //      initialize_range() raises an exception after several
        //      elements have been modified.  One could argue that
        //      this problem is irrelevant, as the elements already
        //      modified are unreachable to conforming applications.
        element_traits::release_range(
          buffer_ + length, buffer_ + length_);
        element_traits::initialize_range(
          buffer_ + length, buffer_ + length_);
      }
      length_ = length;
      return;
    }

    generic_sequence tmp(length, length,
                         allocation_traits::allocbuf_noinit(length),
                         true);
    // First do initialize_range. If it will throw then tmp will be
    // destructed but *this will remain unchanged.
    element_traits::initialize_range(
        tmp.buffer_ + length_, tmp.buffer_ + length);
    element_traits::copy_swap_range(
      buffer_,
      buffer_ + length_,
      ACE_make_checked_array_iterator (tmp.buffer_, tmp.length_));

    swap(tmp);
  }

  /// Get a const element from the sequence
  value_type const & operator[](CORBA::ULong i) const
  {
    range::check(i, length_, maximum_, "operator[]() const");
    return buffer_[i];
  }

  /// Get an element from the sequence
  value_type & operator[](CORBA::ULong i)
  {
    range::check(i, length_, maximum_, "operator[]() non-const");
    return buffer_[i];
  }

  /**
   * Allows the buffer underlying a sequence to be replaced.  The
   * parameters to replace() are identical in type, order, and purpose
   * to those for the <T *data> constructor for the sequence.
   */
  void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release)
  {
    generic_sequence tmp(maximum, length, data, release);
    swap(tmp);
  }

  /**
   * This function allows read-only access to the sequence buffer.
   * The sequence returns its buffer, allocating one of one has not
   * yet been allocated.  No direct modification of the returned
   * buffer by the caller is permitted.
   */
  value_type const * get_buffer() const
  {
    if (buffer_ == 0)
    {
      buffer_ = allocbuf(maximum_);
      release_ = true;
    }
    return buffer_;
  }

  /// Allows read-write access to the underlying buffer.
  /**
   * If @a orphan is FALSE the sequence returns a pointer to its buffer,
   * allocating one if it has not yet done so.  The number of elements in the
   * buffer can be determined from the sequence length() accessor.
   *
   * If the @a orphan argument to get_buffer() is FALSE, the sequence
   * maintains ownership of the underlying buffer.  Elements in the
   * returned buffer may be directly replaced by the caller.  For
   * sequences of strings, wide strings, and object references, the
   * caller must use the sequence @c release accessor to determine
   * whether elements should be freed (using @c string_free,
   * @c wstring_free, or @c CORBA::release for strings, wide strings,
   * and object references, respective) before being directly assigned
   * to.
   *
   * If the @a orphan argument to @a get_buffer is TRUE, the sequence
   * yields ownership of the buffer to the caller.  If @a orphan is
   * TRUE and the sequence does not own its buffer (i.e., its
   * release_ flag is FALSE), the return value is a null pointer.  If
   * the buffer is taken from the sequence using this form of
   * get_buffer(), the sequence reverts to the same state it would
   * have if constructed using its default constructor.  The caller
   * becomes responsible for eventually freeing each element of the
   * returned buffer (for strings, wide string, and object
   * references), and then freeing the returned buffer itself using
   * freebuf().
   */
  value_type * get_buffer(CORBA::Boolean orphan)
  {
    if (orphan && !release_)
    {
      return 0;
    }
    if (buffer_ == 0)
    {
      buffer_ = allocbuf(maximum_);
      if (!orphan)
        {
          release_ = true;
        }
    }
    if (!orphan)
    {
      return buffer_;
    }

    generic_sequence tmp;
    swap(tmp);
    tmp.release_ = false;

    return tmp.buffer_;
  }

  void swap(generic_sequence & rhs) throw()
  {
    std::swap(maximum_, rhs.maximum_);
    std::swap(length_, rhs.length_);
    std::swap(buffer_, rhs.buffer_);
    std::swap(release_, rhs.release_);
  }

  static value_type * allocbuf(CORBA::ULong maximum)
  {
    return allocation_traits::allocbuf(maximum);
  }

  static void freebuf(value_type * buffer)
  {
    allocation_traits::freebuf(buffer);
  }

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

  ///
  /// Additions to support iterator semantics for TAO sequences.
  ///

  // = Traits and factory methods that create iterators.
  typedef Generic_Sequence_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> > iterator;
  typedef Const_Generic_Sequence_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> > const_iterator;
  typedef Generic_Sequence_Reverse_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> > reverse_iterator;
  typedef Const_Generic_Sequence_Reverse_Iterator<generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> > const_reverse_iterator;

  /// Get an iterator that points to the beginning of the sequence.
  iterator begin (void)
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::iterator (this);
  }

  /// Get a const iterator that points to the beginning of the sequence.
  const_iterator begin (void) const
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_iterator (this);
  }

  /// Get an iterator that points to the end of the sequence.
  iterator end (void)
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::iterator (this,
      this->length_);
  }

  /// Get a const iterator that points to the end of the sequence.
  const_iterator end (void) const
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_iterator (this,
            this->length_);
  }

  // Get a reverse iterator that points to the end of the sequence.
  reverse_iterator rbegin (void)
  {
    // Here we need to be at the last element - not one past.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::reverse_iterator (this,
              this->length_ - 1);
  }

  // Get a const reverse iterator that points to the end of the sequence.
  const_reverse_iterator rbegin (void) const
  {
    // Here we need to be at the last element - not one past.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_reverse_iterator (this,
                     this->length_ - 1);
  }

  // Get a reverse iterator that points to one before the
  // beginning of the sequence.
  reverse_iterator rend (void)
  {
    // Here we need to be at one before the first element - not the first.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::reverse_iterator (this,
              -1);
  }

  // Get a const reverse iterator that points to one before the
  // beginning of the sequence.
  const_reverse_iterator rend (void) const
  {
    // Here we need to be at one before the first element - not the first.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_reverse_iterator (this,
              -1);
  }

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

private:
  /// The maximum number of elements the buffer can contain.
  CORBA::ULong maximum_;
  /// The current number of elements in the buffer.
  CORBA::ULong length_;
  /// The buffer with all the elements
  mutable value_type * buffer_;
  /// If true then the sequence should release the buffer when it is
  /// destroyed.
  mutable CORBA::Boolean release_;
};

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

/**
 * @class Generic_Sequence_Iterator
 * @brief Implements a random access iterator for generic sequence type
 *        classes.
 */
template <typename SEQUENCE_T>
  class Generic_Sequence_Iterator
{
  friend class Const_Generic_Sequence_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef CORBA::Long difference_type;

  /// Construct a Generic_Sequence_Iterator at position pos.
  Generic_Sequence_Iterator (generic_sequence<value_type,
                            allocation_traits,
                            element_traits> *sequence,
           size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  Generic_Sequence_Iterator (Generic_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Assignment operator
  Generic_Sequence_Iterator & operator= (
          Generic_Sequence_Iterator<SEQUENCE_T> const & rhs)
    {
      Generic_Sequence_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// typecast operator to Const_Generic_Sequence_Iterator
  operator Const_Generic_Sequence_Iterator<SEQUENCE_T> ()
  {
    return Const_Generic_Sequence_Iterator<SEQUENCE_T> (*this);
  }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  value_type & operator* (void)
    {
      // Access the underlying element in the sequence.
      return (*(this->sequence_))[this->pos_];
    }

  /// Returns a const reference to the item contained at the current position
  const value_type & operator* (void) const
    {
      // Access the underlying element in the sequence.
      return (*(this->sequence_))[this->pos_];
    }

  /// Preincrement operator
  Generic_Sequence_Iterator<SEQUENCE_T> &operator++ (void)
    {
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return *this;
    }

  /// Postincrement operator
  Generic_Sequence_Iterator<SEQUENCE_T> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  Generic_Sequence_Iterator<SEQUENCE_T> &operator-- (void)
    {
      --this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Generic_Sequence_Iterator<SEQUENCE_T> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Generic_Sequence_Iterator<SEQUENCE_T> &operator+= (difference_type n)
    {
      // Move ahead n elements.
      this->pos_ += n;
      return *this;
    }

  /// Iterator addition
  Generic_Sequence_Iterator<SEQUENCE_T> operator+ (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements.
      return Generic_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  Generic_Sequence_Iterator<SEQUENCE_T> &operator-= (difference_type n)
    {
      // Move back n elements.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator subtraction
  Generic_Sequence_Iterator<SEQUENCE_T> operator- (difference_type n)
    {
      // Create a temporary to return and move it back n elements.
      return Generic_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return this->pos_ - rhs.pos_;
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return this->pos_ - rhs.pos_;
    }


  /// Element operator/assignment
  value_type & operator[] (difference_type n)
    {
      // Return the element at position n
      return (*(this->sequence_))[this->pos_ + n];
    }

  /// Less than
  bool operator< (const Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Less than
  bool operator< (const Const_Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }


  /// Equality operator
  bool operator== (const Generic_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Generic_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the sequence with which we are dealing
  generic_sequence<value_type, allocation_traits, element_traits> *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};


/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Generic_Sequence_Iterator<SEQUENCE_T>
  operator+ (typename Generic_Sequence_Iterator<SEQUENCE_T>::difference_type n,
             const Generic_Sequence_Iterator<SEQUENCE_T> & iter)
   {
     // Create an iter and move it ahead n elements.
     return Generic_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Generic_Sequence_Iterator<SEQUENCE_T> operator-
   (typename Generic_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    Generic_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements.
     return Generic_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class Const_Generic_Sequence_Iterator
 * @brief Implements a constant random access iterator for generic sequence
 *        type classes.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename SEQUENCE_T>
class Const_Generic_Sequence_Iterator
{
  friend class Generic_Sequence_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef int difference_type;

  /// Construct a Const_Generic_Sequence_Iterator at position pos from
  /// a const sequence.
  Const_Generic_Sequence_Iterator (const generic_sequence<value_type,
                                                          allocation_traits,
                                                          element_traits> *sequence,
           size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  virtual ~Const_Generic_Sequence_Iterator ()
    {
    }

  /// Copy constructor
  Const_Generic_Sequence_Iterator (
    Const_Generic_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Copy constructor initializing by a Generic_Sequence_Iterator
  explicit Const_Generic_Sequence_Iterator (
    Generic_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Assignment operator
  Const_Generic_Sequence_Iterator & operator= (
                Const_Generic_Sequence_Iterator<SEQUENCE_T> const & rhs)
    {
      Const_Generic_Sequence_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(Const_Generic_Sequence_Iterator<SEQUENCE_T> & rhs)
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const value_type & operator* (void) const
    {
      // Access the underlying element in the sequence.
      return (*(this->sequence_))[this->pos_];
    }

  /// Preincrement operator
  const Const_Generic_Sequence_Iterator<SEQUENCE_T> &operator++ (void) const
    {
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return * this;
    }

  /// Postincrement operator
  Const_Generic_Sequence_Iterator<SEQUENCE_T> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  const Const_Generic_Sequence_Iterator<SEQUENCE_T> &operator-- (void) const
    {
      --this->pos_;
      return * this;
    }

  /// Postdecrement operator
  Const_Generic_Sequence_Iterator<SEQUENCE_T> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  const Const_Generic_Sequence_Iterator<SEQUENCE_T> &operator+= (difference_type n) const
    {
      // Move ahead n elements.
      this->pos_ += n;
      return * this;
    }

  /// Iterator addition
  Const_Generic_Sequence_Iterator<SEQUENCE_T> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements.
      return Const_Generic_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  const Const_Generic_Sequence_Iterator<SEQUENCE_T> &operator-= (difference_type n) const
    {
      // Move back n elements.
      this->pos_ -= n;
      return * this;
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Iterator<SEQUENCE_T> operator- (difference_type n) const
    {
      // Create a temporary to return and move it back n elements.
      return Const_Generic_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Iterator<SEQUENCE_T> & rhs) const
    {
      // I think the order is important here (i.e., rhs before this).
      return this->pos_ - rhs.pos_;
    }

  /// Element operator/assignment
  const value_type & operator[] (difference_type n) const
    {
      // Return the element at position n
      return (*(this->sequence_))[this->pos_ + n];
    }

  /// Less than
  virtual bool operator< (const Const_Generic_Sequence_Iterator<SEQUENCE_T> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_Generic_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Const_Generic_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the array we are dealing with
  const generic_sequence<value_type, allocation_traits, element_traits> *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_Generic_Sequence_Iterator<SEQUENCE_T> operator+
   (typename Generic_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    Const_Generic_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements.
     return Const_Generic_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_Generic_Sequence_Iterator<SEQUENCE_T> operator-
   (typename Generic_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    Const_Generic_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements.
     return Const_Generic_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class Generic_Sequence_Reverse_Iterator
 * @brief Implements a reverse random access iterator for generic sequence
 *        type classes.
 */
template <typename SEQUENCE_T>
class Generic_Sequence_Reverse_Iterator
{
  friend class Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef int difference_type;

  /// Construct a Generic_Sequence_Reverse_Iterator at position pos with before_start flag.
  Generic_Sequence_Reverse_Iterator (generic_sequence<value_type,
                              allocation_traits,
                              element_traits> *sequence,
             size_t pos)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  Generic_Sequence_Reverse_Iterator (
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Assignment operator
  Generic_Sequence_Reverse_Iterator & operator= (
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    {
      Generic_Sequence_Reverse_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  value_type & operator* (void)
    {
      // Access the underlying element in the sequence.
      return (*(this->sequence_))[this->pos_];
    }

  /// Returns a const reference to the item contained at the current position
  const value_type& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return (*(this->sequence_))[this->pos_];
    }

  /// Preincrement operator
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator++ (void)
    {
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return *this;
    }

  /// Postincrement operator
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now past the end.
      --this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-- (void)
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator+= (difference_type n)
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator addition
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator+ (difference_type n)
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-= (difference_type n)
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      return *this;
    }

  /// Iterator subtraction
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator- (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }


  /// Element operator/assignment
  value_type & operator[] (difference_type n)
    {
      // Return the element at position n
      return (*(this->sequence_))[this->pos_ - n];
    }

  /// Less than
  bool operator< (const Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Less than
  bool operator< (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }


  /// Equality operator
  bool operator== (const Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
           && this->pos_ == rhs.pos_;
  }

  /// Equality operator
  bool operator== (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
           && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

  /// Nonequality operator
  bool operator!= (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// The sequence with which we are dealing
  generic_sequence<value_type, allocation_traits, element_traits> *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator+
   (typename Generic_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Generic_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator-
   (typename Generic_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Generic_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }


/**
 * @class Const_Generic_Sequence_Reverse_Iterator
 * @brief Implements a constant reverse random access iterator for generic
 *        sequence type classes.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename SEQUENCE_T>
class Const_Generic_Sequence_Reverse_Iterator
{
  friend class Generic_Sequence_Reverse_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef int difference_type;

  /// Construct a Const_Generic_Sequence_Reverse_Iterator at position pos
  /// using a const sequence.
  Const_Generic_Sequence_Reverse_Iterator (const generic_sequence<value_type,
                                    allocation_traits,
                                    element_traits> *sequence,
             size_t pos)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  Const_Generic_Sequence_Reverse_Iterator (
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Copy constructor
  Const_Generic_Sequence_Reverse_Iterator (
  Generic_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Assignment operator
  Const_Generic_Sequence_Reverse_Iterator & operator= (
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    {
      Const_Generic_Sequence_Reverse_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// typecast operator to Generic_Sequence_Reverse_Iterator

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const value_type& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return (*this->sequence_)[this->pos_];
    }

  /// Preincrement operator
  const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator++ (void) const
    {
      // Decrement the position for reveres iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return *this;
    }

  /// Postincrement operator
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-- (void) const
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator+= (difference_type n) const
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator addition
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-= (difference_type n) const
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      return *this;
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator- (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) const
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  const value_type & operator[] (difference_type n) const
    {
      // Return the element at position n
      return (*(this->sequence_))[this->pos_ - n];
    }

  /// Less than
  bool operator< (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the array we are dealing with
  const generic_sequence<value_type, allocation_traits, element_traits> *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator+
   (typename Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> operator-
   (typename Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return Const_Generic_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_generic_sequence_hpp
