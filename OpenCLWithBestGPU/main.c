//
//  main.c
//  OpenCLWithBestGPU
//
//  Created by Can Firtina on 23/07/15.
//  Copyright (c) 2015 Can Firtina. All rights reserved.
//

#include <stdio.h>

#include <stdio.h>
#include <OpenCL/OpenCL.h>

int main( int argc, const char * argv[]) {
    
    cl_int clerr = CL_SUCCESS;
    
    //create context that includes gpu devices in it
    cl_context clContext = clCreateContextFromType( NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, &clerr);
    if( clerr != CL_SUCCESS) {
        
        printf("Error during clCreateContextFromType\n");
        return EXIT_FAILURE;
    }
    
    //we will get the array of devices, but first we need to get the size (in bytes) of it
    size_t devicesSize;
    clerr = clGetContextInfo( clContext, CL_CONTEXT_DEVICES, 0, NULL, &devicesSize);
    if( clerr != CL_SUCCESS) {
        
        printf("Error during clGetContextInfo to get deviesSize\n");
        return EXIT_FAILURE;
    }
    
    //now we say that here is the array with devicesSize (in bytes). Fill it with devices
    cl_device_id *clDevices = (cl_device_id *)malloc(devicesSize);
    clerr = clGetContextInfo( clContext, CL_CONTEXT_DEVICES, devicesSize, clDevices, NULL);
    if( clerr != CL_SUCCESS) {
        
        printf("Error during clGetContextInfo to fill clDevices\n");
        return EXIT_FAILURE;
    }
    
    cl_uint numOfDevices;
    clerr = clGetContextInfo( clContext, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &numOfDevices, NULL);
    
    cl_uint maxComputeUnits = 0;
    cl_uint maxClockFrequency = 0;
    cl_device_id gpuId;
    cl_uint id = 0;
    
    for( int curDevice = 0; curDevice < numOfDevices; curDevice++){
        
        cl_uint computeUnits;
        cl_uint clockFrequency;
        clerr = clGetDeviceInfo( clDevices[curDevice], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &computeUnits, NULL);
        clerr |= clGetDeviceInfo( clDevices[curDevice], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &clockFrequency, NULL);
        
        if( clerr != CL_SUCCESS) {
            
            printf("Error during clGetDeviceInfo\n");
            return EXIT_FAILURE;
        }
        
        if( maxComputeUnits * maxClockFrequency < computeUnits * clockFrequency){
            
            maxComputeUnits = computeUnits;
            maxClockFrequency = clockFrequency;
            gpuId = clDevices[curDevice];
            id = curDevice;
        }
    }
    
    printf("Best GPU is %d", id);

    // Shutdown and cleanup
    clReleaseContext(clContext);
    
    return 0;
}
