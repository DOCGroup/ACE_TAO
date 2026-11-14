// HTTP_Config.cpp

#include "ace/OS_NS_stdlib.h"
#include "HTTP_Config.h"

HTTP_Config_Info *
HTTP_Config::instance ()
{
  static HTTP_Config_Info config_info;
  return &config_info;
}

HTTP_Config_Info::HTTP_Config_Info ()
{
  this->document_root_ = ACE_OS::getenv ("JAWS_DOCUMENT_ROOT");
  if (this->document_root_ == nullptr)
    this->document_root_ = ".";

  this->cgi_path_ = ACE_OS::getenv ("JAWS_CGI_PATH");
  if (this->cgi_path_ == nullptr)
    this->cgi_path_ = "cgi-bin";

  this->user_dir_ = ACE_OS::getenv ("JAWS_USER_DIR");
  if (this->user_dir_ == nullptr)
    this->user_dir_ = ".www";

  this->dir_index_ = ACE_OS::getenv ("JAWS_DIR_INDEX");
  if (this->dir_index_ == nullptr)
    this->dir_index_ = "index.html";
}

char const *
HTTP_Config_Info::document_root () const
{
  return this->document_root_;
}

char const *
HTTP_Config_Info::cgi_path () const
{
  return this->cgi_path_;
}

char const *
HTTP_Config_Info::user_dir () const
{
  return this->user_dir_;
}

char const *
HTTP_Config_Info::dir_index () const
{
  return this->dir_index_;
}

int
HTTP_Config_Info::proxy_flag () const
{
  return this->proxy_flag_;
}
