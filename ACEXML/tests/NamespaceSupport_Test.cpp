// $Id$

#include "common/NamespaceSupport.h"

int main (int, char *[])
{
  ACEXML_NamespaceSupport xmlns;

  xmlns.pushContext();
  xmlns.declarePrefix("", "http://www.w3.org/1999/xhtml");
  xmlns.declarePrefix("dc", "http://www.purl.org/dc#");
  xmlns.declarePrefix("xc", "http://www.purl.org/dc#");

  const ACEXML_Char *lName, *uri;
  const ACEXML_Char *n1 = "p";

  cout << "*** Checking processName:" << endl;
  if (xmlns.processName (n1, uri, lName, 0) != 0)
    cerr << "Fail to resolve namespace name " << n1 << endl;
  else
    cout << "Namespace URI: " << uri << endl
         << "Local name: " << lName << endl
         << "Raw name: " << n1 << endl;

  ACEXML_Char *n2 = "dc:title";
  if (xmlns.processName(n2, uri, lName, 0) != 0)
    cerr << "Fail to resolve namespace name " << n2 << endl;
  else
    cout << "Namespace URI: " << uri << endl
         << "Local name: " << lName << endl
         << "Raw name: " << n2 << endl;

  ACEXML_Char *n3 = "xml:title";
  if (xmlns.processName(n3, uri, lName, 0) != 0)
    cerr << "Fail to resolve namespace name " << n3 << endl;
  else
    cout << "Namespace URI: " << uri << endl
         << "Local name: " << lName << endl
         << "Raw name: " << n3 << endl;

  cout << endl << "*** Checking getDeclaredPrefixes" << endl;
  ACEXML_STR_LIST prefixes;
  if (xmlns.getDeclaredPrefixes (prefixes) != 0)
    cerr << "Fail to get a list of declared prefixes" << endl;
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        cout << "prefix: \"" << *prefix << "\"" << endl;
    }

  cout << endl << "*** Checking getPrefix" << endl;
  const ACEXML_Char *prefix = 0;

  if ((prefix = xmlns.getPrefix ("http://www.purl.org/dc#")) == 0)
    cerr << "Fail to get prefix of http://www.purl.org/dc#" << endl;
  else
    cout << "Prefix of http://www.purl.org/dc# is " << prefix << endl;

  if ((prefix = xmlns.getPrefix ("http://www.w3.org/1999/xhtml")) == 0)
    cout << "Fail to get prefix of http://www.w3.org/1999/xhtml which is okay" << endl;
  else
    cerr << "ERROR: Prefix of http://www.w3.org/1999/xhtml is " << prefix << endl;

  if ((prefix = xmlns.getPrefix ("http://www.w3.org/XML/1998/namespace")) == 0)
    cerr << "Fail to get prefix of http://www.w3.org/XML/1998/namespace" << endl;
  else
    cout << "Prefix of http://www.w3.org/XML/1998/namespace is " << prefix << endl;

  cout << endl << "*** Checking getPrefixes with known URI" << endl;
  prefixes.reset ();

  if (xmlns.getPrefixes ("http://www.purl.org/dc#", prefixes) != 0)
    cerr << "Fail to getPrefixes based on known URI" << endl;
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        cout << "prefix: \"" << *prefix << "\"" << endl;
    }

  cout << endl << "*** Checking getPrefixes" << endl;
  prefixes.reset ();

  if (xmlns.getPrefixes (prefixes) != 0)
    cerr << "Fail to getPrefixes" << endl;
  else
    {
      ACEXML_STR_LIST::ITERATOR iter = prefixes.begin ();
      const ACEXML_Char **prefix = 0;

      for (; iter.next (prefix);iter.advance ())
        cout << "prefix: \"" << *prefix << "\"" << endl;
    }

  cout << endl << "*** Checking getURI" << endl;
  const ACEXML_Char *URI = 0;

  if ((URI = xmlns.getURI ("dc")) == 0)
    cerr << "Fail to get URI for \"dc\"" << endl;
  else
    cout << "URI for \"dc\" is " << URI << endl;
  if ((URI = xmlns.getURI ("xc")) == 0)
    cerr << "Fail to get URI for \"xc\"" << endl;
  else
    cout << "URI for \"xc\" is " << URI << endl;
  if ((URI = xmlns.getURI ("xml")) == 0)
    cerr << "Fail to get URI for \"xml\"" << endl;
  else
    cout << "URI for \"xml\" is " << URI << endl;
  if ((URI = xmlns.getURI ("")) == 0)
    cerr << "Fail to get default namespace URI" << endl;
  else
    cout << "URI for default namespace is " << URI << endl;

  xmlns.popContext();

  return 0;
}
