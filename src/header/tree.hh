#ifndef __TREE
#define __TREE

/**
 * @authors MizurenNanako
 * @brief A n-ary tree class for further stmt structure implantation.
 */

#include "stdafx.hh"

template<typename T>
struct tree_node_t;
template<typename T>
struct tree_node_iterator;

template<typename T>
class tree_t {
  public:
    using value_type       = T;
    using reference        = T &;
    using const_reference  = const T &;
    using iterator         = tree_node_iterator;
    using const_iterator   = tree_node_iterator;
    using iterator_catagry = std::bidirectional_iterator_tag;
    using difference_type  = int64_t;
    using size_type        = uint64_t;

  private:
    std::unique_ptr<tree_node_t> _root = nullptr; // Work as reset()
  public:
    tree_t();
    tree_t(tree_t t);
    tree_t(tree_t &t);
    tree_t(tree_t &&t);
    ~tree_t() = default;
    iterator begin();
    iterator end();
};


#endif // __TREE
