// -*- C++ -*-

//=============================================================================
/**
 *  @file    Hash_Map_Manager_RT_T.h
 *
 *  $Id$
 *
 *  @author Jeremy M. Nolle <jmn3@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_HASH_MAP_RT_MANAGER_RT_H
#define ACE_HASH_MAP_RT_MANAGER_RT_H
#include "pre.h"

#include "config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Default_Constants.h"
#include "Functor.h"
#include "Log_Msg.h"

/**
 * @class ACE_Hash_Map_RT_Coord
 *
 * @brief An int pair for an Entry's (table, bucket) position
 */
class ACE_Hash_Map_RT_Coord
{
public:
  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Coord (int table, int bucket);
  
  /// Destructor
  ~ACE_Hash_Map_RT_Coord (void);
  
  // = Accessor and mutator methods
  /// Return int table
  int get_table (void);
  
  /// Return int bucket
  int get_bucket (void);
  
  /// Set int table to new_table_number, return 0 for sucess.
  int set_table (int new_table_number);
  
  /// Set int bucket to new_bucket_number, return 0 for sucess.
  int set_bucket (int new_bucket_number);
  
private:
  /// Position relative to the hash_map of which table the ListItem is in.
  int table_;
  
  /// Position relative to the table_ of which Bucket the ListItem is in.
  int bucket_;
};

/**
 * @class ACE_Hash_Map_RT_Hash_Function
 *
 * @brief A class that holds the templated HASH_KEY and does the actual hashing.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY>
class ACE_Hash_Map_RT_Hash_Function
{
public:

  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Hash_Function (int number_of_tables,
                                 int buckets_per_table);
  
  /// Destructor
  ~ACE_Hash_Map_RT_Hash_Function (void);

  /// Take an EXT_ID and hash it, returning a ACE_Hash_Map_RT_Coord&
  int hash (const EXT_ID& ext_id,
            ACE_Hash_Map_RT_Coord& coord);
  
private:
  /// Internal function that does the actual hashing
  int hash_i (int key,
             ACE_Hash_Map_RT_Coord& coord);
  
  /// The number of tables that are available.
  size_t max_tables_;
  
  /// Number of buckets in each table.
  size_t buckets_per_table_;
  
  /// Function object used for hashing keys.
  HASH_KEY hash_key_;
};

/**
 * @class ACE_Hash_Map_RT_POD
 *
 * @brief "Point Of Design" knows when to change Hash_Functions, and how many 
 *         tables there are currently.  A nre POD is created when the number 
 *         of tables is no longer enough.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY>
class ACE_Hash_Map_RT_POD
{

public:
  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_POD (int num_of_tables,
                       ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>& func);
  
  /// Destructor
  ~ACE_Hash_Map_RT_POD (void);
  
  /// All hash requests will come through the POD, which will pass them to the 
  /// ACE_Hash_Map_RT_Hash_Function
  int hash (const EXT_ID& exit_id,
            ACE_Hash_Map_RT_Coord& coord);
  
  // = Accessor and mutator methods
  /// Get the number of available tables of this POD
  size_t get_num_tables(void);
  
private:
  /// The number of available tables
  int num_tables_;
  
  /// the ACE_Hash_Map_RT_Hash_Function that does the hashing for this POD
  ACE_Hash_Map_RT_Hash_Function<EXT_ID, INT_ID, HASH_KEY>& func_;
  
};

/**
 * @class ACE_Hash_Map_RT_Entry
 *
 * @brief Define an Entry that has a (key, value) pair in the hash table.
 *        Each Entry will be contained inside a ACE_Hash_Map_RT_ListItem, and
 *        a Bucket is a list of ACE_Hash_Map_RT_ListItem s.
 */
template <class EXT_ID, class INT_ID>
class ACE_Hash_Map_RT_Entry
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Hash_Map_RT_Entry (const EXT_ID& ext_id,
                         const INT_ID& int_id);
  
  ACE_Hash_Map_RT_Entry (void);
  
# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  /// Destructor.
  ~ACE_Hash_Map_RT_Entry (void);
#endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */
  
  // = Accessor and mutator methods  
  EXT_ID& get_ext_id (void);
  
  INT_ID& get_int_id (void);
  
  void set_ext_id (EXT_ID);
  
  void set_int_id (INT_ID);
  
  /// Key used to look up an entry.
  EXT_ID ext_id_;
  
  /// The contents (value) of the entry.
  INT_ID int_id_;
};

/**
 * @class ACE_Hash_Map_RT_ListItem
 *
 * @brief Has an Entry, and a pointer to the next ListItem in the Bucket.
 *        A Bucket stores ListItems. A ListItem has an Entry.  Entries have (key, value).
 */
template <class EXT_ID, class INT_ID>
class ACE_Hash_Map_RT_ListItem
{
public:
  typedef ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID> LITEM;
  typedef ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID> ENTRY;
  
  // = Initialization and termination methods.
  /// Constructors
  ACE_Hash_Map_RT_ListItem (ENTRY* entry,
                            LITEM* next);
  
  // Default constructor
  ACE_Hash_Map_RT_ListItem (void);

  /// Destructor
  ~ACE_Hash_Map_RT_ListItem (void);
  
  /// The Entry object
  ENTRY* entry_;
  
  /// Pointer to the next item
  ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>* next_;
  
};

/**
 * @class ACE_Hash_Map_RT_Bucket
 *
 * @brief A Bucket has a list of ListItems.
 *        A ListItem has an Entry.  Entries have (key, value).
 */
template <class EXT_ID, class INT_ID, class HASH_KEY>
class ACE_Hash_Map_RT_Bucket
{
public:
  
  typedef ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY> POD;
  typedef ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID> LITEM;
  typedef ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID> ENTRY;

  // = Initialization and termination methods.
  /// Constructors
  ACE_Hash_Map_RT_Bucket (POD* pod);

  ACE_Hash_Map_RT_Bucket (void);

  /// Destructor
  ~ACE_Hash_Map_RT_Bucket (void);
  
  // = Accessor and mutator methods
  /// Get the current length of the Bucket (that is, how many ListItems are present)
  size_t get_length (void);

  int set_length (int new_length);
  
  /// Incr the current length by one.
  int incr_length (void);
  
  /// Decr the current length by one.
  int decr_length (void);

  int is_clean (POD& manager_POD);

  /// Add a new ListItem (which will contain the passed Entry) to the front of this 
  /// Bucket's list
  int prepend (ENTRY& entry);

  /// Return the POD
  //POD get_POD (void);
  
  /// Set the POD
  void set_POD (POD* pod);
  
  /// Create a new head, and set the POD to the passed one.
  int empty_bucket (POD* new_POD);
 
  /// Pointer to the first element in the Bucket.
  /// This ListItem will never have a key, value.
  LITEM* head_;
  
  /// A pointer to the last ListItem in the Bucket.
  /// Initially this is set to the head_
  LITEM* last_;

protected:
  /// Initialises this Bucket, allocating memory
  int init_bucket_i (POD* pod);

private:
  /// Int counter of number of ListItems in the Bucket
  int length_;

  /// This Bucket's POD
  POD* curPOD_;
  
};

// forward declare
class ACE_Allocator;

/**
 * @class ACE_Hash_Map_RT_Table
 *
 * @brief A Table is an array of Buckets.
 *        A hashmap has an array of Tables, a Table has an array of Buckets.
 *        A Bucket stores ListItems. A ListItem has Entries.  Entries have (key, value)
 */
template <class EXT_ID, class INT_ID, class HASH_KEY>
class ACE_Hash_Map_RT_Table
{
public:
  
  typedef ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY> POD;
  typedef ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID> LITEM;
  typedef ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID> ENTRY;
  typedef ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY> BUCKET;

  friend class ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY>;
  
  // = Initialization and termination methods.
  //. Constructor  
  ACE_Hash_Map_RT_Table (ACE_Allocator* allocator,
                         size_t buckets_per_table,
                         POD* pod);

  /// Default constructor
  ACE_Hash_Map_RT_Table (void);

  /// Destructor
  ~ACE_Hash_Map_RT_Table(void);
  
  // = Accessor and mutator methods
  /// Pass the Bucket at Table[loc] back through the BUCKET& bucket, return 0 for sucess.
  int get_bucket (int loc,
                 BUCKET& bucket);

  /// Return the Bucket at Table[loc]
  BUCKET& get_bucket (int loc);
  
  int get_tablesize (void);

  /// Increase tablesize by one.
  void incr_tablesize (void);
  
  /// Decrease tablesize by num_removed
  void set_tablesize (int num_removed);
  
  /// Get the array of Buckets
  BUCKET* get_bucket_array (void);

protected:
  /// Initialise the Table, allocating memory and calling constructors for the Buckets
  int init_table_i (ACE_Allocator* allocator,
                    size_t buckets_per_table,
                    POD* pod);
private:
  /// The array of Buckets
  BUCKET *buckets_;

  /// an int counter of the number of Buckets
  int numBuckets;
};

// forward decl the manager
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> class ACE_Hash_Map_RT_Manager_Ex;

/**
 * @class ACE_Hash_Map_RT_Visitor
 *
 * @brief Parent class of all visitors that will traverse the Tables and Buckets.  The visitors walk through the buckets looking for 
 *        a certain key, and when (if) it is found, the appropritate visitor takes action (by returning the value, or deleting for example.).
 *        The Visitors are responsible for the actual inserting or deleting that bind() or unbind() do.
 *        These are not iterators, the simply perform their respective function at the appropriate place in the map.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Visitor
{
public:
  typedef ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY> POD;
  typedef ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID>LITEM;
  typedef ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID>ENTRY;
  typedef ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY> TABLE;
  typedef ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY> BUCKET;

  virtual BUCKET* start(BUCKET* bucket) = 0;

  virtual int visit(LITEM* litem) = 0;
  
  virtual int done(BUCKET& bucket) = 0;
  
  virtual void found_key (LITEM* litem);

  /// Walks through the bucket, calling start() (then visit() only if the appropriate key has been found)
  /// then done() on each ListItem. start and done only modify the map if is_found() is true, indicating 
  /// that the key has been found.
  int process_bucket(BUCKET& bucket);

  /// Returns 0 for false, 1 for true by comparing found_ to '0'
  int is_found (void);
  
  /// return found_
  LITEM& get_found (void);

protected:
  /// A pointer to the last ListItem we touched.
  /// As we search for the element we are looking for, this pointer is incremented accordingly.
  /// Its used for removing ListItems from the Bucket without keeping
  /// backward pointers to every ListItem
  LITEM* mtfPrev_;

  /// Pointer to the ListItem we are looking for (or null)
  LITEM* found_;

  ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager_;
};

/**
 * @class ACE_Hash_Map_RT_Clean_Visitor
 *
 * @brief Takes care of cleaning the buckets, to make sure the ListItems are in the right Table and Bucket according to the
 *        current POD and Hash_Function.  If it encounters a ListItem that is not, it removes it then inserts it in the correct
 *        place.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Clean_Visitor : public ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:
  ACE_Hash_Map_RT_Clean_Visitor (ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager);

  /// Returns a copy of the contents of that Bucket right before it empties the Bucket.
  BUCKET* start (BUCKET* bucket);

  /// the ListItem to 'visit' and inspect its location compared to where the POD says it should be.
  int visit (LITEM* litem);

  /// Sets the POD of the bucket to be the Table's current POD.
  int done (BUCKET& bucket);

  /// a copy of the contents of a bucket before we just cleared it out
  BUCKET dead_bucket;
};

/**
 * @class ACE_Hash_Map_RT_Searching_Visitor
 *
 * @brief Handles the searching through the Tables and Buckets while looking for a certain EXT_ID.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Searching_Visitor : public ACE_Hash_Map_RT_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:

  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Searching_Visitor (const EXT_ID& entry,
                                     ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager);
  
  /// Default constructor
  ACE_Hash_Map_RT_Searching_Visitor (void);
  
  /// Does nothing.
  BUCKET* start (BUCKET* bucket);
  
  /// Looks at the ListItem to see if its the one we are looking for.
  int visit (LITEM* litem);
  
  /// Does nothing.
  int done (BUCKET& bucket);

  /// the key that we're looking for
  const EXT_ID& ext_id_;
  
  ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager_;
};
  
/**
 * @class ACE_Hash_Map_RT_Put_Visitor
 *
 * @brief Adds a new ListItem (with Entry) to the Bucket where Searching_Visitor says it should go.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Put_Visitor : public ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:

  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Put_Visitor(const EXT_ID& ext_id, 
                             const INT_ID& int_id,
                             ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager);
  
  virtual void found_key (LITEM* litem);
  
  /// Prepends the Entry that we want to insert onto the Bucket
  int done (BUCKET& bucket);
  
private:

  const INT_ID& int_id_;  
};

/**
 * @class ACE_Hash_Map_RT_Put_Visitor
 *
 * @brief Gets the INT_ID from the Bucket where Searching_Visitor finds it.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Get_Visitor : public ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:
  
  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Get_Visitor(const EXT_ID& ext_id, 
                             INT_ID& int_id,
                             ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager);

  /// Calls compareMaxChain
  int done (BUCKET& bucket);
  
private:
  INT_ID& int_id_;
};

/**
 * @class ACE_Hash_Map_RT_Remove_Visitor
 *
 * @brief Removes the  ListItem from the Bucket where Searching_Visitor finds it.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Remove_Visitor : public ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:

  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_Remove_Visitor(const EXT_ID& ext_id,
                                ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager);
  
  /// Deletes the ListItem that we are looking for from the Bucket
  int done (BUCKET& bucket);
  
};

class ACE_Hash_Map_RT_StatsManager
{
public:

  // = Initialization and termination methods.
  /// Constructor
  ACE_Hash_Map_RT_StatsManager (void);

  /// Destructor
  ~ACE_Hash_Map_RT_StatsManager (void);
  
  int get_total_elements (void);
  
  int get_max_chain (void);
  
  /// Stores the greater of chain_size and the stats_manager's max_chain_ in the stats_manager's max_chain_
  void compare_max_chain (int chain_size);
  
  /// max_chain_ = 0;
  void reset_max_chain (void);

  int incr_total_elements (void);
  
  int decr_total_elements (void);
  
private:
  /// the longest list (which translates to the biggest Bucket's size)
  int max_chain_;
  
  /// the Sum of all the elements in all the Buckets (not including any Bucket->head_ because they dont hold an Entry)
  int total_elements_;
};


 /*
  * @class ACE_Hash_Map_RT_Clean_Manager
  * 
  * @brief This class walks through the tables, and buckets looking for ListItems that are in the wrong
  *        position according to the current POD.  When it reaches the end (more_to_do() = false) it is 
  *        killed, and another one is created which starts from the beginning all over again.
  *
  */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Clean_Manager
{
  typedef ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY> TABLE;
  typedef ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY> BUCKET;

public:

  ///Constructor
  ACE_Hash_Map_RT_Clean_Manager (ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* thismanager);

  /// calls clean visitor.process_bucket on the next bucket in the current table.  then goes to the next table, and 
  /// continues until more_to_do() == false
  ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* incremental_clean (void);

private:
  /// if we havent retired the old POD and we're not done with the last table, then there is more to do.
  int more_to_do (void);

  /// A reference to the ACE_Hash_Map_RT_Manager_Ex
  ACE_Hash_Map_RT_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* manager_;

  /// the current table which is being cleaned  
  size_t cur_table_;

  /// the current bucket in cur_table_ that is being cleaned.
  size_t cur_bucket_;
};

// Forward decl.
class ACE_Allocator;

/**
 * @class ACE_Hash_Map_RT_Manager_Ex
 *
 * @brief Define a map abstraction that efficiently associates
 * <EXT_ID>s with <INT_ID>s.
 *
 * This implementation of a map uses a hash table.  Key hashing
 * is achieved through the HASH_KEY object.
 * This class uses an <ACE_Allocator> to allocate memory.  The
 * user can make this a persistent class by providing an
 * <ACE_Allocator> with a persistable memory pool.
 */

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_RT_Manager_Ex
{
public:
  typedef EXT_ID KEY;
  typedef INT_ID VALUE;
  typedef ACE_Hash_Map_RT_Coord COORD;
  typedef ACE_Hash_Map_RT_POD<EXT_ID, INT_ID, HASH_KEY> POD;
  typedef ACE_Hash_Map_RT_Entry<EXT_ID, INT_ID> ENTRY;
  typedef ACE_Hash_Map_RT_Table<EXT_ID, INT_ID, HASH_KEY> TABLE;
  typedef ACE_Hash_Map_RT_Bucket<EXT_ID, INT_ID, HASH_KEY> BUCKET;
  typedef ACE_Hash_Map_RT_ListItem<EXT_ID, INT_ID> LITEM;
  typedef ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> CLEAN;
  typedef ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> CLEANV;

  friend class ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_RT_Get_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_RT_Put_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_RT_Remove_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;

  // = Initialization and termination methods.
  
  /// Initialize a <Hash_Map_Manager_Ex> with default size.
  ACE_Hash_Map_RT_Manager_Ex (size_t buckets_per_table, 
                              size_t RTL,
                              size_t max_tables,
                              ACE_Allocator *alloc = 0,
                              size_t clean_rate = 3);
  
  /// Cleanup the <Hash_Map_Manager_Ex>.
  ~ACE_Hash_Map_RT_Manager_Ex (void);

  /**
   * Associate <ext_id> with <int_id>.  If <ext_id> is already in the
   * map then the <ACE_Hash_Map_RT_Entry> is not changed.  Returns 0 if a
   * new entry is bound successfully, returns 1 if an attempt is made
   * to bind an existing entry, and returns -1 if failures occur.
   */
  int bind (const EXT_ID &item,
            const INT_ID &int_id);
  
  /// Return the value associated with the key.
  int find (const EXT_ID &ext_id,
           INT_ID &int_id);
  
  /// Remove a key, value pair from the map
  int unbind (const EXT_ID &ext_id);
  
  /// find a the coord of the bucket where item is located according to this POD
  int find_bucket (POD* pod,
                   const EXT_ID &item,
                   COORD& coord);
  
  /// Return the bucket where the item is located according to this POD
  BUCKET& find_bucket (POD* pod,
                       const EXT_ID &item);
  
  POD* get_POD (void);

  /// True if this->oldPOD_ != 0
  int is_stable (void);
  
  /// Return table at table_[loc]
  TABLE& get_table (size_t loc);

  size_t get_cleaning_rate (void);

protected:
  /// Retire the POD to old_POD and make this new_POD be the current POD for the hashmap
  int change_POD (POD& new_POD);

  /// Calls close_i while holding a write lock
  int close_manager (void);
  
  ///Iterate through the entire map calling the destuctor of each <ACE_Hash_Map_ListItem>.
  int remove_all_i (void);

  /// returns the coord where the key is located according to the current POD
  int hash (const KEY& key,
            ACE_Hash_Map_RT_Coord& coord);

  /// find where entry should go according to the current POD and prepend it there
  int rehash (ENTRY& entry);

  /// makes a new POD based on the current table size and max_tables variables
  POD* create_POD (void);

  ///total number of elements
  int size (void);

  ///de allocates the buckets and the table array
  int close_i (void);
 
  ///finds the item, then lets the visitor do its thing (bind, unbind, find)
  int locate_i (const EXT_ID& item,
                ACE_Hash_Map_RT_Searching_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>& visitor,
                INT_ID& id);

  /// allocates memory and creates all tables and local variables.
  int open_i (size_t buckets_per_table,
              ACE_Allocator *alloc);
  
  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  ACE_Hash_Map_RT_StatsManager* stats_;

private:
  /// a math function that gets the smallest size_t larger than the double
  size_t ceil (double number);

  /// array of tables
  TABLE* table_;

  size_t buckets_per_table_;

  /// Current number of tables, always < max_tables_
  size_t number_of_tables_;

  /// Rehash Trigger Length
  size_t RTL_;
  
  /// how many tables we have room for 
  size_t max_tables_;
  
  POD* curPOD_;
  
  POD* oldPOD_;
  
  ACE_Hash_Map_RT_Clean_Visitor<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* clean_vis_;

  ACE_Hash_Map_RT_Clean_Manager<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>* clean_;

  /// If the user is willing to put up with increased overhead in order
  /// to clean the table in fewer operations, this will tell the
  /// CleanManager to rehash additional buckets on each operation.
  size_t cleaning_rate_;
  
  /// Synchronization variable for the MT_SAFE <ACE_Hash_Map_Manager_Ex>.
  ACE_LOCK lock_;

};

#if defined (__ACE_INLINE__)
#  include "ace/Hash_Map_Manager_RT_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Hash_Map_Manager_RT_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Hash_Map_Manager_RT_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "post.h"
#endif /* ACE_HASH_MAP_RT_MANAGER_RT_H */
