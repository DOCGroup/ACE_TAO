#include "Timer.h"
#include "Exceptions.h"
#include "ace/Time_Value.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

namespace CIAO
{
  namespace TM_Daemon
  {
    namespace utils
    {

      Timer::Timer ()
        : running_ (false)
      {}

      Timer::Timer (const char* file_name)
        :running_(false)
      {

        // open the file .
        this->file_ = ACE_OS::fopen (file_name, "w");
        if (this->file_ == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "TM_Timer::Cannot open output file %s for writing.\n",
                        this->file_));
            throw new Initialization_Exception ();
          }
      }

      Timer::Timer (const Timer &timer)
        : timer_ (timer.timer_),
          elapsed_time_ (timer.elapsed_time_),
          file_ (timer.file_),
          running_ (timer.running_)
      {}

      void
      Timer::operator = (const Timer &timer)
      {
        this->timer_  = timer.timer_;
        this->elapsed_time_ = timer.elapsed_time_;
        this->file_ = timer.file_;
        this->running_ = timer.running_;
      }


      Timer::~Timer ()
      {
        ACE_OS::fclose (this->file_);
      }


      void Timer::start ()
      {

        // if already running, restart it.
        if (this->running_)
          {
            this->timer_.stop ();
          }

        // start the timer.
        this->timer_.start ();

        this->running_ = true;
      }


      void Timer::stop ()
      {
        this->timer_.stop ();
        this->timer_.elapsed_microseconds (this->elapsed_time_);
        // reset the state
        this->running_ = false;

      }

      void Timer::dump ()
      {
        ACE_OS::fprintf (this->file_, "%u\n", this->elapsed_time_);
        ACE_OS::fflush (this->file_);
      }
    }
  }
}
