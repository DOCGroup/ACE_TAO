/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Binary_Search.h
 *
 *  Defines a binary search abstraction for friend records.
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _BINARY_SEARCH_H
#define _BINARY_SEARCH_H

#include "Search_Struct.h"
#include <memory>

/**
 * @class Binary_Search
 *
 * @brief Defines a binary search abstraction for friend records.
 */
class Binary_Search : public Search_Struct
{
public:
  /// Initialize the values for the iterators...
  Binary_Search ();

  virtual ~Binary_Search ();

  Binary_Search (Binary_Search const &) = delete;
  Binary_Search &operator= (Binary_Search const &) = delete;
  Binary_Search (Binary_Search &&) = delete;
  Binary_Search &operator= (Binary_Search &&) = delete;

  /**
   * Returns the next friend in the sequence of sorted friends.  Note
   * that this function would be simplified if we expanded the
   * iterator interface to include an "initialize" and "next"
   * function!
   */
  Protocol_Record *get_next_entry () override;

  /**
   * An iterator, similar to Binary_Search::get_next_friend, though in
   * this case the friend records are returned in the order they
   * appeared in the friend file, rather than in sorted order.  Also,
   * we skip over entries that don't have any hosts associated with
   * them.
   */
  Protocol_Record *get_each_entry () override;

  /**
   * This function is used to merge the <key_name> from server
   * <host_name> into the sorted list of userids kept on the client's
   * side.
   */
  Protocol_Record *insert (char const *key_name,
                                   int max_len = MAXUSERIDNAMELEN) override = 0;

  /// This function is passed to qsort to perform the comparison
  /// between login names for two friends.
  static int name_compare (void const *, void const *);

protected:
  /// Borrowed iterator into sorted_record_.
  Protocol_Record **current_ptr_ {};
  int current_index_ {};

  std::unique_ptr<Protocol_Record[]> protocol_record_;
  std::unique_ptr<Protocol_Record *[]> sorted_record_;

};

#endif /* _BINARY_SEARCH_H */
