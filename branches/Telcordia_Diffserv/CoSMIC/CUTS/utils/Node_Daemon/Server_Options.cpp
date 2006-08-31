// $Id$

#include "Server_Options.h"

//
// Server_Options
//
Server_Options::Server_Options (void)
: verbose_ (false)
{

}

//
// ~Server_Options
//
Server_Options::~Server_Options (void)
{

}

//
// node_manager_path
//
void Server_Options::node_manager_path (const char * path)
{
  this->path_ = path;
  size_t length = this->path_.length ();
  char last_char = this->path_[length - 1];

#if defined (ACE_WIN32)
#define LAST_CHAR '\\'
#else
#define LAST_CHAR '/'
#endif

  if (last_char != LAST_CHAR)
    this->path_ += LAST_CHAR;
}

//
// node_manager_path
//
const ACE_CString & Server_Options::node_manager_path (void) const
{
  return this->path_;
}
