/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Configuration.h
 *
 *  $Id$
 *
 *  @author Chris Hafey <chafey@stentor.com>
 *
 *  The ACE configuration API provides a portable abstraction for
 *  program configuration similar to the Microsoft Windows registry.  
 *  The API supports a tree based hierarchy of configuration sections.  Each 
 *  section contains other sections or values.  Values may contain string, 
 *  unsigned integer and binary data.
 *
 *  Note: these classes are not thread safe, if multiple threads use these
 *  classes, you are responsible for serializing access.
 *
 *  For examples of using this class, see:
 *   1) The test code in ACE_Wrappers/test
 *   2) wxConfigViewer, a Windows like Registry Editor for ACE_Configuration
 *   3) TAO's IFR, it makes extensive use of ACE_Configuration
 *
 *  @todo Templatize this class with an ACE_LOCK to provide thread safety
 *
 */
//=============================================================================

#ifndef ACE_CONFIGURATION_H
#define ACE_CONFIGURATION_H
#include "ace/pre.h"

#include "ace/ACE.h"
#include "ace/Utils/SString.h"
#include "ace/Utils/Templates/Hash_Map_With_Allocator_T.h"
#include "ace/Memory/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// configurable parameters

#if !defined (ACE_CONFIG_SECTION_INDEX)
#  define ACE_CONFIG_SECTION_INDEX "Config_Section_Index"
#endif /* ! ACE_CONFIG_SECTION_INDEX */

#if !defined (ACE_DEFAULT_CONFIG_SECTION_SIZE)
#define ACE_DEFAULT_CONFIG_SECTION_SIZE 16
#endif /* ACE_DEFAULT_CONFIG_SECTION_SIZE */

/**
 * @class ACE_Section_Key_Internal
 *
 * @brief A base class for internal handles to section keys for
 * configuration implementations
 *
 * Implementations subclass this base class to represent a
 * section key.
 *
 */
class ACE_Export ACE_Section_Key_Internal
{
public:
  /// Virtual destructor, make sure descendants are virtual!
  virtual ~ACE_Section_Key_Internal (void);

  /// Increment reference count
  virtual int add_ref (void);

  /// Decrement reference count.  Will delete this if count gets to 0
  virtual int dec_ref (void);
protected:
  ACE_Section_Key_Internal (void);
  ACE_Section_Key_Internal (const ACE_Section_Key_Internal& rhs);
  ACE_Section_Key_Internal& operator= (ACE_Section_Key_Internal& rhs);

  u_int ref_count_;
};

/**
 * @class ACE_Configuration_Section_Key
 *
 * @brief Referenced counted wrapper for <ACE_Section_Key_Internal>.
 *
 * Reference counted wrapper class for the abstract internal
 * section key.  A user gets one of these to represent a section
 * in the configuration database.
 */
class ACE_Export ACE_Configuration_Section_Key
{
  friend class ACE_Configuration;
public:
  /// Default ctor
  ACE_Configuration_Section_Key (void);

  /// ctor based on a pointer to a concrete internal key, does an
  /// add_ref on <key>.
  ACE_EXPLICIT ACE_Configuration_Section_Key (ACE_Section_Key_Internal *key);

  /// Copy ctor, does an add_ref on rhs.key_.
  ACE_Configuration_Section_Key (const ACE_Configuration_Section_Key &rhs);

  /// destructor, does a <dec_ref> on <key_>.
  ~ACE_Configuration_Section_Key (void);

  /// assignment operator, does a <dec_ref> on <key_> and <add_ref> to
  /// <rhs.key_>
  ACE_Configuration_Section_Key &
    operator= (const ACE_Configuration_Section_Key &rhs);
private:
  ACE_Section_Key_Internal *key_;
};

/**
 * @class ACE_Configuration
 *
 * @brief Base class for configuration databases
 *
 * This class provides an interface for configuration databases.
 */
class ACE_Export ACE_Configuration
{
public:
  /// Enumeration for the various types of values we can store.
  enum VALUETYPE
  {
    STRING,
    INTEGER,
    BINARY,
    INVALID
  };

  /// destructor
  virtual ~ACE_Configuration (void);

  /// Returns the root section of this configuration.
  virtual const ACE_Configuration_Section_Key& root_section (void) const;

  /**
   * Finds a <sub_section> in <base> and places the resulting key in
   * <result>. If create is non zero, the sub_section will be created
   * if it doesn't exist
   */
  virtual int open_section (const ACE_Configuration_Section_Key &base,
                            const ACE_TCHAR *sub_section,
                            int create,
                            ACE_Configuration_Section_Key& result) = 0;

  /// Removes the <sub_section> from <key>.  If recursive is non zero,
  /// any subkeys below <sub_section> are remove as well.
  virtual int remove_section (const ACE_Configuration_Section_Key &key,
                              const ACE_TCHAR *sub_section,
                              int recursive) = 0;

  /**
   * method to enumerate through the <name> and <type> of values in a
   * <key>.  To begin iteration, <index> must be zero. to continue
   * iteration, invoke enumerate_values again while incrementing
   * index.  Each iteration will return the <name> of the value and
   * its <type>.  This method returns 0 on success, <0 on error and 1
   * when there are no more values to iterate through.  Note - you may
   * not delete or add values while enumerating.  If you need to do
   * this, you start the enumeration over again.
   */
  virtual int enumerate_values (const ACE_Configuration_Section_Key& key,
                                int index,
                                ACE_TString& name,
                                VALUETYPE& type) = 0;

  /**
   * method to enumerate through the <name> subsections in <key>.  To
   * begin iteration, <index> must zero. to continue iteration, invoke
   * enumerate_sections again while incrementing index.  Each
   * iteration will return the <name> of the sub section.  This method
   * returns 0 on success, <0 on error and 1 when there are no more
   * subsections to iterate through.  Note - you may not delete or add
   * values while enumerating.  If you need to do this, you start the
   * enumeration over again.
   */
  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key,
                                  int index, ACE_TString& name) = 0;

  /// sets the value in <key> with <name> to a string of <value>
  virtual int set_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const ACE_TString& value) = 0;

  /// sets the value in <key> with <name> to an integer of <value>
  virtual int set_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int value) = 0;

  /// sets the value in <key> with <name> to binary data of <data> with
  /// <length>.
  virtual int set_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const void* data,
                                u_int length) = 0;

  /// gets the string value of <name> from <key> and places it in
  /// <value>.  Returns non zero on error (if value is not a string).
  virtual int get_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                ACE_TString& value) = 0;

  /// gets the integer value of <name> from <key> and places it in
  /// <value>.  Returns non zero on error (if value is not an integer).
  virtual int get_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int& value) = 0;

  /**
   * gets the binary value of <name> from <key> and places a copy in
   * <data> and sets <length> to the length of the data.  caller is
   * responsible for deleting <data>.  Returns non zero on error (if
   * value is not binary).
   */
  virtual int get_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                void*& data,
                                u_int& length) = 0;

  /**
   * checks to see if an entry of <name> is in <key> and places the
   * data type in <type>.  Returns 0 on success (entry is found),
   * -1 on error
   */
  virtual int find_value(const ACE_Configuration_Section_Key& key,
                         const ACE_TCHAR* name,
                         VALUETYPE& type) = 0;


  /// Removes the the value <name> from <key>.  returns non zero on
  /// error.
  virtual int remove_value (const ACE_Configuration_Section_Key& key,
                            const ACE_TCHAR* name) = 0;

  /**
   * Expands <path_in> to <key_out> from <key>.  If create is true,
   * the subsections are created.  Returns 0 on success, non zero on
   * error The path consists of sections separated by the backslash
   * '\' or forward slash '/'.  
   * Returns 0 on success, -1 if <create) is 0 and the path refers 
   * a nonexistant section
   */
  int expand_path (const ACE_Configuration_Section_Key& key,
                   const ACE_TString& path_in,
                   ACE_Configuration_Section_Key& key_out,
                   int create = 1);

  /**
   * Exports the configuration database to filename.  If <filename> is
   * already present, it is overwritten. This function is deprecated and
   * will be removed in a future version of ACE. Please use either
   * ACE_Registry_ImpExp or ACE_Ini_ImpExp instead.
   */
  int export_config (const ACE_TCHAR* filename);

  /**
   * Imports the configuration database from filename.  Any existing
   * data is not removed. This function is deprecated and will be
   * removed in a future version of ACE. Please use ACE_Registry_ImpExp
   * or ACE_Ini_ImpExp instead.
   */
  int import_config (const ACE_TCHAR* filename);

  /**
   * Determine if the contents of this object is the same as the 
   * contents of the object on the right hand side.
   * Returns 1 (True) if they are equal and 0 (False) if they are not equal
   */
  int operator==(const ACE_Configuration& rhs) const;

  /**
   * Determine if the contents of this object are different from the 
   * contents of the object on the right hand side.
   * Returns 0 (False) if they are equal and 1 (True) if they are not equal
   */
  int operator!=(const ACE_Configuration& rhs) const {return !(*this == rhs);}

protected:
  /// Default ctor
  ACE_Configuration (void);

  /// resolves the internal key from a section key
  ACE_Section_Key_Internal* get_internal_key
    (const ACE_Configuration_Section_Key& key);

  /**
   * tests to see if <name> is valid.  <name> must be < 255 characters
   * and not contain the path separator '\', brackets [] or = (maybe
   * just restrict to alphanumeric?) returns non zero if name is not
   * valid
   */
  int validate_name (const ACE_TCHAR* name);

  // Not used
  ACE_Configuration (const ACE_Configuration& rhs);
  ACE_Configuration& operator= (const ACE_Configuration& rhs);


  ACE_Configuration_Section_Key root_;
};

#if defined (ACE_WIN32)

/**
 * @class ACE_Section_Key_Win32
 *
 * @brief The Win32 registry implementation of an internal section key.
 *
 * Holds the HKEY for a section (registry key).
 */
class ACE_Export ACE_Section_Key_Win32 : public ACE_Section_Key_Internal
{
public:
  /// constructor based on an HKEY
  ACE_Section_Key_Win32 (HKEY hKey);

  HKEY hKey_;

protected:
  /// destructor - invokes <RegCloseKey>
  virtual ~ACE_Section_Key_Win32 (void);

  // Not used
  ACE_Section_Key_Win32 (const ACE_Section_Key_Win32& rhs);
  ACE_Section_Key_Win32& operator= (const ACE_Section_Key_Win32& rhs);
};

/**
 * @class ACE_Configuration_Win32Registry
 *
 * @brief The win32 registry implementation of a configuration database
 *
 * The win32 implementation basically makes calls through to the
 * registry functions. The API is very similar so very little
 * work must be done
 */
class ACE_Export ACE_Configuration_Win32Registry : public ACE_Configuration
{
public:

  /**
   * constructor for registry configuration database.  hKey is the
   * base registry key to attach to.  This class takes ownership of
   * hKey, it will invoke <RegCloseKey> on it upon destruction.
   */
  ACE_EXPLICIT ACE_Configuration_Win32Registry (HKEY hKey);

  /// destructor
  virtual ~ACE_Configuration_Win32Registry (void);

  virtual int open_section (const ACE_Configuration_Section_Key& base,
                            const ACE_TCHAR* sub_section,
                            int create,
                            ACE_Configuration_Section_Key& result);

  virtual int remove_section (const ACE_Configuration_Section_Key& key,
                              const ACE_TCHAR* sub_section,
                              int recursive);

  virtual int enumerate_values (const ACE_Configuration_Section_Key& key,
                                int Index,
                                ACE_TString& name,
                                VALUETYPE& type);

  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key,
                                  int Index,
                                  ACE_TString& name);

  virtual int set_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const ACE_TString& value);

  virtual int set_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int value);

  virtual int set_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const void* data,
                                u_int length);

  virtual int get_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                ACE_TString& value);

  virtual int get_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int& value);

  virtual int get_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                void*& data,
                                u_int& length);

  virtual int find_value(const ACE_Configuration_Section_Key& key,
                         const ACE_TCHAR* name,
                         VALUETYPE& type);

  /// Removes the the value <name> from <key>.  returns non zero on error
  virtual int remove_value (const ACE_Configuration_Section_Key& key,
                            const ACE_TCHAR* name);

  /**
   * This method traverses <path> through <hKey>.  It is useful when
   * you want the HKEY for a specific registry key, especially when
   * initializing this implementation.  Caller is responsible for
   * closeing this key when it is no longer used.  If create is 1
   * (default) the keys are create if they don't already exist.
   * Returns 0 on error
   */
  static HKEY resolve_key (HKEY hKey,
                           const ACE_TCHAR* path,
                           int create = 1);
  virtual int operator== (const ACE_Configuration_Win32Registry &rhs) const;
  virtual int operator!= (const ACE_Configuration_Win32Registry &rhs) const;

protected:

  /// Gets the HKEY for a configuration section
  int load_key (const ACE_Configuration_Section_Key& key, HKEY& hKey);

  // Not used
  ACE_Configuration_Win32Registry (void);
  ACE_Configuration_Win32Registry (const ACE_Configuration_Win32Registry& rhs);
  ACE_Configuration_Win32Registry& operator= (const ACE_Configuration_Win32Registry& rhs);
};
#endif /* ACE_WIN32 */

// ACE_Allocator version

typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL,
                                           ACE_SYNCH_MUTEX> >
        PERSISTENT_ALLOCATOR;
typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_LOCAL_MEMORY_POOL,
                                           ACE_SYNCH_MUTEX> >
        HEAP_ALLOCATOR;

/**
 * @class ACE_Configuration_ExtId
 *
 * @brief External ID for the section and value hash
 *
 * Contains a pointer to the section or value name.
 */
class ACE_Export ACE_Configuration_ExtId
{
public:
  /// defeault ctor
  ACE_Configuration_ExtId (void);

  /// named constructor
  ACE_EXPLICIT ACE_Configuration_ExtId (const ACE_TCHAR* name);

  /// copy ctor
  ACE_Configuration_ExtId (const ACE_Configuration_ExtId& rhs);

  /// destructor
  ~ACE_Configuration_ExtId (void);

  /// assignment operator
  ACE_Configuration_ExtId& operator= (const ACE_Configuration_ExtId& rhs);

  /// Equality comparison operator (must match name_).
  int operator==  (const ACE_Configuration_ExtId &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const ACE_Configuration_ExtId &rhs) const;

  /// Frees the name of the value.  needed since we don't know the
  /// allocator name_ was created in
  void free (ACE_Allocator *alloc);

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash  (void) const;

  // = Data members.

  const ACE_TCHAR * name_;

  // Accessors
  const ACE_TCHAR *name (void);
};

typedef ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, int>
        SUBSECTION_MAP;
typedef ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId,
                                int,
                                ACE_Hash<ACE_Configuration_ExtId>,
                                ACE_Equal_To<ACE_Configuration_ExtId>,
                                ACE_Null_Mutex>
        SUBSECTION_HASH;
typedef ACE_Hash_Map_Entry<ACE_Configuration_ExtId, int>
        SUBSECTION_ENTRY;

/**
 * @class ACE_Configuration_Value_IntId
 *
 * @brief The section hash table internal value class
 *
 * This class is present as the internal portion of a section's
 * value hash table It may store string, integer or binary data.
 */
class ACE_Export ACE_Configuration_Value_IntId
{
public:
  /// default constructor
  ACE_Configuration_Value_IntId (void);

  /// string constructor, takes ownership of string
  ACE_EXPLICIT ACE_Configuration_Value_IntId (ACE_TCHAR* string);

  /// integer constructor
  ACE_EXPLICIT ACE_Configuration_Value_IntId (u_int integer);

  /// binary constructor, takes ownership of data
  ACE_Configuration_Value_IntId (void* data, u_int length);

  /// copy ctor
  ACE_Configuration_Value_IntId (const ACE_Configuration_Value_IntId& rhs);

  /// destructor
  ~ACE_Configuration_Value_IntId (void);

  /// Assignment operator
  ACE_Configuration_Value_IntId& operator= (
    const ACE_Configuration_Value_IntId& rhs);

  void free (ACE_Allocator *alloc);

  // = Data members.

  /**
   * points to the string value or binary data or IS the integer
   * (XXX need to change this since sizeof (u_int) is
   * not the same accross different platforms)
   * Length is only used when type_ == BINARY
   */
  ACE_Configuration::VALUETYPE      type_;
  void*                             data_;
  u_int                      length_;
};

typedef ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId,
                                    ACE_Configuration_Value_IntId>
        VALUE_MAP;
typedef ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId,
                                ACE_Configuration_Value_IntId,
                                ACE_Hash<ACE_Configuration_ExtId>,
                                ACE_Equal_To<ACE_Configuration_ExtId>,
                                ACE_Null_Mutex>
        VALUE_HASH;
typedef ACE_Hash_Map_Entry<ACE_Configuration_ExtId,
                           ACE_Configuration_Value_IntId>
        VALUE_ENTRY;

/**
 * @class ACE_Configuration_Section_IntId
 *
 * @brief The internal ID for a section hash table
 *
 * Contains a hash table containing value name/values
 */
class ACE_Export ACE_Configuration_Section_IntId
{
public:
  /// default ctor
  ACE_Configuration_Section_IntId (void);

  /// named ctor
  ACE_EXPLICIT ACE_Configuration_Section_IntId (VALUE_MAP* value_hash_map,
                                                SUBSECTION_MAP* section_hash_map);

  /// copy ctor
  ACE_Configuration_Section_IntId (const ACE_Configuration_Section_IntId& rhs);

  /// destructor
  ~ACE_Configuration_Section_IntId (void);

  /// asignment operator
  ACE_Configuration_Section_IntId& operator= (
    const ACE_Configuration_Section_IntId& rhs);

  /// frees the hash table and all its values
  void free (ACE_Allocator *alloc);

  // = Data Members.
  VALUE_MAP* value_hash_map_;

  SUBSECTION_MAP* section_hash_map_;
};

typedef ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId,
                                    ACE_Configuration_Section_IntId>
        SECTION_MAP;
typedef ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId,
                                ACE_Configuration_Section_IntId,
                                ACE_Hash<ACE_Configuration_ExtId>,
                                ACE_Equal_To<ACE_Configuration_ExtId>,
                                ACE_Null_Mutex>
        SECTION_HASH;
typedef ACE_Hash_Map_Entry<ACE_Configuration_ExtId,
                           ACE_Configuration_Section_IntId>
        SECTION_ENTRY;

/**
 * @class ACE_Configuration_Section_Key_Heap
 *
 * @brief Internal section key class for heap based configuration
 *    database.
 *
 *     Contains a value iterator and full path name of section.
 */
class ACE_Export ACE_Configuration_Section_Key_Heap
  : public ACE_Section_Key_Internal
{
public:
  /// constructor based on the full path of the section
  ACE_Configuration_Section_Key_Heap (const ACE_TCHAR* path);

  ///the path itself
  ACE_TCHAR* path_;

  /// The value iterator
  VALUE_HASH::ITERATOR* value_iter_;

  /// The sub section iterator
  SUBSECTION_HASH::ITERATOR* section_iter_;
protected:
  /// destructor - will delete the iterators
  virtual ~ACE_Configuration_Section_Key_Heap (void);

  // Not used
  ACE_Configuration_Section_Key_Heap (const ACE_Configuration_Section_Key_Heap& rhs);
  ACE_Configuration_Section_Key_Heap& operator= (const ACE_Configuration_Section_Key_Heap& rhs);
};

/**
 * @class ACE_Configuration_Heap
 *
 * @brief The concrete implementation of a allocator based
 * configuration database
 *
 * This class uses ACE's Allocators to manage a memory
 * representation of a configuraiton database. A persistent heap
 * may be used to store configurations persistently
 *
 * Note: Before using this class you must call one of the open methods.
 *
 * @todo
 *  - Need to investigate what happens if memory mapped file gets mapped to
 *    a location different than it was created with.
 */
class ACE_Export ACE_Configuration_Heap : public ACE_Configuration
{
public:

  /// Default ctor
  ACE_Configuration_Heap (void);

  /// destructor
  virtual ~ACE_Configuration_Heap (void);

  /// opens a configuration based on a file name
  int open (const ACE_TCHAR* file_name,
            void* base_address = ACE_DEFAULT_BASE_ADDR,
            int default_map_size = ACE_DEFAULT_CONFIG_SECTION_SIZE);

  /// opens a heap based configuration
  int open (int default_map_size = ACE_DEFAULT_CONFIG_SECTION_SIZE);

  virtual int open_section (const ACE_Configuration_Section_Key& base,
                            const ACE_TCHAR* sub_section,
                            int create, ACE_Configuration_Section_Key& result);

  virtual int remove_section (const ACE_Configuration_Section_Key& key,
                              const ACE_TCHAR* sub_section,
                              int recursive);

  virtual int enumerate_values (const ACE_Configuration_Section_Key& key,
                                int Index,
                                ACE_TString& name,
                                VALUETYPE& type);

  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key,
                                  int Index,
                                  ACE_TString& name);

  virtual int set_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const ACE_TString& value);

  virtual int set_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int value);

  virtual int set_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                const void* data,
                                u_int length);

  virtual int get_string_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                ACE_TString& value);

  virtual int get_integer_value (const ACE_Configuration_Section_Key& key,
                                 const ACE_TCHAR* name,
                                 u_int& value);

  virtual int get_binary_value (const ACE_Configuration_Section_Key& key,
                                const ACE_TCHAR* name,
                                void* &data,
                                u_int &length);

  virtual int find_value(const ACE_Configuration_Section_Key& key,
                         const ACE_TCHAR* name,
                         VALUETYPE& type);

  /// Removes the the value <name> from <key>.  returns non zero on error
  virtual int remove_value (const ACE_Configuration_Section_Key& key,
                            const ACE_TCHAR* name);

private:
  /// adds a new section
  int add_section (const ACE_Configuration_Section_Key& base,
                   const ACE_TCHAR* sub_section,
                   ACE_Configuration_Section_Key& result);

  /// Helper for the <open> method.
  int create_index (void);

  /// Helper for <create_index> method: places hash table into an
  /// allocated space.
  int create_index_helper (void *buffer);

  int value_open_helper (size_t hash_table_size, void *buffer);

  int section_open_helper (size_t hash_table_size, void *buffer);

  int load_key (const ACE_Configuration_Section_Key& key, ACE_TString& name);

  int new_section (const ACE_TString& section,
                   ACE_Configuration_Section_Key& result);

  ACE_Configuration_Heap (const ACE_Configuration_Heap& rhs);
  ACE_Configuration_Heap& operator= (const ACE_Configuration_Heap& rhs);

  ACE_Allocator *allocator_;
  SECTION_MAP *index_;
  int default_map_size_;
};

#include "ace/post.h"
#endif /* ACE_CONFIGURATION_H */
