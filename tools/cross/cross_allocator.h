#ifndef CROSS_ALLOCATOR_H_
#define CROSS_ALLOCATOR_H_
#include <boost/dynamic_bitset.hpp>
#include <boost/noncopyable.hpp>
#include "cross.h"
namespace cross {

class page {
 public:
    page();
    ~page();
    void clear();
    uint64_t address() const { return address_; }

 private:
    uint64_t address_;
};

class allocator : private boost::noncopyable {
 public:
    static const uint64_t kPageSize = 0x1000;

    allocator(uint64_t start, uint64_t end);
    uint64_t allocate();  // 1page
    void deallocate(uint64_t addr);

 private:
    uint64_t start_;
    uint64_t size_;
    boost::dynamic_bitset<> vector_;
};

}  // namespace cross
#endif  // CROSS_ALLOCATOR_H_
/* vim: set sw=4 ts=4 et tw=80 : */
