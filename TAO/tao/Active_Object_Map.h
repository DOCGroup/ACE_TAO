// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Active_Object_Map.h
//
// = AUTHOR
//    Aniruddha Gokhale
//    Irfan Pyarali
//    Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_ACTIVE_OBJECT_MAP_H
#define TAO_ACTIVE_OBJECT_MAP_H

#include "ace/Hash_Map_Manager_T.h"
#include "tao/corbafwd.h"
#include "tao/Servant_Base.h"

class TAO_Export TAO_ObjectId_Hash
{
public:
  u_long operator () (const PortableServer::ObjectId &id) const;
  // Returns hash value.
};

// Defined in Stub.cpp for TAO_opaque (an alias of
// PortableServer::ObjectId).
extern TAO_Export int operator== (const PortableServer::ObjectId &l,
                                  const PortableServer::ObjectId &r);

class TAO_Export TAO_Active_Object_Map_Entry
{
  // = TITLE
  //
  //     Common entry for all maps
  //
public:
  TAO_Active_Object_Map_Entry (void);
  // Constructor

  PortableServer::ObjectId id_;
  // Object id

  CORBA::ULong generation_;
  // Generation count

  PortableServer::Servant servant_;
  // Servant pointer

  int is_free_;
  // Is the entry currently in use
};

// Forward declaration
class TAO_Active_Object_Map_Iterator_Impl;

class TAO_Export TAO_Active_Object_Map_Iterator
{
  // = TITLE
  //
  //   Bridge for abstract iterator.
  //
  // = DESCRIPTION
  //
  //   This class provides the "abstraction" for iteration over the
  //   active object maps.
  //
public:
  TAO_Active_Object_Map_Iterator (TAO_Active_Object_Map_Iterator_Impl *impl);
  // Constructor taking an implementation.

  TAO_Active_Object_Map_Iterator (const TAO_Active_Object_Map_Iterator &x);
  TAO_Active_Object_Map_Iterator &operator= (const TAO_Active_Object_Map_Iterator &x);
  virtual ~TAO_Active_Object_Map_Iterator (void);
  // This is a well behaved class

  const TAO_Active_Object_Map_Entry &operator *(void) const;
  // Return the current item.

  TAO_Active_Object_Map_Iterator operator++ (void);
  TAO_Active_Object_Map_Iterator operator++ (int);
  // Increase the current position.

  friend int operator== (const TAO_Active_Object_Map_Iterator &l,
                         const TAO_Active_Object_Map_Iterator &r);
  friend int operator!= (const TAO_Active_Object_Map_Iterator &l,
                         const TAO_Active_Object_Map_Iterator &r);
  // Compare two iterators.

protected:
  TAO_Active_Object_Map_Iterator_Impl *impl_;
};

// Forward declaration
class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;

class TAO_Export TAO_Active_Object_Map
{
  // = TITLE
  //
  //     Interface class for maintaining a mapping of object ids to
  //     pointers to servants.
  //
public:
  TAO_Active_Object_Map (int user_id_policy,
                         int unique_id_policy);
  // System creates map based on <user_id_policy> and
  // <unique_id_policy>

  virtual ~TAO_Active_Object_Map (void);
  // Destructor.

  int bind (const PortableServer::ObjectId &id,
            PortableServer::Servant servant);
  // Associate <id> with <servant>, returning 0 if object is
  // registered successfully, 1 if it's already registered, and -1 if
  // a failure occurs during registration.

  int unbind (const PortableServer::ObjectId &id,
              PortableServer::Servant &servant);
  // Remove any association among <id> and <servant>.  Returns 0 if
  // the operation was succesful, <-1> otherwise.

  int find (const PortableServer::ObjectId &id);
  // Returns 0 if there is a servant for <id> in the map, <-1>
  // otherwise.

  int find (const PortableServer::ObjectId &id,
            PortableServer::Servant &servant);
  // Find object associated with <id>.  If the <id> is found it sets
  // <servant> and returns 0.  If not found, <servant> is unchanged
  // and the value <-1> is returned.

  int find (const PortableServer::Servant servant);
  // Returns 0 if <servant> is in the map, <-1> otherwise.

  int find (const PortableServer::Servant servant,
            PortableServer::ObjectId &id);
  // This method is only used with unique ids.  Find the <id> for
  // <servant>.  Returns <-1> if <servant> is not found, returns <0>
  // if <servant> is found.

  PortableServer::ObjectId *create_object_id (PortableServer::Servant servant,
                                              CORBA::Environment &TAO_IN_ENV);
  // Create an object id

  virtual CORBA::ULong system_id_size (void) const;
  // Size of the system generated id.

  typedef TAO_Active_Object_Map_Iterator iterator;
  iterator begin (void) const;
  iterator end (void) const;

protected:
  TAO_Active_Object_Map (const TAO_Active_Object_Map &);
  TAO_Active_Object_Map &operator= (const TAO_Active_Object_Map &);
  // Disallow copying.

protected:
  TAO_Active_Object_Map_Impl *impl_;
  // Implementation pointer

  TAO_Reverse_Active_Object_Map_Impl *reverse_impl_;
  // Reverse implementation pointer

  int unique_id_policy_;
  // Flag to indicate whether we have the UNIQUE_ID policy or the
  // SYSTEM_ID policy
};

class TAO_Export TAO_Active_Object_Map_Iterator_Impl
{
  // = TITLE
  //
  //   Abstract iterator for all active object maps.
  //
  // = DESCRIPTION
  //
  //   We want to provide an common interface for the different active
  //   object maps and their iterators.  Active object maps are
  //   handled using base classes and virtual methods; but we must
  //   provide a "by value" interface for iterators.
  //
  //   To do this we use several well know patterns:
  //
  //     TAO_Active_Object_Map::iterator uses the Bridge pattern to
  //     give a consistent and "by-value" interface to all the
  //     iterators.  The base class for all the iterators is
  //     TAO_Active_Object_Map_Iterator, here we use external
  //     polymorphism to adapt all the iterators to this common
  //     interface.
  //
public:
  virtual ~TAO_Active_Object_Map_Iterator_Impl (void);
  // Dtor

  virtual TAO_Active_Object_Map_Iterator_Impl *clone (void) const = 0;
  // Make a copy of the iterator, pointing to the current position.

  virtual const TAO_Active_Object_Map_Entry &item (void) const = 0;
  // Obtain the current item

  virtual void advance (void) = 0;
  // Advance to the next element.

  virtual int done (const TAO_Active_Object_Map_Iterator_Impl *end) const = 0;
  // Returns 1 if "this" points to the same position as <end>, returns
  // 0 otherwise.
};

class TAO_Export TAO_Active_Object_Map_Impl
{
  // = TITLE
  //
  //   Abstract class for maintaining a mapping from object ids to
  //   servants.
  //
  // = DESCRIPTION
  //
  // = NOTES
  //
  //   Iterators may return free entries, whose "int_id" (the servant)
  //   is 0.
  //
public:
  virtual ~TAO_Active_Object_Map_Impl (void);
  // Destructor.

  virtual TAO_Active_Object_Map_Iterator_Impl *begin () const = 0;
  virtual TAO_Active_Object_Map_Iterator_Impl *end () const = 0;
  // Iterator interface

  virtual int bind (const PortableServer::ObjectId &id,
                    PortableServer::Servant servant) = 0;
  virtual int unbind (const PortableServer::ObjectId &id,
                      PortableServer::Servant &servant) = 0;
  virtual int find (const PortableServer::ObjectId &id,
                    PortableServer::Servant &servant) = 0;
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant,
                                                      CORBA::Environment &TAO_IN_ENV) = 0;
  virtual CORBA::ULong system_id_size (void) const = 0;
  virtual int is_free (const TAO_Active_Object_Map_Entry &item) const = 0;
};

class TAO_Export TAO_Reverse_Active_Object_Map_Impl
{
  // = TITLE
  //
  //     Abstract base class for maintaining a mapping of servant to
  //     object ids.
public:
  TAO_Reverse_Active_Object_Map_Impl (void);
  // Constructor

  virtual ~TAO_Reverse_Active_Object_Map_Impl (void);
  // Destructor.

  virtual int bind (PortableServer::Servant servant,
                    const PortableServer::ObjectId &id) = 0;
  // Associate <servant> with <id>.

  virtual int unbind (PortableServer::Servant servant) = 0;
  // Remote <servant> from table.

  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id) = 0;
  // Find <id> of <servant>.

  virtual int find (PortableServer::Servant servant) = 0;
  // Find <servant> in the table.

private:
  TAO_Reverse_Active_Object_Map_Impl (const TAO_Reverse_Active_Object_Map_Impl &);
  TAO_Reverse_Active_Object_Map_Impl &operator= (const TAO_Reverse_Active_Object_Map_Impl &);
  // Disallow copying.
};

class TAO_Export TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy : public TAO_Reverse_Active_Object_Map_Impl
{
  // = TITLE
  //
  //     Table for maintaining a mapping of servant to object ids (for
  //     the UNIQUE_ID POA_Policy)
public:
  TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy (size_t size);
  // Constructor

  virtual ~TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy (void);
  // Destructor

  virtual int bind (PortableServer::Servant servant,
                    const PortableServer::ObjectId &id);
  // Associate <servant> with <id>.

  virtual int unbind (PortableServer::Servant servant);
  // Remote <servant> from table.

  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  // Find <id> of <servant>.

  virtual int find (PortableServer::Servant servant);
  // Find <servant> in the table.

protected:

  // = Typedef for the hash map
  typedef ACE_Hash_Map_Manager_Ex<PortableServer::Servant,
                                  PortableServer::ObjectId,
                                  TAO_Servant_Hash,
                                  ACE_Equal_To<PortableServer::Servant>,
                                  ACE_Null_Mutex>
          REVERSE_MAP;

  REVERSE_MAP map_;
  // Hash map instance
};

class TAO_Export TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy : public TAO_Reverse_Active_Object_Map_Impl
{
  // = TITLE
  //
  //     Table for maintaining a mapping of servant to object ids (for
  //     the MULTIPLE_ID POA_Policy)
public:
  TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy (void);
  // Constructor

  virtual ~TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy (void);
  // Destructor

  virtual int bind (PortableServer::Servant servant,
                    const PortableServer::ObjectId &id);
  // Associate <servant> with <id>.

  virtual int unbind (PortableServer::Servant servant);
  // Remote <servant> from table.

  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  // Find <id> of <servant>.

  virtual int find (PortableServer::Servant servant);
  // Find <servant> in the table.
};

/*******************************************************************/

// In the next section we implement a few concrete active object map
// implementations, namely:
//
//   TAO_Dynamic_Hash_Active_Object_Map, based on dynamic hashing
//   (ACE_Hash_Map_Manager).
//
//   TAO_Linear_Active_Object_Map, using linear search and a simple
//   dynamically growing array.
//
//   TAO_Active_Demux_Active_Object_Map, using also a dynamically
//   allocated array, but using active demultiplexing to do the
//   lookups.

/*******************************************************************/

class TAO_Export TAO_Dynamic_Hash_Active_Object_Map : public TAO_Active_Object_Map_Impl
{
  // = TITLE
  //
  //   Lookup strategy based on dynamic hashing.
  //
  // = DESCRIPTION
  //
  //   The active object map is implemented using a ACE_Hash_Map_Manager,
  //   the iterators are implemented using the ACE_Hash_Map_Iterator
  //   class.
  //
public:
  TAO_Dynamic_Hash_Active_Object_Map (CORBA::ULong size);

  virtual ~TAO_Dynamic_Hash_Active_Object_Map (void);

  virtual TAO_Active_Object_Map_Iterator_Impl *begin (void) const;
  virtual TAO_Active_Object_Map_Iterator_Impl *end (void) const;

  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  virtual int find (const PortableServer::ObjectId &id,
                    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id,
                    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
                      PortableServer::Servant &servant);
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant,
                                                      CORBA::Environment &TAO_IN_ENV);
  virtual CORBA::ULong system_id_size (void) const;
  virtual int is_free (const TAO_Active_Object_Map_Entry &item) const;

  typedef ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                                  PortableServer::Servant,
                                  TAO_ObjectId_Hash,
                                  ACE_Equal_To<PortableServer::ObjectId>,
                                  ACE_Null_Mutex>
          Hash_Map;

  typedef Hash_Map::iterator iterator;

protected:
  Hash_Map hash_map_;
  // Internal hash map.

  CORBA::ULong counter_;
  // Internal counter for generating unique ids
};

class TAO_Export TAO_Dynamic_Hash_Active_Object_Map_Iterator : public TAO_Active_Object_Map_Iterator_Impl
{
  // = TITLE
  //
  //   Iterator for TAO_Dynamic_Hash_Active_Object_Map.
  //
public:
  typedef TAO_Dynamic_Hash_Active_Object_Map::iterator Impl;

  TAO_Dynamic_Hash_Active_Object_Map_Iterator (const Impl &impl);
  virtual ~TAO_Dynamic_Hash_Active_Object_Map_Iterator (void);
  // default copy ctor and dtor

  // TAO_Active_Object_Map_Impl methods...
  virtual TAO_Active_Object_Map_Iterator_Impl *clone (void) const;
  virtual const TAO_Active_Object_Map_Entry &item (void) const;
  virtual void advance (void);
  virtual int done (const TAO_Active_Object_Map_Iterator_Impl *end) const;

protected:
  Impl impl_;

  TAO_Active_Object_Map_Entry entry_;
};

/****************************************************************/

class TAO_Export TAO_Array_Active_Object_Map_Iterator : public TAO_Active_Object_Map_Iterator_Impl
{
  // = TITLE
  //
  //   Iterator for TAO_Linear_Active_Object_Map and
  //   TAO_Active_Demux_Active_Object_Map
  //
public:
  TAO_Array_Active_Object_Map_Iterator (TAO_Active_Object_Map_Entry *pos);
  virtual ~TAO_Array_Active_Object_Map_Iterator (void);
  // default copy ctor and dtor

  // TAO_Active_Object_Map_Impl methods...
  virtual TAO_Active_Object_Map_Iterator_Impl *clone (void) const;
  virtual const TAO_Active_Object_Map_Entry &item (void) const;
  virtual void advance (void);
  virtual int done (const TAO_Active_Object_Map_Iterator_Impl *end) const;

protected:
  TAO_Active_Object_Map_Entry *pos_;
};

/****************************************************************/

class TAO_Export TAO_Linear_Active_Object_Map : public TAO_Active_Object_Map_Impl
{
  // = TITLE
  //
  //    Lookup strategy based on a simple linear search.  Not
  //    efficient, but most likely will always work.
  //
  // = DESCRIPTION
  //
  //   Uses a dynamic array to store the objects and linear search for
  //   the lookups.
  //
public:
  TAO_Linear_Active_Object_Map (CORBA::ULong size);
  virtual ~TAO_Linear_Active_Object_Map (void);

  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  virtual int find (const PortableServer::ObjectId &id,
                    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id,
                    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
                      PortableServer::Servant &servant);
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant,
                                                      CORBA::Environment &TAO_IN_ENV);
  virtual CORBA::ULong system_id_size (void) const;
  virtual int is_free (const TAO_Active_Object_Map_Entry &item) const;
  virtual TAO_Active_Object_Map_Iterator_Impl *begin () const;
  virtual TAO_Active_Object_Map_Iterator_Impl *end () const;

protected:

  enum
  {
    // Grow map exponentially up to 64K
    MAX_EXPONENTIAL = 64 *1024,

    // Afterwards grow in chunks of 32K
    LINEAR_INCREASE = 32 * 1024
  };

  virtual int resize (void);

  CORBA::ULong next_;
  CORBA::ULong mapsize_;
  TAO_Active_Object_Map_Entry *map_;
  PortableServer::ObjectId empty_id_;
  CORBA::ULong counter_;
};

/****************************************************************/

class TAO_Export TAO_Active_Demux_Active_Object_Map : public TAO_Linear_Active_Object_Map
{
  // = TITLE
  //
  //   An active object map lookup strategy based on active
  //   demultiplexing strategy.
  //
  // = DESCRIPTION
  //
  //   Use the linear active object map as the base; keys must be the
  //   string representation of the indices into the array and a
  //   generation count, so lookups can be done in O(1).
  //
  //   Iterators are implemented using pointers on the array.
  //
public:
  TAO_Active_Demux_Active_Object_Map (CORBA::ULong size);
  // Constructor, including an initial size.

  virtual ~TAO_Active_Demux_Active_Object_Map (void);
  // Destructor

  // Implement TAO_Dynamic_Hash_Active_Object_Map....
  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
                    PortableServer::ObjectId &id);
  virtual int find (const PortableServer::ObjectId &id,
                    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id,
                    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
                      PortableServer::Servant &servant);
  virtual CORBA::ULong system_id_size (void) const;
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant,
                                                      CORBA::Environment &TAO_IN_ENV);

protected:

  enum
  {
    INDEX_FIELD = 0,
    GENERATION_FIELD = 1
  };

  virtual CORBA::ULong next_free (void);

  virtual int parse_object_id (const PortableServer::ObjectId &id,
                               CORBA::ULong &index,
                               CORBA::ULong &generation);
};

#if defined (__ACE_INLINE__)
# include "tao/Active_Object_Map.i"
#endif /* ! __ACE_INLINE__ */

#endif /* TAO_ACTIVE_OBJECT_MAP_H */
