// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cache_Manager_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the <ACE_Cache_Manager> and illustrates how
//     to use it to cache objects keyed from strings.
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

#include "ace/Cache_Manager.h"


typedef ACE_String_Referenced_Cache_Manager
        String_Table_Manager;

typedef ACE_Cache_Proxy<const char *, const char, String_Table_Manager>
        String_Table_Proxy;

static String_Table_Manager *JXH_String_Table_Manager;

static void put1 (void)
{
  String_Table_Proxy a((const char *) "hello",
                       (const char *) "guten Tag", sizeof ( "guten Tag"));

  String_Table_Proxy b((const char *) "good",
                       (const char *) "gut", sizeof ( "gut"));

  String_Table_Proxy c((const char *) "bad",
                       (const char *) "schlecht", sizeof ( "schlecht"));

  String_Table_Proxy d((const char *) "ugly",
                       (const char *) "unsehenlich", sizeof ( "unsehenlich"));

  String_Table_Proxy e((const char *) "bye bye",
                       (const char *) "tschuess", sizeof ( "tschuess"));
}

static void put2 (void)
{
  String_Table_Proxy a((const char *) "hello",
                       (const char *) "guten Nacht", sizeof ( "guten Nacht"));

  String_Table_Proxy b((const char *) "good",
                       (const char *) "vielen glueck", sizeof ( "vielen glueck"));

  String_Table_Proxy c((const char *) "bad",
                       (const char *) "sehr schlecht", sizeof ( "sehr schlecht"));

  String_Table_Proxy d((const char *) "ugly",
                       (const char *) "unsehenlicher", sizeof ( "unsehenlicher"));

  String_Table_Proxy e((const char *) "bye bye",
                       (const char *) "aufwiedersehen", sizeof ( "aufwiedersehen"));
}

static void get1 (void)
{
  {
    String_Table_Proxy a((const char *) "hello");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "good");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "bad");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "ugly");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "bye bye");
    cerr << a.data() << endl;
  }
}

static void get2 (void)
{
  {
    String_Table_Proxy a((const char *) "bye bye");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "ugly");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "bad");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "good");
    cerr << a.data() << endl;
  }

  {
    String_Table_Proxy a((const char *) "hello");
    cerr << a.data() << endl;
  }
}

int main (void)
{
  ACE_START_TEST (ASYS_TEXT ("Cache_Manager_Test"));

  JXH_String_Table_Manager
  = String_Table_Proxy::Cache_Manager_Singleton::instance ();
  JXH_String_Table_Manager->open (0, 0, 2, 5, 1, 0, 1, 0);

  put1 ();

  get1 ();
  get2 ();

  put2 ();

  get1 ();
  get2 ();

  put1 ();

  get1 ();
  get2 ();

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cache_Proxy<const char *, const char, String_Table_Manager>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cache_Proxy<const char *, const char, String_Table_Manager>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
