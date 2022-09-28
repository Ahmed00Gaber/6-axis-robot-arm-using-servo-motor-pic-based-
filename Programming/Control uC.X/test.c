#include <stdio.h>

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n, int order[])
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
        swap(&order[min_idx], &order[i]);
    }
}

void Difference(int difference[6], int values[6], int count) {
   difference[0] = values[0];
   for(int i = 1; i < count; i++) {
      difference[i] = values[i] - values[i-1];
   }
   difference[0] = values[0] + 69;  // the 69 is the 0.5 ms min. duty cycle 
}

void Buffer(int buffer[], int values[], int count) {
	for(int i = 0; i<count; i++) {
		buffer[i] = values[i];
	}
}

int main(){
   int i;
   int count = 6;
   // int number[6] = {230, 23, 14, 12, 9, 5};
   // int number[6] = {210, 211, 243, 236, 251, 249};
   int number[6] = {128, 190, 110, 245, 70, 30};
   int order[6] = {1, 2, 3, 4, 5, 6};
   int difference[6];
   int buffer[6];

	printf("Received: ");
    for(i=0;i<count;i++)
      printf(" %d",number[i]);

	printf("\nOrder: ");
    for(i=0;i<count;i++)
      printf(" %d",order[i]);

   selectionSort(number, count,order);

   printf("\nSorted: ");
   for(i=0;i<count;i++)
      printf(" %d",number[i]);

	printf("\nNew Order: ");
	for(i=0;i<count;i++)
      printf(" %d",order[i]);

  	Difference(difference, number, count);

  	printf("\nDifference: ");
	for(i=0;i<count;i++)
      printf(" %d",difference[i]);

	Buffer(buffer, number, count);

	printf("\nBuffer: ");
	for(i=0;i<count;i++)
      printf(" %d",buffer[i]);


   return 0;
}