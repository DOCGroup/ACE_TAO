// -*- C++ -*-  $Id$

#include "SyntaxTree.h"

//Copies as much of the type name to the buffer as fits; buf will null-term.
void visitableTypeToString (VisitableSyntax::element type, char *buf, int buflen) {
  if (buflen <= 0) return;

  char* typestr[] = {
    "ECCONFIGURATION",
    "EVENT",
    "CRITICALITY",
    "IMPORTANCE",
    "WORSTEXECUTION",
    "TYPICALEXECUTION",
    "TIMEOUT",
    "PERIOD",
    "PHASE",
    "LOCALEVENTCHANNEL",
    "REMOTEEVENTCHANNEL",
    "SCHEDULINGSTRATEGY",
    "CONSUMER",
    "SUBSCRIPTIONS",
    "EVENTNAME",
    "DEPENDANTS",
    "SUPPLIERNAME",
    "SUPPLIER",
    "PUBLICATIONS",
    "TRIGGERS",
    "TIMEOUTNAME",
    "IORFILE",
    "TESTDRIVER",
    "STARTCONDITION",
    "TIME",
    "STOPCONDITION",
    "VALUE",
    "UNKNOWN_ELEMENT"
  };
  //assume type is just index
  char *str = typestr[type];

  //copy over type string
  for(int i=0; i<buflen-1; ++i) {
    buf[i] = str[i];
    if (str[i] == '\0')
      return;
  }
  buf[buflen-1] = '\0'; //null terminate
}
