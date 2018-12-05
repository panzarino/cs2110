/*
 * CS 2110 Fall 2018
 * Author: Zachary Panzarino
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"
/* include this for any boolean methods */
#include <stdbool.h>

/*Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

/* Our freelist structure - our freelist is represented as two doubly linked lists
 * the address_list orders the free blocks in ascending address
 * the size_list orders the free blocks by size
 */

metadata_t *address_list;
metadata_t *size_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;

static unsigned long calcCanary(metadata_t *block) {
    return ((uintptr_t)block ^ CANARY_MAGIC_NUMBER) + 1;
}

static unsigned long* calcTailCanary(metadata_t *block) {
    return (unsigned long*)((uint8_t*)block + block->size - sizeof(unsigned long));
}

static void setCanary(metadata_t *block) {
    block->canary = calcCanary(block);
}

static void setTailCanary(metadata_t* block) {
    unsigned long* canary = calcTailCanary(block);
    *canary = block->canary;
}

static void canary(metadata_t *block) {
    setCanary(block);
    setTailCanary(block);
}

static metadata_t* leftBlock(metadata_t* block) {
    if (address_list == NULL) {
        return NULL;
    }
    metadata_t* blocboyJB = address_list;
    while (blocboyJB->next_addr !=  NULL && blocboyJB->next_addr < block) {
        blocboyJB = blocboyJB->next_addr;
    }
    if (blocboyJB == NULL || blocboyJB < block) {
        return blocboyJB;
    }
    return NULL;
}

static metadata_t* rightBlock(metadata_t* block) {
    metadata_t* blocboyJB = address_list;
    while (blocboyJB != NULL && blocboyJB < block) {
        blocboyJB = blocboyJB->next_addr;
    }
    return blocboyJB;
}

static void addAddress(metadata_t* block) {
    if (address_list == NULL) {
        address_list = block;
        return;
    }
    metadata_t* left = address_list;
    if ((uintptr_t)left > (uintptr_t)block) {
        block->next_addr = left;
        left->prev_addr = block;
        block->prev_addr = NULL;
        address_list = block;
        return;
    }
    while (left->next_addr != NULL && ((uintptr_t)(left->next_addr)) < (uintptr_t)block) {
        left = left->next_addr;
    }
    block->next_addr = left->next_addr;
    block->prev_addr = left;
    left->next_addr = block;
    if (block->next_addr != NULL) {
        block->next_addr->prev_addr = block;
    }
}

static void addSize(metadata_t* block) {
    if (size_list == NULL) {
        size_list = block;
        return;
    }
    metadata_t* left = size_list;
    if (left->size > block->size) {
        block->next_size = left;
        left->prev_size = block;
        block->prev_size = NULL;
        size_list = block;
        return;
    }
    while (left->next_size != NULL && left->next_size->size < block->size) {
        left = left->next_size;
    }
    block->next_size = left->next_size;
    block->prev_size = left;
    left->next_size = block;
    if (block->next_size != NULL) {
        block->next_size->prev_size = block;
    }
}

static void addBoth(metadata_t* block) {
    addAddress(block);
    addSize(block);
}

static void removeAddress(metadata_t* block) {
    if (block == address_list) {
        address_list = block->next_addr;
        if (block->next_addr != NULL) {
            block->next_addr->prev_addr = NULL;
            block->next_addr = NULL;
        }
        return;
    }
    if (block->next_addr == NULL) {
        block->prev_addr->next_addr = NULL;
        block->prev_addr = NULL;
        return;
    }
    block->prev_addr->next_addr = block->next_addr;
    block->next_addr->prev_addr = block->prev_addr;
    block->prev_addr = NULL;
    block->next_addr = NULL;
}

static void removeSize(metadata_t* block) {
    if (block == size_list) {
        size_list = block->next_size;
        if (block->next_size != NULL) {
            block->next_size->prev_size = NULL;
            block->next_size = NULL;
        }
        return;
    }
    if (block->next_size == NULL) {
        block->prev_size->next_size = NULL;
        block->prev_size = NULL;
        return;
    }
    block->prev_size->next_size = block->next_size;
    block->next_size->prev_size = block->prev_size;
    block->prev_size = NULL;
    block->next_size = NULL;
}

static void removeBoth(metadata_t* block) {
    removeAddress(block);
    removeSize(block);
}

static void merge(metadata_t* left, metadata_t* right) {
    left->size = left->size + (right->size);
    right = NULL;
}

static void mergeDouble(metadata_t* left, metadata_t* center, metadata_t* right) {
    left->size = left->size + center->size + right->size;
    center = NULL;
    right = NULL;
}

static metadata_t* split(metadata_t* block, size_t size) {
    metadata_t* new_block = ((metadata_t*)(((uint8_t*)block) + (block->size - size)));
    new_block->size = size;
    block->size = (block->size - size);
    return new_block;
}

/* MALLOC
 * See my_malloc.h for documentation
 */
void *my_malloc(size_t size) {
    if (size == 0) {
        my_malloc_errno = NO_ERROR;
    	return NULL;
    }
    size_t total_size = TOTAL_METADATA_SIZE + size;
    if (total_size > SBRK_SIZE) {
    	my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
    	return NULL;
    }
    metadata_t *block = size_list;
    while (block != NULL && block->size < total_size) {
        block = block->next_size;
    }
    if (block != NULL && block->size != total_size) {
        while (block != NULL && block->size < total_size + MIN_BLOCK_SIZE) {
            block = block->next_size;
        }
    }
    if (block == NULL) {
        metadata_t *new_block = (metadata_t*) my_sbrk(SBRK_SIZE);
        if (new_block == NULL) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }
        new_block->prev_addr = NULL;
        new_block->next_addr = NULL;
        new_block->prev_size = NULL;
        new_block->next_size = NULL;
        new_block->size = SBRK_SIZE;
        metadata_t* left = leftBlock(new_block);
        if (left != NULL && (metadata_t*)(((uint8_t*) left) + (left->size)) == new_block) {
            removeBoth(left);
            merge(left, new_block);
            block = split(left, total_size);
            addBoth(left);
            canary(block);
            block += 1;
            my_malloc_errno = NO_ERROR;
            return block;
        }
        block = split(new_block, total_size);
        addBoth(new_block);
        canary(block);
        block += 1;
        my_malloc_errno = NO_ERROR;
        return block;
    }
    if (block->size == total_size) {
        removeBoth(block);
        canary(block);
        block += 1;
        my_malloc_errno = NO_ERROR;
        return block;
    }
    removeSize(block);
    metadata_t* return_block = split(block, total_size);
    addSize(block);
    canary(return_block);
    return_block += 1;
    my_malloc_errno = NO_ERROR;
    return return_block;
}

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }
    if (ptr != NULL && size == 0) {
        my_free(ptr);
        return NULL;
    }
    metadata_t* meta_ptr = (metadata_t*)ptr - 1;
    unsigned long canary_correct = calcCanary(meta_ptr);
    if (meta_ptr->canary != canary_correct) {
        my_malloc_errno = CANARY_CORRUPTED;
        return NULL;
    }
    unsigned long* canary_tail = calcTailCanary(meta_ptr);
    if (*canary_tail != canary_correct) {
        my_malloc_errno = CANARY_CORRUPTED;
        return NULL;
    }
    void* new_ptr = my_malloc(size);
    if (size < meta_ptr->size - TOTAL_METADATA_SIZE) {
        new_ptr = memcpy(new_ptr, ptr, size);
    } else {
        new_ptr = memcpy(new_ptr, ptr, meta_ptr->size - TOTAL_METADATA_SIZE);
    }
    my_free(ptr);
    meta_ptr = NULL;
    my_malloc_errno = NO_ERROR;
    return new_ptr;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
    void* new_ptr = my_malloc(nmemb * size);
    if (new_ptr == NULL) {
        return NULL;
    }
    metadata_t* meta_ptr = (metadata_t*)new_ptr - 1;
    meta_ptr->prev_addr = NULL;
    meta_ptr->next_addr = NULL;
    meta_ptr->prev_size = NULL;
    meta_ptr->next_size = NULL;
    meta_ptr = NULL;
    new_ptr = memset(new_ptr, 0, nmemb * size);
    my_malloc_errno = NO_ERROR;
    return new_ptr;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
    if (ptr == NULL) {
        my_malloc_errno = NO_ERROR;
        return;
    }
    metadata_t* block = (metadata_t*)ptr - 1;
    ptr = NULL;
    unsigned long canary_correct = calcCanary(block);
    if (block->canary != canary_correct) {
        my_malloc_errno = CANARY_CORRUPTED;
        return;
    }
    unsigned long* canary_tail = calcTailCanary(block);
    if (*canary_tail != canary_correct) {
        my_malloc_errno = CANARY_CORRUPTED;
        return;
    }
    metadata_t* left = leftBlock(block);
    metadata_t* right = rightBlock(block);
    if (left != NULL && (((metadata_t*)(((uint8_t*)left + left->size)) == block))) {
        if (right != NULL && (metadata_t*)((uint8_t*)block + block->size) == right) {
            removeBoth(left);
            removeBoth(right);
            mergeDouble(left, block, right);
            addBoth(left);
            my_malloc_errno = NO_ERROR;
            return;
        }
        removeBoth(left);
        merge(left, block);
        addBoth(left);
        my_malloc_errno = NO_ERROR;
        return;
    }
    if (right != NULL && ((metadata_t*)((uint8_t*)block + block->size)) == right) {
        removeBoth(right);
        merge(block, right);
        addBoth(block);
        my_malloc_errno = NO_ERROR;
        return;
    }
    addBoth(block);
    my_malloc_errno = NO_ERROR;
}
