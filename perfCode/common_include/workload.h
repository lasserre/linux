// workload.h
#pragma once

#include "experimentDebug.h"

namespace Workload
{    
    //// Variables
    const int FactorialOperand = 10;

    //// Function Prototypes
    void partition(int arr[],int low,int high);
    void mergeSort(int arr[],int low,int mid,int high);
    int CallMergeSort();
    int factor(int i);

    // ------------------------------------------------------------------- //
    // ------------------- Actual Function to be called ------------------ //
    void DoWork()
    {
        LOG_DEBUG(DEBUG_EXP_OUTPUT, "\tCalculating " << FactorialOperand << "! ..." << endl)
        factor(FactorialOperand);
        LOG_DEBUG(DEBUG_EXP_OUTPUT, "\tPerforming merge sort on " << SIZE << " items..." << endl)
        CallMergeSort();        
        return;
    }
    // ------------------------------------------------------------------- //
    // ------------------------------------------------------------------- //

    int CallMergeSort()
    {
    	int partition_size = SIZE / (NUM_THREADS); //this will get the size of each section
    	long i = (long)0; //will run from 0-7 

    	partition(merge,(i*partition_size),((i+1)*partition_size - 1));       
        
       	return 0;
    }

    void partition(int arr[],int low,int high){

        int mid;

        if(low<high){
             mid=(low+high)/2;
             partition(arr,low,mid);
             partition(arr,mid+1,high);
    		 
             mergeSort(arr,low,mid,high);
    		 
        }
    }

    void mergeSort(int arr[],int low,int mid,int high){

        int i,m,k,l,temp[SIZE];

        l=low;
        i=low;
        m=mid+1;

        while((l<=mid)&&(m<=high)){

             if(arr[l]<=arr[m]){
                 temp[i]=arr[l];
                 l++;
             }
             else{
                 temp[i]=arr[m];
                 m++;
             }
             i++;
        }

        if(l>mid){
             for(k=m;k<=high;k++){
                 temp[i]=arr[k];
                 i++;
             }
        }
        else{
             for(k=l;k<=mid;k++){
                 temp[i]=arr[k];
                 i++;
             }
        }
       
        for(k=low;k<=high;k++){
             arr[k]=temp[k];
        }
    }

    int factor(int i)
    {
    	if(i < 2)
    		return 1;
    	else
    	{
    		return (factor(i-1) + factor(i -2));
    	}
    }
}