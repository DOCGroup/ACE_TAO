// $Id$

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "test_config.h"

bool match (const char *str, const char *pat, bool cs = true, bool cc = false)
{
  bool const result = ACE::wild_match (str, pat, cs, cc);
  ACE_DEBUG ((LM_DEBUG, "string {%C} %C pattern {%s}\t%C\t%C\n", str,
              (result ? "matches" : "does not match"), pat,
              (cs ? "" : "case-insensitive"), (cc ? "char classes" : "")));
  return result;
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Wild_Match_Test"));

  bool ok = true;
  ok &=  match ("foo_baz_bar",      "foo*bar");
  ok &=  match ("lksfj;laskf;jbaz", "*baz");
  ok &= !match ("abc",              "abc?");
  ok &=  match ("simple",           "simple");
  ok &= !match ("not so simple",    "simple");
  ok &=  match ("AbC",              "abc",                false);
  ok &=  match ("*\\",              "\\*\\\\");
  ok &=  match ("Nonwild[foo]",     "*[foo]");
  ok &=  match ("Apple][",          "[zxya]p*[]125]\\[",  false,  true);
  ok &=  match ("[!]",              "[][!][][!][][!]",    true,   true);
  ok &=  match ("ace",              "[a-e][a-e][a-e]",    true,   true);
  ok &=  match ("--x",              "[-1][2-][!-]",       true,   true);

  ok &= !match ("C2",               "[!C]?",              true,   true);
  ok &=  match ("D1",               "[!C]?",              true,   true);
  ok &=  match ("D2",               "[!C]?",              true,   true);

  ok &= !match ("C2",               "?[!2]",              true,   true);
  ok &=  match ("C1",               "?[!2]",              true,   true);
  ok &= !match (0,                  "[!C]?",              true,   true);

  // invalid classes: results are undefined but we shouldn't crash
  match ("foo", "f[o-a]o", true, true);
  match ("bar", "[f-", true, true);
  match ("bar", "[z", true, true);
  match ("bar", "[]x", true, true);
  match ("foo", "[f-f]oo", true, true);

  if (!ok)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Wild_Match_Test failed\n")));
    }

  ACE_END_TEST;
  return ok ? 0 : 1;
}
