// $Id$

namespace ADBC
{
namespace SQLite
{
//
// Parameter_List
//
ADBC_INLINE
Parameter_List::
Parameter_List (const Query & parent)
: parent_ (parent)
{

}

//
// Parameter_List
//
ADBC_INLINE
Parameter_List::~Parameter_List (void)
{

}

//
// size
//
ADBC_INLINE
size_t Parameter_List::size (void) const
{
  return this->params_.size ();
}

//
// operator []
//
ADBC_INLINE
Parameter &
Parameter_List::operator [] (size_t index)
{
  return this->params_[index];
}

//
// operator []
//
ADBC_INLINE
const Parameter &
Parameter_List::operator [] (size_t index) const
{
  return this->params_[index];
}

//
// parent
//
ADBC_INLINE
const Query & Parameter_List::owner (void) const
{
  return this->parent_;
}

}
}
