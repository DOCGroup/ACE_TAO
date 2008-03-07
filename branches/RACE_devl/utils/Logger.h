// $Id$

/**
 * @file Logger.h
 *
 * This file defines RACE logger.
 *
 * @author Nishanth Shankaran <nshankar@nospam.com>
 */

#ifndef RACE_LOGGER_H
#define RACE_LOGGER_H

#include <string>
#include "Logger_export.h"

namespace CIAO
{
  namespace RACE
  {
    class LOGGER_Export Logger
    {
      public:
      Logger(const char* filename);

      virtual ~Logger(void);

      virtual void log (::std::string msg);


      private:
      FILE *log_file_;

    };
  }
}
#endif /* RACE_LOGGER_H */
