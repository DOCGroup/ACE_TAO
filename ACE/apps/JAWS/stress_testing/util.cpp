#include "util.h"

URL::URL (char const *input)
{
  if (input == nullptr)
    return;

  std::string const url (input);
  std::string::size_type const scheme_end = url.find ("://");
  if (scheme_end == std::string::npos)
    return;

  this->protocol_ = url.substr (0, scheme_end);
  std::string::size_type const host_begin = scheme_end + 3;
  std::string::size_type const path_begin = url.find ('/', host_begin);
  this->hostname_ = url.substr (host_begin, path_begin - host_begin);
  this->filename_ = path_begin == std::string::npos
    ? INDEX_NAME
    : url.substr (path_begin);
}

std::string const &
URL::get_protocol () const
{
  return this->protocol_;
}

std::string const &
URL::get_hostname () const
{
  return this->hostname_;
}

std::string const &
URL::get_filename () const
{
  return this->filename_;
}




void cleanup() {
  ACE_OS::unlink(TEMPORARY_FILE_NAME);
  ACE_OS::unlink(INCOMING_FILE_NAME);
}

void sigint(int) {
  cleanup();
}
