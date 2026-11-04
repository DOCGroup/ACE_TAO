
#ifndef bogus_h
#define bogus_h
#include /**/ "ace/pre.h"

#include "bogusS.h"

class bogus
  : public virtual POA_Test::bogus
{
public:
  bogus () = default;
  bogus (bogus const &) = delete;
  bogus (bogus &&) = delete;
  bogus &operator= (bogus const &) = delete;
  bogus &operator= (bogus &&) = delete;
  ~bogus () override = default;

  void noop () override;
};

#include /**/ "ace/post.h"
#endif //bogus_h
