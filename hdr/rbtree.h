/**
* Author: pvoid, 2026
* License: https://www.gnu.org/licenses/gpl-3.0.html
*/

#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <concepts>
#include <iterator>
#include <memory>

enum class rb_tree_node_color
{
  red,
  black
};

template <typename T> struct rb_tree_node
{
  using node_type = rb_tree_node<T>;
  using node_ptr = std::unique_ptr<node_type>;

  node_ptr left = nullptr;
  node_ptr right = nullptr;
  node_type* parent = nullptr;
  T payload;
  rb_tree_node_color color = rb_tree_node_color::black;

  template <typename U>
  explicit rb_tree_node(U&& val) : payload(std::forward<U>(val))
  {
  }

  void set_red() noexcept
  {
    color = rb_tree_node_color::red;
  }

  void set_black() noexcept
  {
    color = rb_tree_node_color::black;
  }

  constexpr bool is_red() const noexcept
  {
    return color == rb_tree_node_color::red;
  }

  constexpr bool is_black() const noexcept
  {
    return color == rb_tree_node_color::black;
  }
};

template <typename T> class rbtree
{
public:
  using node_type = rb_tree_node<T>;
  using node_ptr = std::unique_ptr<node_type>;

  rbtree() = default;

  template <typename U> explicit rbtree(U&& value)
  {
    insert(std::forward<U>(value));
  }

  template <typename U> void insert(U&& value)
  {
    if (empty())
    {
      m_root = std::make_unique<node_type>(std::forward<U>(value));
      ++m_size;
      return;
    }

    node_type* parent = nullptr;
    node_type* current = m_root.get();
    bool value_less = false;

    while (current != nullptr)
    {
      parent = current;
      if (value < current->payload)
      {
        current = current->left.get();
        value_less = true;
      }
      else if (current->payload < value)
      {
        current = current->right.get();
        value_less = false;
      }
      else
      {
        return;
      }
    }

    auto new_node = std::make_unique<node_type>(std::forward<U>(value));
    new_node->set_red();

    node_type* inserted_node_ptr = new_node.get();

    if (value_less)
    {
      parent->left = std::move(new_node);
    }
    else
    {
      parent->right = std::move(new_node);
    }

    inserted_node_ptr->parent = parent;

    ++m_size;

    balance(inserted_node_ptr);
  }

  node_type* find(const T& value) const
  {
    if (empty())
    {
      return nullptr;
    }

    node_type* current = m_root.get();
    while (current)
    {
      if (current->payload == value)
      {
        return current;
      }

      if (value < current->payload)
      {
        current = current->left.get();
      }
      else
      {
        current = current->right.get();
      }
    }
    return nullptr;
  }

  constexpr bool empty() const noexcept
  {
    return m_root == nullptr;
  }

  void clear()
  {
    m_root.reset();
    m_size = 0;
  }

  constexpr size_t size() const noexcept
  {
    return m_size;
  }

  template <typename U = T>
    requires(!std::same_as<U, int>)
  bool erase(int) = delete;

  bool erase(std::nullptr_t) = delete;

  bool erase(const T& value)
  {
    return erase(find(value));
  }

  bool erase(node_type* p)
  {
    if (!p)
    {
      return false;
    }

    if (!p->parent)
    {
      if (p->left && p->right)
      {
        node_type* y = p->right.get();
        while (y->left)
        {
          y = y->left.get();
        }
        p->payload = std::move(y->payload);
        p = y;
      }
      else
      {
        node_ptr new_root = p->left
                                ? std::move(p->left)
                                : (p->right ? std::move(p->right) : nullptr);
        m_root = std::move(new_root);
        if (m_root)
        {
          m_root->parent = nullptr;
          m_root->set_black();
        }
        --m_size;
        return true;
      }
    }

    if (p->left && p->right)
    {
      node_type* y = p->right.get();
      while (y->left)
      {
        y = y->left.get();
      }
      p->payload = std::move(y->payload);
      p = y;
    }

    node_type* x =
        p->left ? p->left.get() : (p->right ? p->right.get() : nullptr);
    node_type* x_parent = p->parent;
    bool is_left_child = (x_parent->left.get() == p);

    bool removed_was_black = p->is_black();

    if (x)
    {
      x->parent = x_parent;
    }
    node_ptr child_uptr = p->left ? std::move(p->left) : std::move(p->right);

    if (is_left_child)
    {
      x_parent->left = std::move(child_uptr);
    }
    else
    {
      x_parent->right = std::move(child_uptr);
    }

    if (removed_was_black)
    {
      fix_delete(x, x_parent);
    }
    --m_size;
    return true;
  }

  struct iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    explicit iterator(node_type* node) : m_current(node)
    {
    }

    reference operator*() const
    {
      return m_current->payload;
    }
    pointer operator->() const
    {
      return &(m_current->payload);
    }

    iterator& operator++()
    {
      if (m_current->right)
      {
        m_current = m_current->right.get();
        while (m_current->left)
        {
          m_current = m_current->left.get();
        }
      }
      else
      {
        node_type* parent = m_current->parent;
        while (parent && m_current == parent->right.get())
        {
          m_current = parent;
          parent = parent->parent;
        }
        m_current = parent;
      }
      return *this;
    }

    iterator operator++(int)
    {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& other) const
    {
      return m_current == other.m_current;
    }
    bool operator!=(const iterator& other) const
    {
      return !(*this == other);
    }

  private:
    node_type* m_current;
  };

  struct const_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    explicit const_iterator(const node_type* node) : m_current(node)
    {
    }

    reference operator*() const
    {
      return m_current->payload;
    }
    pointer operator->() const
    {
      return &(m_current->payload);
    }

    const_iterator& operator++()
    {
      if (m_current->right)
      {
        m_current = m_current->right.get();
        while (m_current->left)
        {
          m_current = m_current->left.get();
        }
      }
      else
      {
        const node_type* parent = m_current->parent;
        while (parent && m_current == parent->right.get())
        {
          m_current = parent;
          parent = parent->parent;
        }
        m_current = parent;
      }
      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& other) const
    {
      return m_current == other.m_current;
    }
    bool operator!=(const const_iterator& other) const
    {
      return !(*this == other);
    }

  private:
    const node_type* m_current;
  };

  iterator begin()
  {
    if (!m_root)
      return end();
    node_type* current = m_root.get();
    while (current->left)
    {
      current = current->left.get();
    }
    return iterator(current);
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  const_iterator begin() const
  {
    return cbegin();
  }
  const_iterator end() const
  {
    return cend();
  }

  const_iterator cbegin() const
  {
    if (!m_root)
      return cend();
    const node_type* current = m_root.get();
    while (current->left)
    {
      current = current->left.get();
    }
    return const_iterator(current);
  }

  const_iterator cend() const
  {
    return const_iterator(nullptr);
  }

private:
  void rotate_left(node_type* x)
  {
    auto y = std::move(x->right);
    if (!y)
      return;

    node_type* y_ptr = y.get();

    x->right = std::move(y->left);
    if (x->right)
    {
      x->right->parent = x;
    }

    node_type* x_parent = x->parent;
    bool was_left_child = x_parent && (x == x_parent->left.get());

    node_ptr x_safe;
    if (!x_parent)
    {
      x_safe = std::move(m_root);
    }
    else if (was_left_child)
    {
      x_safe = std::move(x_parent->left);
    }
    else
    {
      x_safe = std::move(x_parent->right);
    }

    y->parent = x_parent;
    if (!x_parent)
    {
      m_root = std::move(y);
    }
    else if (was_left_child)
    {
      x_parent->left = std::move(y);
    }
    else
    {
      x_parent->right = std::move(y);
    }

    y_ptr->left = std::move(x_safe);
    x->parent = y_ptr;
  }

  void rotate_right(node_type* y)
  {
    auto x = std::move(y->left);
    if (!x)
    {
      return;
    }

    node_type* x_ptr = x.get();

    y->left = std::move(x->right);
    if (y->left)
    {
      y->left->parent = y;
    }

    node_type* y_parent = y->parent;
    bool was_left_child = y_parent && (y == y_parent->left.get());

    node_ptr y_safe;
    if (!y_parent)
    {
      y_safe = std::move(m_root);
    }
    else if (was_left_child)
    {
      y_safe = std::move(y_parent->left);
    }
    else
    {
      y_safe = std::move(y_parent->right);
    }

    x->parent = y_parent;
    if (!y_parent)
    {
      m_root = std::move(x);
    }
    else if (was_left_child)
    {
      y_parent->left = std::move(x);
    }
    else
    {
      y_parent->right = std::move(x);
    }

    x_ptr->right = std::move(y_safe);
    y->parent = x_ptr;
  }

  void balance(node_type* node)
  {

    while (node != m_root.get() && node->parent->is_red())
    {
      node_type* parent = node->parent;
      node_type* grandparent = parent->parent;

      if (parent == grandparent->left.get())
      {
        node_type* uncle = grandparent->right.get();

        if (uncle && uncle->is_red())
        {
          parent->set_black();
          uncle->set_black();
          grandparent->set_red();
          node = grandparent;
        }
        else
        {
          if (node == parent->right.get())
          {
            node = parent;
            rotate_left(node);
            parent = node->parent;
          }

          parent->set_black();
          grandparent->set_red();
          rotate_right(grandparent);
        }
      }
      else
      {
        node_type* uncle = grandparent->left.get();

        if (uncle && uncle->is_red())
        {
          parent->set_black();
          uncle->set_black();
          grandparent->set_red();
          node = grandparent;
        }
        else
        {
          if (node == parent->left.get())
          {
            node = parent;
            rotate_right(node);
            parent = node->parent;
          }

          parent->set_black();
          grandparent->set_red();
          rotate_left(grandparent);
        }
      }
    }
    m_root->set_black();
  }

  void fix_delete(node_type* x, node_type* x_parent)
  {
    while (x != m_root.get() && (!x || x->is_black()))
    {
      if (x == x_parent->left.get())
      {
        node_type* w = x_parent->right.get();

        if (w && w->is_red())
        {
          w->set_black();
          x_parent->set_red();
          rotate_left(x_parent);
          w = x_parent->right.get();
        }

        if (!w) // for cppcheck
        {
          return;
        }

        bool w_left_black = !w->left || w->left->is_black();
        bool w_right_black = !w->right || w->right->is_black();

        if (w_left_black && w_right_black)
        {
          w->set_red();
          x = x_parent;
          x_parent = x->parent;
        }
        else
        {
          if (w_right_black)
          {
            if (w->left)
              w->left->set_black();
            w->set_red();
            rotate_right(w);
            w = x_parent->right.get();
          }

          if (w)
          {
            w->color = x_parent->color;
            x_parent->set_black();
            if (w->right)
              w->right->set_black();
            rotate_left(x_parent);
          }
          x = m_root.get();
        }
      }
      else
      {
        node_type* w = x_parent->left.get();

        if (w && w->is_red())
        {
          w->set_black();
          x_parent->set_red();
          rotate_right(x_parent);
          w = x_parent->left.get();
        }

        if (!w) // for cppcheck
        {
          return;
        }

        bool w_left_black = !w->left || w->left->is_black();
        bool w_right_black = !w->right || w->right->is_black();

        if (w_left_black && w_right_black)
        {
          w->set_red();
          x = x_parent;
          x_parent = x->parent;
        }
        else
        {
          if (w_left_black)
          {
            if (w->right)
              w->right->set_black();
            w->set_red();
            rotate_left(w);
            w = x_parent->left.get();
          }

          if (w)
          {
            w->color = x_parent->color;
            x_parent->set_black();
            if (w->left)
              w->left->set_black();
            rotate_right(x_parent);
          }
          x = m_root.get();
        }
      }
    }

    if (x)
    {
      x->set_black();
    }
  }

private:
  node_ptr m_root = nullptr;
  size_t m_size = 0;
};

#endif // _RB_TREE_H_
