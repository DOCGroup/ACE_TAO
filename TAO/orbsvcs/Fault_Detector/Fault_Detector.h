// $Id$
#ifndef FAULT_DETECTOR_H_
#define FAULT_DETECTOR_H_

#include "tao/corba.h"
class TAO_Fault_Detector
{
};

class TAO_Fault_Detector_Factory
{
  ///////////////////////////
  // Construction/Destruction
 public:
  TAO_Fault_Detector_Factory ();
  ~TAO_Fault_Detector_Factory ();

  int parse_args (int argc, char * argv[]);
  void set_orb (CORBA::ORB_var & orb);

  ////////////////////
  // Forbidden methods
 private:
  TAO_Fault_Detector_Factory (const TAO_Fault_Detector_Factory & rhs);
  TAO_Fault_Detector_Factory & operator = (const TAO_Fault_Detector_Factory & rhs);

  ///////////////
  // Data Members
 private:
  CORBA::ORB_var orb_;

};
#endif /* FAULT_DETECTOR_H_  */

