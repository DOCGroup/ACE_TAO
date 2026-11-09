// -*- C++ -*-
// File: Signature.h

// Author: Phil Mesnier

#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_

// Signature class encapsulates a single line of nm output. This line may be
// either an "undefined" name to be resolved, or text or data which resolves
// the unknowns. Further code reduction could avoid resolving references
// that only point to symbols in otherwise unused modules. GNU nm does not
// provide enough information to determine whether those references are
// actually needed.
//
// Shared signatures avoid duplicating symbol strings across lists.

#include <ace/SString.h>

class Signature {
public:
  explicit Signature (ACE_CString const &);
  Signature (Signature const &) = delete;
  Signature (Signature &&) = delete;
  Signature &operator= (Signature const &) = delete;
  Signature &operator= (Signature &&) = delete;
  void used ();
  int used_count() const;

  ACE_CString const &name() const;

private:
  ACE_CString name_;
  int used_ {};
};

#endif
