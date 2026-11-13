/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Hash_Table.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include "Search_Struct.h"
#include <array>

/**
 * @class Hash_Table
 *
 * @brief Provides a hash function lookup abstraction for friend records.
 */
class Hash_Table : public Search_Struct
{
public:
  Hash_Table ();
  ~Hash_Table () override;
  Hash_Table (Hash_Table const &) = delete;
  Hash_Table &operator= (Hash_Table const &) = delete;
  Hash_Table (Hash_Table &&) = delete;
  Hash_Table &operator= (Hash_Table &&) = delete;
  Protocol_Record *insert (const char *key_name,
                           int max_len = MAXUSERIDNAMELEN) override = 0;
  Protocol_Record *get_next_entry () override;
  Protocol_Record *get_each_entry () override;

protected:
  static constexpr int HASH_TABLE_SIZE = 500;

  // Each bucket owns its linked Protocol_Record chain.
  std::array<Protocol_Record *, HASH_TABLE_SIZE> hash_table {};
  // Non-owning iterator into one of the bucket chains.
  Protocol_Record *current_ptr {};
  int current_index {};
};
#endif /* _HASH_TABLE_H */
