#ifndef mm_sequence_iterator_h
#define mm_sequence_iterator_h

/**
 * @file
 *
 * @brief Implement the iterator for CORBA sequences that need
 *        memory management.
 *
 * There are some CORBA sequences that need to manage memory (e.g.,
 * string sequences). Iterators for these classes also need to
 * manage memory and therefore do not use the iterators defined
 * for the generic sequences.
 *
 * Essentially, only the iterators operators that reference and return
 * elements (e.g.,operator*, operator[]) need to be overridden from the
 * behavior provided by the generic sequence iterators. Template
 * specialization code is appropriate here but I couldn't figure out how
 * to make the compiler happy.
 *
 * In lieu of template specialization code separate iterators exist
 * for sequences that manage memory. Here are those iterators.
 *
 * $Id$
 *
 * @author Joe Hoffert
 */

#if TAO_HAS_SEQUENCE_ITERATORS

namespace TAO
{

// Forward declare the iterators
template<typename T>
class MM_Sequence_Iterator;

template<typename T>
class Const_MM_Sequence_Iterator;

template<typename T>
class MM_Sequence_Reverse_Iterator;

template<typename T>
class Const_MM_Sequence_Reverse_Iterator;


/**
 * @class MM_Sequence_Iterator
 * @brief Implements a random access iterator for generic sequence type
 *        classes that need to manage memory. MM stands for memory managed.
 */
template <typename SEQUENCE_T>
  class MM_Sequence_Iterator
{
  friend class Const_MM_Sequence_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::element_type element_type;
  typedef typename SEQUENCE_T::const_element_type const_element_type;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef typename SEQUENCE_T::implementation_type implementation_type;
  typedef CORBA::Long difference_type;

  /// Construct a MM_Sequence_Iterator at position pos.
  MM_Sequence_Iterator (implementation_type *sequence, 
			 size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  MM_Sequence_Iterator (MM_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }
  
  /// Assignment operator
  MM_Sequence_Iterator & operator= (MM_Sequence_Iterator<SEQUENCE_T> const & rhs)
    {
      MM_Sequence_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(MM_Sequence_Iterator<SEQUENCE_T> & rhs) 
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// typecast operator to Const_MM_Sequence_Iterator
  operator Const_MM_Sequence_Iterator<SEQUENCE_T> ()
  {
    return Const_MM_Sequence_Iterator<SEQUENCE_T> (*this);
  }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  element_type operator* (void)
    {
      // Access the underlying element in the sequence.
      return element_type ((*(this->sequence_))[this->pos_],
                           this->sequence_->release ());
    }

  /// Returns a const reference to the item contained at the current position
  const_element_type operator* (void) const
    {
      // Access the underlying element in the sequence.
      return const_element_type ((*(this->sequence_))[this->pos_],
                                 this->sequence_->release ());
    }

  /// Preincrement operator
  MM_Sequence_Iterator<SEQUENCE_T> &operator++ (void)
    {
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return *this;
    }

  /// Postincrement operator
  MM_Sequence_Iterator<SEQUENCE_T> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      MM_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  MM_Sequence_Iterator<SEQUENCE_T> &operator-- (void)
    {
      --this->pos_;
      return *this;
    }

  /// Postdecrement operator
  MM_Sequence_Iterator<SEQUENCE_T> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      MM_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  MM_Sequence_Iterator<SEQUENCE_T> &operator+= (difference_type n)
    {
      // Move ahead n elements.
      this->pos_ += n;
      return *this;
    }

  /// Iterator addition
  MM_Sequence_Iterator<SEQUENCE_T> operator+ (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements.
      return MM_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  MM_Sequence_Iterator<SEQUENCE_T> &operator-= (difference_type n)
    {
      // Move back n elements.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator subtraction
  MM_Sequence_Iterator<SEQUENCE_T> operator- (difference_type n)
    {
      // Create a temporary to return and move it back n elements.
      return MM_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const MM_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., this before rhs).
      return this->pos_ - rhs.pos_;
    }

  /// Difference
  difference_type operator- (const Const_MM_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., this before rhs).
      return this->pos_ - rhs.pos_;
    }

  /// Element operator/assignment
  element_type operator[] (difference_type n)
    {
      // Return the element at position pos_ + n
      return element_type ((*(this->sequence_))[this->pos_ + n],
                           this->sequence_->release ());
    }

  /// Element operator/assignment
  const_element_type operator[] (difference_type n) const
    {
      // Return the element at position pos_ + n
      return const_element_type ((*(this->sequence_))[this->pos_ + n],
                                 this->sequence_->release ());
    }

  /// Less than
  bool operator< (const MM_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Less than
  bool operator< (const Const_MM_Sequence_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }


  /// Equality operator
  bool operator== (const MM_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const MM_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the sequence with which we are dealing
  implementation_type *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};


/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 MM_Sequence_Iterator<SEQUENCE_T>
  operator+ (typename MM_Sequence_Iterator<SEQUENCE_T>::difference_type n,
             const MM_Sequence_Iterator<SEQUENCE_T> & iter)
   {
     // Create an iter and move it ahead n elements.
     return MM_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 MM_Sequence_Iterator<SEQUENCE_T> operator-
   (typename MM_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    MM_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements.
     return MM_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class Const_MM_Sequence_Iterator
 * @brief Implements a constant random access iterator for generic sequence
 *        type classes that need to manage memory. MM stands for memory
 *        managed.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename SEQUENCE_T>
class Const_MM_Sequence_Iterator 
{
  friend class MM_Sequence_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::const_element_type const_element_type;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef typename SEQUENCE_T::implementation_type implementation_type;
  typedef int difference_type;

  /// Construct a Const_MM_Sequence_Iterator at position pos from
  /// a const sequence.
  Const_MM_Sequence_Iterator (const implementation_type *sequence,
                               size_t pos = 0)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  virtual ~Const_MM_Sequence_Iterator ()
    {
    }

  /// Copy constructor
  Const_MM_Sequence_Iterator (
		Const_MM_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Copy constructor initializing by a MM_Sequence_Iterator
  Const_MM_Sequence_Iterator (
		MM_Sequence_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }
  
  /// Assignment operator
  Const_MM_Sequence_Iterator & operator= (
                Const_MM_Sequence_Iterator<SEQUENCE_T> const & rhs)
    {
      Const_MM_Sequence_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members  
  void swap(Const_MM_Sequence_Iterator<SEQUENCE_T> & rhs) 
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const_element_type operator* (void) const
    {
      // Access the underlying element in the sequence.
      return const_element_type ((*(this->sequence_))[this->pos_],
                                 this->sequence_->release ());
    }

  /// Preincrement operator
  const Const_MM_Sequence_Iterator<SEQUENCE_T> &operator++ (void) const
    {
      // Increment the position.
      ++this->pos_;
      return * this;
    }

  /// Postincrement operator
  Const_MM_Sequence_Iterator<SEQUENCE_T> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_MM_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position.
      // We also need to check if we're now past the end.
      ++this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  const Const_MM_Sequence_Iterator<SEQUENCE_T> &operator-- (void) const
    {
      --this->pos_;
      return * this;
    }

  /// Postdecrement operator
  Const_MM_Sequence_Iterator<SEQUENCE_T> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_MM_Sequence_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      --this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  const Const_MM_Sequence_Iterator<SEQUENCE_T> &operator+= (difference_type n) const
    {
      // Move ahead n elements.
      this->pos_ += n;
      return * this;
    }

  /// Iterator addition
  Const_MM_Sequence_Iterator<SEQUENCE_T> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements.
      return Const_MM_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Iterator subtraction
  const Const_MM_Sequence_Iterator<SEQUENCE_T> &operator-= (difference_type n) const
    {
      // Move back n elements.
      this->pos_ -= n;
      return * this;
    }

  /// Iterator subtraction
  Const_MM_Sequence_Iterator<SEQUENCE_T> operator- (difference_type n) const
    {
      // Create a temporary to return and move it back n elements.
      return Const_MM_Sequence_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Difference
  difference_type operator- (const Const_MM_Sequence_Iterator<SEQUENCE_T> & rhs) const
    {
      // I think the order is important here (i.e., this before rhs).
      return this->pos_ - rhs.pos_;
    }

  /// Element operator
  const_element_type operator[] (difference_type n) const
    {
      // Return the element at position pos_ + n
      return const_element_type ((*(this->sequence_))[this->pos_ + n],
                                 this->sequence_->release ());
    }

  /// Less than
  virtual bool operator< (const Const_MM_Sequence_Iterator<SEQUENCE_T> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      return this->pos_ < rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_MM_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Const_MM_Sequence_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the sequence with which we are dealing
  const implementation_type *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_MM_Sequence_Iterator<SEQUENCE_T> operator+
   (typename MM_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    Const_MM_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements.
     return Const_MM_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_MM_Sequence_Iterator<SEQUENCE_T> operator-
   (typename MM_Sequence_Iterator<SEQUENCE_T>::difference_type n,
    Const_MM_Sequence_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements.
     return Const_MM_Sequence_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }


/**
 * @class MM_Sequence_Reverse_Iterator
 * @brief Implements a reverse random access iterator for generic sequence type
 *        classes that need to manage memory. MM stands for memory managed.
 */
template <typename SEQUENCE_T>
class MM_Sequence_Reverse_Iterator 
{
  friend class Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::element_type element_type;
  typedef typename SEQUENCE_T::const_element_type const_element_type;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef typename SEQUENCE_T::implementation_type implementation_type;
  typedef int difference_type;

  /// Construct a MM_Sequence_Reverse_Iterator at position pos
  MM_Sequence_Reverse_Iterator (implementation_type *sequence, 
                                 size_t pos)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  MM_Sequence_Reverse_Iterator (
	MM_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }
  
  /// Assignment operator
  MM_Sequence_Reverse_Iterator & operator= (
	MM_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    {
      MM_Sequence_Reverse_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) 
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  element_type operator* (void)
    {
      // Access the underlying element in the sequence.
      return element_type ((*(this->sequence_))[this->pos_],
                           this->sequence_->release ());
    }

  /// Returns a const reference to the item contained at the current position
  const_element_type operator* (void) const
    {
      // Access the underlying element in the sequence.
      return const_element_type ((*(this->sequence_))[this->pos_],
                                 this->sequence_->release ());
    }

  /// Preincrement operator
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator++ (void)
    {
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return *this;
    }

  /// Postincrement operator
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator++ (int)
    {
      // Create a temporary to return so that it's not modified.
      MM_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now past the end.
      --this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-- (void)
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator-- (int)
    {
      // Create a temporary to return so that it's not modified.
      MM_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator+= (difference_type n)
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator addition
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator+ (difference_type n)
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return MM_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-= (difference_type n)
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      return *this;
    }

  /// Iterator subtraction
  MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator- (difference_type n)
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return MM_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Difference
  difference_type operator- (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  element_type operator[] (difference_type n)
    {
      // Return the element at position pos_ + n
      return element_type ((*(this->sequence_))[this->pos_ + n],
                           this->sequence_->release ());
    }

  /// Element operator/assignment
  const_element_type operator[] (difference_type n) const
    {
      // Return the element at position pos_ + n
      return const_element_type ((*(this->sequence_))[this->pos_ + n],
                                 this->sequence_->release ());
    }

  /// Less than
  bool operator< (const MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Less than
  bool operator< (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs)
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }


  /// Equality operator
  bool operator== (const MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
           && this->pos_ == rhs.pos_;
  }

  /// Equality operator
  bool operator== (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
           && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

  /// Nonequality operator
  bool operator!= (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// The sequence with which we are dealing
  implementation_type *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator+
   (typename MM_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    MM_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return MM_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator-
   (typename MM_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    MM_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return MM_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }


/**
 * @class Const_MM_Sequence_Reverse_Iterator
 * @brief Implements a constant reverse random access iterator for generic
 *        sequence type classes that need to manage memory. MM stands for
 *        memory managed.
 *
 * Note:  Having a const Iterator does not guarantee that the current
 * *position* that it points to will not change, it only guarantees that
 * you cannot change the underlying array!
 */
template <typename SEQUENCE_T>
class Const_MM_Sequence_Reverse_Iterator 
{
  friend class MM_Sequence_Reverse_Iterator<SEQUENCE_T>;

public:
  // = Necessary traits
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename SEQUENCE_T::value_type value_type;
  typedef typename SEQUENCE_T::value_type *pointer;
  typedef typename SEQUENCE_T::value_type &reference;
  typedef typename SEQUENCE_T::const_element_type const_element_type;
  typedef typename SEQUENCE_T::allocation_traits allocation_traits;
  typedef typename SEQUENCE_T::element_traits element_traits;
  typedef typename SEQUENCE_T::implementation_type implementation_type;
  typedef int difference_type;

  /// Construct a Const_MM_Sequence_Reverse_Iterator at position pos
  /// using a const sequence.
  Const_MM_Sequence_Reverse_Iterator (const implementation_type *sequence, 
                                       size_t pos)
    : sequence_ (sequence),
    pos_ (pos)
      {
      }

  /// Copy constructor
  Const_MM_Sequence_Reverse_Iterator (
	Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }

  /// Copy constructor
  Const_MM_Sequence_Reverse_Iterator (
	MM_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    : sequence_ (rhs.sequence_),
    pos_ (rhs.pos_)
      {
      }
  
  /// Assignment operator
  Const_MM_Sequence_Reverse_Iterator & operator= (
	Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> const & rhs)
    {
      Const_MM_Sequence_Reverse_Iterator tmp(rhs);
      swap (tmp);
      return * this;
    }

  /// swaps all members
  void swap(Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) 
    throw()
    {
      std::swap (sequence_, rhs.sequence_);
      std::swap (this->pos_, rhs.pos_);
    }

  /// typecast operator to MM_Sequence_Reverse_Iterator

  /// Dereference operator returns a reference to the item contained
  /// at the current position
  const_element_type operator* (void) const
    {
      // Access the underlying element in the sequence.
      return const_element_type ((*(this->sequence_))[this->pos_],
                                 this->sequence_->release ());
    }

  /// Preincrement operator
  const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator++ (void) const
    {
      // Decrement the position for reveres iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return *this;
    }

  /// Postincrement operator
  Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator++ (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Decrement the position for reverse iterators.
      // We also need to check if we're now before the start.
      --this->pos_;
      return temp_iter;
    }

  /// Predecrement operator
  const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-- (void) const
    {
      // Increment the position for reverse iterators.
      ++this->pos_;
      return *this;
    }

  /// Postdecrement operator
  Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator-- (int) const
    {
      // Create a temporary to return so that it's not modified.
      Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> temp_iter (this->sequence_, this->pos_);
      // Increment the position for reverse iterators.
      ++this->pos_;
      return temp_iter;
    }

  /// Iterator addition
  const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator+= (difference_type n) const
    {
      // Move back n elements for reverse iterators.
      this->pos_ -= n;
      return *this;
    }

  /// Iterator addition
  Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator+ (difference_type n) const
    {
      // Create a temporary to return and move it back n elements for
      // reverse iterators.
      return Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ - n);
    }

  /// Iterator subtraction
  const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &operator-= (difference_type n) const
    {
      // Move ahead n elements for reverse iterators.
      this->pos_ += n;
      return *this;
    }

  /// Iterator subtraction
  Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator- (difference_type n) const
    {
      // Create a temporary to return and move it ahead n elements for
      // reverse iterators.
      return Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> (this->sequence_, this->pos_ + n);
    }

  /// Difference
  difference_type operator- (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) const
    {
      // I think order is important here (i.e., rhs before this).
      return rhs.pos_ - this->pos_;
    }

  /// Element operator/assignment
  const_element_type operator[] (difference_type n) const
    {
      // Return the element at position pos_ + n
      return const_element_type ((*(this->sequence_))[this->pos_ + n],
                                 this->sequence_->release ());
    }

  /// Less than
  bool operator< (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> & rhs) const
    {
      // Return if this iterator is less than the passed in iterator.
      // For reverse iterators reverse the logic.
      return this->pos_ > rhs.pos_;
    }

  /// Equality operator
  bool operator== (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    // Compare all the data members for equality.
    return this->sequence_ == rhs.sequence_
        && this->pos_ == rhs.pos_;
  }

  /// Nonequality operator
  bool operator!= (const Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> &rhs) const
  {
    return !(*this == rhs);
  }

private:
  /// the sequence with which we are dealing
  const implementation_type *sequence_;

  /// Our current position in the sequence.
  mutable difference_type pos_;
};

/// Iterator addition with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator+
   (typename Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it back n elements for reverse iterators.
     return Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ - n);
   }

/// Iterator subtraction with the difference_type being the first argument.
template<typename SEQUENCE_T>
 Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> operator-
   (typename Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T>::difference_type n,
    Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> iter)
   {
     // Create an iter and move it ahead n elements for reverse iterators.
     return Const_MM_Sequence_Reverse_Iterator<SEQUENCE_T> (iter.sequence_, iter.pos_ + n);
   }


} // namespace TAO
  
#endif // TAO_HAS_SEQUENCE_ITERATORS

#endif // mm_sequence_iterator_h
