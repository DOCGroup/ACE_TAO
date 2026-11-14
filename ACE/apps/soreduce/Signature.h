// -*- C++ -*-
// File: Signature.h

// Author: Phil Mesnier

#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_

#include <ace/SString.h>

/**
 * @brief Encapsulates one symbol reported by nm.
 *
 * Shared signatures avoid duplicating symbol strings across lists.
 */
class Signature {
public:
  explicit Signature (ACE_CString const &);
  Signature (Signature const &) = delete;
  Signature (Signature &&) = delete;
  Signature &operator= (Signature const &) = delete;
  Signature &operator= (Signature &&) = delete;
  void used ();
  size_t used_count () const;

  ACE_CString const &name() const;

private:
  ACE_CString name_;
  size_t used_ {};
};

#endif
