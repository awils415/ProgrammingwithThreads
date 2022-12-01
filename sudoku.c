#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
//9x9 board
int board[SIZE][SIZE] = {
    {1,6,8,4,5,7,9,3,2},
    {5,7,2,3,9,1,4,6,8},
    {9,3,4,6,2,8,5,1,7},
    {8,2,9,7,4,3,1,5,6},
    {6,5,1,2,8,9,3,7,4},
    {7,4,3,5,1,6,2,8,9},
    {3,9,5,8,7,2,6,4,1},
    {4,1,7,9,6,5,8,2,3},
    {2,8,6,1,3,4,7,9,5},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
		int dupe[SIZE];
		int i = 0;
		for (i = 0; i < SIZE; i++) {
				dupe[i] = 0;
		}

		int row = *((int *)args);

		for (i = 0; i < SIZE; i++) {
				dupe[board[i][row] - 1] += 1;
		}

		row_check[row] = true;
		for (i = 0; i < SIZE; i++) {
				if (dupe[i] != 1) {
					row_check[row] = false;
				}
		}
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    int dupe[SIZE];
		int i = 0;
		for (i = 0; i < SIZE; i++) {
				dupe[i] = 0;
		}

		int col = *((int *)args);

		for (i = 0; i < SIZE; i++) {
				dupe[board[col][i] - 1] += 1;
		}

		col_check[col] = true;
		for (i = 0; i < SIZE; i++) {
				if (dupe[i] != 1) {
					col_check[col] = false;
				}
		}
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    int dupe[SIZE];
		int i = 0;
		for (i = 0; i < SIZE; i++) {
				dupe[i] = 0;
		}

		int box = *((int *)args);
		int j = 0;
		int x = (box / 3) * 3;
		int y = (box % 3) * 3;

		for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
						dupe[board[i + x][j + y] - 1] += 1;
				}
		}

		box_check[box] = true;
		for (i = 0; i < SIZE; i++) {
				if (dupe[i] != 1) {
						box_check[box] = false;
				}
		}
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
		pthread_t rowThreads[SIZE];
		pthread_t colThreads[SIZE];
		pthread_t boxThreads[SIZE];
    
    // 3. Create a thread for each cell of each matrix operation.
    int i = 0;
		for (i = 0; i < SIZE; i++) {
			int *a = malloc(sizeof(int));
			int *b = malloc(sizeof(int));
			int *c = malloc(sizeof(int));

			*a = i;
			*b = i;
			*c = i;

			pthread_create(&rowThreads[i], NULL, checkRow, a);
			pthread_create(&colThreads[i], NULL, checkCol, b);
			pthread_create(&boxThreads[i], NULL, checkBox, c);
		}

    // 4. Wait for all threads to finish.
    for (i = 0; i < SIZE; i++) {
			pthread_join(rowThreads[i], NULL);
			pthread_join(colThreads[i], NULL);
			pthread_join(boxThreads[i], NULL);
		}

    // 5. Print the results.

    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

