#ifndef JAWS_BUILD_DLL
#define JAWS_BUILD_DLL
#endif

#include "jaws3/Event_Completer.h"

// Default do nothing completers.

JAWS_Event_Completer::~JAWS_Event_Completer () = default;

void
JAWS_Event_Completer::default_complete (JAWS_Event_Result const &, void *)
{
}

void
JAWS_Event_Completer::accept_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

void
JAWS_Event_Completer::connect_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

void
JAWS_Event_Completer::input_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

void
JAWS_Event_Completer::output_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

void
JAWS_Event_Completer::timer_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

void
JAWS_Event_Completer::lambda_complete (JAWS_Event_Result const &r, void *d)
{
  this->default_complete (r, d);
}

