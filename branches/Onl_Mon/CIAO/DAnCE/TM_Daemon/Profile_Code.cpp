#include "Profile_Code.h"
#include "ace/Time_Value.h"

Profile_Code::Profile_Code (const std::string& file)
 :running_(false)
{
  // open the file ...
  this->file_.open (file.c_str (), std::ios::app);
}


Profile_Code::~Profile_Code ()
{
  this->file_.close ();
}


void Profile_Code::start ()
{

// if still running ... stop it and then run again ...
  if (running_)
     time.stop ();

  // start the timer ....
  time.start ();

  running_ = true;
}


void Profile_Code::stop ()
{
  time.stop ();

  ACE_Time_Value tv;

  //  time.elapsed_time (tv);

  ACE_hrtime_t tm;

  time.elapsed_microseconds (tm);

  //  file_ << tv.msec () << std::endl;
  file_ << tm << std::endl;

  // reset the state 
  running_ = false;
}
