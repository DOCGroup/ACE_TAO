/**
 * @file QoSMappingVisitor.tpp
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 */

namespace PICML
{
  template <typename T>
  size_t
  QoSMappingVisitor::visit_kind_children (std::set <T> &kind_set)
  {
    size_t count = 0;
    for (std::set <T>::iterator iter = kind_set.begin ();
	 iter != kind_set.end ();
	 ++iter)
      {
	T item = *iter;
	item.Accept (*this);
	++count;
      }
    
    return count;
  }
  
}
