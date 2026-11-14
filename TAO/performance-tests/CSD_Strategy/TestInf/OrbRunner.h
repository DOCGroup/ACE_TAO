#ifndef ORB_RUNNER_H
#define ORB_RUNNER_H

#include "CSD_PT_TestInf_Export.h"
#include "tao/ORB.h"

class CSD_PT_TestInf_Export OrbRunner
{
public:
  explicit OrbRunner (CORBA::ORB_ptr orb, unsigned num_orb_threads = 1);
  OrbRunner (OrbRunner const &) = delete;
  OrbRunner (OrbRunner &&) = delete;
  OrbRunner &operator= (OrbRunner const &) = delete;
  OrbRunner &operator= (OrbRunner &&) = delete;
  virtual ~OrbRunner();

  void run ();

private:
  CORBA::ORB_var orb_;
  unsigned       num_orb_threads_;
};

#endif
