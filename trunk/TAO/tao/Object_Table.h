// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    objtable.h
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#ifndef TAO_OBJTABLE_H
#define TAO_OBJTABLE_H

struct TAO_Object_Table_Entry
{
  TAO_Object_Table_Entry (void);

  PortableServer::ObjectId id_;
  CORBA::ULong generation_;
  PortableServer::Servant servant_;
  int is_free_;
};

class TAO_Export TAO_Object_Table_Iterator_Impl
{
  // = TITLE
  //   Abstract iterator for all Object Table strategies.
  //
  // = DESCRIPTION
  //   We want to provide an common interface for the different object
  //   tables and their iterators.  Object Tables are handled using
  //   base classes and virtual methods; but we must provide a "by
  //   value" interface for iterators.
  //   To do this we use several well know patterns:
  //     TAO_Object_Table::iterator uses the Bridge pattern to give a
  //     consistent and "by-value" interface to all the iterators.
  //     The base class for all the iterators is
  //     TAO_Object_Table_Iterator, here we use external polymorphism
  //     to adapt all the iterators to this common interface.
  //
public:
  virtual ~TAO_Object_Table_Iterator_Impl (void);
  // dtor

  virtual TAO_Object_Table_Iterator_Impl *clone (void) const = 0;
  // Make a copy of the iterator, pointing to the current position.

  virtual const TAO_Object_Table_Entry &item (void) const = 0;
  // Obtain the current item

  virtual void advance (void) = 0;
  // Advance to the next element.

  virtual int done (const TAO_Object_Table_Iterator_Impl *end) const = 0;
  // Returns 1 if "this" points to the same position as <end>, returns
  // 0 otherwise.
};

class TAO_Export TAO_Object_Table_Impl
{
  // = TITLE
  //   Abstract class for maintaining a mapping from ObjectId's to
  //   Servants.
  //
  // = DESCRIPTION
  //   
  // = NOTES
  //   Iterators may return free entries, whose "int_id" (the servant)
  //   is 0.
public:
  enum 
  {
    // Default table size
    DEFAULT_TABLE_SIZE = TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE
  };

  virtual ~TAO_Object_Table_Impl (void);
  // Destructor.

  virtual int bind (const PortableServer::ObjectId &id, 
		    PortableServer::Servant servant) = 0;
  // Associate <{id}> with <{servant}>, returning 0 if object is
  // registered successfully, 1 if it's already registered, and -1 if
  // a failure occurs during registration.

  virtual int unbind (const PortableServer::ObjectId &id,
		      PortableServer::Servant &servant) = 0;
  // Remote any association among <id> and <servant>.
  // Returns 0 if the operation was succesful, <-1> otherwise.

  virtual TAO_Object_Table_Iterator_Impl *begin () const = 0;
  virtual TAO_Object_Table_Iterator_Impl *end () const = 0;
  // Iterator interface

  virtual int find (const PortableServer::ObjectId &id, 
		    PortableServer::Servant &servant) = 0;
  // Find object associated with <{id}>.
  // If the ObjectId is found it sets <{servant}> and returns a
  // non-negative integer.  If not found, <{servant}> is unchanged and
  // the value <-1> is returned. 

  virtual int find (const PortableServer::Servant servant);
  // Returns 0 if <servant> is in the table, <-1> otherwise.

  virtual int find (const PortableServer::ObjectId &id);
  // Returns 0 if there is a servant for <id> in the table, <-1>
  // otherwise.

  // This method is only used with unique ids.  Therefore, selecting
  // the first match is ok. Also the "default" implementation (linear
  // search on the iterator) makes sense, since the operation is not
  // time critical.
  virtual int find (const PortableServer::Servant servant,
		    PortableServer::ObjectId_out id);
  // Find the <id> for <servant>.
  // Returns <-1> if <servant> is not found or if more than one <id>
  // is associated with <servant>, returns <0> otherwise.

  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant, 
                                                      CORBA::Environment &env) = 0;
  // Create an object id

  virtual CORBA::ULong system_id_size (void) const = 0;
  // Size of the system generated id.
};

class TAO_Export TAO_Object_Table_Iterator
{
  // = TITLE
  //   Bridge for abstract iterator.
  //
  // = DESCRIPTION
  //   This class provides the "Abstraction" for iteration over Object
  //   Tables.
public:
  TAO_Object_Table_Iterator (TAO_Object_Table_Iterator_Impl *impl);
  // Constructor taking an implementation.

  TAO_Object_Table_Iterator (const TAO_Object_Table_Iterator &x);
  TAO_Object_Table_Iterator &operator=(const TAO_Object_Table_Iterator &x);
  virtual ~TAO_Object_Table_Iterator (void);
  // This is a well behaved class

  const TAO_Object_Table_Entry &operator *(void) const;
  // Return the current item.

  TAO_Object_Table_Iterator operator++ (void);
  TAO_Object_Table_Iterator operator++ (int);
  // Increase the current position.

  friend int operator==(const TAO_Object_Table_Iterator &l,
			const TAO_Object_Table_Iterator &r);
  friend int operator!=(const TAO_Object_Table_Iterator &l,
			const TAO_Object_Table_Iterator &r);
  // Compare two iterators.

protected:
  TAO_Object_Table_Iterator_Impl *impl_;
};

class TAO_Export TAO_Object_Table
  // = TITLE
  //     Abstract class for maintaining a mapping of CORBA object keys
  //     to pointers to CORBA objects.
{
public:
  TAO_Object_Table (TAO_Object_Table_Impl *impl, 
                    int delete_impl = 0);
  // Constructor: Must pass valid <impl>

  TAO_Object_Table (int user_id_policy);
  // Constructor: System creates table based on <user_id_policy>
  
  virtual ~TAO_Object_Table (void);
  // Destructor.

  int bind (const PortableServer::ObjectId &id, 
	    PortableServer::Servant servant);
  // Associate <{id}> with <{servant}>, returning 0 if object is
  // registered successfully, 1 if it's already registered, and -1 if
  // a failure occurs during registration.

  int unbind (const PortableServer::ObjectId &id,
	      PortableServer::Servant &servant);
  // Remote any association among <id> and <servant>.
  // Returns 0 if the operation was succesful, <-1> otherwise.

  int find (const PortableServer::ObjectId &id, 
	    PortableServer::Servant &servant);
  // Find object associated with <{id}>.
  // If the ObjectId is found it sets <{servant}> and returns a
  // non-negative integer.  If not found, <{servant}> is unchanged and
  // the value <-1> is returned. 

  int find (const PortableServer::Servant servant);
  // Returns 0 if <servant> is in the table, <-1> otherwise.

  int find (const PortableServer::ObjectId &id);
  // Returns 0 if there is a servant for <id> in the table, <-1>
  // otherwise.

  // This method is only used with unique ids.  Therefore, selecting
  // the first match is ok. Also the "default" implementation (linear
  // search on the iterator) makes sense, since the operation is not
  // time critical.
  int find (const PortableServer::Servant servant,
	    PortableServer::ObjectId_out id);
  // Find the <id> for <servant>.
  // Returns <-1> if <servant> is not found or if more than one <id>
  // is associated with <servant>, returns <0> otherwise.

  PortableServer::ObjectId *create_object_id (PortableServer::Servant servant, 
                                              CORBA::Environment &env);
  // Create an object id

  virtual CORBA::ULong system_id_size (void) const;
  // Size of the system generated id.

  typedef TAO_Object_Table_Iterator iterator;
  iterator begin (void) const;
  iterator end (void) const;

protected:
  TAO_Object_Table (const TAO_Object_Table&);
  TAO_Object_Table &operator= (const TAO_Object_Table&);
  // disallow copying.

protected:
  TAO_Object_Table_Impl *impl_;
  // Implementation pointer

  int delete_impl_;
  // Flag to know if impl should be deleted
};

/****************************************************************/

// In the next section we implement a few concrete object table
// implementations, namely:
//   TAO_Dynamic_Hash_ObjTable, based on dynamic hashing
//   (ACE_Hash_Map_Manager).
//   TAO_Linear_ObjTable, using linear search and a simple dynamically
//   growing array.
//   TAO_Active_Demux_ObjTable, using also a dynamically allocated
//   array, but using active demultiplexing to do the lookups.

/****************************************************************/

class TAO_Export TAO_Dynamic_Hash_ObjTable : public TAO_Object_Table_Impl
{
  // = TITLE
  //   Lookup strategy based on dynamic hashing. This works on the
  //   assumption that the object keys are essentially strings.
  //
  // = DESCRIPTION
  //   The object table is implemented using a ACE_Hash_Map_Manager,
  //   the iterators are implemented using the ACE_Hash_Map_Iterator
  //   class.
  //
public:
  TAO_Dynamic_Hash_ObjTable (CORBA::ULong size);
  // constructor. If size is 0, some default is used.
  
  virtual ~TAO_Dynamic_Hash_ObjTable (void);

  // Implement TAO_Dynamic_Hash_ObjTable....
  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
		    PortableServer::ObjectId_out id);
  virtual int find (const PortableServer::ObjectId &id, 
		    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id, 
		    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
		      PortableServer::Servant &servant);
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant, 
                                                      CORBA::Environment &env);
  virtual CORBA::ULong system_id_size (void) const;
  virtual TAO_Object_Table_Iterator_Impl *begin (void) const;
  virtual TAO_Object_Table_Iterator_Impl *end (void) const;

  // Dynamic Hashing scheme using template specialization for char*
  typedef ACE_Hash_Map_Manager<PortableServer::ObjectId,
                               PortableServer::Servant,
                               ACE_SYNCH_NULL_MUTEX> 
          Hash_Map;
  typedef ACE_Hash_Map_Iterator<PortableServer::ObjectId,
                                PortableServer::Servant,
                                ACE_SYNCH_NULL_MUTEX> 
          Iterator;

protected:
  Hash_Map hash_map_;
  // internal hash table.
};

class TAO_Export TAO_Dynamic_Hash_ObjTable_Iterator : public TAO_Object_Table_Iterator_Impl
{
  // = TITLE
  //   Iterator for TAO_Dynamic_Hash_ObjTable.
public:
  typedef TAO_Dynamic_Hash_ObjTable::Iterator Impl;
  TAO_Dynamic_Hash_ObjTable_Iterator (const Impl &impl);
  virtual ~TAO_Dynamic_Hash_ObjTable_Iterator (void);

  // default copy ctor, dtor and operator=

  // TAO_Object_Table_Impl methods...
  virtual TAO_Object_Table_Iterator_Impl *clone (void) const;
  virtual const TAO_Object_Table_Entry &item (void) const;
  virtual void advance (void);
  virtual int done (const TAO_Object_Table_Iterator_Impl *end) const;

protected:
  Impl impl_;
  TAO_Object_Table_Entry entry_;
};

/****************************************************************/

class TAO_Export TAO_Array_ObjTable_Iterator : public TAO_Object_Table_Iterator_Impl
{
  // = TITLE
  //   Iterator for TAO_Linear_ObjTable and TAO_Active_Demux_ObjTable
public:
  TAO_Array_ObjTable_Iterator (TAO_Object_Table_Entry *pos);
  virtual ~TAO_Array_ObjTable_Iterator (void);

  // default copy ctor, dtor and operator=

  // TAO_Object_Table_Impl methods...
  virtual TAO_Object_Table_Iterator_Impl *clone (void) const;
  virtual const TAO_Object_Table_Entry &item (void) const;
  virtual void advance (void);
  virtual int done (const TAO_Object_Table_Iterator_Impl *end) const;

protected:
  TAO_Object_Table_Entry *pos_;
};

/****************************************************************/

class TAO_Export TAO_Linear_ObjTable : public TAO_Object_Table_Impl
{
  // = TITLE
  //    Lookup strategy based on a simple linear search.  Not
  //    efficient, but most likely will always work.
  //
  // = DESCRIPTION
  //   Uses a dynamic array to store the objects and linear search
  //   for the lookups.
  //
public:
  
  enum 
  {
    // Grow table exponentially up to 64K
    MAX_EXPONENTIAL = 65536, 

    // Afterwards grow in chunks of 32K
    LINEAR_INCREASE = 32768  
  };

  TAO_Linear_ObjTable (CORBA::ULong size);
  virtual ~TAO_Linear_ObjTable (void);

  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
		    PortableServer::ObjectId_out id);
  virtual int find (const PortableServer::ObjectId &id, 
		    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id, 
		    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
		      PortableServer::Servant &servant);
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant, 
                                                      CORBA::Environment &env);
  virtual CORBA::ULong system_id_size (void) const;
  virtual TAO_Object_Table_Iterator_Impl *begin () const;
  virtual TAO_Object_Table_Iterator_Impl *end () const;

protected:

  virtual int resize (void);

  CORBA::ULong next_;
  CORBA::ULong tablesize_;
  TAO_Object_Table_Entry *table_;
  PortableServer::ObjectId empty_id_;
  CORBA::ULong counter_;
};

/****************************************************************/

class TAO_Export TAO_Active_Demux_ObjTable : public TAO_Linear_ObjTable
{
  // = TITLE
  //   An object table lookup strategy based on active
  //   demultiplexing strategy.
  //
  // = DESCRIPTION
  //
  //   Use the linear object table as the base; keys must be the
  //   string representation of the indices into the array and a
  //   generation count, so lookups can be done in O(1).
  //
  //   Iterators are implemented using pointers on the array.
public:
  TAO_Active_Demux_ObjTable (CORBA::ULong size);
  // Constructor, including an initial size.

  virtual ~TAO_Active_Demux_ObjTable (void);
  // Destructor

  // Implement TAO_Dynamic_Hash_ObjTable....
  virtual int find (const PortableServer::Servant servant);
  virtual int find (const PortableServer::ObjectId &id);
  virtual int find (const PortableServer::Servant servant,
		    PortableServer::ObjectId_out id);
  virtual int find (const PortableServer::ObjectId &id, 
		    PortableServer::Servant &servant);
  virtual int bind (const PortableServer::ObjectId &id, 
		    PortableServer::Servant servant);
  virtual int unbind (const PortableServer::ObjectId &id,
		      PortableServer::Servant &servant);
  virtual CORBA::ULong system_id_size (void) const;
  virtual PortableServer::ObjectId *create_object_id (PortableServer::Servant servant, 
                                                      CORBA::Environment &env);

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
# include "tao/Object_Table.i"
#endif /* ! __ACE_INLINE__ */

#endif /* TAO_OBJTABLE_H */
