/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Map_Test.h
 *
 *  $Id$
 *
 * This file has the class definitions needed for template generation in
 * Map_Test.cpp.  They have to be in a separate file so AIX xlC can
 * find them at auto-instantiate time.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_MAP_TEST_H
#define ACE_TESTS_MAP_TEST_H

#include "ace/OS_NS_string.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Active_Map_Manager.h"
#include "ace/Containers.h"

// Key data type.
typedef ACE_Array<char> KEY;

/**
 * @class Key_Generator
 *
 * @brief Defines a key generator.
 *
 * This class is used in adapters of maps that do not produce keys.
 */
class Key_Generator
{
public:

  Key_Generator (void)
    : counter_ (0)
    {
    }

  int operator() (KEY &key)
    {
      // Keep original information in the key intact.
      size_t original_size = key.size ();

      // Size of this counter key.
      size_t counter_key_size = sizeof this->counter_;

      // Resize to accommodate both the original data and the new key.
      key.size (counter_key_size + original_size);

      // Add new key data.
      ACE_OS::memcpy (&key[original_size],
                      &++this->counter_,
                      sizeof this->counter_);

      // Success.
      return 0;
    }

private:
  u_long counter_;
};

class Hash_Key
{
public:
  u_long operator () (const KEY &key) const
    {
      // Recover system generated part of key.
      u_long value;
      size_t counter_key_size = sizeof (u_long);

      // Copy system key from user key.
      ACE_OS::memcpy (&value,
                      &key[key.size () - counter_key_size],
                      sizeof value);

      // Return the system key value as the hash value.
      return value;
    }
};

class Key_Adapter
{
public:

  int encode (const KEY &original_key,
              const ACE_Active_Map_Manager_Key &active_key,
              KEY &modified_key)
    {
      // Keep original information in the key intact.
      modified_key = original_key;
      size_t original_size = modified_key.size ();

      // Size of active key.
      size_t active_key_size = active_key.size ();

      // Resize to accommodate both the original data and the new active key.
      modified_key.size (active_key_size + original_size);

      // Copy active key data into user key.
      active_key.encode (&modified_key[original_size]);

      // Success.
      return 0;
    }

  int decode (const KEY &modified_key,
              ACE_Active_Map_Manager_Key &active_key)
    {
      // Read the active key data from the back of the key.
      size_t active_key_size = active_key.size ();
      size_t original_size = modified_key.size () - active_key_size;

      // Read off value of index and generation.
      active_key.decode (&modified_key[original_size]);

      // Success.
      return 0;
    }

  int decode (const KEY &modified_key,
              KEY &original_key)
    {
      // Read the original user key data from the front of the
      // modified key.
      size_t active_key_size = ACE_Active_Map_Manager_Key::size ();

      // Copy all the data.
      original_key = modified_key;

      // Resize to ignore active key data.
      original_key.size (original_key.size () - active_key_size);

      // Success.
      return 0;
    }
};

#endif /* ACE_TESTS_MAP_TEST_H */
