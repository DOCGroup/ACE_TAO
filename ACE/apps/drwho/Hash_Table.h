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

/**
 * @class Hash_Table
 *
 * @brief Provides a hash function lookup abstraction for friend records.
 */
class Hash_Table : public Search_Struct
{
public:
  Hash_Table ();
  virtual ~Hash_Table ();
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN) = 0;
  virtual Protocol_Record *get_next_entry ();
  virtual Protocol_Record *get_each_entry ();

protected:
  enum
  {
    HASH_TABLE_SIZE = 500
  };

  Protocol_Record **hash_table;
  Protocol_Record *current_ptr;
  int current_index;
  int hash_table_size;
};
#endif /* _HASH_TABLE_H */
