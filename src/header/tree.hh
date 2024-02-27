#ifndef __TREE
#define __TREE

/**
 * @authors MizurenNanako
 * @brief A n-ary tree class for further stmt structure implantation.
 */

#include "stdafx.hh"

template<typename T>
class tree_t {
  public:
    class tree_node_iterator;

    using value_type      = T;
    using reference       = T &;
    using const_reference = const T &;
    using iterator        = tree_node_iterator;
    using const_iterator  = tree_node_iterator;
    using difference_type = int64_t;
    using size_type       = uint64_t;

  private:
  public:
};

template<typename T>
class tree_t<T>::tree_node_iterator
{

};

#endif // __TREE
