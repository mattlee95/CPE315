#define AMAX 10         /* Maximum (square) array size */

#define CACHESIM 1      /* Set to 1 if simulating Cache */

#define CASHE_SIZE 256   /* Size of Cashe */

#define ASSOC 4         /* Set associativity */

#include <stdio.h>

/* memory management, code density, Cache emulation - statistics generation */
/* Generated for CSC 315 Lab 5 */

typedef struct
{
   int data;
   int *ptr;
} CASHE_NODE;

int num_hit;
int num_miss;

int num_read;
int num_write;

CASHE_NODE cashe[ASSOC][CASHE_SIZE/ASSOC];
// 16 or 256 total
// // assoc = 1, 2 or 4

int round_robin_replacement_ptr[ASSOC];

/* This function gets called with each "read" reference to memory */

void mem_read(int *mp)
{
   int set = ((int)mp % 100*ASSOC) / 100;
   int cashes = CASHE_SIZE / ASSOC;
   int i, j, n;
   for (i = 0; i < cashes; i++)
   {
      if (cashe[set][i].ptr == (int)mp)
      {
         num_hit += 1;
         num_read += 1;
         cashe[set][i].data = *mp;
         return;
      }
   }
   num_miss += 1;
   num_read += 1;
   num_write += 1;

   cashe[set][round_robin_replacement_ptr[set]].ptr = mp;
   cashe[set][round_robin_replacement_ptr[set]].data = *mp;
   round_robin_replacement_ptr[set] += 1;
   if (round_robin_replacement_ptr[set] == cashes)
   {
      round_robin_replacement_ptr[set] = 0;
   }
   /* printf("Memory read from location %p\n", mp);  */
}


/* This function gets called with each "write" reference to memory */

void mem_write(int *mp)
{
   num_write += 1;
   /* printf("Memory write to location %p\n", mp); */
}


/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b. */

static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];
// 2x2 or 10x10


void matmul( r1, c1, c2 )
   {
   int i,j,k;
   int *mp1, *mp2, *mp3;



/* Initializing elements of matrix mult to 0.*/
    for(i=0; i<r1; ++i)
     for(j=0; j<c2; ++j)
       {
       mult[i][j]=0;
       }

/* Multiplying matrix a and b and storing in array mult. */

    for(i=0; i<r1; ++i)
     for(j=0; j<c2; ++j)
      for(k=0; k<c1; ++k)
        {

#if CACHESIM      /* "Hooks" to measure memory references - enabled if CACHESIM  */

        mp1 = &mult[i][j];
   mp2 = &a[i][k];
   mp3 = &b[k][j];   
   mem_read(mp1);
   mem_read(mp2);
   mem_read(mp3);
   mem_write(mp1); 
#endif

        mult[i][j]+=a[i][k]*b[k][j];

        }
   }





int main()
    {
    int r1, c1, r2, c2, i, j, k;

    int *mp1, *mp2, *mp3;

    num_hit = 0;
    num_miss = 0;
    num_read = 0;
    num_write = 0;

    for (i = 0; i < ASSOC; i++)
    {
      round_robin_replacement_ptr[i] = 0;
    }

    printf("Size of pointer is: %d\n\n", sizeof(mp1));

    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d",&r2, &c2);

/* If column of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
    while (c1 != r2)
        {
        printf("Error! column of first matrix not equal to row of second.\n");
        printf("Enter rows and column for first matrix: ");
        scanf("%d%d", &r1, &c1);
        printf("Enter rows and column for second matrix: ");
        scanf("%d%d",&r2, &c2);
        }

/* Storing elements of first matrix. */

    printf("\nEnter elements of matrix 1:\n");
    for(i=0; i<r1; ++i)
    for(j=0; j<c1; ++j)
          {
//        printf("Enter elements a%d%d: ",i+1,j+1);
//        scanf("%d",&a[i][j]);
   a[i][j] = i+j; // build sample data

          }

/* Storing elements of second matrix. */
    printf("\nEnter elements of matrix 2:\n");
    for(i=0; i<r2; ++i)
    for(j=0; j<c2; ++j)
    {
//        printf("Enter elements b%d%d: ",i+1,j+1);
//        scanf("%d",&b[i][j]);

         b[i][j] = 10 + i + j;
    }


   matmul(r1, c1, c2);     /* Invoke matrix multiply function */  


/* Displaying the multiplication of two matrix. */
    printf("\nOutput Matrix:\n");
    for(i=0; i<r1; ++i)
    for(j=0; j<c2; ++j)
    {
        printf("%d  ",mult[i][j]);
        if(j==c2-1)
            printf("\n\n");
    }

    printf("HITS: %d\nMISSES: %d\nREADS: %d\nWRITES: %d\n",num_hit,num_miss,num_read,num_write);
    return 0;
}
