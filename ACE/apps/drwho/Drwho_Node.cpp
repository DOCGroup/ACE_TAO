#include "Drwho_Node.h"

Drwho_Node::Drwho_Node (char const *h_name, Drwho_Node *n)
     : key_name1_ (h_name),
       next_ (n)
{}

Drwho_Node::Drwho_Node () = default;

char const *
Drwho_Node::get_login_name ()
{
  return this->key_name1_;
}

char const *
Drwho_Node::set_login_name (char const *str)
{
  this->key_name1_ = str;
  return str;
}

char const *
Drwho_Node::get_real_name ()
{
  return this->key_name2_;
}

char const *
Drwho_Node::set_real_name (char const *str)
{
  this->key_name2_ = str;
  return str;
}

char const *
Drwho_Node::get_host_name ()
{
  return this->key_name1_;
}

char const *
Drwho_Node::set_host_name (char const *str)
{
  this->key_name1_ = str;
  return str;
}

int
Drwho_Node::get_active_count ()
{
  return this->active_count_;
}

int
Drwho_Node::get_inactive_count ()
{
  return this->inactive_count_;
}

int
Drwho_Node::set_active_count (int count)
{
  this->active_count_ = count;
  return count;
}

int
Drwho_Node::set_inactive_count (int count)
{
  this->inactive_count_ = count;
  return count;
}

int
Drwho_Node::set_idle_time (int idle_time)
{
  this->idle_time_ = idle_time;
  return idle_time;
}

int
Drwho_Node::get_idle_time ()
{
  return this->idle_time_;
}
