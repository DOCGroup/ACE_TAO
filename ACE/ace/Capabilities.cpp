#include "ace/Capabilities.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "ace/Capabilities.inl"
#endif /* !__ACE_INLINE__ */

#include "ace/OS_NS_stdio.h"

#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Capabilities::ACE_Capabilities ()
  : caps_ ()
{
}

ACE_Capabilities::~ACE_Capabilities ()
{
  this->resetcaps ();
}

ACE_TCHAR const *
ACE_Capabilities::parse (ACE_TCHAR const *buf, ACE_TString &cap)
{
  while (*buf != ACE_TEXT ('\0') && *buf != ACE_TEXT (','))
    {
      if (*buf == ACE_TEXT ('\\'))
        {
          ++buf;
          if (*buf == ACE_TEXT ('E') || *buf == ACE_TEXT ('e'))
            {
              cap += ACE_TEXT ('\033');
              ++buf;
              continue;
            }
          else if (*buf == ACE_TEXT ('r'))
            {
              cap += ACE_TEXT ('\r');
              ++buf;
              continue;
            }
          else if (*buf == ACE_TEXT ('n'))
            {
              cap += ACE_TEXT ('\n');
              ++buf;
              continue;
            }
          else if (*buf == ACE_TEXT ('t'))
            {
              cap += ACE_TEXT ('\t');
              ++buf;
              continue;
            }
          else if (*buf == ACE_TEXT ('\\'))
            {
              cap += *buf++;
              continue;
            }
          if (ACE_OS::ace_isdigit(*buf))
            {
              // @@ UNICODE Does this work with unicode?
              int oc {};
              for (int i {};
                   i < 3 && *buf && ACE_OS::ace_isdigit (*buf);
                   ++i)
                {
                  oc = oc * 8 + (*buf++ - ACE_TEXT ('0'));
                }

              cap += static_cast<ACE_TCHAR> (oc);
              continue;
            }
        }
      cap += *buf++;
    }
  return buf;
}

ACE_TCHAR const *
ACE_Capabilities::parse (ACE_TCHAR const *buf, int &cap)
{
  int n {};

  while (*buf && ACE_OS::ace_isdigit (*buf))
    {
      n = n * 10 + (*buf++ - ACE_TEXT ('0'));
    }

  cap = n;

  return buf;
}

void
ACE_Capabilities::resetcaps ()
{
  for (CAPABILITIES_MAP::ITERATOR iter (this->caps_);
       !iter.done ();
       iter.advance ())
    {
      CAPABILITIES_MAP::ENTRY *entry {};
      iter.next (entry);
      delete entry->int_id_;
    }

  this->caps_.unbind_all ();
}

int
ACE_Capabilities::fillent (ACE_TCHAR const *buf)
{
  this->resetcaps ();
  while (*buf)
    {
      ACE_TString s;
      int n;
      ACE_TString name;
      ACE_CapEntry *ce;

      // Skip blanks
      while (*buf && ACE_OS::ace_isspace(*buf))
        {
          ++buf;
        }
      // If we get end of line return

      if (*buf == ACE_TEXT ('\0'))
        {
          break;
        }

      if (*buf == ACE_TEXT ('#'))
        {
          while (*buf && *buf != ACE_TEXT ('\n'))
            {
              ++buf;
            }
          if (*buf == ACE_TEXT ('\n'))
            {
              ++buf;
            }
          continue;
        }
      while(*buf && *buf != ACE_TEXT ('=')
            && *buf!= ACE_TEXT ('#')
            && *buf != ACE_TEXT (','))
        {
          name += *buf++;
        }

      // If name is null.
      switch (*buf)
        {
        case ACE_TEXT ('='):
          // String property
          buf = this->parse (buf + 1, s);
          ACE_NEW_RETURN (ce,
                          ACE_StringCapEntry (s),
                          -1);
          break;
        case ACE_TEXT ('#'):
          // Integer property
          buf = this->parse (buf + 1, n);
          ACE_NEW_RETURN (ce,
                          ACE_IntCapEntry (n),
                          -1);
          break;
        case ACE_TEXT (','):
          // Boolean
          ACE_NEW_RETURN (ce,
                          ACE_BoolCapEntry (1),
                          -1);
          break;
        default:
          return 0;
        }

      std::unique_ptr<ACE_CapEntry> entry (ce);
      int const bind_result = this->caps_.bind (name, entry.get ());
      if (bind_result == -1)
        {
          return -1;
        }
      if (bind_result == 0)
        {
          entry.release ();
        }

      if (*buf++ != ACE_TEXT (','))
      {
        return -1;
      }
    }

  return 0;
}

bool
ACE_Capabilities::is_entry (ACE_TCHAR const *name, ACE_TCHAR const *line)
{
  for (;;)
    {
      // Skip blanks or irrelevant characters
      while (*line && ACE_OS::ace_isspace(*line))
        {
          ++line;
        }

      // End of line reached
      if (*line == ACE_TEXT ('\0'))
        {
          break;
        }

      // Build the entry name
      ACE_TString nextname;
      while (*line && *line != ACE_TEXT ('|') && *line != ACE_TEXT (','))
        {
          nextname += *line++;
        }

      // We have found the required entry?
      if (ACE_OS::strcmp (nextname.c_str (), name) == 0)
        {
          return true;
        }

      // Skip puntuaction char if neccesary.
      if (*line == ACE_TEXT ('|') || *line == ACE_TEXT (','))
        {
          ++line;
        }
      else
        {
          ACELIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Invalid entry\n")));
          break;
        }
    }
  return false;
}

int
ACE_Capabilities::getline (FILE *fp, ACE_TString &line)
{
  int ch;

  line.set (nullptr, false);

  while ((ch = ACE_OS::fgetc (fp)) != EOF && ch != ACE_TEXT ('\n'))
    {
      line += static_cast<ACE_TCHAR> (ch);
    }

  if (ch == EOF && line.length () == 0)
    {
      return -1;
    }
  else
  {
    return 0;
  }
}

int
ACE_Capabilities::getval (ACE_TCHAR const *keyname, ACE_TString &val)
{
  ACE_CapEntry* cap {};
  if (this->caps_.find (keyname, cap) == -1)
  {
    return -1;
  }

  ACE_StringCapEntry *scap = dynamic_cast<ACE_StringCapEntry *> (cap);
  if (scap == nullptr)
  {
    return -1;
  }

  val = scap->getval ();
  return 0;
}

int
ACE_Capabilities::getval (ACE_TCHAR const *keyname, int &val)
{
  ACE_CapEntry *cap {};
  if (this->caps_.find (keyname, cap) == -1)
  {
    return -1;
  }

  ACE_IntCapEntry *icap = dynamic_cast<ACE_IntCapEntry *> (cap);
  if (icap != nullptr)
    {
      val = icap->getval ();
      return 0;
    }

  ACE_BoolCapEntry *bcap = dynamic_cast<ACE_BoolCapEntry *> (cap);

  if (bcap == nullptr)
  {
    return -1;
  }

  val = bcap->getval ();
  return 0;
}

static bool
is_empty (ACE_TCHAR const *line)
{
  while (*line && ACE_OS::ace_isspace (*line))
    {
      ++line;
    }

  return *line == ACE_TEXT ('\0') || *line == ACE_TEXT ('#');
}

static bool
is_line (ACE_TCHAR const *line)
{
  while (*line && ACE_OS::ace_isspace (*line))
    {
      ++line;
    }

  return *line != ACE_TEXT ('\0');
}

int
ACE_Capabilities::getent (ACE_TCHAR const *fname, ACE_TCHAR const *name)
{
  std::unique_ptr<FILE, int (*) (FILE *)> fp {
    ACE_OS::fopen (fname, ACE_TEXT ("r")), ACE_OS::fclose};

  if (fp == nullptr)
    {
      ACELIB_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("Can't open %s file\n"),
                            fname),
                           -1);
    }

  bool done {};
  ACE_TString line;

  while (!(done = (this->getline (fp.get (), line) == -1))
         && is_empty (line.c_str ()))
    {
      continue;
    }

  while (!done)
    {
      ACE_TString newline;
      ACE_TString description;

      while (!(done = (this->getline (fp.get (), newline) == -1)))
        {
          if (is_line (newline.c_str ()))
            {
              description += newline;
            }
          else
            {
              break;
            }
        }

      if (this->is_entry (name, line.c_str()))
        {
          fp.reset ();
          return this->fillent (description.c_str ());
        }

      line = newline;
      while (!done && is_empty (line.c_str ()))
        {
          done = this->getline (fp.get (), line) == -1;
        }
    }

  return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_StringCapEntry)
ACE_ALLOC_HOOK_DEFINE(ACE_IntCapEntry)
ACE_ALLOC_HOOK_DEFINE(ACE_BoolCapEntry)

ACE_END_VERSIONED_NAMESPACE_DECL
