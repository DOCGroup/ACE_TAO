// $Id$
#include "Options.h"
#include "Protocol_Record.h"

/* Static initialization. */

Drwho_Node Protocol_Record::drwho_node;

Protocol_Record::~Protocol_Record (void)
{
  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "disposing Protocol_Record\n");

  for (Drwho_Node *np = this->get_drwho_list (); np != 0; )
    {
      Drwho_Node *t = np;
      np	   = np->next;
    }
}

#ifndef __OPTIMIZE__
Protocol_Record::Protocol_Record (void)
     : is_active (0), drwho_list (0), key_name1 (0), key_name2 (0), next (0)
{}

Protocol_Record::Protocol_Record (int)
     : is_active (0), drwho_list (&Protocol_Record::drwho_node), key_name1 (0), key_name2 (0), next (0)
{}


Protocol_Record::Protocol_Record (char *kn1, Protocol_Record *next)
     : is_active (0), drwho_list (0), key_name2 (0)
{
  this->key_name1 = kn1;
  this->next = next;
}

char *
Protocol_Record::get_login (void) 
{ 
  return this->key_name1; 
}

char *
Protocol_Record::set_login (char *str)
{ 
  return this->key_name1 = str; 
}

char *
Protocol_Record::get_real (void) 
{ 
  return this->key_name2; 
}

char *
Protocol_Record::get_host (void)
{
  return this->key_name1;
}

char *
Protocol_Record::set_host (char *str)
{ 
  return this->key_name1 = str; 
}

char *
Protocol_Record::set_real (char *str)
{
  return this->key_name2 = str;
}

Drwho_Node *
Protocol_Record::get_drwho_list (void)
{
  return this->drwho_list;
}
#endif /* __OPTIMIZE__ */
