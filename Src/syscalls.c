/* @file syscalls.c 
 * @brief Implementation of Newlib system calls
 */

#include "usbd_cdc_if.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

int _write(int file, char *ptr, int len) {

    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    
    while (hcdc->TxState == 1){
        asm("nop");
    }

    CDC_Transmit_FS(ptr, len);
    return len;
}

static char myHeap[1024];

caddr_t sbrk(int incr) {
    static char *heap_end;
    char *prev_heap_end;
 
    /* first use, set up pointer */
    if (heap_end == 0) {
        heap_end = myHeap;
    }
  
    /* store last value */
    prev_heap_end = heap_end;

    /* check that there is enough space */
    if ((heap_end + incr) > (myHeap + sizeof(myHeap))) {
        return (caddr_t) -1;   
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}