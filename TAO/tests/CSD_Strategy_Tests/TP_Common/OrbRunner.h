#ifndef ORB_RUNNER_H
#define ORB_RUNNER_H

#include "CSD_TP_Test_Export.h"
#include "tao/ORB.h"

class CSD_TP_Test_Export OrbRunner
{
public:
  explicit OrbRunner(CORBA::ORB_ptr orb, unsigned num_orb_threads = 1);
  virtual ~OrbRunner();

  void run();

  OrbRunner (OrbRunner const &) = delete;
  OrbRunner (OrbRunner &&) = delete;
  OrbRunner & operator= (OrbRunner const &) = delete;
  OrbRunner & operator= (OrbRunner &&) = delete;

private:
  CORBA::ORB_var orb_;
  unsigned       num_orb_threads_;
};

#endif
