/* $Id$
 * Codeset registry DB, generated Fri Feb 28 21:01:30 2003
 * source: code_set_registry1.2g.txt
 *
 * To populate the registry_db, construct a codeset registry text file based
 * on the OSF's Character and Code Set Registry. See DCE RFC 40.1 for details
 * on obtaining the full text for the current registry. Once you have composed
 * a text file containing all the desired codeset information, build and run
 * mkcsregdb. The source is in $ACE_ROOT/apps/mkcsregdb. It will generate a new
 * copy of this file, with the registry_db_ array properly initialized.
 */

#include "ace/Codeset_Registry.h"

ACE_Codeset_Registry::registry_entry
ACE_Codeset_Registry::registry_db_[] =
{
  {"ISO 8859-1:1987; Latin Alphabet No. 1","ASCII",0x00010001,1,{0x0011},1},
  {"IBM-1047 (CCSID 01047); Latin-1 Open System","EBCDIC",0x10020417,1,{0x0011},1},
  {"ISO/IEC 10646-1:1993; UCS-4, Level 1","UCS-4",0x00010104,1,{0x1000},4},
  {"ISO/IEC 10646-1:1993; UTF-16, UCS Transformation Format 16-bit form","Unicode",0x00010109,1,{0x1000},2}
};

size_t ACE_Codeset_Registry::num_registry_entries_ = 4;
