//Gautam Ramachandruni
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define true 1
#define false 0
//Declarations
int indices[10000000000]; //Stores the smallest subset values
double space; //Performance metric
int main()
{
    int store1, store2; //Random variables to store values obtained from loops
    clock_t start, finish; //To record CPU time
    double time;
    int input, targetSum; //Declaring variables to read number of input integers and their target sum
    printf("Enter the number of input integers & the target sum:\n"); 
    scanf("%d %d", &input, &targetSum);
    signed int list[input]; //List is initiated
    for(store1 = 0; store1 < input; store1++)
    {
        scanf("%d", &list[store1]);
        if(list[store1] < 0) //Invalid input condition
        {
            printf("Invalid: Enter positive value!\n");
            return 0;
        }
    }
    start = clock(); //Starting clock
    int i;
    int count = 0;
    int latent; //Reads latent sum which is compared to target
    int rem; //Remaining
    indices[0] = 0;
    for(latent = 1; latent <= targetSum; latent++)
    {
        for(i=1; i <= input; i++)
        {
            rem = latent - list[i]; //With regard to other values as well
            if(rem >= 0 && indices[rem]<i) //Indices are arranged in increasing order, a solution may be available
                break;                         
            count++; //Increment count
            space++;
        }
        indices[latent] = i;
        count = 0;
    }
    if(indices[targetSum] == input + 1)
    {
        store2 = 0;
    }
    else
    {
        store2 = 1;
    }
    finish = clock(); //Clock stops and time is recorded
    if(store2 == 1)
    {
        printf("\nSuccess!\n\n");
        printf("Displaying performance metrics...\n\n");
        time = ((double)(finish - start)) / CLOCKS_PER_SEC; //Computing the CPU time
        printf("Total CPU time: %6f\n", time);
        printf("Total space used: %lf\n", space / 2);
        printf("\nThe solution:\n\n");
        for (store1 = targetSum; store1 >= 0; store1 -= list[indices[store1]])
            printf("%3d %3d\n", indices[store1], list[indices[store1]]);
    }
    else
    {
        printf("No solution found!\n");
    }
    return 0;
}
