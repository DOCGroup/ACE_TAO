#ifndef guard_generic_sequence_hpp
#define guard_generic_sequence_hpp
/**
 * @file
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

// Forward declare the iterators
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class Generic_Sequence_Iterator;

template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class Const_Generic_Sequence_Iterator;

template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class Generic_Sequence_Reverse_Iterator;

template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class Const_Generic_Sequence_Reverse_Iterator;

template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
class generic_sequence
{
  friend class Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>;
  friend class Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>;
  friend class Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>;
  friend class Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>;

public:
  typedef T value_type;
  typedef ALLOCATION_TRAITS allocation_traits;
  typedef ELEMENT_TRAITS element_traits;
  typedef range_checking<value_type,true> range;

  /// Default constructor.
  generic_sequence()
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
    if (rhs.maximum_ == 0) return;
    generic_sequence tmp(rhs.maximum_);
    tmp.length_ = rhs.length_;
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
    if (length <= maximum_ || length <= length_)
    {
      if (buffer_ == 0)
        {
          buffer_ = allocbuf(maximum_);
          release_ = true;
        }

      if (length_ < length)
      {
        // TODO This code does not provide the strong-exception
        //      guarantee, but it does provide the weak-exception
        //      guarantee.  The problem would appear when
        //      initialize_range() raises an exception after several
        //      elements have been modified.  One could argue that
        //      this problem is irrelevant, as the elements already
        //      modified are unreachable to conforming applications.
        element_traits::initialize_range(
            buffer_ + length_, buffer_ + length);
      }
      length_ = length;
      return;
    }

    generic_sequence tmp(length);
    tmp.length_ = length;
    element_traits::copy_range(
      buffer_,
      buffer_ + length_,
      ACE_make_checked_array_iterator (tmp.buffer_, tmp.length_));
    element_traits::initialize_range(
        tmp.buffer_ + length_, tmp.buffer_ + length);

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

  ///
  /// Additions to support iterator semantics for TAO sequences.
  ///

  // = Traits and factory methods that create iterators.
  typedef Generic_Sequence_Iterator<T,
                                    ALLOCATION_TRAITS,
                                    ELEMENT_TRAITS> iterator;
  typedef Const_Generic_Sequence_Iterator<T,
                                          ALLOCATION_TRAITS,
                                          ELEMENT_TRAITS> const_iterator;
  typedef Generic_Sequence_Reverse_Iterator<T,
                                            ALLOCATION_TRAITS,
                                            ELEMENT_TRAITS> reverse_iterator;
  typedef Const_Generic_Sequence_Reverse_Iterator<T,
                                                  ALLOCATION_TRAITS,
                                                  ELEMENT_TRAITS> const_reverse_iterator;

  // Get an iterator that points to the beginning of the sequence.
  iterator begin (void)
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::iterator (*this);
  }

  // Get a const iterator that points to the beginning of the sequence.
  const_iterator begin (void) const
  {
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_iterator (*this);
  }

  // Get an iterator that points to the end of the sequence.
  iterator end (void)
  {
    // Specify being past the "real" end with a flag.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::iterator (*this,
       this->length_);
  }

  // Get a const iterator that points to the end of the sequence.
  const_iterator end (void) const
  {
    // Specify being past the "real" end with a flag.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_iterator (*this,
             this->length_);
  }

  // Get a reverse iterator that points to the end of the sequence.
  reverse_iterator rbegin (void)
  {
    // Here we need to be at the last element - not one past.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::reverse_iterator (*this,
              this->length_);
  }

  // Get a const reverse iterator that points to the end of the sequence.
  const_reverse_iterator rbegin (void) const
  {
    // Here we need to be at the last element - not one past.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::const_reverse_iterator (*this,
                    this->length_);
  }

  // Get a reverse iterator that points to one before the
  // beginning of the sequence.
  reverse_iterator rend (void)
  {
    // Here we need to be at one before the first element - not the first.
    // Specify being before the "real" beginning with a flag.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::reverse_iterator (*this,
              -1);
  }

  // Get a const reverse iterator that points to one before the
  // beginning of the sequence.
  const_reverse_iterator rend (void) const
  {
    // Here we need to be at one before the first element - not the first.
    // Specify being before the "real" beginning with a flag.
    return typename generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::reverse_iterator (*this,
              -1);
  }

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


/**
 * @class Generic_Sequence_Iterator
 * @brief Implements a random access iterator for generic sequence type classes.
 */
template <typename T,
          class ALLOCATION_TRAITS,
          class ELEMENT_TRAITS>
class Generic_Sequence_Iterator 
{
public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef CORBA::Long difference_type;

  /// Construct a Generic_Sequence_Iterator at position pos with past_end flag.
  Generic_Sequence_Iterator (generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence, size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos),
    past_end_ (pos >= sequence.length_)
      {
      }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  T& operator* (void)
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Returns a const reference to the item contained at the current position
  const T& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Preincrement operator
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator++ (void)
    {
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      this->check_position ();
      return *this;
    }

  /// Postincrement operator
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      this->check_position ();
      return temp_iter;
    }

  /// Predecrement operator
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-- (void)
    {
      --this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator+= (difference_type n)
    {
      // Move ahead n elements.
      this->pos_ += n;
      this->check_position ();
      return *this;
    }

  /// Iterator addition
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+ (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements.
      return Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-= (difference_type n)
    {
      // Move back n elements.
      this->pos_ -= n;
      this->check_position ();
      return *this;
    }

  /// Iterator subtraction
  Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator- (difference_type n)
    {
      // Create a temporary to return and move it back n elements.
      return Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  T& operator[] (difference_type n)
    {
      // Return the element at position n
      return this->sequence_[n];
    }

  /// Less than
  bool operator< (const Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    // Compare all the data members for equality.
    return &this->sequence_ == &rhs.sequence_
           && this->pos_ == rhs.pos_
           && this->past_end_ == rhs.past_end_;
  }

  /// Nonequality operator
  bool operator!= (const Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// Check the length of the sequence to see if we're past the end.
  void check_position (void)
  {
    if (this->pos_ >= static_cast<CORBA::Long> (this->sequence_.length_))
      {
        this->past_end_ = true;
      }
    else
      {
        // We add this here to allow iterators to
        // "come back from the dead" (i.e., come back from
        // being at an invalid/undefined position).
        this->past_end_ = false;
      }
  }

  /// the array with which we are dealing
  generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;

  /// Designate if the iterator is past the end of the sequence.
  mutable bool past_end_;
};


/// Iterator addition with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>
  operator+ (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
             const Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & iter)
   {
     // Create an iter and move it ahead n elements.
     return Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it back n elements.
     return Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class Const_Generic_Sequence_Iterator
 * @brief Implements a random access iterator for generic sequence type classes.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename T,
          class ALLOCATION_TRAITS,
          class ELEMENT_TRAITS>
class Const_Generic_Sequence_Iterator 
{
public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef int difference_type;

  /// Construct a Const_Generic_Sequence_Iterator at position pos with past_end flag.
  Const_Generic_Sequence_Iterator (generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence, size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos),
    past_end_ (pos >= sequence.length_)
      {
      }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const T& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Preincrement operator
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator++ (void) const
    {
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      this->check_position ();
      return *this;
    }

  /// Postincrement operator
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      this->check_position ();
      return temp_iter;
    }

  /// Predecrement operator
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-- (void) const
    {
      --this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator+= (difference_type n) const
    {
      // Move ahead n elements.
      this->pos_ += n;
      this->check_position ();
      return *this;
    }

  /// Iterator addition
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements.
      return Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-= (difference_type n) const
    {
      // Move back n elements.
      this->pos_ -= n;
      this->check_position ();
      return *this;
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator- (difference_type n) const
    {
      // Create a temporary to return and move it back n elements.
      return Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs) const
    {
      // I think the order is important here (i.e., rhs before this).
      return rhs.pos - this->pos_;
    }

  /// Element operator/assignment
  const T& operator[] (difference_type n) const
    {
      // Return the element at position n
      return this->sequence_[n];
    }

  /// Less than
  bool operator< (const Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    // Compare all the data members for equality.
    return &this->sequence_ == &rhs.sequence_
           && this->pos_ == rhs.pos_
           && this->past_end_ == rhs.past_end_;
  }

  /// Nonequality operator
  bool operator!= (const Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// Check the length of the sequence to see if we're past the end.
  void check_position (void)
  {
    if (this->pos_ >= static_cast<CORBA::Long> (this->sequence_.length_))
      {
        this->past_end_ = true;
      }
    else
      {
        // We add this here to allow iterators to
        // "come back from the dead" (i.e., come back from
        // being at an invalid/undefined position).
        this->past_end_ = false;
      }
  }

  /// the array we are dealing with
  generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;

  /// Designate if the iterator is past the end of the sequence.
  mutable bool past_end_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it ahead n elements.
     return Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it back n elements.
     return Const_Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class Generic_Sequence_Reverse_Iterator
 * @brief Implements a random access iterator for generic sequence type classes.
 */
template <typename T,
          class ALLOCATION_TRAITS,
          class ELEMENT_TRAITS>
class Generic_Sequence_Reverse_Iterator 
{
public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef int difference_type;

  /// Construct a Generic_Sequence_Reverse_Iterator at position pos with before_start flag.
  Generic_Sequence_Reverse_Iterator (generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence, size_t pos)
    : sequence_ (sequence),
    pos_ (pos),
    before_start_ (pos < 0)
      {
      }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  T& operator* (void)
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Returns a const reference to the item contained at the current position
  const T& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Preincrement operator
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator++ (void)
    {
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      this->check_position ();
      return *this;
    }

  /// Postincrement operator
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now past the end.
      --this->pos_;
      this->check_position ();
      return temp_iter;
    }

  /// Predecrement operator
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-- (void)
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator+= (difference_type n)
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      this->check_position ();
      return *this;
    }

  /// Iterator addition
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+ (difference_type n)
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-= (difference_type n)
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      this->check_position ();
      return *this;
    }

  /// Iterator subtraction
  Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator- (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  T& operator[] (difference_type n)
    {
      // Return the element at position n
      return this->sequence_[n];
    }

  /// Less than
  bool operator< (const Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    // Compare all the data members for equality.
    return &this->sequence_ == &rhs.sequence_
           && this->pos_ == rhs.pos_
           && this->before_start_ == rhs.before_start_;
  }

  /// Nonequality operator
  bool operator!= (const Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// Check the length of the sequence to see if we're past the end.
  void check_position (void)
  {
    if (this->pos_ < 0)
      {
        this->before_start_ = true;
      }
    else
      {
        // We add this here to allow iterators to
        // "come back from the dead" (i.e., come back from
        // being at an invalid/undefined position).
        this->before_start_ = false;
      }
  }

  /// The sequence with which we are dealing
  generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;

  /// Designate if the iterator is before the start of the sequence.
  mutable bool before_start_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ + n);
   }


/**
 * @class Const_Generic_Sequence_Reverse_Iterator
 * @brief Implements a random access iterator for generic sequence type classes.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename T,
          class ALLOCATION_TRAITS,
          class ELEMENT_TRAITS>
class Const_Generic_Sequence_Reverse_Iterator 
{
public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef int difference_type;

  /// Construct a Const_Generic_Sequence_Reverse_Iterator at position pos with before_start flag.
  Const_Generic_Sequence_Reverse_Iterator (generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence, size_t pos)
    : sequence_ (sequence),
    pos_ (pos),
    before_start_ (pos < 0)
      {
      }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const T& operator* (void) const
    {
      // Access the underlying element in the sequence.
      return this->sequence_[this->pos_];
    }

  /// Preincrement operator
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator++ (void) const
    {
      // Decrement the position for reveres iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      this->check_position ();
      return *this;
    }

  /// Postincrement operator
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      this->check_position ();
      return temp_iter;
    }

  /// Predecrement operator
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-- (void) const
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator+= (difference_type n) const
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      this->check_position ();
      return *this;
    }

  /// Iterator addition
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &operator-= (difference_type n) const
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      this->check_position ();
      return *this;
    }

  /// Iterator subtraction
  Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator- (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs) const
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  const T& operator[] (difference_type n) const
    {
      // Return the element at position n
      return this->sequence_[n];
    }

  /// Less than
  bool operator< (const Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    // Compare all the data members for equality.
    return &this->sequence_ == &rhs.sequence_
           && this->pos_ == rhs.pos_
           && this->before_start_ == rhs.before_start_;
  }

  /// Nonequality operator
  bool operator!= (const Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// Check the length of the sequence to see if we're past the end.
  void check_position (void)
  {
    if (this->pos_ < 0)
      {
        this->before_start_ = true;
      }
    else
      {
        // We add this here to allow iterators to
        // "come back from the dead" (i.e., come back from
        // being at an invalid/undefined position).
        this->before_start_ = false;
      }
  }

  /// the array we are dealing with
  generic_sequence<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> &sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;

  /// Designate if the iterator is past the end of the sequence.
  mutable bool before_start_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator+
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename T,
         class ALLOCATION_TRAITS,
         class ELEMENT_TRAITS>
 Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> operator-
   (typename Generic_Sequence_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS>::difference_type n,
    Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return Const_Generic_Sequence_Reverse_Iterator<T, ALLOCATION_TRAITS, ELEMENT_TRAITS> (iter.sequence_, iter.pos_ + n);
   }


} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_generic_sequence_hpp
