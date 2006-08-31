// $Id$

#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <memory>
#include <cctype>
#include "Utils/Utils_Export.h"

namespace Utils
{

  /// This method prompts a dialog to allow the user to specify a folder
  Utils_Export bool getPath (const std::string& description, std::string& path);

  /// This method will create a UUID.
  Utils_Export std::string CreateUuid ();

  /// This method will determine if a UUID is valid.
  Utils_Export bool ValidUuid (const std::string & uuid);
} // namespace Utils

#endif // UTILS_UTILS_H
