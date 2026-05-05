#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

#include <cstddef>
#include <algorithm>

template< class T >
class PoolAllocator
{
  static const size_t BLOCKS_IN_CHUNK = 64;

  struct FreeBlock {
    FreeBlock* next;
  };

  struct Chunk {
    void* memory;
    Chunk* next;
  };

  struct Pool {
    FreeBlock* free_blocks = nullptr;
    Chunk* chunks = nullptr;

    ~Pool() {
      while (chunks) {
        Chunk* next = chunks->next;
        ::operator delete(chunks->memory);
        ::operator delete(chunks);
        chunks = next;
      }
    }
  };

  static Pool& get_pool() noexcept {
    static Pool p;
    return p;
  }

  static size_t block_size() noexcept {
    return std::max(sizeof(T), sizeof(FreeBlock));
  }

  static void add_chunk(Pool& p) {
    void* memory = ::operator new(block_size() * BLOCKS_IN_CHUNK);
    Chunk* chunk = nullptr;
    try {
      chunk = static_cast<Chunk*>(::operator new(sizeof(Chunk)));
    } catch (...) {
      ::operator delete(memory);
      throw;
    }

    chunk->memory = memory;
    chunk->next = p.chunks;
    p.chunks = chunk;

    char* current = static_cast<char*>(memory);
    for (size_t i = 0; i < BLOCKS_IN_CHUNK; ++i) {
      FreeBlock* block = reinterpret_cast<FreeBlock*>(current + i * block_size());
      block->next = p.free_blocks;
      p.free_blocks = block;
    }
  }

public:
  using value_type = T;

  PoolAllocator() noexcept = default;
  template< class U >
  PoolAllocator(const PoolAllocator< U >&) noexcept
  {}

  T* allocate(size_t n) {
    if (n != 1) return static_cast<T*>(::operator new(n * sizeof(T)));

    Pool& p = get_pool();
    if (!p.free_blocks) {
      add_chunk(p);
    }

    FreeBlock* block = p.free_blocks;
    p.free_blocks = block->next;
    return reinterpret_cast<T*>(block);
  }

  void deallocate(T* ptr, size_t n) noexcept {
    if (!ptr) return;
    if (n != 1) {
      ::operator delete(ptr);
      return;
    }

    Pool& p = get_pool();
    FreeBlock* block = reinterpret_cast<FreeBlock*>(ptr);
    block->next = p.free_blocks;
    p.free_blocks = block;
  }
};

template<class T, class U>
bool operator==(const PoolAllocator<T>&, const PoolAllocator<U>&) noexcept
{
  return true;
}

template<class T, class U>
bool operator!=(const PoolAllocator<T>&, const PoolAllocator<U>&) noexcept
{
  return false;
}

#endif
