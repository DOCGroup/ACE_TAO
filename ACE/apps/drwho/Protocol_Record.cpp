#include "Options.h"
#include "Protocol_Record.h"
#include "ace/Log_Msg.h"

// Static initialization.

Drwho_Node Protocol_Record::drwho_node_;

Protocol_Record::~Protocol_Record ()
{
  if (Options::get_opt (Options::DEBUGGING))
    ACE_DEBUG ((LM_DEBUG,
                "disposing Protocol_Record\n"));

  for (Drwho_Node *np = this->get_drwho_list ();
       np != nullptr; )
    {
      Drwho_Node *t = np;
      np = np->next_;
      delete t;
    }
}

Protocol_Record::Protocol_Record ()
  : key_name1_ (nullptr),
    key_name2_ (nullptr),
    drwho_list_ (nullptr),
    next_ (nullptr)
{
}

Protocol_Record::Protocol_Record (int)
  : key_name1_ (nullptr),
    key_name2_ (nullptr),
    drwho_list_ (&Protocol_Record::drwho_node_),
    next_ (nullptr)
{
}

Protocol_Record::Protocol_Record (const char *kn1,
                                  Protocol_Record *next)
  : key_name1_ (kn1),
    key_name2_ (nullptr),
    drwho_list_ (nullptr),
    next_ (next)
{
}

const char *
Protocol_Record::get_login ()
{
  return this->key_name1_;
}

const char *
Protocol_Record::set_login (const char *str)
{
  this->key_name1_ = str;
  return str;
}

const char *
Protocol_Record::get_real ()
{
  return this->key_name2_;
}

const char *
Protocol_Record::get_host ()
{
  return this->key_name1_;
}

const char *
Protocol_Record::set_host (const char *str)
{
  this->key_name1_ = str;
  return str;
}

const char *
Protocol_Record::set_real (const char *str)
{
  this->key_name2_ = str;
  return str;
}

Drwho_Node *
Protocol_Record::get_drwho_list ()
{
  return this->drwho_list_;
}
