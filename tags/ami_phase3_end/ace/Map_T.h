/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Map_T.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef ACE_MAP_T_H
#define ACE_MAP_T_H

#include "ace/Map.h"
#include "ace/Pair.h"
#include "ace/Map_Manager.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Active_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Noop_Key_Generator
{
  // = TITLE
  //     Defines a noop key generator.
public:

  int operator() (T &);
  // Functor method: generates a new key.
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Incremental_Key_Generator
{
  // = TITLE
  //     Defines a simple incremental key generator.
  //
  // = DESCRIPTION
  //     Generates a new key of type T by incrementing current
  //     value. Requirements on T are:
  //
  //       - Constructor that accepts 0 in the constructor.
  //       - Prefix increment.
  //       - Assignment.
  //
  //     Note that a primitive types such as u_long, int, etc., are
  //     suitable for this class.
public:

  ACE_Incremental_Key_Generator (void);
  // Constructor.

  int operator() (T &t);
  // Functor method: generates a new key.

  T& current_value (void);
  // Returns the current value.

protected:

  T t_;
  // Current value.
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Iterator_Impl
{
  // = TITLE
  //     Defines a abstract iterator.
  //
  // = DESCRIPTION
  //     Implementation to be provided by subclasses.
public:

  virtual ~ACE_Iterator_Impl (void);
  // Destructor.

  virtual ACE_Iterator_Impl<T> *clone (void) const = 0;
  // Clone.

  virtual int compare (const ACE_Iterator_Impl<T> &rhs) const = 0;
  // Comparison.

  virtual T dereference () const = 0;
  // Dereference.

  virtual void plus_plus (void) = 0;
  // Advance.

  virtual void minus_minus (void) = 0;
  // Reverse.
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Reverse_Iterator_Impl
{
  // = TITLE
  //     Defines a abstract reverse iterator.
  //
  // = DESCRIPTION
  //     Implementation to be provided by subclasses.
public:

  virtual ~ACE_Reverse_Iterator_Impl (void);
  // Destructor.

  virtual ACE_Reverse_Iterator_Impl<T> *clone (void) const = 0;
  // Clone.

  virtual int compare (const ACE_Reverse_Iterator_Impl<T> &rhs) const = 0;
  // Comparison.

  virtual T dereference () const = 0;
  // Dereference.

  virtual void plus_plus (void) = 0;
  // Advance.

  virtual void minus_minus (void) = 0;
  // Reverse.
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Iterator
{
  // = TITLE
  //     Defines the iterator interface.
  //
  // = DESCRIPTION
  //     Implementation to be provided by forwarding.
public:

  // = Traits.
  typedef T value_type;
  typedef ACE_Iterator_Impl<T> implementation;

  ACE_Iterator (ACE_Iterator_Impl<T> *impl);
  // Constructor.

  ACE_Iterator (const ACE_Iterator<T> &rhs);
  // Copy constructor.

  ~ACE_Iterator (void);
  // Destructor.

  ACE_Iterator<T> &operator= (const ACE_Iterator<T> &rhs);
  // Assignment operator.

  int operator== (const ACE_Iterator<T> &rhs) const;
  int operator!= (const ACE_Iterator<T> &rhs) const;
  // Comparison operators.

  T operator* () const;
  // Dereference operator.

  ACE_Iterator<T> &operator++ (void);
  // Prefix advance.

  ACE_Iterator<T> operator++ (int);
  // Postfix advance.

  ACE_Iterator<T> &operator-- (void);
  // Prefix reverse.

  ACE_Iterator<T> operator-- (int);
  // Postfix reverse.

  ACE_Iterator_Impl<T> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Iterator_Impl<T> *implementation_;
  // Implementation pointer.
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class ACE_Reverse_Iterator
{
  // = TITLE
  //     Defines the reverse iterator interface.
  //
  // = DESCRIPTION
  //     Implementation to be provided by forwarding.
public:

  // = Traits.
  typedef T value_type;
  typedef ACE_Reverse_Iterator_Impl<T> implementation;

  ACE_Reverse_Iterator (ACE_Reverse_Iterator_Impl<T> *impl);
  // Constructor.

  ACE_Reverse_Iterator (const ACE_Reverse_Iterator<T> &rhs);
  // Copy constructor.

  ~ACE_Reverse_Iterator (void);
  // Destructor.

  ACE_Reverse_Iterator<T> &operator= (const ACE_Reverse_Iterator<T> &rhs);
  // Assignment operator.

  int operator== (const ACE_Reverse_Iterator<T> &rhs) const;
  int operator!= (const ACE_Reverse_Iterator<T> &rhs) const;
  // Comparison operators.

  T operator* () const;
  // Dereference operator.

  ACE_Reverse_Iterator<T> &operator++ (void);
  // Prefix advance.

  ACE_Reverse_Iterator<T> operator++ (int);
  // Postfix advance.

  ACE_Reverse_Iterator<T> &operator-- (void);
  // Prefix reverse.

  ACE_Reverse_Iterator<T> operator-- (int);
  // Postfix reverse.

  ACE_Reverse_Iterator_Impl<T> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Reverse_Iterator_Impl<T> *implementation_;
  // Implementation pointer.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE>
class ACE_Map
{
  // = TITLE
  //     Defines a map interface.
  //
  // = DESCRIPTION
  //     Implementation to be provided by subclasses.
public:

  // = Traits.
  typedef KEY key_type;
  typedef VALUE mapped_type;
  typedef ACE_Reference_Pair<const KEY, VALUE> value_type;
  typedef ACE_Iterator<value_type> iterator;
  typedef ACE_Reverse_Iterator<value_type> reverse_iterator;
  typedef ACE_Iterator_Impl<value_type> iterator_implementation;
  typedef ACE_Reverse_Iterator_Impl<value_type> reverse_iterator_implementation;

  virtual ~ACE_Map (void);
  // Close down and release dynamically allocated resources.

  virtual int open (size_t length = ACE_DEFAULT_MAP_SIZE,
                    ACE_Allocator *alloc = 0) = 0;
  // Initialize a <Map> with size <length>.

  virtual int close (void) = 0;
  // Close down a <Map> and release dynamically allocated resources.

  virtual int bind (const KEY &key,
                    const VALUE &value) = 0;
  // Add <key>/<value> pair to the map.  If <key> is already in the
  // map then no changes are made and 1 is returned.  Returns 0 on a
  // successful addition.  This function fails for maps that do not
  // allow user specified keys. <key> is an "in" parameter.

  virtual int bind_modify_key (const VALUE &value,
                               KEY &key) = 0;
  // Add <key>/<value> pair to the map.  <key> is an "inout" parameter
  // and maybe modified/extended by the map to add additional
  // information.  To recover original key, call the <recover_key>
  // method.

  virtual int bind_create_key (const VALUE &value,
                               KEY &key) = 0;
  // Add <value> to the map, and the corresponding key produced by the
  // Map is returned through <key> which is an "out" parameter.  For
  // maps that do not naturally produce keys, the map adapters will
  // use the <KEY_GENERATOR> class to produce a key.  However, the
  // users are responsible for not jeopardizing this key production
  // scheme by using user specified keys with keys produced by the key
  // generator.

  virtual int bind_create_key (const VALUE &value) = 0;
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Map. For maps that do not
  // naturally produce keys, the map adapters will use the
  // <KEY_GENERATOR> class to produce a key.  However, the users are
  // responsible for not jeopardizing this key production scheme by
  // using user specified keys with keys produced by the key
  // generator.

  virtual int recover_key (const KEY &modified_key,
                           KEY &original_key) = 0;
  // Recovers the original key potentially modified by the map during
  // <bind_modify_key>.

  virtual int rebind (const KEY &key,
                      const VALUE &value) = 0;
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      VALUE &old_value) = 0;
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      KEY &old_key,
                      VALUE &old_value) = 0;
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the map for maps that do not
  // allow user specified keys.  However, for maps that allow user
  // specified keys, if the key is not in the map, a new <key>/<value>
  // association is created.

  virtual int trybind (const KEY &key,
                       VALUE &value) = 0;
  // Associate <key> with <value> if and only if <key> is not in the
  // map.  If <key> is already in the map, then the <value> parameter
  // is overwritten with the existing value in the map. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  virtual int find (const KEY &key,
                    VALUE &value) = 0;
  // Locate <value> associated with <key>.

  virtual int find (const KEY &key) = 0;
  // Is <key> in the map?

  virtual int unbind (const KEY &key) = 0;
  // Remove <key> from the map.

  virtual int unbind (const KEY &key,
                      VALUE &value) = 0;
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  virtual size_t current_size (void) = 0;
  // Return the current size of the map.

  virtual size_t total_size (void) = 0;
  // Return the total size of the map.

  virtual void dump (void) const = 0;
  // Dump the state of an object.

  // = STL styled iterator factory functions.

  iterator begin (void);
  iterator end (void);
  // Return forward iterator.

  reverse_iterator rbegin (void);
  reverse_iterator rend (void);
  // Return reverse iterator.

protected:

  // = Protected no-op constructor.
  ACE_Map (void);

  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *begin_impl (void) = 0;
  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *end_impl (void) = 0;
  // Return forward iterator.

  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rbegin_impl (void) = 0;
  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rend_impl (void) = 0;
  // Return reverse iterator.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Map<KEY, VALUE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Map (const ACE_Map<KEY, VALUE> &))
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class IMPLEMENTATION, class ENTRY>
class ACE_Map_Impl_Iterator_Adapter : public ACE_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a iterator implementation for the Map_Impl class.
  //
  // = DESCRIPTION
  //     Implementation to be provided by <IMPLEMENTATION>.
public:

  // = Traits.
  typedef IMPLEMENTATION implementation;

  ACE_Map_Impl_Iterator_Adapter (const IMPLEMENTATION &impl);
  // Constructor.

  virtual ~ACE_Map_Impl_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  IMPLEMENTATION &impl (void);
  // Accessor to implementation object.

protected:

  IMPLEMENTATION implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class IMPLEMENTATION, class ENTRY>
class ACE_Map_Impl_Reverse_Iterator_Adapter : public ACE_Reverse_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a reverse iterator implementation for the Map_Impl class.
  //
  // = DESCRIPTION
  //     Implementation to be provided by IMPLEMENTATION.
public:

  // = Traits.
  typedef IMPLEMENTATION
          implementation;

  ACE_Map_Impl_Reverse_Iterator_Adapter (const IMPLEMENTATION &impl);
  // Constructor.

  virtual ~ACE_Map_Impl_Reverse_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Reverse_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Reverse_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  IMPLEMENTATION &impl (void);
  // Accessor to implementation object.

protected:

  IMPLEMENTATION implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY>
class ACE_Map_Impl : public ACE_Map<KEY, VALUE>
{
  // = TITLE
  //     Defines a map implementation.
  //
  // = DESCRIPTION
  //     Implementation to be provided by <IMPLEMENTATION>.
public:

  // = Traits.
  typedef ACE_Map_Impl_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, ITERATOR, ENTRY> iterator_impl;
  typedef ACE_Map_Impl_Reverse_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, REVERSE_ITERATOR, ENTRY> reverse_iterator_impl;
  typedef IMPLEMENTATION implementation;

  // = Initialization and termination methods.
  ACE_Map_Impl (ACE_Allocator *alloc = 0);
  // Initialize with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Map_Impl (size_t size,
                ACE_Allocator *alloc = 0);
  // Initialize with <size> entries.  The <size> parameter is ignore
  // by maps for which an initialize size does not make sense.

  virtual ~ACE_Map_Impl (void);
  // Close down and release dynamically allocated resources.

  virtual int open (size_t length = ACE_DEFAULT_MAP_SIZE,
                    ACE_Allocator *alloc = 0);
  // Initialize a <Map> with size <length>.

  virtual int close (void);
  // Close down a <Map> and release dynamically allocated resources.

  virtual int bind (const KEY &key,
                    const VALUE &value);
  // Add <key>/<value> pair to the map.  If <key> is already in the
  // map then no changes are made and 1 is returned.  Returns 0 on a
  // successful addition.  This function fails for maps that do not
  // allow user specified keys. <key> is an "in" parameter.

  virtual int bind_modify_key (const VALUE &value,
                               KEY &key);
  // Add <key>/<value> pair to the map.  <key> is an "inout" parameter
  // and maybe modified/extended by the map to add additional
  // information.  To recover original key, call the <recover_key>
  // method.

  virtual int bind_create_key (const VALUE &value,
                               KEY &key);
  // Add <value> to the map, and the corresponding key produced by the
  // Map is returned through <key> which is an "out" parameter.  For
  // maps that do not naturally produce keys, the map adapters will
  // use the <KEY_GENERATOR> class to produce a key.  However, the
  // users are responsible for not jeopardizing this key production
  // scheme by using user specified keys with keys produced by the key
  // generator.

  virtual int bind_create_key (const VALUE &value);
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Map. For maps that do not
  // naturally produce keys, the map adapters will use the
  // <KEY_GENERATOR> class to produce a key.  However, the users are
  // responsible for not jeopardizing this key production scheme by
  // using user specified keys with keys produced by the key
  // generator.

  virtual int recover_key (const KEY &modified_key,
                           KEY &original_key);
  // Recovers the original key potentially modified by the map during
  // <bind_modify_key>.

  virtual int rebind (const KEY &key,
                      const VALUE &value);
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      KEY &old_key,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the map for maps that do not
  // allow user specified keys.  However, for maps that allow user
  // specified keys, if the key is not in the map, a new <key>/<value>
  // association is created.

  virtual int trybind (const KEY &key,
                       VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // map.  If <key> is already in the map, then the <value> parameter
  // is overwritten with the existing value in the map. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  virtual int find (const KEY &key,
                    VALUE &value);
  // Locate <value> associated with <key>.

  virtual int find (const KEY &key);
  // Is <key> in the map?

  virtual int unbind (const KEY &key);
  // Remove <key> from the map.

  virtual int unbind (const KEY &key,
                      VALUE &value);
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  virtual size_t current_size (void);
  // Return the current size of the map.

  virtual size_t total_size (void);
  // Return the total size of the map.

  virtual void dump (void) const;
  // Dump the state of an object.

  IMPLEMENTATION &impl (void);
  // Accessor to implementation object.

protected:

  IMPLEMENTATION implementation_;
  // All implementation details are forwarded to this class.

  // = STL styled iterator factory functions.

  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *begin_impl (void);
  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *end_impl (void);
  // Return forward iterator.

  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rbegin_impl (void);
  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rend_impl (void);
  // Return reverse iterator.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Map_Impl (const ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY> &))
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class VALUE>
class ACE_Active_Map_Manager_Iterator_Adapter : public ACE_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a iterator implementation for the Active_Map_Manager_Adapter.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Active_Map_Manager::iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Active_Map_Manager<VALUE>::iterator implementation;

  ACE_Active_Map_Manager_Iterator_Adapter (const ACE_Map_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Active_Map_Manager_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Map_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Map_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class VALUE>
class ACE_Active_Map_Manager_Reverse_Iterator_Adapter : public ACE_Reverse_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a reverse iterator implementation for the Active_Map_Manager_Adapter.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Active_Map_Manager::reverse_iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Active_Map_Manager<VALUE>::reverse_iterator implementation;

  ACE_Active_Map_Manager_Reverse_Iterator_Adapter (const ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Active_Map_Manager_Reverse_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Reverse_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Reverse_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class KEY_ADAPTER>
class ACE_Active_Map_Manager_Adapter : public ACE_Map<KEY, VALUE>
{
  // = TITLE
  //     Defines a map implementation.
  //
  // = DESCRIPTION
  //     Implementation to be provided by <ACE_Active_Map_Manager>.
public:

  // = Traits.
  typedef ACE_Pair<KEY, VALUE> expanded_value;
  typedef ACE_Active_Map_Manager_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, expanded_value> iterator_impl;
  typedef ACE_Active_Map_Manager_Reverse_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, expanded_value> reverse_iterator_impl;
  typedef ACE_Active_Map_Manager<expanded_value> implementation;

  // = Initialization and termination methods.
  ACE_Active_Map_Manager_Adapter (ACE_Allocator *alloc = 0);
  // Initialize with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Active_Map_Manager_Adapter (size_t size,
                                  ACE_Allocator *alloc = 0);
  // Initialize with <size> entries.  The <size> parameter is ignore
  // by maps for which an initialize size does not make sense.

  virtual ~ACE_Active_Map_Manager_Adapter (void);
  // Close down and release dynamically allocated resources.

  virtual int open (size_t length = ACE_DEFAULT_MAP_SIZE,
                    ACE_Allocator *alloc = 0);
  // Initialize a <Map> with size <length>.

  virtual int close (void);
  // Close down a <Map> and release dynamically allocated resources.

  virtual int bind (const KEY &key,
                    const VALUE &value);
  // Add <key>/<value> pair to the map.  If <key> is already in the
  // map then no changes are made and 1 is returned.  Returns 0 on a
  // successful addition.  This function fails for maps that do not
  // allow user specified keys. <key> is an "in" parameter.

  virtual int bind_modify_key (const VALUE &value,
                               KEY &key);
  // Add <key>/<value> pair to the map.  <key> is an "inout" parameter
  // and maybe modified/extended by the map to add additional
  // information.  To recover original key, call the <recover_key>
  // method.

  virtual int bind_create_key (const VALUE &value,
                               KEY &key);
  // Add <value> to the map, and the corresponding key produced by the
  // Map is returned through <key> which is an "out" parameter.  For
  // maps that do not naturally produce keys, the map adapters will
  // use the <KEY_GENERATOR> class to produce a key.  However, the
  // users are responsible for not jeopardizing this key production
  // scheme by using user specified keys with keys produced by the key
  // generator.

  virtual int bind_create_key (const VALUE &value);
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Map. For maps that do not
  // naturally produce keys, the map adapters will use the
  // <KEY_GENERATOR> class to produce a key.  However, the users are
  // responsible for not jeopardizing this key production scheme by
  // using user specified keys with keys produced by the key
  // generator.

  virtual int recover_key (const KEY &modified_key,
                           KEY &original_key);
  // Recovers the original key potentially modified by the map during
  // <bind_modify_key>.

  virtual int rebind (const KEY &key,
                      const VALUE &value);
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      KEY &old_key,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the map for maps that do not
  // allow user specified keys.  However, for maps that allow user
  // specified keys, if the key is not in the map, a new <key>/<value>
  // association is created.

  virtual int trybind (const KEY &key,
                       VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // map.  If <key> is already in the map, then the <value> parameter
  // is overwritten with the existing value in the map. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  virtual int find (const KEY &key,
                    VALUE &value);
  // Locate <value> associated with <key>.

  virtual int find (const KEY &key);
  // Is <key> in the map?

  virtual int unbind (const KEY &key);
  // Remove <key> from the map.

  virtual int unbind (const KEY &key,
                      VALUE &value);
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  virtual size_t current_size (void);
  // Return the current size of the map.

  virtual size_t total_size (void);
  // Return the total size of the map.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_Active_Map_Manager<ACE_Pair<KEY, VALUE> > &impl (void);
  // Accessor to implementation object.

  KEY_ADAPTER &key_adapter (void);
  // Accessor to key adapter.

protected:

  virtual int find (const KEY &key,
                    expanded_value *&internal_value);
  // Find helper.

  virtual int unbind (const KEY &key,
                      expanded_value *&internal_value);
  // Unbind helper.

  ACE_Active_Map_Manager<ACE_Pair<KEY, VALUE> > implementation_;
  // All implementation details are forwarded to this class.

  KEY_ADAPTER key_adapter_;
  // Adapts between the user key and the Active_Map_Manager_Key.

  // = STL styled iterator factory functions.

  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *begin_impl (void);
  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *end_impl (void);
  // Return forward iterator.

  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rbegin_impl (void);
  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rend_impl (void);
  // Return reverse iterator.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Active_Map_Manager_Adapter (const ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER> &))
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS>
class ACE_Hash_Map_Manager_Ex_Iterator_Adapter : public ACE_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a iterator implementation for the Hash_Map_Manager_Adapter.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Hash_Map_Manager_Ex::iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>::iterator implementation;

  ACE_Hash_Map_Manager_Ex_Iterator_Adapter (const ACE_Hash_Map_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Hash_Map_Manager_Ex_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Hash_Map_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Hash_Map_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS>
class ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter : public ACE_Reverse_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a reverse iterator implementation for the Hash_Map_Manager_Adapter.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Hash_Map_Manager_Ex::reverse_iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>::reverse_iterator implementation;

  ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter (const ACE_Hash_Map_Reverse_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Reverse_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Reverse_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Hash_Map_Reverse_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Hash_Map_Reverse_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR>
class ACE_Hash_Map_Manager_Ex_Adapter : public ACE_Map<KEY, VALUE>
{
  // = TITLE
  //     Defines a map implementation.
  //
  // = DESCRIPTION
  //     Implementation to be provided by <ACE_Hash_Map_Manager_Ex>.
public:

  // = Traits.
  typedef ACE_Hash_Map_Manager_Ex_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, KEY, VALUE, HASH_KEY, COMPARE_KEYS> iterator_impl;
  typedef ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, KEY, VALUE, HASH_KEY, COMPARE_KEYS> reverse_iterator_impl;
  typedef ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> implementation;

  // = Initialization and termination methods.
  ACE_Hash_Map_Manager_Ex_Adapter (ACE_Allocator *alloc = 0);
  // Initialize with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Hash_Map_Manager_Ex_Adapter (size_t size,
                                   ACE_Allocator *alloc = 0);
  // Initialize with <size> entries.  The <size> parameter is ignore
  // by maps for which an initialize size does not make sense.

  virtual ~ACE_Hash_Map_Manager_Ex_Adapter (void);
  // Close down and release dynamically allocated resources.

  virtual int open (size_t length = ACE_DEFAULT_MAP_SIZE,
                    ACE_Allocator *alloc = 0);
  // Initialize a <Map> with size <length>.

  virtual int close (void);
  // Close down a <Map> and release dynamically allocated resources.

  virtual int bind (const KEY &key,
                    const VALUE &value);
  // Add <key>/<value> pair to the map.  If <key> is already in the
  // map then no changes are made and 1 is returned.  Returns 0 on a
  // successful addition.  This function fails for maps that do not
  // allow user specified keys. <key> is an "in" parameter.

  virtual int bind_modify_key (const VALUE &value,
                               KEY &key);
  // Add <key>/<value> pair to the map.  <key> is an "inout" parameter
  // and maybe modified/extended by the map to add additional
  // information.  To recover original key, call the <recover_key>
  // method.

  virtual int bind_create_key (const VALUE &value,
                               KEY &key);
  // Add <value> to the map, and the corresponding key produced by the
  // Map is returned through <key> which is an "out" parameter.  For
  // maps that do not naturally produce keys, the map adapters will
  // use the <KEY_GENERATOR> class to produce a key.  However, the
  // users are responsible for not jeopardizing this key production
  // scheme by using user specified keys with keys produced by the key
  // generator.

  virtual int bind_create_key (const VALUE &value);
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Map. For maps that do not
  // naturally produce keys, the map adapters will use the
  // <KEY_GENERATOR> class to produce a key.  However, the users are
  // responsible for not jeopardizing this key production scheme by
  // using user specified keys with keys produced by the key
  // generator.

  virtual int recover_key (const KEY &modified_key,
                           KEY &original_key);
  // Recovers the original key potentially modified by the map during
  // <bind_modify_key>.

  virtual int rebind (const KEY &key,
                      const VALUE &value);
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      KEY &old_key,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the map for maps that do not
  // allow user specified keys.  However, for maps that allow user
  // specified keys, if the key is not in the map, a new <key>/<value>
  // association is created.

  virtual int trybind (const KEY &key,
                       VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // map.  If <key> is already in the map, then the <value> parameter
  // is overwritten with the existing value in the map. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  virtual int find (const KEY &key,
                    VALUE &value);
  // Locate <value> associated with <key>.

  virtual int find (const KEY &key);
  // Is <key> in the map?

  virtual int unbind (const KEY &key);
  // Remove <key> from the map.

  virtual int unbind (const KEY &key,
                      VALUE &value);
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  virtual size_t current_size (void);
  // Return the current size of the map.

  virtual size_t total_size (void);
  // Return the total size of the map.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

  KEY_GENERATOR &key_generator (void);
  // Accessor to key generator.

protected:

  ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.

  KEY_GENERATOR key_generator_;
  // Functor class used for generating key.

  // = STL styled iterator factory functions.

  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *begin_impl (void);
  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *end_impl (void);
  // Return forward iterator.

  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rbegin_impl (void);
  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rend_impl (void);
  // Return reverse iterator.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Hash_Map_Manager_Ex_Adapter (const ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR> &))
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE>
class ACE_Map_Manager_Iterator_Adapter : public ACE_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a iterator implementation for the Map_Manager_Adapter.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Map_Manager::iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex>::iterator implementation;

  ACE_Map_Manager_Iterator_Adapter (const ACE_Map_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Map_Manager_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Map_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Map_Iterator<KEY, VALUE, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE>
class ACE_Map_Manager_Reverse_Iterator_Adapter : public ACE_Reverse_Iterator_Impl<T>
{
  // = TITLE
  //     Defines a reverse iterator implementation for the Map Manager.
  //
  // = DESCRIPTION
  //     Implementation to be provided by ACE_Map_Manager::reverse_iterator.
public:

  // = Traits.
  typedef ACE_TYPENAME ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex>::reverse_iterator implementation;

  ACE_Map_Manager_Reverse_Iterator_Adapter (const ACE_Map_Reverse_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl);
  // Constructor.

  virtual ~ACE_Map_Manager_Reverse_Iterator_Adapter (void);
  // Destructor.

  virtual ACE_Reverse_Iterator_Impl<T> *clone (void) const;
  // Clone.

  virtual int compare (const ACE_Reverse_Iterator_Impl<T> &rhs) const;
  // Comparison.

  virtual T dereference () const;
  // Dereference.

  virtual void plus_plus (void);
  // Advance.

  virtual void minus_minus (void);
  // Reverse.

  ACE_Map_Reverse_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

protected:

  ACE_Map_Reverse_Iterator<KEY, VALUE, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class KEY_GENERATOR>
class ACE_Map_Manager_Adapter : public ACE_Map<KEY, VALUE>
{
  // = TITLE
  //     Defines a map implementation.
  //
  // = DESCRIPTION
  //     Implementation to be provided by <ACE_Map_Manager>.
public:

  // = Traits.
  typedef ACE_Map_Manager_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, KEY, VALUE> iterator_impl;
  typedef ACE_Map_Manager_Reverse_Iterator_Adapter<ACE_Reference_Pair<const KEY, VALUE>, KEY, VALUE> reverse_iterator_impl;
  typedef ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex> implementation;

  // = Initialization and termination methods.
  ACE_Map_Manager_Adapter (ACE_Allocator *alloc = 0);
  // Initialize with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Map_Manager_Adapter (size_t size,
                           ACE_Allocator *alloc = 0);
  // Initialize with <size> entries.  The <size> parameter is ignore
  // by maps for which an initialize size does not make sense.

  virtual ~ACE_Map_Manager_Adapter (void);
  // Close down and release dynamically allocated resources.

  virtual int open (size_t length = ACE_DEFAULT_MAP_SIZE,
                    ACE_Allocator *alloc = 0);
  // Initialize a <Map> with size <length>.

  virtual int close (void);
  // Close down a <Map> and release dynamically allocated resources.

  virtual int bind (const KEY &key,
                    const VALUE &value);
  // Add <key>/<value> pair to the map.  If <key> is already in the
  // map then no changes are made and 1 is returned.  Returns 0 on a
  // successful addition.  This function fails for maps that do not
  // allow user specified keys. <key> is an "in" parameter.

  virtual int bind_modify_key (const VALUE &value,
                               KEY &key);
  // Add <key>/<value> pair to the map.  <key> is an "inout" parameter
  // and maybe modified/extended by the map to add additional
  // information.  To recover original key, call the <recover_key>
  // method.

  virtual int bind_create_key (const VALUE &value,
                               KEY &key);
  // Add <value> to the map, and the corresponding key produced by the
  // Map is returned through <key> which is an "out" parameter.  For
  // maps that do not naturally produce keys, the map adapters will
  // use the <KEY_GENERATOR> class to produce a key.  However, the
  // users are responsible for not jeopardizing this key production
  // scheme by using user specified keys with keys produced by the key
  // generator.

  virtual int bind_create_key (const VALUE &value);
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Map. For maps that do not
  // naturally produce keys, the map adapters will use the
  // <KEY_GENERATOR> class to produce a key.  However, the users are
  // responsible for not jeopardizing this key production scheme by
  // using user specified keys with keys produced by the key
  // generator.

  virtual int recover_key (const KEY &modified_key,
                           KEY &original_key);
  // Recovers the original key potentially modified by the map during
  // <bind_modify_key>.

  virtual int rebind (const KEY &key,
                      const VALUE &value);
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map for maps that do not allow user specified keys.
  // However, for maps that allow user specified keys, if the key is
  // not in the map, a new <key>/<value> association is created.

  virtual int rebind (const KEY &key,
                      const VALUE &value,
                      KEY &old_key,
                      VALUE &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the map for maps that do not
  // allow user specified keys.  However, for maps that allow user
  // specified keys, if the key is not in the map, a new <key>/<value>
  // association is created.

  virtual int trybind (const KEY &key,
                       VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // map.  If <key> is already in the map, then the <value> parameter
  // is overwritten with the existing value in the map. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  virtual int find (const KEY &key,
                    VALUE &value);
  // Locate <value> associated with <key>.

  virtual int find (const KEY &key);
  // Is <key> in the map?

  virtual int unbind (const KEY &key);
  // Remove <key> from the map.

  virtual int unbind (const KEY &key,
                      VALUE &value);
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  virtual size_t current_size (void);
  // Return the current size of the map.

  virtual size_t total_size (void);
  // Return the total size of the map.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex> &impl (void);
  // Accessor to implementation object.

  KEY_GENERATOR &key_generator (void);
  // Accessor to key generator.

protected:

  ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex> implementation_;
  // All implementation details are forwarded to this class.

  KEY_GENERATOR key_generator_;
  // Functor class used for generating key.

  // = STL styled iterator factory functions.

  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *begin_impl (void);
  virtual ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *end_impl (void);
  // Return forward iterator.

  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rbegin_impl (void);
  virtual ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *rend_impl (void);
  // Return reverse iterator.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Map_Manager_Adapter (const ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR> &))
};

////////////////////////////////////////////////////////////////////////////////

#if defined (__ACE_INLINE__)
#include "ace/Map_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Map_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Map_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MAP_T_H */
