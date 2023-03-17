#ifndef LIB_HACKNET_MM
#define LIB_HACKNET_MM

#define HACKNET_MM_DEBUG false

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// This file is deprecated - we're switching to a better allocator soon.

/// @brief Used to define a single memory allocation in the list.
typedef struct _Allocation {
    // Allocation Metadata
    size_t length;
    bool free;

    // Linked List metadata
    struct _Allocation *previous;
    struct _Allocation *next;

    // The actual allocation
    void *base;
} Allocation;

#define HACKNET_KHEAP_SIZE 0x100000

static uint8_t kheap[HACKNET_KHEAP_SIZE];

/// @brief Iniitalizes the kernel heap to a static size of HACKNET_KHEAP_SIZE.
[[deprecated]]
void kheap_init() {

    Allocation *base = (Allocation *)kheap;

    base->base = kheap + sizeof(Allocation);
    base->free = true;
    base->length = HACKNET_KHEAP_SIZE - sizeof(Allocation);
    base->previous = NULL;
    base->next = NULL;
}

/// @brief Merges all contiguous free allocation blocks into larger ones for faster traversal & general defragmentation.
static void merge_all_contiguous() {
    Allocation *current = (Allocation *)kheap;

    while(true) {
        if(current->next == NULL)
            return;

        Allocation *next = current->next;

        if(current->free && next->free) {
            current->length = current->length + (sizeof(Allocation) + next->length - 1);
            current->next = next->next;
        }

        current = current->next;
    }
}

/// @brief Locates the first Allocation equal or larger than min_size bytes.
/// @param min_size The minimum (inclusive) size in bytes the allocation must meet.
/// @return The first viable free allocation, or NULL if one is not found.
static Allocation *find_first_minimal_entry(size_t min_size) {
    Allocation *base = (Allocation *)kheap;
    if(base->free && base->length >= min_size + 1) return base;

    Allocation *current = base->next;
    while(true) {
        if(current == NULL)
            return NULL;

        if(current->free && current->length >= min_size + 1)
            return current;

        if(current->next == NULL)
            return NULL;

        current = current->next;
    }
}

/// @brief Allocates a block of kernel memory of `size` bytes or larger.
/// @param size The minimum size -in bytes- of memory that must be allocated.
/// @return A pointer to the beginning of the block, or NULL if the memory cannot be allocated.
[[deprecated]]
void *kmalloc(size_t size) {
    Allocation *a = find_first_minimal_entry(size);

    if(a == NULL) {
        // No allocations of proper size, defrag and try again

        merge_all_contiguous();
        a = find_first_minimal_entry(size);

        // welp, we tried
        if(a == NULL)
            return NULL;
    }

    if(a->length > size + sizeof(Allocation)) {
        // Subdivide to minimal size.
        Allocation *b = (Allocation *)(a->base + size);

        b->previous = a;
        b->next = a->next;
        b->length = a->length - (size + sizeof(Allocation));
        b->base = b + sizeof(Allocation);
        b->free = true;

        a->length = size;
        a->next = b;
    }

    a->free = false;
    return a->base;
}

/// @brief Allocates an aligned block of memory.
/// @param size The size of memory you need.
/// @param alignment The alignment you need.
/// @param free_handle A pointer to a variable you can use to free this memory.
/// @return The aligned pointer.
[[deprecated]]
void *kmalloc_aligned(size_t size, size_t align_to, void **free_handle) {
    void *ptr = kmalloc(size + align_to);
    size_t offset = ((size_t)ptr) % align_to;
    *free_handle = (void *)ptr;
    return (void *)(((size_t)ptr) + offset);
}

[[deprecated]]
void kfree(void *ptr) {
    Allocation *base = (Allocation *)kheap;

    if(base->base == ptr) {
        base->free = true;
        merge_all_contiguous();
        return;
    }

    Allocation *current = base->next;
    while(true) {

        if(current->base == ptr) {
            current->free = true;
            merge_all_contiguous();
            return;
        }

        current = current->next;
    }
}
#endif