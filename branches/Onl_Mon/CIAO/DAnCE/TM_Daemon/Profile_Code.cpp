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
  if (this->running_)
    {
      this->timer_.stop ();
    }


  // start the timer ....
  this->timer_.start ();

  this->running_ = true;
}


void Profile_Code::stop ()
{
  this->timer_.stop ();
  this->timer_.elapsed_microseconds (this->elapsed_time_);
    // reset the state
  this->running_ = false;

}

void
Profile_Code::dump ()
{
  this->file_ << this->elapsed_time_ << std::endl;

}
