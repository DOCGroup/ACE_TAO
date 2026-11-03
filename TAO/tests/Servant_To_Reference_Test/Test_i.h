
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

class One_Impl
  : public virtual POA_Test::One
{
public:
  /// Constructor
  One_Impl () = default;
  One_Impl (One_Impl const &) = delete;
  One_Impl (One_Impl &&) = delete;
  One_Impl & operator= (One_Impl const &) = delete;
  One_Impl & operator= (One_Impl &&) = delete;

  // = The skeleton methods
  char * get_string () override;
};

class Two_Impl
  : public virtual POA_Test::Two
{
public:
  /// Constructor
  Two_Impl () = default;
  Two_Impl (Two_Impl const &) = delete;
  Two_Impl (Two_Impl &&) = delete;
  Two_Impl & operator= (Two_Impl const &) = delete;
  Two_Impl & operator= (Two_Impl &&) = delete;

  // = The skeleton methods
  char * get_string () override;
};

class Three_Impl
  : public virtual POA_Test::Three
{
public:
  /// Constructor
  Three_Impl () = default;
  Three_Impl (Three_Impl const &) = delete;
  Three_Impl (Three_Impl &&) = delete;
  Three_Impl & operator= (Three_Impl const &) = delete;
  Three_Impl & operator= (Three_Impl &&) = delete;

  // = The skeleton methods
  char * get_string () override;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
