
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MU_DEBUG
#define MU_ASSERTCHECK
#include "mu_ds.h" 


int main(void)
{
    int value = 0;
    size_t count = 0;
    srand(time(NULL));

    /* 
     * TEST MEMORY ALLOCATION AND
     * AUTO EXPANSION
     */
    fprintf(stdout, "\n\n TEST1: Memory expansion\n\n");

    MArray *arr = m_array_new(0, sizeof(int));
    printf("%3zu: %4zu %4zu\n", count++, ((_MArrayReal*)arr)->mem_alloc, ((_MArrayReal*)arr)->mem_alloc / ((_MArrayReal*)arr)->element_size);

    for(size_t i=0; i< 12; ++i)
    {
        value = rand();
        m_array_append(arr, value);
        printf("%3zu: %4zu %4zu\n", count++, ((_MArrayReal*)arr)->mem_alloc, ((_MArrayReal*)arr)->mem_alloc / ((_MArrayReal*)arr)->element_size);
    }
    
    for(size_t i=0; i<arr->len; ++i)
    {
        printf("%3zu: %12d   ", i, m_array_index(arr, int, i));
    }
    m_array_free(arr);

    /*
     * TEST ELEMENT REMOVAL
     */

    fprintf(stdout, "\n\n TEST2: Element Removal\n\n");

    MArray *arr2 = m_array_new(0, sizeof(int));

    for(size_t i=0; i< 16; ++i)
    {
        m_array_append(arr2, i);
    }
    m_array_remove_index(arr2, 3); // Removes 3
    m_array_remove_index(arr2, 5); // Removes 6
    m_array_remove_index(arr2, 9); // Removes 11

    for(size_t i=0; i<arr2->len; ++i)
    {
        printf("%3zu: %12d\n", i, m_array_index(arr2, int, i));
    }
    m_array_free(arr2);

    /*
     * TEST INSERT POS
     */

    fprintf(stdout, "\n\n TEST3: Insert\n\n");

    MArray *arr3 = m_array_new(0, sizeof(int));

    for(size_t i=0; i< 8; ++i)
    {
        m_array_append(arr3, i);
    }
    for(size_t i=0; i<arr3->len; ++i)
    {
        printf("%3zu: %12d\n", i, m_array_index(arr3, int, i));
    }
    printf("\n");

    value=42;
    m_array_insert(arr3, 1, value);
    m_array_insert(arr3, 4, value);
    m_array_insert(arr3, 7, value);
    int mvalues[2] = {43, 44};
    m_array_insert_group(arr3, 10, &mvalues, 2);

    for(size_t i=0; i<arr3->len; ++i)
    {
        printf("%3zu: %12d\n", i, m_array_index(arr3, int, i));
    }
    m_array_free(arr3);

    /*
     * TEST INSERT POS
     */

    fprintf(stdout, "\n\n TEST4: Swaps\n\n");

    MArray *arr4 = m_array_new(0, sizeof(int));

    for(size_t i=0; i< 8; ++i)
    {
        m_array_append(arr4, i);
    }
    for(size_t i=0; i<arr4->len; ++i)
    {
        printf("%3zu: %3d\n", i, m_array_index(arr4, int, i));
    }
    printf("\n");
    printf("\n");

    m_array_swap(arr4, 0, 1);
    for(size_t i=0; i<arr4->len; ++i)
    {
        printf("%3zu: %3d\n", i, m_array_index(arr4, int, i));
    }
    printf("\n");
    printf("\n");

    m_array_swap(arr4, 2, 3);
    for(size_t i=0; i<arr4->len; ++i)
    {
        printf("%3zu: %3d\n", i, m_array_index(arr4, int, i));
    }
    printf("\n");
    printf("\n");

    m_array_swap(arr4, 4, 5);
    for(size_t i=0; i<arr4->len; ++i)
    {
        printf("%3zu: %3d\n", i, m_array_index(arr4, int, i));
    }
    printf("\n");
    printf("\n");

    m_array_swap(arr4, 6, 7);
    for(size_t i=0; i<arr4->len; ++i)
    {
        printf("%3zu: %3d\n", i, m_array_index(arr4, int, i));
    }
    printf("\n");
    printf("\n");
    m_array_free(arr4);

    return 0;
}
