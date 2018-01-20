//Gautam Ramachandruni
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define true 1
#define false 0
//Declarations
int indices[10000000000]; //Stores the smallest subset values
int targetSum; //To read the value of target sum of input integers
int key = 0;
int rem; //Remaining
int latent; //Reads latent sum which is compared to target
double space; //Performance metric
int subsetSum(int a, int b, int* m, int* n) //Function to solve exact subset sum
{
    int i;
    int count = 0;
    n[0] = 0;
    for(latent = 1; latent <= b; latent++)
    {
        for(i = 1; i <= a; i++)
        {
            rem = latent - m[i];      
            if(rem >= 0 && n[rem] < i)                
                break;                        
            space++;
        }
        n[latent] = i;
        key += m[i];
    }
    if(n[b] == a + 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int main()
{
    int store1, store2; //Random variables to store values obtained from loops
    clock_t start, end; //To record CPU time
    double time;
    int input; //Declaring variable to read number of input integers
    printf("Enter the number of input integers & the target sum:\n");
    scanf("%d %d", &input, &targetSum);
    int firstList[input / 2], secondList[input / 2]; //List initialization
    for(store1 = 0; store1 < input; store1++)
    {
        if(store1 < (input) / 2) //For values in first list
        {
            scanf("%d", &firstList[store1]);
        }
        else //For values in second list
        {
            scanf("%d", &secondList[store1]);
        }
    }
    key = targetSum; //Storing the value of target sum
    start = clock(); //Starting clock
    store2 = subsetSum(input / 2, targetSum, firstList, indices); //Interpreting values stored in subsetSum function
    if(store2 != 1)
    {
        printf("\nNot first!\n");
        targetSum -= latent;
        store2 = subsetSum(input / 2, targetSum, secondList, indices); //Improvising with second list
    }
    end = clock(); //Clock stops and time is recorded
    time = ((double)(end - start)) / CLOCKS_PER_SEC; //Computing the CPU time
    if(store2 == 1)
    {
        printf("\nSuccess!\n\n");
        printf("Displaying performance metrics...\n\n");
        time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Total CPU time: %6f\n", time);
        printf("Total space used: %lf\n", space / 2);
        printf("\nThe solution:\n\n");
        printf("%d", indices[targetSum]);
        for (store1 = key; store1 >= 0; store1 -= firstList[indices[store1]])
        {
                printf("%3d %3d\n", indices[store1], firstList[indices[store1]]);
        }
    }
    else
    {
        printf("No solution found!\n");
    }
    return 0;
}
