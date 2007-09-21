#ifndef TIMER_H
#define TIMER_H
#include "ace/High_Res_Timer.h"

namespace CIAO
{
  namespace TM_Daemon
  {
    namespace utils
    {

      class Timer
      {
      public:
        Timer (const char* file);

        Timer (const Timer &timer);

        Timer ();

        virtual void operator = (const Timer &timer);

        virtual ~Timer ();

        virtual void start ();

        virtual void stop ();

        virtual void dump ();


      private:
        /// The high resolution timer.
        ACE_High_Res_Timer timer_;

        /// Elapsed time.
        ACE_hrtime_t elapsed_time_;

        /// output file.
        FILE * file_;

        /// state of the timer
        bool running_;
      };
    }
  }
}
#endif
