#include "indentation.h"

Indentation::Indentation ()
{
  this->indent = "  ";
}

void Indentation::operator ++ ()
{
  this->indent += "  ";
}

void Indentation::operator -- ()
{
  this->indent.erase (this->indent.length()-2, 2);
}
