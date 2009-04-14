// $Id$
// wxBindNewContext.h

#ifndef i_wxBindNewContext_h
#define i_wxBindNewContext_h

#include "orbsvcs/CosNamingC.h"

class WxBindNewContext:
  public wxDialog
{
public:
  WxBindNewContext( wxWindow* parent);

  CosNaming::Name& getName() { return name;}

private:
  virtual bool TransferDataFromWindow();
  CosNaming::Name name;
};

#endif

