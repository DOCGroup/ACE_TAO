/*$Id$*/

// Automatically generated header file "ec_dsui_table.h" by dsui-parse.py
#ifndef _ec_DSUI_TABLE_H
#define _ec_DSUI_TABLE_H

#include "ec_dsui_families.h"







extern int dsui_family_table;
extern int ec_table_offset;
extern family_t *ec_family_table[NUM_FAMILIES];
#define DSUI_INIT(identifier, enabled) dsui_init((identifier), (enabled))

#endif
