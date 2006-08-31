// $Id$

#ifndef PICML_UTILS_H
#define PICML_UTILS_H

#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <memory>
#include <cctype>
#include "PICML/PICML_Export.h"

namespace PICML
{

  /// This method prompts a dialog to allow the user to specify a folder
  PICML_Export bool getPath (const std::string& description, std::string& path);

  /// This method will create a UUID.
  PICML_Export std::string CreateUuid ();

  /// This method will determine if a UUID is valid.
  PICML_Export bool ValidUuid (const std::string & uuid);
} // namespace PICML

#endif // PICML_UTILS_H
