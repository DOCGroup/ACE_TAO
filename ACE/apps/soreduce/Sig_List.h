// -*- C++ -*-
// File: Sig_List.h

// Author: Phil Mesnier

#ifndef _SIG_LIST_H_
#define _SIG_LIST_H_

#include "Signature.h"
#include <memory>
#include <vector>

/**
 * @brief Stores groups of signatures for incremental symbol resolution.
 *
 * The list supports traversal and efficient reuse of slots while the
 * unresolved-signature collection is being updated.
 */
class Sig_List {
public:
  explicit Sig_List (size_t cap = 500);
  Sig_List (Sig_List const &) = delete;
  Sig_List (Sig_List &&) = delete;
  Sig_List &operator= (Sig_List const &) = delete;
  Sig_List &operator= (Sig_List &&) = delete;
  void add (ACE_CString const &s);
  void add (Sig_List const &other);
  void remove_current ();

  bool find_and_mark_used (Signature const *s);
  bool contains (ACE_CString const &s) const;
  bool hasmore () const;
  Signature const *first();
  Signature const *next();

  bool modified ();
  size_t size() const;

private:
  size_t index_ {};
  size_t has_nulls_ {};
  bool modified_ {};
  std::vector<std::shared_ptr<Signature>> array_;
};

#endif /* _SIG_LIST_H_ */
