// $Id$

#include "fe_template_header_common.h"

#include "ast_template_common.h"
#include "utl_err.h"
#include "global_extern.h"

FE_TemplateHeader_Common::FE_TemplateHeader_Common (
      FE_Utils::T_PARAMLIST_INFO *params)
  : param_info_ (params)
{
}

FE_TemplateHeader_Common::~FE_TemplateHeader_Common (void)
{
}

FE_Utils::T_PARAMLIST_INFO *
FE_TemplateHeader_Common::param_info (void) const
{
  return this->param_info_;
}

void
FE_TemplateHeader_Common::destroy (void)
{
  // Queue element members have self-managed memory.
  delete this->param_info_;
  this->param_info_ = 0;
}

bool
FE_TemplateHeader_Common::duplicate_param_id (void)
{
  size_t cur_pos = 0UL;
  size_t size = this->param_info_->size ();

  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*this->param_info_);
       !i.done ();
       i.advance (), ++cur_pos)
    {
      FE_Utils::T_Param_Info *this_one = 0;
      FE_Utils::T_Param_Info *that_one = 0;

      i.next (this_one);

      for (size_t j = cur_pos + 1; j < size; ++j)
        {
          this->param_info_->get (that_one, j);

          if (this_one->name_ == that_one->name_)
            {
              return true;
            }
        }
    }

  return false;
}


