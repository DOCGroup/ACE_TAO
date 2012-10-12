// $Id$
#ifndef BUG_680_REGRESSION_I_H
#define BUG_680_REGRESSION_I_H

#include "bug_689_regressionS.h"

class bug_689_regression_i: public POA_bug_689_regression
{
public:
  bug_689_regression_i (CORBA::ORB_ptr orb);
  ~bug_689_regression_i (void);

  virtual void shutdown (void);

  virtual void noop (void);

private:
  CORBA::ORB_var orb_;
};

#endif
