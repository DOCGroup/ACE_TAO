#include "Callee_i.h"

void Callee_i::callback (char const *message)
{
  ACE_DEBUG ((LM_DEBUG, "Callee_i::callback: message is %C", message));
}
