// $Id$

#include "HTTP_10.h"

#include "JAWS/JAWS.h"

JAWS_HTTP_10_Headers::JAWS_HTTP_10_Headers (void)
  : iter_ (*this)
{
}

JAWS_HTTP_10_Headers::~JAWS_HTTP_10_Headers (void)
{
}

int
JAWS_HTTP_10_Headers::lessthan (const char *const &k1, const char *const &k2)
{
  return ACE_OS::strcmp (k1, k2) < 0;
}

Symbol_Table_Iterator &
JAWS_HTTP_10_Headers::iter (void)
{
  return this->iter_;
}

char *
JAWS_HTTP_10_Helper::HTTP_decode_string (char *path)
{
  // replace the percentcodes with the actual character
  int i, j;
  char percentcode[3];

  for (i = j = 0; path[i] != '\0'; i++, j++)
    {
      if (path[i] == '%')
	{
	  percentcode[0] = path[++i];
	  percentcode[1] = path[++i];
	  percentcode[2] = '\0';
	  path[j] = (char) ACE_OS::strtol (percentcode, (char **) 0, 16);
	}
      else
	path[j] = path[i];
    }

  path[j] = path[i];

  return path;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class RB_Tree_Node<const char *, const char *>;
template class RB_Tree<const char *, const char *>;
template class RB_Tree_Iterator<const char *, const char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate RB_Tree_Node<const char *, const char *>
#pragma instantiate RB_Tree<const char *, const char *>
#pragma instantiate RB_Tree_Iterator<const char *, const char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
