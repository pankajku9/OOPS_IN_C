// cd C://Users//pankajku//Desktop//practice//lilium//ex2
//gcc -std=c99 DriverMem.c -o DriverMem
#include <stdio.h>

#include <stdlib.h>
#include "DriverMem.h"



#define DEBUG

/** DEBUG and PRINT MACRO for extendablity*/
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)    fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#define ERROR_PRINT(fmt,  ...)    fprintf(stderr, fmt, ##__VA_ARGS__)

/** Boiler Plate Macros of NULL CHECK RETURN for code reuse and Reablity*/
#define RETURN_IF_NULL(arg, msg)   do{ if((arg) == NULL) { \
                ERROR_PRINT("%s: Caught null pointer for parameter %s\r\n", \
                __func__, (msg));\
                return ERR_CODE_FAILURE;\
                }} while(0)


#define RETURN_ON_FAILURE(arg, msg)   do{ if((arg) < ERR_CODE_SUCCESS) { \
                ERROR_PRINT(" In: %s Code: %d Message: %s \r\n", \
                __func__, arg, (msg));\
                return ERR_CODE_FAILURE;\
                }} while(0)

/**
 *  External Interface of low level memory
 */
extern uint8_t GetMemoryNumPages(uint8_t DeviceId);
extern uint64_t GetMemoryPageSize(uint8_t DeviceId);
extern uint8_t ** GetMemoryBaseAddress(uint8_t DeviceId);

/**
 *  Struct encapsulating private member of DriverMem
 *  Not  accessible outside the  file
 */
typedef struct __MemAttributes {
    uint8_t NumOfPages;
    uint64_t PageSize;
    uint8_t **BaseAddress;
} MemAttributes;


// How can you validate your container ??
/** This will verify that instance has been properly constructed.
 * In case a statically allocated instance  is passed it will return failure
 **/
ErrorCode InstanceInalid(DriverMemPtr DevMemPtr, uint8_t InitDone){

    if(DevMemPtr == NULL){
		DEBUG_PRINT("InsatnceInavalid:DriverMem Pointer NULL");
        return ERR_CODE_INSTANCE_INVALID;
    }

    if(DevMemPtr->PrivateAttributes == NULL ){
        DEBUG_PRINT("InsatnceInavalid: Private Member Pointer NULL");
        return ERR_CODE_INSTANCE_INVALID;
    }

    if(InitDone != 0) {
        MemAttributes *Attr = (MemAttributes*)(DevMemPtr->PrivateAttributes);
        if(Attr->BaseAddress == NULL){
            DEBUG_PRINT("InsatnceInavalid: Base Pointer NULL");
            return ERR_CODE_INSTANCE_INVALID;
        }

        if((DevMemPtr->Address > Attr->PageSize) ||
                        (DevMemPtr->Page > Attr->NumOfPages -1))
        {
            DEBUG_PRINT("InsatnceInavalid: Public Member NULL");
            return ERR_CODE_INSTANCE_INVALID;
        }
    }

    return  ERR_CODE_SUCCESS;
}
/**
 *  Initialize the Memory attributes Details Details
 *
 * @param DevMemPtr     DriverMem Struct Pointer
 * @param DeviceId      Device ID
 * @return              0 on Success or -1 for Failure
 */
ErrorCode DriverMem_Init(DriverMemPtr DevMemPtr, uint8_t DeviceId)
{
   DEBUG_PRINT("%s called\n", __func__);
   MemAttributes *currDevice;


   ErrorCode RetCode = InstanceInalid(DevMemPtr, 0);
   RETURN_ON_FAILURE(RetCode, "Instance Invalid");


   if(DevMemPtr->PrivateAttributes != NULL )
   {
       currDevice = (MemAttributes *)DevMemPtr->PrivateAttributes;
   }
   else
   {
       ERROR_PRINT("%s Unconstructed Object passed\n", __func__);
       return ERR_CODE_FAILURE;
   }

   if(currDevice->NumOfPages == 0){
       currDevice->NumOfPages = GetMemoryNumPages(DeviceId);
   }
   else
   {
       ERROR_PRINT("%s Unconstructed Object passed\n", __func__);
       return ERR_CODE_FAILURE;
   }

   if(currDevice->PageSize == 0)
   {
       currDevice->PageSize = GetMemoryPageSize(DeviceId);
   }
   else
   {
       ERROR_PRINT("%s Unconstructed Object passed\n", __func__);
       return ERR_CODE_FAILURE;
   }

   if(currDevice->BaseAddress == NULL)
   {
       currDevice->BaseAddress = GetMemoryBaseAddress(DeviceId);
   }
   else
   {
       ERROR_PRINT("%s Unconstructed Object passed\n", __func__);
       return ERR_CODE_FAILURE;
   }
   return ERR_CODE_SUCCESS;
}
/**
 *  Reads a short from memory. It decrement i.e updates page and the address
 *
 * @param DevMemPtr     DriverMem Struct Pointer
 * @param Data          Pointer to Data to be returned
 * @return              0 on Success or -1 for Failure
 */
ErrorCode DriverMem_Read(DriverMemPtr DevMemPtr, uint8_t* Data)
{
    DEBUG_PRINT("%s called\n", __func__);
    RETURN_IF_NULL(DevMemPtr, "DriverMemPtr");
    RETURN_IF_NULL(Data, "Data");

    ErrorCode RetCode = InstanceInalid(DevMemPtr, 1);
    RETURN_ON_FAILURE(RetCode, "Instance Invalid");

    MemAttributes *Attr = (MemAttributes *)(DevMemPtr->PrivateAttributes);

    if(DevMemPtr->Address == 0 && DevMemPtr->Page == 0)
    {
        ERROR_PRINT("%s Memory Empty\n", __func__);
        return -1;
    }
    else if(DevMemPtr->Address == 0 && (DevMemPtr->Page > 0 && DevMemPtr->Page < Attr->PageSize)){

        DevMemPtr->Page = DevMemPtr->Page-1;
        DevMemPtr->Address = Attr->PageSize;


        *Data = Attr->BaseAddress[DevMemPtr->Page][--DevMemPtr->Address];
    }
    else
    {
        MemAttributes *Attr = (MemAttributes *)(DevMemPtr->PrivateAttributes);
        *Data = Attr->BaseAddress[DevMemPtr->Page][--DevMemPtr->Address];
    }
    return 0;
}
/**
 *  Writes short data into memory. Updates the page and address
 *
 * @param DevMemPtr     DriverMem Struct Pointer
 * @param Data          Pointer to Data to be returned
 * @return              0 on Success or -1 for Failure
 */
ErrorCode DriverMem_Write(DriverMemPtr DevMemPtr, uint8_t Data)
{
    DEBUG_PRINT("%s called\n", __func__);
    RETURN_IF_NULL(DevMemPtr, "DriverMemPtr");

    ErrorCode RetCode = InstanceInalid(DevMemPtr, 1);
    RETURN_ON_FAILURE(RetCode, "Instance Invalid");

    MemAttributes *Attr = (MemAttributes *)(DevMemPtr->PrivateAttributes);

    if(DevMemPtr->Address == Attr->PageSize &&
        DevMemPtr->Page == Attr->NumOfPages-1)
    {
        ERROR_PRINT("%s Memory Full\n", __func__);
        return -1;
    }
    // Until second last page
    else if(DevMemPtr->Address == Attr->PageSize &&
                    DevMemPtr->Page < Attr->NumOfPages-1)
    {
        DevMemPtr->Page = DevMemPtr->Page + 1;
        DevMemPtr->Address = ((MemAttributes*)DevMemPtr->PrivateAttributes)->PageSize;
    }
    else
    {
        Attr->BaseAddress[DevMemPtr->Page][DevMemPtr->Address++] = Data;
    }
    return 0;
}
/**
 *  frees  the dynamic memory allocated. Acts like Destructor
 * @param DevMemPtr  DriverMem Struct Pointer to be De initialized
 */
void DriverMem_Destroy(DriverMemPtr DevMemPtr)
{
    free(DevMemPtr->PrivateAttributes);
    DevMemPtr->PrivateAttributes = NULL;

    free(DevMemPtr);
    DevMemPtr = NULL;
}
/**
 *  Allocates the memory and initializes the function pointer
 * @return Initialized DriverMem Struct Pointer
 *
 * This help simulates the object oriented interface and will act like constructor
 */
DriverMemPtr DriverMem_Construct()
{

    DEBUG_PRINT("%s called\n", __func__);
    DriverMemPtr DevMemPtr = (DriverMemPtr)calloc(1, sizeof(DriverMem));
    if(DevMemPtr == NULL)
    {
        ERROR_PRINT("%s malloc for DriverMem failed\n", __func__);
        return NULL;
    }

    DevMemPtr->PrivateAttributes = calloc(1, sizeof(MemAttributes));
    if(DevMemPtr->PrivateAttributes == NULL)
    {
        ERROR_PRINT("%s malloc for Private Attributes failed\n", __func__);
        free(DevMemPtr);
        DevMemPtr = NULL;
        return NULL;
    }


    DEBUG_PRINT("%s allocation passed\n", __func__);

    // Default init private Memory
    ((MemAttributes*)DevMemPtr->PrivateAttributes)->PageSize = 0;
    ((MemAttributes*)DevMemPtr->PrivateAttributes)->NumOfPages = 0;
    ((MemAttributes*)DevMemPtr->PrivateAttributes)->BaseAddress = NULL;

    // initialize interface
    DevMemPtr->Init = DriverMem_Init;
    DevMemPtr->Read = DriverMem_Read;
    DevMemPtr->Write = DriverMem_Write;

    // initialize public member
    DevMemPtr->Page = 0;
    DevMemPtr->Address = 0;

    return DevMemPtr;
}







