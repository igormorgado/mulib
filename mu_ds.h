#ifndef __MU_DS_H__
#define __MU_DS_H__

#include <stdlib.h>
#include <string.h>

#ifdef MU_ASSERTCHECK
#include <assert.h>
#endif

#ifdef MU_DEBUG
#include <stdio.h>
#endif

/* TODO:
 * HASH TABLE (unordered set)
 * QUEUE (Double linked list/deque)
 * Heap (Array/nao ordenado posicao fixa) // implementar swap elementos;
 * BST(array) -> ordered set
 * stack -> Add a pop without malloc.. how??!?!?!
 * 
 *  In Expandable array:
 *   0. Is empty?
 *   1. Implement POP without malloc
 *   2. Implement remove fast (replacing last element with position/disordering
 *   3. insert sorted
 *   4. Implement remove range 
 *   5. Implement binary search
 *   6. CleanFunc for array element
 *   7. Merge sort
 *   8. Custom comp func
 *   9. default int/string comp func.
 *   10. Se fizer pop vira uma queue.
 *
 *   C++ methods:
 *   back == array->len
 *   push_back() == _append
 *   insert == _append
 *
 * Structures to add 
 */

struct _MArrayReal {
    char * data;            // Pointer to data
    size_t len;             // Number of data elements
    size_t mem_alloc;       // Ammount of memory allocated
    size_t element_size;    // Sizeof each data element
};

struct _MArray {
    char * data;
    size_t len;
};

typedef struct _MArrayReal _MArrayReal;
typedef struct _MArray     MArray;


#undef MAXINT
#define MAXINT(a,b)  (((a) > (b)) ? (a):(b))

#undef MININT
#define MININT(a,b)  (((a) < (b)) ? (a):(b))

#undef ABSINT
#define ABSINT(a)    (((a) < 0)  ? -(a):(a))

#undef CLAMP
#define CLAMP(x, lower, upper) \
    (((x) > upper)) ? (upper) : ((((x) < (lower)) ? (lower) : (x)))

#undef APPROX
#define APPROX(a, b, epsilon) \
    (((a) > (b) ? (a)-(b) : (b)-(a)) < (epsilon))


/* Returns the offset position of given element in array */
#define _m_array_element_offset(array,element) ((array)->element_size * (element))

/* Returns the memory address to given element in array */
#define _m_array_element_mempos(array,element) ((array)->data + _m_array_element_offset((array),(element)))

/* Append element into array */
#define m_array_append(array,element) m_array_append_group((array), &(element), 1)

/* Prepend element into array */
#define m_array_prepend(array,element)   m_array_prepend_group((array), &(element), 1)

/* Insert a element into array */
#define m_array_insert(array,index,data) m_array_insert_group((array), (index), &(data), 1)

/* Retrieve the given element from array casting it as required */
#define m_array_index(array,type,position) (( (type*) (void *) (array)->data) [(position)])

/* peek the last array element */
#define m_array_peek(array, type)  (( (type*) (void *) (array)->data) [(array)->len])

MArray * m_array_new(size_t reserved, size_t element_size);
MArray * m_array_append_group(MArray *array, void* data, size_t len);
MArray * m_array_prepend_group(MArray *array, void *data, size_t len);
MArray * m_array_insert_group(MArray *array, size_t index, void* data, size_t len); 

MArray * m_array_remove_index(MArray *array, size_t index);
MArray * m_array_remove_index_fast(MArray *array, size_t index);

MArray * m_array_swap(MArray *array, size_t index_a, size_t index_b); 

size_t   m_array_element_size(MArray *array);

void m_array_set_clearfunc(MArray *array, void (*clearfunc) (void*)); 
void m_array_free (MArray *array);

/* Return index if found or negative if not found. */
/* TODO: int m_array_search(MArray *array, (void*) searchtarget, (int)(*comparefunc)((void*)a,(void*)b)) */

/* TODO: MArray * m_array_set_size(MArray *array, size_t len); */
/* TODO: static void _m_array_shrink (_MArrayReal *array);  */

static void _m_array_expand (_MArrayReal *array, size_t len);

/*
 * reserved: number of elements to allocate upfront
 * element_size: size of stored element
 */
MArray *
m_array_new (size_t elements_reserved, size_t element_size)
{

#ifdef MU_ASSERTCHECK
    assert(element_size >0);
#endif

   _MArrayReal * array = malloc (sizeof (*array));
   array->data = NULL;
   array->len = 0;
   array->mem_alloc = 0;
   array->element_size = element_size;

   _m_array_expand(array, elements_reserved);
   return (MArray *) array;
}


MArray *
m_array_append_group(MArray *array, void* data, size_t len)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
    assert(len > 0);
#endif
    _MArrayReal *arrayr =  (_MArrayReal *) array; 
    _m_array_expand(arrayr, len);
    memcpy(_m_array_element_mempos(arrayr, arrayr->len), data, _m_array_element_offset(arrayr, len));
    arrayr->len += len;

    return array;
}

MArray *
m_array_prepend_group(MArray *array, void *data, size_t len)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
    assert(data != NULL);
    assert(len > 0);
#endif
    return m_array_insert_group(array, 0, data, len);
}

MArray * 
m_array_insert_group(MArray *array, size_t index, void* data, size_t len)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
    assert(data != NULL);
    assert(index <= array->len);
    assert(len > 0);
#endif 
    _MArrayReal *arrayr = (_MArrayReal *) array;
    _m_array_expand(arrayr, len);

    memmove (_m_array_element_mempos(arrayr, index+len),
             _m_array_element_mempos(arrayr, index),
             _m_array_element_offset(arrayr, array->len - index));
    memcpy  (_m_array_element_mempos(arrayr, index), data, _m_array_element_offset(arrayr, len));
    arrayr->len +=len;
    return array;
}

/*
 * array: A pointer to array to be expanded
 * len: the number of elements we wish to add;
 */
static void 
_m_array_expand (_MArrayReal *array, size_t len)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
#endif
    /* TODO: Check for overflows */

    size_t request_size;

    request_size = _m_array_element_offset(array, array->len + len);
    if (request_size > array->mem_alloc)
    {
        float  growth_factor = 1.5f;
        size_t new_size;
        new_size = array->mem_alloc + array->element_size;
        new_size = (size_t)(new_size * growth_factor);
        new_size = MAXINT(request_size, new_size);
        /* Align the growth_factor to nearest greater element size */
        new_size = (new_size + array->element_size - 1) & ~(array->element_size - 1);
        array->data = realloc(array->data, new_size);
        /* Zero the new region */
        memset(array->data + array->mem_alloc, 0, new_size - array->mem_alloc);
        array->mem_alloc = new_size;
    }
}

MArray *
m_array_remove_index(MArray *array, size_t index)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
    assert(index <= array->len);
#endif
    _MArrayReal * arrayr = (_MArrayReal*)array;

    /* TODO: CAll a clear function in case of structs that call mallocs */
    memset(_m_array_element_mempos(arrayr, index), 0, arrayr->element_size);

    memmove(_m_array_element_mempos(arrayr, index),
            _m_array_element_mempos(arrayr, index+1),
            _m_array_element_offset(arrayr, arrayr->len - index -1));

    arrayr->len -= 1;

    return array;
}

MArray * m_array_remove_index_fast(MArray *array, size_t index)
{
    MArray * marray = m_array_swap(array, index, (array->len)-1);
    array->len -= 1;
    return marray;

}

MArray *
m_array_swap(MArray *array, size_t index_a, size_t index_b)
{
#ifdef MU_ASSERTCHECK
    {
    assert(array != NULL);
    assert(index_a < array->len);
    assert(index_b < array->len);
    }
#endif

    _MArrayReal * arrayr = (_MArrayReal*) array;
    char *tmp = malloc(sizeof (arrayr->element_size));

#ifdef MU_DEBUG
    {
    fprintf(stderr, "DEBUG INIT: tmp: %3d   a: %3d   b: %3d\n",
            (int) *tmp,
            m_array_index(arrayr, int, index_a),
            m_array_index(arrayr, int, index_b));
    }
#endif

    memcpy(tmp,
           _m_array_element_mempos(arrayr, index_b),
           sizeof(arrayr->element_size));

#ifdef MU_DEBUG
    {
    fprintf(stderr, "DEBUG b->t: tmp: %3d   a: %3d   b: %3d\n",
            (int) *tmp,
            m_array_index(arrayr, int, index_a),
            m_array_index(arrayr, int, index_b));
    }
#endif

    memcpy(_m_array_element_mempos(arrayr, index_b),
           _m_array_element_mempos(arrayr, index_a),
           sizeof(arrayr->element_size));

#ifdef MU_DEBUG
    {
    fprintf(stderr, "DEBUG a->b: tmp: %3d   a: %3d   b: %3d\n",
            (int) *tmp,
            m_array_index(arrayr, int, index_a),
            m_array_index(arrayr, int, index_b));
    }
#endif

    memcpy(_m_array_element_mempos(arrayr, index_a),
           tmp,
           sizeof(arrayr->element_size));

#ifdef MU_DEBUG
    {
    fprintf(stderr, "DEBUG t->a: tmp: %3d   a: %3d   b: %3d\n",
            (int) *tmp,
            m_array_index(arrayr, int, index_a),
            m_array_index(arrayr, int, index_b));
    }
#endif

    free(tmp);
    return (MArray *)arrayr;
}

void 
m_array_free (MArray *array)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
#endif
    if(array->data) 
    {
        free(array->data);
    }
    free(array);
}

size_t
m_array_element_size(MArray *array)
{
#ifdef MU_ASSERTCHECK
    assert(array != NULL);
#endif
    return ((_MArrayReal*)array)->element_size;
}

#endif  /* __MU_DS_H__ */

