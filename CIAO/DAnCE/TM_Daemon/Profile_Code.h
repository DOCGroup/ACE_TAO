#ifndef PROFILE_CODEH
#define PROFILE_CODEH

#include "ace/High_Res_Timer.h"
#include <fstream>

class ACE_High_Res_Timer;

class Profile_Code
{
public:
  Profile_Code (const std::string &file);

  Profile_Code (){};

  ~Profile_Code ();

  void start ();

  void stop ();

  void dump ();


private:

  /// The high resolution Timer ...
  ACE_High_Res_Timer timer_;

  /// Elapsed time.
  ACE_hrtime_t elapsed_time_;

  std::ofstream file_;

  /// state of the timer
  bool running_;
};



#endif
