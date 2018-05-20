#ifndef __DRIVER_MEM_H_
#define __DRIVER_MEM_H_

#include <inttypes.h>

/**
 * \file Object oriented interface for memory driver
 *
 * Sample implementation is for virtual memory driver which supports read and write of a BYTE.
 * Modeling is similar to char device where data can be read
 * and written from device node. After reading the space is freed.
 *
 *
 *  Address 0                    PageSize
 *          ---------------------
 *  Page0   |   |   |   |   |   |
 *          ---------------------
 *          |   |   |   |   |   |
 *          ---------------------
 *  PageN   |   |   |   |   |   |
 *          ---------------------
 *
 */
#define ERR_CODE_SUCCESS            0
#define ERR_CODE_FAILURE            -1
#define ERR_CODE_INSTANCE_INVALID   -2

typedef int8_t ErrorCode;
/**
 *  Driver memory Class
 */
typedef struct __DriverMem DriverMem, *DriverMemPtr;

/**
 *
 * function pointer of interface to support polymorphism by inheriting class
 */
typedef ErrorCode (*InitFuncPtr)(DriverMem*, uint8_t);
typedef ErrorCode (*ReadFuncPtr)(DriverMem*, uint8_t*);
typedef ErrorCode (*WriteFuncPtr)(DriverMem*, uint8_t);

/**
 *  Driver memory Class definition
 */
struct __DriverMem
{
    void* PrivateAttributes;
    uint64_t Address;
    uint8_t Page;
    InitFuncPtr Init;
    ReadFuncPtr Read;
    WriteFuncPtr Write;
};

/** Constructor **/
DriverMemPtr DriverMem_Construct();

/** Destructor **/
void DriverMem_Destroy(DriverMemPtr DevMemPtr);

#endif
