// $Id$

#include "common/NamespaceSupport.h"

int main (int, ACE_TCHAR *[])
{
  ACEXML_NamespaceSupport xmlns;

  xmlns.pushContext();
  xmlns.declarePrefix(ACE_TEXT (""),
                      ACE_TEXT ("http://www.w3.org/1999/xhtml"));
  xmlns.declarePrefix(ACE_TEXT ("dc"),
                      ACE_TEXT ("http://www.purl.org/dc#"));
  xmlns.declarePrefix(ACE_TEXT ("xc"),
                      ACE_TEXT ("http://www.purl.org/dc#"));

  const ACEXML_Char *lName, *uri;
  const ACEXML_Char *n1 = ACE_TEXT ("p");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("*** Checking processName:\n")));
  if (xmlns.processName (n1, uri, lName, 0) != 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to resolve namespace name %s\n"),
                n1));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Namespace URI: %s\nLocal name: %s\nRaw name: %s\n"),
                uri, lName, n1));

  ACEXML_Char *n2 = ACE_TEXT ("dc:title");
  if (xmlns.processName(n2, uri, lName, 0) != 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to resolve namespace name %S\n"),
                n2));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Namespace URI: %s\nLocal name: %s\nRaw name: %s\n"),
                uri, lName, n2));

  ACEXML_Char *n3 = ACE_TEXT ("xml:title");
  if (xmlns.processName(n3, uri, lName, 0) != 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to resolve namespace name %S\n"),
                n3));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Namespace URI: %s\nLocal name: %s\nRaw name: %s\n"),
                uri, lName, n3));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n*** Checking getDeclaredPrefixes\n")));

  ACEXML_STR_LIST prefixes;
  if (xmlns.getDeclaredPrefixes (prefixes) != 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to get a list of declared prefixes\n")));
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("prefix: \"%s\"\n"), *prefix));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** Checking getPrefix\n")));
  const ACEXML_Char *prefix = 0;

  if ((prefix = xmlns.getPrefix (ACE_TEXT ("http://www.purl.org/dc#"))) == 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to get prefix of http://www.purl.org/dc#\n")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Prefix of http://www.purl.org/dc# is %s\n"),
                prefix));

  if ((prefix = xmlns.getPrefix (ACE_TEXT ("http://www.w3.org/1999/xhtml"))) == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Fail to get prefix of http://www.w3.org/1999/xhtml which is okay\n")));
  else
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("ERROR: Prefix of http://www.w3.org/1999/xhtml is %s\n"),
                prefix));

  if ((prefix = xmlns.getPrefix (ACE_TEXT ("http://www.w3.org/XML/1998/namespace"))) == 0)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("Fail to get prefix of http://www.w3.org/XML/1998/namespace\n")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Prefix of http://www.w3.org/XML/1998/namespace is %s\n"),
                prefix));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** Checking getPrefixes with known URI\n")));
  prefixes.reset ();

  if (xmlns.getPrefixes (ACE_TEXT ("http://www.purl.org/dc#"), prefixes) != 0)
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Fail to getPrefixes based on known URI\n")));
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("prefix: \"%s\"\n"), *prefix));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** Checking getPrefixes\n")));
  prefixes.reset ();

  if (xmlns.getPrefixes (prefixes) != 0)
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Fail to getPrefixes\n")));
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("prefix: \"%s\"\n"), *prefix));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** Checking getURI\n")));
  const ACEXML_Char *URI = 0;

  if ((URI = xmlns.getURI (ACE_TEXT ("dc"))) == 0)
    ACE_DEBUG((LM_ERROR, ACE_TEXT ("Fail to get URI for \"dc\"\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("URI for \"dc\" is %s\n"), URI));
  if ((URI = xmlns.getURI (ACE_TEXT ("xc"))) == 0)
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Fail to get URI for \"xc\"\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("URI for \"xc\" is %s\n"), URI));
  if ((URI = xmlns.getURI (ACE_TEXT ("xml"))) == 0)
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Fail to get URI for \"xml\"\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("URI for \"xml\" is %s\n"), URI));
  if ((URI = xmlns.getURI (ACE_TEXT (""))) == 0)
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Fail to get default namespace URI\n")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("URI for default namespace is %s\n"), URI));

  xmlns.popContext();

  return 0;
}
