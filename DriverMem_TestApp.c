
#include <stdio.h>
#include <stdlib.h>
#include "DriverMem.h"

#define PAGE_SIZE 10
#define NUM_OF_PAGE 10

uint8_t GetMemoryNumPages(uint8_t DeviceId);
uint64_t GetMemoryPageSize(uint8_t DeviceId);
uint8_t ** GetMemoryBaseAddress(uint8_t DeviceId);

#define RETURN_ON_FAILURE(arg, msg)   do{ if((arg) <= ERR_CODE_SUCCESS) { \
                ERROR_PRINT(" In: %s Code: %d Message: %s \r\n", \
                __func__, arg, (msg));\
                return ERR_CODE_FAILURE;\
                }} while(0)


int main(int argc, char** argv){

    printf("Starting Lilium Deign Exercise\n");

    ErrorCode ErrCode;
    uint8_t data_write, data_read = 0;
    uint8_t input;
    int ret;

    DriverMemPtr Mem1 = DriverMem_Construct();
    if(Mem1 == NULL){
        printf("Driver Construct Failed");
        return 0;
    }

    ErrCode = Mem1->Init(Mem1, 1);
    if(ErrCode != ERR_CODE_SUCCESS){
        printf("Driver Init Failed");
        return 0;
    }

    while(1)
    {
        printf("Enter 0 - exit, 1 - Read 2- Write \n");
        ret = scanf("%"SCNu8, &input);
        if(ret != 1)
            break;

        if(input == 0)
            break;
        else if(input == 1)
        {
            ErrCode = Mem1->Read(Mem1, &data_read);
            if(ErrCode != ERR_CODE_SUCCESS){
                printf("Driver Read Failed");
                continue;
            }
            else
                printf(" Data Read %"PRIu8"  \n", data_read);
        }
        else if(input == 2)
        {
            printf("Enter Data to Write \n");
            ret = scanf("%"SCNu8, &data_write);
            if(ret != 1)
                break;
            ErrCode = Mem1->Write(Mem1, data_write);
            if(ErrCode != ERR_CODE_SUCCESS){
                printf("Driver Write Failed");
                continue;
            }
            else
                printf(" Data Written %"PRIu8"  \n", data_write);
        }
        else{
            printf(" Invalid Input \n");
            continue;
        }

    }

    DriverMem_Destroy(Mem1);


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
