#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <utility>
#include <cstddef>
#include <memory>

namespace tarasenko
{
  template< class T >
  struct Node
  {
    Node(const T& val, Node< T >* next, Node< T >* prev) :
      _val(val),
      _next(next),
      _prev(prev)
    {}

    Node(T&& val, Node< T >* next, Node< T >* prev) :
      _val(std::move(val)),
      _next(next),
      _prev(prev)
    {}

    T _val;
    Node< T >* _next;
    Node< T >* _prev;
  };

  template< class T, class Alloc = std::allocator< T > >
  class BidirList;

  template< class T >
  class ListIter
  {
    template< class, class >
    friend class BidirList;
    Node< T >* _ptr;
    Node< T >* _tail;
  public:
    ListIter();
    bool operator==(const ListIter< T >& it) const noexcept;
    bool operator!=(const ListIter< T >& it) const noexcept;
    T& operator*() const;
    T* operator->() const;
    ListIter< T >& operator++() noexcept;
    ListIter< T > operator++(int) noexcept;
    ListIter< T >& operator--() noexcept;
    ListIter< T > operator--(int) noexcept;
  private:
    ListIter(Node< T >* node, Node< T >* tail) noexcept;
  };

  template< class T >
  class ListConstIter
  {
    template< class, class >
    friend class BidirList;
    Node< T >* _ptr;
    Node< T >* _tail;
  public:
    ListConstIter();
    bool operator==(const ListConstIter< T >& it) const noexcept;
    bool operator!=(const ListConstIter< T >& it) const noexcept;
    const T& operator*() const;
    const T* operator->() const;
    ListConstIter< T >& operator++() noexcept;
    ListConstIter< T > operator++(int) noexcept;
    ListConstIter< T >& operator--() noexcept;
    ListConstIter< T > operator--(int) noexcept;
  private:
    ListConstIter(Node< T >* node, Node< T >* tail) noexcept;
  };

  template< class T, class Alloc >
  class BidirList
  {
    friend class ListIter< T >;
    friend class ListConstIter< T >;
    using AllocTraits = std::allocator_traits< Alloc >;
    using NodeAlloc = typename AllocTraits::template rebind_alloc< Node< T > >;
    using NodeAllocTraits = std::allocator_traits< NodeAlloc >;

    Node< T >* _head;
    Node< T >* _tail;
    size_t _size;
    NodeAlloc _alloc;

    Node< T >* create_node(const T& val, Node< T >* next, Node< T >* prev);
    Node< T >* create_node(T&& val, Node< T >* next, Node< T >* prev);
    void destroy_node(Node< T >* node) noexcept;
  public:
    BidirList();
    ~BidirList();
    BidirList(const BidirList< T, Alloc >& list);
    BidirList(BidirList< T, Alloc >&& list) noexcept;
    BidirList< T, Alloc >& operator=(const BidirList< T, Alloc >& list);
    BidirList< T, Alloc >& operator=(BidirList< T, Alloc >&& list) noexcept;
    size_t size() const noexcept;
    ListIter< T > begin() noexcept;
    ListConstIter< T > begin() const noexcept;
    ListConstIter< T > cbegin() const noexcept;
    ListIter< T > end() noexcept;
    ListConstIter< T > end() const noexcept;
    ListConstIter< T > cend() const noexcept;
    void push_back(const T& val);
    void push_front(const T& val);
    void push_back(T&& val);
    void push_front(T&& val);
    bool empty() const noexcept;
    const T& front() const;
    const T& back() const;
    ListIter< T > erase(ListIter< T > it) noexcept;
    ListIter< T > erase(ListIter< T > start, ListIter< T > end) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;
    ListIter< T > insert(ListIter< T > it, const T& val);
    ListIter< T > insert(ListIter< T > it, T&& val);
    void clear() noexcept;
    void swap(BidirList< T, Alloc >& list1, BidirList< T, Alloc >& list2) noexcept;
  };

  template< class T, class Alloc >
  BidirList< T, Alloc >::BidirList() :
    _head(nullptr),
    _tail(nullptr),
    _size(0),
    _alloc()
  {}

  template< class T, class Alloc >
  BidirList< T, Alloc >::~BidirList()
  {
    clear();
  }

  template< class T, class Alloc >
  size_t BidirList< T, Alloc >::size() const noexcept
  {
    return _size;
  }

  template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::begin() noexcept
  {
    return ListIter< T >(_head, _tail);
  }

  template< class T, class Alloc >
  ListConstIter< T > BidirList< T, Alloc >::begin() const noexcept
  {
    return ListConstIter< T >(_head, _tail);
  }

  template< class T, class Alloc >
  ListConstIter< T > BidirList< T, Alloc >::cbegin() const noexcept
  {
    return ListConstIter< T >(_head, _tail);
  }

   template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::end() noexcept
  {
    return ListIter< T >(nullptr, _tail);
  }

  template< class T, class Alloc >
  ListConstIter< T > BidirList< T, Alloc >::end() const noexcept
  {
    return ListConstIter< T >(nullptr, _tail);
  }

  template< class T, class Alloc >
  ListConstIter< T > BidirList< T, Alloc >::cend() const noexcept
  {
    return ListConstIter< T >(nullptr, _tail);
  }

  template< class T >
  ListIter< T >::ListIter() :
    _ptr(nullptr),
    _tail(nullptr)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter() :
    _ptr(nullptr),
    _tail(nullptr)
  {}

  template< class T >
  ListIter< T >::ListIter(Node< T >* node, Node< T >* tail) noexcept :
    _ptr(node),
    _tail(tail)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(Node< T >* node, Node< T >* tail) noexcept :
    _ptr(node),
    _tail(tail)
  {}

  template< class T >
  bool ListIter< T >::operator==(const ListIter< T >& it) const noexcept
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListIter< T >::operator!=(const ListIter< T >& it) const noexcept
  {
    return !(_ptr == it._ptr);
  }

  template< class T >
  bool ListConstIter< T >::operator==(const ListConstIter< T >& it) const noexcept
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListConstIter< T >::operator!=(const ListConstIter< T >& it) const noexcept
  {
    return !(_ptr == it._ptr);
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::push_back(const T& val)
  {
    insert(end(), val);
  }

    template< class T, class Alloc >
  void BidirList< T, Alloc >::push_back(T&& val)
  {
    insert(end(), std::move(val));
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::push_front(const T& val)
  {
    insert(begin(), val);
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::push_front(T&& val)
  {
    insert(begin(), std::move(val));
  }

  template< class T >
  T& ListIter< T >::operator*() const
  {
    return _ptr->_val;
  }

  template< class T >
  const T& ListConstIter< T >::operator*() const
  {
    return _ptr->_val;
  }

  template< class T >
  T* ListIter< T >:: operator->() const
  {
    return &_ptr->_val;
  }

  template< class T >
  const T* ListConstIter< T >:: operator->() const
  {
    return &_ptr->_val;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator++() noexcept
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator++() noexcept
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator++(int) noexcept
  {
    ListIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator++(int) noexcept
  {
    ListConstIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator--() noexcept
  {
    if (_ptr == nullptr)
    {
      _ptr = _tail;
      return *this;
    }
    _ptr = _ptr->_prev;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator--() noexcept
  {
    if (_ptr == nullptr)
    {
      _ptr = _tail;
      return *this;
    }
    _ptr = _ptr->_prev;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator--(int) noexcept
  {
    if (_ptr == nullptr)
    {
      _ptr = _tail;
      return *this;
    }
    ListIter< T > copy(*this);
    _ptr = _ptr->_prev;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator--(int) noexcept
  {
    if (_ptr == nullptr)
    {
      _ptr = _tail;
      return *this;
    }
    ListConstIter< T > copy(*this);
    _ptr = _ptr->_prev;
    return copy;
  }

  template< class T, class Alloc >
  bool BidirList< T, Alloc >::empty() const noexcept
  {
    return !_size;
  }

  template< class T, class Alloc >
  const T& BidirList< T, Alloc >::front() const
  {
    return _head->_val;
  }
  template< class T, class Alloc >
  const T& BidirList< T, Alloc >::back() const
  {
    return _tail->_val;
  }

  template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::erase(ListIter< T > it) noexcept
  {
    Node< T >* next = it._ptr->_next;
    Node< T >* prev = it._ptr->_prev;
    destroy_node(it._ptr);
    if (next != nullptr)
    {
      next->_prev = prev;
    }
    else
    {
      _tail = prev;
    }

    if (prev != nullptr)
    {
      prev->_next = next;
    }
    else
    {
      _head = next;
    }
    it._ptr = next;
    it._tail = _tail;
    _size--;
    return it;
  }

  template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::erase(ListIter< T > first, ListIter< T > last) noexcept
  {
    while(first != last)
    {
      first = erase(first);
    }
    return first;
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::pop_front() noexcept
  {
    erase(begin());
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::pop_back() noexcept
  {
    erase(--end());
  }

  template< class T, class Alloc >
  Node< T >* BidirList< T, Alloc >::create_node(const T& val, Node< T >* next, Node< T >* prev)
  {
    Node< T >* node = NodeAllocTraits::allocate(_alloc, 1);
    try
    {
      NodeAllocTraits::construct(_alloc, node, val, next, prev);
    }
    catch (...)
    {
      NodeAllocTraits::deallocate(_alloc, node, 1);
      throw;
    }
    return node;
  }

  template< class T, class Alloc >
  Node< T >* BidirList< T, Alloc >::create_node(T&& val, Node< T >* next, Node< T >* prev)
  {
    Node< T >* node = NodeAllocTraits::allocate(_alloc, 1);
    try
    {
      NodeAllocTraits::construct(_alloc, node, std::move(val), next, prev);
    }
    catch (...)
    {
      NodeAllocTraits::deallocate(_alloc, node, 1);
      throw;
    }
    return node;
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::destroy_node(Node< T >* node) noexcept
  {
    NodeAllocTraits::destroy(_alloc, node);
    NodeAllocTraits::deallocate(_alloc, node, 1);
  }

  template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::insert(ListIter< T > it, const T& val)
  {
    Node< T >* new_node;
    if (empty())
    {
      new_node = create_node(val, nullptr, nullptr);
      _head = new_node;
      _tail = new_node;
    }
    else if (it._ptr == nullptr)
    {
      new_node = create_node(val, nullptr, _tail);
      _tail->_next = new_node;
      _tail = new_node;
    }
    else
    {
      new_node = create_node(val, it._ptr, it._ptr->_prev);
      if (it._ptr->_prev == nullptr)
      {
        _head->_prev = new_node;
        _head = new_node;
      }
      else
      {
        it._ptr->_prev->_next = new_node;
        it._ptr->_prev = new_node;
      }
    }
    _size++;
    return ListIter< T >(new_node, _tail);
  }

  template< class T, class Alloc >
  ListIter< T > BidirList< T, Alloc >::insert(ListIter< T > it, T&& val)
  {
    Node< T >* new_node;
    if (empty())
    {
      new_node = create_node(std::move(val), nullptr, nullptr);
      _head = new_node;
      _tail = new_node;
    }
    else if (it._ptr == nullptr)
    {
      new_node = create_node(std::move(val), nullptr, _tail);
      _tail->_next = new_node;
      _tail = new_node;
    }
    else
    {
      new_node = create_node(std::move(val), it._ptr, it._ptr->_prev);
      if (it._ptr->_prev == nullptr)
      {
        _head->_prev = new_node;
        _head = new_node;
      }
      else
      {
        it._ptr->_prev->_next = new_node;
        it._ptr->_prev = new_node;
      }
    }
    _size++;
    return ListIter< T >(new_node, _tail);
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::clear() noexcept
  {
    erase(begin(), end());
  }

  template< class T, class Alloc >
  BidirList< T, Alloc >::BidirList(const BidirList< T, Alloc >& list) :
    _head(nullptr),
    _tail(nullptr),
    _size(0),
    _alloc(NodeAllocTraits::select_on_container_copy_construction(list._alloc))
  {
    try
    {
      for(ListConstIter< T > it = list.begin(); it != list.end(); ++it)
      {
        push_back(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T, class Alloc >
  BidirList< T, Alloc >::BidirList(BidirList< T, Alloc >&& list) noexcept :
    _head(list._head),
    _tail(list._tail),
    _size(list._size),
    _alloc(std::move(list._alloc))
  {
    list._head = nullptr;
    list._tail = nullptr;
    list._size = 0;
  }

  template< class T, class Alloc >
  void BidirList< T, Alloc >::swap(BidirList< T, Alloc >& list1, BidirList< T, Alloc >& list2) noexcept
  {
    std::swap(list1._head, list2._head);
    std::swap(list1._tail, list2._tail);
    std::swap(list1._size, list2._size);
    std::swap(list1._alloc, list2._alloc);
  }

  template< class T, class Alloc >
  BidirList< T, Alloc >& BidirList< T, Alloc >::operator=(const BidirList< T, Alloc >& list)
  {
    BidirList< T, Alloc > temp(list);
    swap(*this, temp);
    return *this;
  }

  template< class T, class Alloc >
  BidirList< T, Alloc >& BidirList< T, Alloc >::operator=(BidirList< T, Alloc >&& list) noexcept
  {
    if (this == std::addressof(list))
    {
      return *this;
    }
    clear();
    _head = list._head;
    _tail = list._tail;
    _size = list._size;
    _alloc = std::move(list._alloc);
    list._head = nullptr;
    list._tail = nullptr;
    list._size = 0;
    return *this;
  }
}

#endif
