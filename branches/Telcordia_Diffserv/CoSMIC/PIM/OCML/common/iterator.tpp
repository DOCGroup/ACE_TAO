/* -*- C++ -*- */

// $Id$

template <typename Container>
common::append_iterator<Container>::
common::append_iterator(Container& container,
			typename Container::iterator first,
			typename Container::iterator last)
  :  _container(container), _first(first), _last(last)
{
}

template <typename Container>
common::append_iterator<Container>&
common::append_iterator<Container>::
operator=(const typename Container::value_type& value)
{
  _container.push_back(value);
  std::copy(_first, _last,
	    std::back_insert_iterator<Container>(_container));
  return *this;
}

template <typename Container>
common::append_iterator<Container>&
common::append_iterator<Container>::operator*()
{
  return *this;
}

template <typename Container>
common::append_iterator<Container>&
common::append_iterator<Container>::operator++()
{
  return *this;
}

template <typename Container>
common::append_iterator<Container>&
common::append_iterator<Container>::operator++(int)
{
  return *this;
}
