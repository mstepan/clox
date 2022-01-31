
#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#define GROW_CAPACITY(capacity) \
    ((capacity < 8) ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, arrayPtr, oldCapacity, newCapacity) \
    (type*) reallocate(arrayPtr, sizeof(type) * (oldCapacity), sizeof(type) * (newCapacity));

#define FREE_ARRAY(type, arrayPtr, oldCapacity) \
    (type*) reallocate(arrayPtr, sizeof(type) * (oldCapacity), 0);

void *reallocate(void *arrayPtr, size_t oldSize, size_t newSize);

#endif //CLOX_MEMORY_H
