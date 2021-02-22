//***************************************************
// Filename: othello.c
//
// Author(s): Chase Johnston, Christian Van Eerden
//***************************************************

#include "othello.h"

// Constructs and returns a string representation of the board
char *toString(int size, char board[][size])
{
	char *str = (char *) calloc(3 * (size+1) * (size+1), sizeof(char));
	char *ptr = str;

	for (int i = 0; i < size; i++) {
		ptr += sprintf(ptr,(i == 0 ? "%3d" : "%2d"),i+1);
	}
	sprintf(ptr,"\n");
	ptr++;

	for (int i = 0; i < size; i++) {
		sprintf(ptr,"%1d",i+1);
		ptr++;
		for (int j = 0; j < size; j++) {
			sprintf(ptr,"%2c",board[i][j]);
			ptr += 2;
		}
		sprintf(ptr,"\n");
		ptr++;
	}

	return str;
}

// Initializes the board with start configuration of discs (see project specs)
void initializeBoard(int size, char board[][size])
{
		for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			board[i][j] = EMPTY;
		}
	}
	//Init starting board discs

	board[size/2 - 1][size/2 - 1] = BLACK;
	board[size/2 - 1][size/2] = WHITE;
	board[size/2][size/2 - 1] = WHITE;
	board[size/2][size/2] = BLACK;

}
//Loop function that takes paramR for increment/decrement
//paramC is increment/decrement of col
//check_dir will only recieve -1, 0, or 1 for 
static bool check_dir(int size, char board[][size], int row, int col, char disc, int paramR, int paramC) {
    bool done = false;
    int r = row + paramR;
    int c = col + paramC;
    //Since the a matching disc has been found
    while(!done) {
  //      printf("Checking %d, %d\n", r+1, c+1);
        if(paramR == 1) {
            if(r >= size) {
 //               printf("Stop condition found at %d, %d\n", r+1, c+1);
                break;
            }
        }
        if(paramR == -1) {
            if(r < 0) {
   //             printf("Stop condition found at %d, %d\n", r+1, c+1);
                break;
            }
        }
        if(paramC == 1) {
            if(c >= size) {
  //              printf("Stop condition found at %d, %d\n", r+1, c+1);
                break;
            }
        }
        if(paramC == -1) {
            if(c < 0) {
   //             printf("Stop condition found at %d, %d\n", r+1, c+1);
                break;
            }
        }
        
        if(board[r][c] == EMPTY) {
            return false;
        }
        if(board[r][c] == disc) {
  //          printf("Stop condition found at %d, %d\n", r+1, c+1);
            return true;
        }
        
        
        r += paramR;
        c += paramC;
        if(paramR >= size || paramC >= size) {
            done = true;
        }
    }
   // printf("We got problems\n");
    
    return false;
}

// Returns true if moving the disc to location row,col is valid; false otherwise
bool isValidMove(int size, char board[][size], int row, int col, char disc)
{
	// Check North, east, south, west
    bool maincheck = false;
    //Up direction: row--
    if(row-1 > 0 && board[row-1][col] != EMPTY && board[row-1][col] != disc) {
        //Call function check_dir to see if up direction is a valid move
        bool check = check_dir(size, board, row, col, disc, -1, 0);
        if(check) {
            maincheck = true;
        }
    }
    //FIXME: TEST DIAGONALS
    
    //Up-Left Direction: row--, col--
    if(row-1 > 0 && col-1 > 0 && board[row-1][col-1] != EMPTY && board[row-1][col-1] != disc) {
        bool check = check_dir(size, board, row, col, disc, -1, -1);
        if(check) {
            maincheck = true;
        }
    }
    //Up-Right Direction: row--, col++
    if(row-1 > 0 && col+1 < size && board[row-1][col+1] != EMPTY && board[row-1][col+1] != disc) {
        bool check = check_dir(size, board, row, col, disc, -1, 1);
        if(check) {
            maincheck = true;
        }
    }
    
    //Down direction: row++
    if(row+1 < size && board[row+1][col] != EMPTY && board[row+1][col] != disc) {
        //Call function check_dir to see if up direction is a valid move
        bool check = check_dir(size, board, row, col, disc, 1, 0);
        if(check) {
            maincheck = true;
        }
    }
    //Down-Left Direction: row++, col--
    if(row+1 < size && col-1 > 0 && board[row+1][col-1] != EMPTY && board[row+1][col-1] != disc) {
        bool check = check_dir(size, board, row, col, disc, 1, -1);
        if(check) {
            maincheck = true;
        }
    }
    //Down-Right Direction: row++, col++
    if(row+1 < size && col+1 < size && board[row+1][col+1] != EMPTY && board[row+1][col+1] != disc) {
        bool check = check_dir(size, board, row, col, disc, 1, 1);
        if(check) {
            maincheck = true;
        }
    }
    
    //Left direction: col--
    if(col-1 > 0 && board[row][col-1] != EMPTY && board[row][col-1] != disc) {
        //Call function check_dir to see if up direction is a valid move
        bool check = check_dir(size, board, row, col, disc, 0, -1);
        if(check) {
            maincheck = true;
        }
    }
    //Right direction: col++
    if(col+1 < size && board[row][col+1] != EMPTY && board[row][col+1] != disc) {
        //Call function check_dir to see if up direction is a valid move
        bool check = check_dir(size, board, row, col, disc, 0, 1);
        if(check) {
            maincheck = true;
        }
    }
    if(maincheck) {
        return true;
    }
    
    return false;
}


// Places the disc at location row,col and flips the opponent discs as needed
void placeDiscAt(int size, char board[][size], int row, int col, char disc)
{
	if (!isValidMove(size,board,row,col,disc)) {
		return;
	}
    
    //Conditonals that tell where the discs are flipped
    bool up_check = check_dir(size, board, row, col, disc, -1, 0);
    if(up_check) {
        for(int i = row-1; i > 0; i--) {
            if(board[i][col] == disc) {
                //Stop condition
                break;
            }
            if(board[i][col] != disc && board[i][col] != EMPTY) {
                board[i][col] = disc;
            }
        }
    }
    bool down_check = check_dir(size, board, row, col, disc, 1, 0);
    if(down_check) {
        for(int i = row+1; i < size; i++) {
            if(board[i][col] == disc) {
                break;
            }
            if(board[i][col] != disc && board[i][col] != EMPTY) {
                board[i][col] = disc;
            }
        }
    }
    bool left_check = check_dir(size, board, row, col, disc, 0, -1);
    if(left_check) {
        for(int j = col-1; j > 0; j--) {
            if(board[row][j] == disc) {
                break;
            }
            if(board[row][j] != disc && board[row][j] != EMPTY) {
                board[row][j] = disc;
            }
        }
    }
    bool right_check = check_dir(size, board, row, col, disc, 0, 1);
    if(right_check) {
        for(int j = col+1; j < size; j++) {
            if(board[row][j] == disc) {
                break;
            }
            if(board[row][j] != disc && board[row][j] != EMPTY) {
                board[row][j] = disc;
            }
        }
    }
    bool up_left_check = check_dir(size, board, row, col, disc, -1, -1);
    if(up_left_check) {
        int i = row - 1;
        int j = col - 1;
        bool done = false;
        while(!done) {
            if(i < 0 || j < 0) {
                done = true;
                break;
            }
            if(board[i][j] == disc) {
                break;
            }
            if(board[i][j] != disc && board[i][j] != EMPTY) {
                board[i][j] = disc;
            }
            i--;
            j--;
        }
    }
    bool up_right_check = check_dir(size, board, row, col, disc, -1, 1);
    if(up_right_check) {
        int i = row - 1;
        int j = col + 1;
        bool done = false;
        while(!done) {
            if(i < 0 || j >= size) {
                done = true;
                break;
            }
            if(board[i][j] == disc) {
                break;
            }
            if(board[i][j] != disc && board[i][j] != EMPTY) {
                board[i][j] = disc;
            }
            i--;
            j++;
        }
    }
    bool down_left_check = check_dir(size, board, row, col, disc, 1, -1);
    if(down_left_check) {
        int i = row + 1;
        int j = col - 1;
        bool done = false;
        while(!done) {
            if(i >= size || j < 0) {
                done = true;
                break;
            }
            if(board[i][j] == disc) {
                break;
            }
            if(board[i][j] != disc && board[i][j] != EMPTY) {
                board[i][j] = disc;
            }
            i++;
            j--;
        }
    }
    bool down_right_check = check_dir(size, board, row, col, disc, 1, 1);
    if(down_right_check) {
        int i = row + 1;
        int j = col + 1;
        bool done = false;
        while(!done) {
            if(i >= size || j >= size) {
                done = true;
                break;
            }
            if(board[i][j] == disc) {
                break;
            }
            if(board[i][j] != disc && board[i][j] != EMPTY) {
                board[i][j] = disc;
            }
            i++;
            j++;
        }
    }
    board[row][col] = disc;
}

// Returns true if a valid move for disc is available; false otherwise
bool isValidMoveAvailable(int size, char board[][size], char disc)
{
	//Loops thru each tile in the board and checks each direction
	//If a valid move is found return true
	//If nothing is found and the loop is broken return false
    bool validMove = false;
	for(int i = 0; i < size; i++) {
		
		for(int j = 0; j < size; j++) {
			if(board[i][j] == EMPTY) {
				if(isValidMove(size, board, i, j, disc) && i < size && j < size) {
				    printf("VALID MOVES FOUND for %c at %d, %d \n", disc, i + 1, j + 1);
				    validMove = true;
				}
			}
			
			
			

		}
	}
    if(validMove) {
        return true;
    }
    return false;
}

// Returns true if the board is fully occupied with discs; false otherwise
bool isBoardFull(int size, char board[][size])
{
	for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;	
}

// Returns true if either the board is full or a valid move is not available for either disc; false otherwise
bool isGameOver(int size, char board[][size])
{
	if(isBoardFull(size, board)) {
		return true;
	}
	if(!isValidMoveAvailable(size, board, BLACK) && !isValidMoveAvailable(size,board,WHITE)) {
		return true;
	}
	
	return false;
}

// If there is a winner, it returns the disc (BLACK or WHITE) associated with the winner.
// In case of a tie, it returns TIE. When called before the game is over, it returns 0.
char checkWinner(int size, char board[][size])
{
	int whitetotal = 0;
	int blacktotal = 0;

	if (!isGameOver(size,board)) {
		return 0;
	} else {
		for (int i = 0; i < size; i++) {
			//Tally discs for black and white, ignore empty tile
			for(int j = 0; j < size; j++) {
				if(board[i][j] == BLACK) {
					blacktotal++;
				}
				if(board[i][j] == WHITE) {
					whitetotal++;
				}
			}
		}
	}
	//Return winner values
	if(whitetotal < blacktotal) {
		return BLACK;
	}
	if(blacktotal < whitetotal) {
		return WHITE;
	}

	return TIE;
}
