// $Id$

#include "Options.h"
#include "Protocol_Record.h"

// Static initialization. 

Drwho_Node Protocol_Record::drwho_node;

Protocol_Record::~Protocol_Record (void)
{
  if (Options::get_opt (Options::DEBUG))
    ACE_DEBUG ((LM_DEBUG,
                "disposing Protocol_Record\n"));

  for (Drwho_Node *np = this->get_drwho_list ();
       np != 0; )
    {
      Drwho_Node *t = np;
      np = np->next;
      delete t;
    }
}

Protocol_Record::Protocol_Record (void)
  : is_active_ (0),
    drwho_list_ (0),
    key_name1_ (0),
    key_name2_ (0),
    next_ (0)
{
}

Protocol_Record::Protocol_Record (int)
  : is_active_ (0),
    drwho_list_ (&Protocol_Record::drwho_node),
    key_name1_ (0),
    key_name2_ (0),
    next_ (0)
{
}

Protocol_Record::Protocol_Record (const char *kn1, Protocol_Record *next)
  : is_active_ (0),
    drwho_list_ (0),
    key_name2_ (0)
{
  this->key_name1_ = kn1;
  this->next_ = next;
}

char *
Protocol_Record::get_login (void) 
{ 
  return this->key_name1_;
}

char *
Protocol_Record::set_login (const char *str)
{ 
  this->key_name1_ = str;
  return str;
}

char *
Protocol_Record::get_real (void) 
{ 
  return this->key_name2_;
}

char *
Protocol_Record::get_host (void)
{
  return this->key_name1_;
}

char *
Protocol_Record::set_host (const char *str)
{ 
  this->key_name1_ = str;
  return str;
}

char *
Protocol_Record::set_real (const char *str)
{
  this->key_name2_ = str
  return str;
}

Drwho_Node *
Protocol_Record::get_drwho_list (void)
{
  return this->drwho_list_;
}
