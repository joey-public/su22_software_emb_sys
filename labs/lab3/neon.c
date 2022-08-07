#include <stdio.h>

#include "arm_neon.h"

int main () {
    /* Create custom arbitrary data. */
    const uint8_t uint8_data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    /* Create the vector with our data. (uint, 8-bit, 16)*/
    // write code here (1)
    uint8x16_t data_vec;
    
    /* Load our custom data into the vector register. Use function: vld1q_u8 */
    // write code here (2)
    data_vec = vld1q_u8(uint8_data);
    
    /* print your data */
    printf ("%s = ", "data");
    for (int i = 0; i < 16; i++) {
        // write code here (3)
        printf("%d ",data_vec[i]);
    }
    printf ("\n");
    
    
    /* Create an uint8x16 vector and set all elements to 3. use function: vmovq_n_u8 */
    // write code here (4)
    uint8x16_t three_vec = vmovq_n_u8(3);
    
    /* add data vector and vector with all elements=3. use function: vaddq_u8 */ 
    // write code here (5)
    uint8x16_t sum_vec = vaddq_u8(data_vec, three_vec);
    
    /* print the results */
    printf ("%s = ", "data_new");
    for (int i = 0; i < 16; i++) {
        // write code here (6)
        printf("%d ", sum_vec[i]); 
    }
    printf ("\n");
    
    return 0;
}