#include "Entity_Manager.h"

#if !defined (__ACEXML_INLINE__)
# include "Entity_Manager.inl"
#endif /* __ACEXML_INLINE__ */


ACEXML_Entity_Manager::ACEXML_Entity_Manager (void)
  : entities_ (0)
{

}

ACEXML_Entity_Manager::~ACEXML_Entity_Manager (void)
{
  this->reset();
}

