// $Id$

#ifndef ACE_ADAPTER_DYNAMIC_SERVICE_H
#define ACE_ADAPTER_DYNAMIC_SERVICE_H
#include "ace/pre.h"

#include "../../Dynamic_Service.h"

template <class SERVICE>
class ACE_Dynamic_Service_W : public ACE_Dynamic_Service<SERVICE>
{
public:
  static SERVICE *instance (const wchar_t *name)
  { return ACE_Dynamic_Service::instance (ACE_TEXT_WCHAR_TO_CHAR (name)); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_DYNAMIC_SERVICE_H */
