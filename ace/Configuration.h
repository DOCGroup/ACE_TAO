/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Configuration.h
//
// = AUTHOR
//    Chris Hafey <chris@stentorsoft.com>
//
// = DESCRIPTION
//    The ACE configuration API provides a portable abstraction for
//    program configuration.  The API supports a tree based hierarchy
//    of configuration sections.  Each section contains other sections
//    or values.  Values may contain string, unsigned integer and
//    binary data.
//
// = TODO
//    - Add locking for thread safety.
//    - Need to investigate what happens if memory mapped file gets mapped to 
//      a location different than it was created with.
//    - Implement export and import of binary values
//    - Add dynamic buffer when importing.  currently it will not allow
//      importing of values greater than a fixed ammount (4096 bytes)
//    - Replace unsigned int with a type that is fixed accross platforms.
//
// ============================================================================

#ifndef ACE_CONFIGURATION_H
#include "ace/pre.h"
#define ACE_CONFIGURATION_H

#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Hash_Map_With_Allocator_T.h"
#include "ace/Malloc.h"

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

class ACE_Export ACE_Section_Key_Internal
{
  // = TITLE
  //     A base class for internal handles to section keys for
  //     configuration implementations
  //
  // = DESCRIPTION
  //     Implementations subclass this base class to represent a
  //     section key.
public:
  virtual ~ACE_Section_Key_Internal (void);
  // Virtual destructor, make sure descendants are virtual!
  
  virtual int add_ref (void);
  // Increment reference count
  
  virtual int dec_ref (void);
  // Decrement reference count.  Will delete this if count gets to 0
protected:
  ACE_Section_Key_Internal (void);
  ACE_Section_Key_Internal (const ACE_Section_Key_Internal& rhs);
  ACE_Section_Key_Internal& operator= (ACE_Section_Key_Internal& rhs);

  u_int ref_count_;
};

class ACE_Export ACE_Configuration_Section_Key
{
  // = TITLE
  //     Referenced counted wrapper for <ACE_Section_Key_Internal>.
  // 
  // = DESCRIPTION
  //     Reference counted wrapper class for the abstract internal
  //     section key.  A user gets one of these to represent a section
  //     in the configuration database.
  friend class ACE_Configuration;
public:
  ACE_Configuration_Section_Key (void);
  // Default ctor
  
  ACE_EXPLICIT ACE_Configuration_Section_Key (ACE_Section_Key_Internal *key);
  // ctor based on a pointer to a concrete internal key, does an
  // add_ref on <key>.
  
  ACE_Configuration_Section_Key (const ACE_Configuration_Section_Key &rhs);
  // Copy ctor, does an add_ref on rhs.key_.
  
  ~ACE_Configuration_Section_Key (void);
  // destructor, does a <dec_ref> on <key_>.
  
  ACE_Configuration_Section_Key &
    operator= (const ACE_Configuration_Section_Key &rhs);
  // assignment operator, does a <dec_ref> on <key_> and <add_ref> to
  // <rhs.key_>
private:
  ACE_Section_Key_Internal *key_;
};

class ACE_Export ACE_Configuration
{
  // = TITLE
  //     Base class for configuration databases
  //
  // = DESCRIPTION
  //     This class provides an interface for configuration databases.
public:
  enum VALUETYPE 
  {
    STRING,
    INTEGER,
    BINARY,
    INVALID
  };
  // Enumeration for the various types of values we can store.

  virtual ~ACE_Configuration (void);
  // destructor
  
  virtual const ACE_Configuration_Section_Key& root_section (void);
  // Returns the root section of this configuration.
  
  virtual int open_section (const ACE_Configuration_Section_Key &base,
                            const TCHAR *sub_section, 
                            int create, 
                            ACE_Configuration_Section_Key& result) = 0;
  // Finds a <sub_section> in <base> and places the resulting key in
  // <result>. If create is non zero, the sub_section will be created
  // if it doesn't exist
  
  virtual int remove_section (const ACE_Configuration_Section_Key &key,
                              const TCHAR *sub_section, 
                              int recursive) = 0;
  // Removes the <sub_section> from <key>.  If recursive is non zero,
  // any subkeys below <sub_section> are remove as well.

  virtual int enumerate_values (const ACE_Configuration_Section_Key& key, 
                                int index, 
                                ACE_TString& name, 
                                VALUETYPE& type) = 0;
  // method to enumerate through the <name> and <type> of values in a
  // <key>.  To begin iteration, <index> must be zero. to continue
  // iteration, invoke enumerate_values again while incrementing
  // index.  Each iteration will return the <name> of the value and
  // its <type>.  This method returns 0 on success, <0 on error and 1
  // when there are no more values to iterate through.  Note - you may
  // not delete or add values while enumerating.  If you need to do
  // this, you start the enumeration over again.
  
  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key, 
                                  int index, ACE_TString& name) = 0;
  // method to enumerate through the <name> subsections in <key>.  To
  // begin iteration, <index> must zero. to continue iteration, invoke
  // enumerate_sections again while incrementing index.  Each
  // iteration will return the <name> of the sub section.  This method
  // returns 0 on success, <0 on error and 1 when there are no more
  // subsections to iterate through.  Note - you may not delete or add
  // values while enumerating.  If you need to do this, you start the
  // enumeration over again.
    
  virtual int set_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const ACE_TString& value) = 0;
  // sets the value in <key> with <name> to a string of <value>
  
  virtual int set_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int value) = 0;
  // sets the value in <key> with <name> to an integer of <value>
  
  virtual int set_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const void* data, 
                                u_int length) = 0;
  // sets the value in <key> with <name> to binary data of <data> with
  // <length>.

  virtual int get_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                ACE_TString& value) = 0;
  // gets the string value of <name> from <key> and places it in
  // <value>.  Returns non zero on error (if value is not a string).
  
  virtual int get_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int& value) = 0;
  // gets the integer value of <name> from <key> and places it in
  // <value>.  Returns non zero on error (if value is not an integer).
  
  virtual int get_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                void*& data, 
                                u_int& length) = 0;
  // gets the binary value of <name> from <key> and places a copy in
  // <data> and sets <length> to the length of the data.  caller is
  // responsible for freeing <data>.  Returns non zero on error (if
  // value is not binary).
  
  virtual int remove_value (const ACE_Configuration_Section_Key& key, 
                            const TCHAR* name) = 0;
  // Removes the the value <name> from <key>.  returns non zero on
  // error.

  int expand_path (const ACE_Configuration_Section_Key& key, 
                   const ACE_TString& path_in, 
                   ACE_Configuration_Section_Key& key_out, 
                   int create = 1);
  // Expands <path_in> to <key_out> from <key>.  If create is true,
  // the subsections are created.  Returns 0 on success, non zero on
  // error The path consists of sections separated by the backslash
  // '\'.

  virtual int export_config (const TCHAR* filename);
  // Exports the configuration database to filename.  If <filename> is
  // alredy present, it is overwritten.

  virtual int import_config (const TCHAR* filename);
  // Imports the configuration database from filename.  Any existing
  // data is not removed.

protected:
  ACE_Configuration (void);
  // Default ctor
  
  ACE_Section_Key_Internal* get_internal_key 
    (const ACE_Configuration_Section_Key& key);
  // resolves the internal key from a section key
  
  int validate_name (const TCHAR* name);
  // tests to see if <name> is valid.  <name> must be < 255 characters
  // and not contain the path separator '\', brackets [] or = (maybe
  // just restrict to alphanumeric?) returns non zero if name is not
  // valid

  int export_section (const ACE_Configuration_Section_Key& section, 
                      const ACE_TString& path, 
                      FILE* out);
  // Used when exporting a configuration to a file

  // Not used
  ACE_Configuration (const ACE_Configuration& rhs);
  ACE_Configuration& operator= (const ACE_Configuration& rhs);
  ACE_Configuration_Section_Key root_;
};

#if defined (ACE_WIN32)

class ACE_Export ACE_Section_Key_Win32 : public ACE_Section_Key_Internal
{
  // = TITLE
  //     The Win32 registry implementation of an internal section key.
  //
  // = DESCRIPTION
  //     Holds the HKEY for a section (registry key).
public:
  ACE_Section_Key_Win32 (HKEY hKey);
  // constructor based on an HKEY

  HKEY hKey_;

protected:
  virtual ~ACE_Section_Key_Win32 (void);
  // destructor - invokes <RegCloseKey>
  
  // Not used
  ACE_Section_Key_Win32 (const ACE_Section_Key_Win32& rhs);
  ACE_Section_Key_Win32& operator= (const ACE_Section_Key_Win32& rhs);
};

class ACE_Export ACE_Configuration_Win32Registry : public ACE_Configuration
{
  // = TITLE
  //     The win32 registry implementation of a configuration database
  //
  // = DESCRIPTION
  //     The win32 implementation basically makes calls through to the
  //     registry functions. The API is very similar so very little
  //     work must be done
public:

  ACE_EXPLICIT ACE_Configuration_Win32Registry (HKEY hKey);
  // constructor for registry configuration database.  hKey is the
  // base registry key to attach to.  This class takes ownership of
  // hKey, it will invoke <RegCloseKey> on it upon destruction.

  virtual ~ACE_Configuration_Win32Registry (void);
  // destructor
  
  virtual int open_section (const ACE_Configuration_Section_Key& base, 
                            const TCHAR* sub_section, 
                            int create, 
                            ACE_Configuration_Section_Key& result);
  
  virtual int remove_section (const ACE_Configuration_Section_Key& key, 
                              const TCHAR* sub_section, 
                              int recursive);

  virtual int enumerate_values (const ACE_Configuration_Section_Key& key, 
                                int Index, 
                                ACE_TString& name, 
                                VALUETYPE& type);
  
  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key, 
                                  int Index, 
                                  ACE_TString& name);
  
  virtual int set_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const ACE_TString& value);
  
  virtual int set_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int value);
  
  virtual int set_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const void* data, 
                                u_int length);

  virtual int get_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                ACE_TString& value);
  
  virtual int get_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int& value);
  
  virtual int get_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                void*& data, 
                                u_int& length);

  virtual int remove_value (const ACE_Configuration_Section_Key& key, 
                            const TCHAR* name);
  // Removes the the value <name> from <key>.  returns non zero on error

  static HKEY resolve_key (HKEY hKey, 
                           const TCHAR* path, 
                           int create = 1);
  // This method traverses <path> through <hKey>.  It is useful when
  // you want the HKEY for a specific registry key, especially when
  // initializing this implementation.  Caller is responsible for
  // closeing this key when it is no longer used.  If create is 1
  // (default) the keys are create if they don't already exist.
  // Returns 0 on error

protected:

  int load_key (const ACE_Configuration_Section_Key& key, HKEY& hKey);
  // Gets the HKEY for a configuration section

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

class ACE_Export ACE_Configuration_ExtId
{
  // = TITLE
  //     External ID for the section and value hash
  //
  // = DESCRIPTION
  //     Contains a pointer to the section or value name.
public:
  ACE_Configuration_ExtId (void);
  // defeault ctor

  ACE_EXPLICIT ACE_Configuration_ExtId (const TCHAR* name);
  // named constructor

  ACE_Configuration_ExtId (const ACE_Configuration_ExtId& rhs);
  // copy ctor

  ~ACE_Configuration_ExtId (void);
  // destructor

  ACE_Configuration_ExtId& operator= (const ACE_Configuration_ExtId& rhs);
  // assignment operator

  int operator==  (const ACE_Configuration_ExtId &rhs) const;
  // Equality comparison operator (must match name_).

  int operator!= (const ACE_Configuration_ExtId &rhs) const;
  // Inequality comparison operator.

  void free (ACE_Allocator* allocator);
  // Frees the name of the value.  needed since we don't know the
  // allocator name_ was created in

  u_long hash  (void) const;
  // <hash> function is required in order for this class to be usable by
  // ACE_Hash_Map_Manager.

  // = Data members.

  const TCHAR * name_;

  // Accessors
  const TCHAR *name (void);
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

class ACE_Export ACE_Configuration_Value_IntId
{
  // = TITLE
  //     The section hash table internal value class
  //
  // = DESCRIPTION
  //     This class is present as the internal portion of a section's
  //     value hash table It may store string, integer or binary data.
public:
  ACE_Configuration_Value_IntId (void);
  // default constructor

  ACE_EXPLICIT ACE_Configuration_Value_IntId (TCHAR* string);
  // string constructor, takes ownership of string
  
  ACE_EXPLICIT ACE_Configuration_Value_IntId (u_int integer);
  // integer constructor
  
  ACE_Configuration_Value_IntId (void* data, u_int length);
  // binary constructor, takes ownership of data
  
  ACE_Configuration_Value_IntId (const ACE_Configuration_Value_IntId& rhs);
  // copy ctor
  
  ~ACE_Configuration_Value_IntId (void);
  // destructor

  ACE_Configuration_Value_IntId& operator= (
    const ACE_Configuration_Value_IntId& rhs);
  // Assignment operator

  void free (ACE_Allocator* allocator);

  // = Data members.

  ACE_Configuration::VALUETYPE      type_;
  void*                             data_;
  // points to the string value or binary data or IS the integer 
  // (XXX need to change this since sizeof (u_int) is
  // not the same accross different platforms)
  u_int                      length_;
  // Length is only used when type_ == BINARY
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

class ACE_Export ACE_Configuration_Section_IntId
{
  // = TITLE
  //     The internal ID for a section hash table
  //
  // = DESCRIPTION
  //     Contains a hash table containing value name/values 
public:
  ACE_Configuration_Section_IntId (void);
  // default ctor

  ACE_EXPLICIT ACE_Configuration_Section_IntId (VALUE_MAP* value_hash_map, 
                                                SUBSECTION_MAP* section_hash_map);
  // named ctor
  
  ACE_Configuration_Section_IntId (const ACE_Configuration_Section_IntId& rhs);
  // copy ctor

  ~ACE_Configuration_Section_IntId (void);
  // destructor

  ACE_Configuration_Section_IntId& operator= (
    const ACE_Configuration_Section_IntId& rhs);
  // asignment operator

  void free (ACE_Allocator* allocator);
  // frees the hash table and all its values

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

class ACE_Export ACE_Configuration_Section_Key_Heap 
  : public ACE_Section_Key_Internal
{
  // = TITLE
  //     Internal section key class for heap based configuration
  //     database.
  //
  // = DESCRIPTION
  //     Contains a value iterator and full path name of section.
public:
  ACE_Configuration_Section_Key_Heap (const TCHAR* path);
  // constructor based on the full path of the section
  
  TCHAR* path_;
  //the path itself
  
  VALUE_HASH::ITERATOR* value_iter_;
  // The value iterator

  SUBSECTION_HASH::ITERATOR* section_iter_;
  // The sub section iterator
protected:
  virtual ~ACE_Configuration_Section_Key_Heap (void);
  // destructor - will delete the iterators
  
  // Not used
  ACE_Configuration_Section_Key_Heap (const ACE_Configuration_Section_Key_Heap& rhs);
  ACE_Configuration_Section_Key_Heap& operator= (const ACE_Configuration_Section_Key_Heap& rhs);
};

class ACE_Export ACE_Configuration_Heap : public ACE_Configuration
{
  // = TITLE
  //     The concrete implementation of a allocator based
  //     configuration database
  //
  // = DESCRIPTION
  //     This class uses ACE's Allocators to manage a memory
  //     representation of a configuraiton database. A persistent heap
  //     may be used to store configurations persistently
public:
  
  ACE_Configuration_Heap (void);
  // Default ctor
  
  virtual ~ACE_Configuration_Heap (void);
  // destructor

  int open (const TCHAR* file_name, 
            void* base_address = ACE_DEFAULT_BASE_ADDR, 
            int default_map_size = ACE_DEFAULT_CONFIG_SECTION_SIZE);
  // opens a configuration based on a file name

  int open (int default_map_size = ACE_DEFAULT_CONFIG_SECTION_SIZE);
  // opens a heap based configuration

  virtual int open_section (const ACE_Configuration_Section_Key& base, 
                            const TCHAR* sub_section, 
                            int create, ACE_Configuration_Section_Key& result);
  
  virtual int remove_section (const ACE_Configuration_Section_Key& key, 
                              const TCHAR* sub_section, 
                              int recursive);

  virtual int enumerate_values (const ACE_Configuration_Section_Key& key, 
                                int Index, 
                                ACE_TString& name, 
                                VALUETYPE& type);
  
  virtual int enumerate_sections (const ACE_Configuration_Section_Key& key, 
                                  int Index, 
                                  ACE_TString& name);
  
  virtual int set_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const ACE_TString& value);
  
  virtual int set_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int value);
  
  virtual int set_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                const void* data, 
                                u_int length);

  virtual int get_string_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                ACE_TString& value);
  
  virtual int get_integer_value (const ACE_Configuration_Section_Key& key, 
                                 const TCHAR* name, 
                                 u_int& value);
  
  virtual int get_binary_value (const ACE_Configuration_Section_Key& key, 
                                const TCHAR* name, 
                                void* &data, 
                                u_int &length);

  virtual int remove_value (const ACE_Configuration_Section_Key& key, 
                            const TCHAR* name);
  // Removes the the value <name> from <key>.  returns non zero on error

private:
  int add_section (const ACE_Configuration_Section_Key& base, 
                   const TCHAR* sub_section, 
                   ACE_Configuration_Section_Key& result);
  // adds a new section
  
  int create_index (void);
  // Helper for the <open> method.

  int create_index_helper (void *buffer);
  // Helper for <create_index> method: places hash table into an
  // allocated space.

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

#if defined (__ACE_INLINE__)
#include "ace/Configuration.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CONFIGURATION_H */
