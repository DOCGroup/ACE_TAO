// $Id$

#include "ace/Map_Manager.h"
#include "ace/Capabilities.h"

#if !defined (__ACE_INLINE__)
#include "ace/Capabilities.i"
#endif /* !__ACE_INLINE__ */

#define ACE_ESC ((char)0x1b)

ACE_CapEntry::~ACE_CapEntry (void)
{
}

ACE_Capabilities::ACE_Capabilities (void)
{
}

ACE_Capabilities::~ACE_Capabilities (void)
{
  this->resetcaps ();
}

const char *
ACE_Capabilities::parse (const char *buf,
                         ACE_CString &cap)
{
  while (*buf!='\0' && *buf!=',')
    {
      if (*buf == '\\')
        {
          buf++;
          if (*buf == 'E' || *buf == 'e')
            {
              cap += ACE_ESC;
              buf++;
              continue;
            }
          else if (*buf == 'r')
            {
              cap += '\r';
              buf++;
              continue;
            }
          else if (*buf == 'n')
            {
              cap += '\n';
              buf++;
              continue;
            }
          else if (*buf == 't')
            {
              cap += '\t';
              buf++;
              continue;
            }
          else if (*buf == '\\')
            {
              cap += *buf++;
              continue;
            }
          if (isdigit(*buf))
            {
              int oc = 0;
              for (int i = 0;
                   i < 3 && *buf && isdigit (*buf);
                   i++)
                oc = oc * 8 + (*buf++ - '0');

              cap += (char) oc;
              continue;
            }
        }
      cap += *buf++;
    }
  return buf;
}

const char *
ACE_Capabilities::parse (const char *buf,
                         int &cap)
{
  int n = 0;

  while (*buf && isdigit (*buf))
    n = n * 10 + (*buf++ - '0');

  cap = n;

  return buf;
}

void
ACE_Capabilities::resetcaps (void)
{
  for (ACE_Hash_Map_Iterator<ACE_CString, ACE_CapEntry *, ACE_Null_Mutex> iter (caps_);
       !iter.done ();
       iter.advance ())
    {
      ACE_Hash_Map_Entry<ACE_CString,ACE_CapEntry*> *entry;
      iter.next (entry);
      delete entry->int_id_;
    }

  caps_.close ();
  caps_.open ();
}

int
ACE_Capabilities::fillent (const char *buf)
{
  this->resetcaps ();
  while (*buf)
    {
      ACE_CString s;
      int n;
      ACE_CString name;
      ACE_CapEntry *ce;

      // Skip blanks
      while (*buf && isspace(*buf)) buf++;
      // If we get end of line return

      if (*buf=='\0')
        break;

      if (*buf=='#')
        {
          while (*buf && *buf!='\n')
            buf++;
          if (*buf=='\n')
            buf++;
          continue;
        }
      while(*buf && *buf != '='
            && *buf!= '#'
            && *buf != ',')
        name += *buf++;

      // If name is null.
      switch (*buf)
        {
        case '=':
          // String property
          buf = this->parse (buf + 1, s);
          ACE_NEW_RETURN (ce,
                          ACE_StringCapEntry (s),
                          -1);
          if (caps_.bind (name, ce) == -1)
            {
              delete ce;
              return -1;
            }
          break;
        case '#':
          // Integer property
          buf = this->parse (buf + 1, n);
          ACE_NEW_RETURN (ce,
                          ACE_IntCapEntry (n),
                          -1);
          if (caps_.bind (name, ce) == -1)
            {
              delete ce;
              return -1;
            }
          break;
        case ',':
          // Boolean
          ACE_NEW_RETURN (ce,
                          ACE_BoolCapEntry (1),
                          -1);
          if (caps_.bind (name, ce) == -1)
            {
              delete ce;
              return -1;
            }
          break;
        default:
          return 0;
        }

      if (*buf++!=',')
        return -1;
    }

  return 0;
}

int
ACE_Capabilities::is_entry (const char *name,
                            const char *line)
{
  for (;;)
    {
      // Skip blanks or irrelevant characters
      while (*line && isspace(*line))
        line++;

      // End of line reached
      if (*line=='\0')
        break;

      // Build the entry name
      ACE_CString nextname;
      while (*line && *line != '|' && *line != ',')
        nextname += *line++;

      // We have found the required entry?
      if (ACE_OS::strcmp (nextname.c_str (), name) == 0)
        return 1;

      // Skip puntuaction char if neccesary.
      if (*line=='|' || *line==',')
        line++;
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Invalid entry\n"));
          break;
        }
    }
  return 0;
}

int
ACE_Capabilities::getline (FILE *fp,
                           ACE_CString &line)

{
  int ch;

  line.set (0, 0);

  while ((ch = fgetc (fp)) != EOF && ch != '\n')
    line += (char) ch;

  if (ch == EOF && line.length () == 0)
    return -1;
  else
    return 0;
}

int
ACE_Capabilities::getval (const char *keyname,
                          ACE_CString &val)
{
  ACE_CapEntry* cap;
  if (caps_.find (keyname, cap) == -1)
    return -1;

  ACE_StringCapEntry *scap =
    ACE_dynamic_cast (ACE_StringCapEntry *,
                      cap);
  if (scap == 0)
    return -1;

  val = scap->getval ();
  return 0;
}

int
ACE_Capabilities::getval (const char *keyname,
                          int &val)
{
  ACE_CapEntry *cap;
  if (caps_.find (keyname, cap) == -1)
    return -1;

  ACE_IntCapEntry *icap =
    ACE_dynamic_cast (ACE_IntCapEntry *,
                      cap);
  if (icap != 0)
    {
      val = icap->getval ();
      return 0;
    }

  ACE_BoolCapEntry *bcap =
    ACE_dynamic_cast (ACE_BoolCapEntry *,
                      cap);

  if (bcap == 0)
    return -1;

  val = bcap->getval ();
  return 0;
}

static int
is_empty (const char *line)
{
  while (*line && isspace (*line))
    line++;

  return *line == '\0' || *line == '#';
}

static int
is_line (const char *line)
{
  while (*line && isspace (*line))
    line++;

  return *line != '\0';
}

int
ACE_Capabilities::getent (const char *fname,
                          const char *name)
{
  FILE *fp = ACE_OS::fopen (fname,
                            "r");

  if (fp == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Can't open %s file\n",
                       fname),
                      -1);

  int done;
  ACE_CString line;

  while (!(done = (this->getline (fp, line) == -1))
         && is_empty (line.c_str ()))
    continue;

  while (!done)
    {
      ACE_CString newline;
      ACE_CString description;

      while (!(done= (this->getline (fp, newline) == -1)))
        if (is_line (newline.c_str ()))
          description += newline;
        else
          break;

      if (this->is_entry (name, line.c_str()))
        {
          ACE_OS::fclose (fp);
          return this->fillent (description.c_str ());
        }

      line = newline;
      while (!done && is_empty (line.c_str ()))
        done = this->getline (fp, line) == -1;
    }

  ACE_OS::fclose (fp);
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString,ACE_CapEntry*>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>;
template class ACE_Hash<ACE_CString>;
template class ACE_Equal_To<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString,ACE_CapEntry*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString,ACE_CapEntry*>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString,ACE_CapEntry*,ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Equal_To<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
