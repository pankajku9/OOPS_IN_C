#include <stdio.h>
#include <stdlib.h>
#include "DriverMem.h"
#include <string.h>

#define PAGE_SIZE 10
#define NUM_OF_PAGE 10


#define TESTER(__Msg, __Expected, __Callfunc, ...)  do {\
                                  ErrorCode  __Ret = (__Callfunc)( __VA_ARGS__); \
                                  if( __Ret == __Expected) {\
                                  printf(" In: %s -- Testcase:%s Expected:%d Return:%d Passed \n", __func__, __Msg, __Expected, __Ret); } \
                                  else{ \
                                  printf(" In: %s -- Testcase:%s Expected:%d Return:%d Failed\n", __func__, __Msg, __Expected, __Ret); } \
                                } while(0)


uint8_t GetMemoryNumPages(uint8_t DeviceId);
uint64_t GetMemoryPageSize(uint8_t DeviceId);
uint8_t** GetMemoryBaseAddress(uint8_t DeviceId);

extern ErrorCode DriverMem_Init(DriverMemPtr DevMemPtr, uint8_t DeviceId);
extern ErrorCode DriverMem_Read(DriverMemPtr DevMemPtr, uint8_t* Data);
extern ErrorCode DriverMem_Write(DriverMemPtr DevMemPtr, uint8_t Data);

void UnitTest_Init_Func()
{
    ErrorCode ErrCode = ERR_CODE_SUCCESS;
    DriverMemPtr MemPtr = NULL;


    //tc 1 Garbage object
    DriverMem DevMem;
    DevMem.Init = DriverMem_Init;
    TESTER("tc1", ERR_CODE_FAILURE, DevMem.Init, &DevMem, 0);


    //tc 2 Memset object
    memset(&DevMem, 0, sizeof(DriverMem));
    DevMem.Init = DriverMem_Init;
    TESTER("tc2", ERR_CODE_FAILURE, DevMem.Init, &DevMem, 0);

    //tc 3
    MemPtr = DriverMem_Construct();
    TESTER("tc3", ERR_CODE_SUCCESS, DevMem.Init, MemPtr, 0);

    DriverMem_Destroy(MemPtr);
}

void UnitTest_Read_Func()
{
    ErrorCode ErrCode = ERR_CODE_SUCCESS;
    DriverMemPtr MemPtr = NULL;
    uint8_t  Data;


    //tc 1
    DriverMem DevMem;
    DevMem.Read = DriverMem_Read;
    TESTER("tc1", ERR_CODE_FAILURE, DevMem.Read, &DevMem, &Data);


    //tc 2
    memset(&DevMem, 0, sizeof(DriverMem));
    DevMem.Read = DriverMem_Read;
    TESTER("tc2", ERR_CODE_FAILURE, DevMem.Read, &DevMem, &Data);


    //tc 3
    MemPtr = DriverMem_Construct();
    TESTER("tc3", ERR_CODE_FAILURE, MemPtr->Read, MemPtr, &Data);

    //tc 4
    MemPtr->Page = 0;
    MemPtr->Address = 1;
    TESTER("tc4", ERR_CODE_FAILURE, MemPtr->Read, MemPtr, &Data);

    DriverMem_Init(MemPtr, 0);
    //tc 4
    MemPtr->Page = 0;
    MemPtr->Address = 1;
    TESTER("tc4", ERR_CODE_SUCCESS, MemPtr->Read, MemPtr, &Data);


    //tc 5
    MemPtr->Page = NUM_OF_PAGE-1;
    MemPtr->Address = PAGE_SIZE-1;
    TESTER("tc5", ERR_CODE_SUCCESS, MemPtr->Read, MemPtr, &Data);

    //tc 6
    MemPtr->Page = NUM_OF_PAGE;
    MemPtr->Address = PAGE_SIZE;
    TESTER("tc6", ERR_CODE_FAILURE, MemPtr->Read, MemPtr, &Data);

    DriverMem_Destroy(MemPtr);
}

void UnitTest_Write_Func()
{
    ErrorCode ErrCode = ERR_CODE_SUCCESS;
    DriverMemPtr MemPtr = NULL;
    uint8_t  Data = 10;


    //tc 1
    DriverMem DevMem;
    DevMem.Write = DriverMem_Write;
    TESTER("tc1", ERR_CODE_FAILURE, DevMem.Write, &DevMem, Data);


    //tc 2
    memset(&DevMem, 0, sizeof(DriverMem));
    DevMem.Write = DriverMem_Write;
    TESTER("tc2", ERR_CODE_FAILURE, DevMem.Write, &DevMem, Data);


    //tc 3
    MemPtr = DriverMem_Construct();
    TESTER("tc3", ERR_CODE_FAILURE, MemPtr->Write, MemPtr, Data);

    //tc 4
    MemPtr->Page = 0;
    MemPtr->Address = 1;
    TESTER("tc4", ERR_CODE_FAILURE, MemPtr->Write, MemPtr, Data);

    DriverMem_Init(MemPtr, 0);
    //tc 4
    MemPtr->Page = 0;
    MemPtr->Address = 1;
    TESTER("tc4", ERR_CODE_SUCCESS, MemPtr->Write, MemPtr, Data);


    //tc 5
    MemPtr->Page = PAGE_SIZE -1;
    MemPtr->Address = NUM_OF_PAGE -1;
    TESTER("tc5", ERR_CODE_SUCCESS, MemPtr->Write, MemPtr, Data);

    DriverMem_Destroy(MemPtr);
}

int main(int argc, char** argv){
    printf("Unit testing Memory Driver");
    UnitTest_Init_Func();
    UnitTest_Read_Func();
    UnitTest_Write_Func();

    return 0;
}

// If memory Interface id provides this canbe
uint8_t ** GetMemoryBaseAddress(uint8_t DeviceId)
{
    uint8_t **DataStore;

    DataStore = (uint8_t**)malloc(sizeof(uint8_t*)*NUM_OF_PAGE);
    for(int i=0; i < NUM_OF_PAGE; i++)
    {
        DataStore[i] = (uint8_t*)malloc(sizeof(uint8_t)*PAGE_SIZE);
    }

    return DataStore;
}

uint8_t GetMemoryNumPages(uint8_t DeviceId)
{
    return NUM_OF_PAGE;
}
uint64_t GetMemoryPageSize(uint8_t DeviceId){

    return PAGE_SIZE;
}
