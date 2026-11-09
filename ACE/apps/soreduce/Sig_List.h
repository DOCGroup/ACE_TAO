// -*- C++ -*-
// File: Sig_List.h

// Author: Phil Mesnier

#ifndef _SIG_LIST_H_
#define _SIG_LIST_H_

// A Sig_List is a specialized container of signatures. The initial use of a
// Sig_List was to manage a variable length of undefined Signatures, so the
// program could know when all possible resolutions were determined. As the
// program grows in complexity, Sig_Lists are used to store other groups as
// well.  The methods provide simple list traversal, as well as efficient use
// of space.

#include "Signature.h"
#include <memory>
#include <vector>

class Sig_List {
public:
  explicit Sig_List (int cap = 500);
  Sig_List (Sig_List const &) = delete;
  Sig_List (Sig_List &&) = delete;
  Sig_List &operator= (Sig_List const &) = delete;
  Sig_List &operator= (Sig_List &&) = delete;
  void add (ACE_CString const &s);
  void add (Sig_List const &other);
  void remove_current ();

  int index_of (Signature const *s);
  int index_of (ACE_CString const &s) const;
  bool hasmore () const;
  Signature const *first();
  Signature const *next();

  bool modified ();
  int size() const;

private:
  int index_ {};
  int has_nulls_ {};
  bool modified_ {};
  std::vector<std::shared_ptr<Signature>> array_;
};

#endif /* _SIG_LIST_H_ */
