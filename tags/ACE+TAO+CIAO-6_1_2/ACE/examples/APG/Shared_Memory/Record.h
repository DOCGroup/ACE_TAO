/**
 * $Id$
 *
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __RECORD_H_
#define __RECORD_H_

#include "ace/OS_NS_string.h"
#include "ace/Based_Pointer_T.h"

// Listing 11 code/ch17
class Record
{
public:
  Record () { }
  ~Record () { }

  Record (const Record& rec)
    : id1_(rec.id1_), id2_(rec.id2_)
  {
    ACE_OS::strcpy (recName_, rec.name_);
    this->name_ = recName_;
  }
  Record (int id1, int id2, char *name)
    : id1_(id1), id2_(id2)
  {
    ACE_OS::strcpy (recName_, name);
    this->name_ = recName_;
  }
  char *name (void) { return recName_; }
  int id1 (void) { return id1_; }
  int id2 (void) { return id2_; }

private:
  int id1_;
  int id2_;
  char recName_[128];
  ACE_Based_Pointer_Basic<char> name_;
};
// Listing 11

#endif /* __RECORD_H_ */
