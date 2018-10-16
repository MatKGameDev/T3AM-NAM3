// Filename:    main.cpp
// Author:      Group 8 - <T34M N4M3>: 
//               Mathew Kostrzewa - 100591924
//               member2 - studentNum
//               member3 - studentNum
//               member4 - studentNum
//               member5 - studentNum
// Date:        Oct 9, 2018
// Description: This code is used as the main cpp file for our chess game, developed for the GDW2 project in the Game Development Workshop course at UOIT.

/*
* TODO: (Incomplete list)
*  Validate player's entry for start and end coordinates
*  Main menu screen
*  How to play guide option on main menu
*  Short message with last turn's action displayed after each move
*  Properly structured 2 player gameplay
*  Coloured pieces to specify owner for each piece
*  Implement checks for check/stalemate
*  Turn pawn into queen when it reaches the enemy's last row
*
* If time permits (lower priority TODOs):
*  Play vs AI option
*  Allow the user to perform moves with keyboard (or mouse, whichever is easier)
*  Time limit for turns
*/

#include <iostream>
#include <string>
#include <Windows.h>

std::string chessBoard[8][8]; //a 2d array to represent the chess board, standard size is 8x8


							  //initializeBoard function prototype
							  //resets the logical chess board to place pieces at starting positions
void initializeBoard();

//drawBoard function prototype
//draws out a neatly formatted chess board with pieces dynamically placed
void drawBoard();

//isValidPieceMovement function prototype
//figures out if a piece is allowed to be moved to the user's destination
bool isValidPieceMovement(int startX, int startY, int destinationX, int destinationY);

//isValidHorizontalOrVerticalMove function prototype
//determines if a piece can be moved to the destination horizontally or vertically (includes collision detection)
bool isValidHorizontalOrVerticalMove(int startX, int startY, int destinationX, int destinationY);

//isValidDiagonalMove function prototype
//determines if a piece can be moved to the destination diagonally (includes collision detection)
bool isValidDiagonalMove(int startX, int startY, int destinationX, int destinationY);

int main()
{
	//used the following thread for help with resizing the console window: stackoverflow.com/questions/21238806/how-to-set-output-console-width-in-visual-studio
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 470, 700, TRUE); // width, height

	initializeBoard(); //reset the chess board
	drawBoard();       //display the chess board to the user

					   // test piece validation and moving them around the board
	std::string userInputStart;
	std::string userInputEnd;
	while (1)
	{
		//***NOTE*** 
		//this is a demo and will need to be refined later on to make it more user friendly
		//when testing the program, enter x,y coordinate. Do not enter the letter, instead enter it's number equivalent (ex. column A would be a y value of 1, B would be 2, H would be 8, etc.)
		//example input (move second player's leftmost pawn up 2 spaces): 
		// first line  - 1,7
		// second line - 1,5
		std::cout << "\nEnter the piece's starting position <x,y>: ";
		std::cin >> userInputStart;
		std::cout << "\nEnter the desired end position <x,y>: ";
		std::cin >> userInputEnd;

		//validate and move the piece to demo gameplay
		int startX = userInputStart[0] - '0' - 1; //convert the char into an int and subtract 1 so it can be used as an index value
		int startY = userInputStart[2] - '0' - 1;
		int endX = userInputEnd[0] - '0' - 1;
		int endY = userInputEnd[2] - '0' - 1;

		if (isValidPieceMovement(startX, startY, endX, endY)) //if piece movement is valid, move it
		{
			chessBoard[endY][endX] = chessBoard[startY][startX];
			chessBoard[startY][startX] = "";
		}
		drawBoard(); //update board
	}

	system("pause");
	return 0;
}

//initializeBoard function
//fills the chessBoard 2d array with the pieces at their starting positions
void initializeBoard()
{
	//######## STRING FORMAT FOR GAME PIECES ########
	//2 chars: [Char signifying piece's type] [Player number that owns the piece]

	//player 1 pieces
	chessBoard[0][0] = "R1";
	chessBoard[0][1] = "N1";
	chessBoard[0][2] = "B1";
	chessBoard[0][3] = "Q1";
	chessBoard[0][4] = "K1";
	chessBoard[0][5] = "B1";
	chessBoard[0][6] = "N1";
	chessBoard[0][7] = "R1";
	//player 1 pawns
	chessBoard[1][0] = "P1";
	chessBoard[1][1] = "P1";
	chessBoard[1][2] = "P1";
	chessBoard[1][3] = "P1";
	chessBoard[1][4] = "P1";
	chessBoard[1][5] = "P1";
	chessBoard[1][6] = "P1";
	chessBoard[1][7] = "P1";

	//reset all pieces inbetween (row index 2 to row index 5)
	for (int i = 2; i <= 5; i++)
	{
		for (int j = 0; j <= 7; j++)
			chessBoard[i][j] = "";
	}

	//player 2 pawns
	chessBoard[6][0] = "P2";
	chessBoard[6][1] = "P2";
	chessBoard[6][2] = "P2";
	chessBoard[6][3] = "P2";
	chessBoard[6][4] = "P2";
	chessBoard[6][5] = "P2";
	chessBoard[6][6] = "P2";
	chessBoard[6][7] = "P2";
	//player 2 pieces
	chessBoard[7][0] = "R2";
	chessBoard[7][1] = "N2";
	chessBoard[7][2] = "B2";
	chessBoard[7][3] = "Q2";
	chessBoard[7][4] = "K2";
	chessBoard[7][5] = "B2";
	chessBoard[7][6] = "N2";
	chessBoard[7][7] = "R2";
}

//drawBoard function
//outputs the boardgame with neat formatting to the console window
void drawBoard()
{
	system("cls"); //clear the screen

				   //identify each column with a letter and each row with a number
				   //uses * chars to define boarders
				   //leave 1 block space in every direction around the pieces
				   //uses chessBoard[x][y][char in the string to be accessed] to get the letter that represents the chess piece
	std::cout
		<< "\n"
		<< "      A     B     C     D     E     F     G     H\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 1 *  " << chessBoard[0][0][0] << "  *  " << chessBoard[0][1][0] << "  *  " << chessBoard[0][2][0] << "  *  " << chessBoard[0][3][0] << "  *  " << chessBoard[0][4][0] << "  *  " << chessBoard[0][5][0] << "  *  " << chessBoard[0][6][0] << "  *  " << chessBoard[0][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 2 *  " << chessBoard[1][0][0] << "  *  " << chessBoard[1][1][0] << "  *  " << chessBoard[1][2][0] << "  *  " << chessBoard[1][3][0] << "  *  " << chessBoard[1][4][0] << "  *  " << chessBoard[1][5][0] << "  *  " << chessBoard[1][6][0] << "  *  " << chessBoard[1][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 3 *  " << chessBoard[2][0][0] << "  *  " << chessBoard[2][1][0] << "  *  " << chessBoard[2][2][0] << "  *  " << chessBoard[2][3][0] << "  *  " << chessBoard[2][4][0] << "  *  " << chessBoard[2][5][0] << "  *  " << chessBoard[2][6][0] << "  *  " << chessBoard[2][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 4 *  " << chessBoard[3][0][0] << "  *  " << chessBoard[3][1][0] << "  *  " << chessBoard[3][2][0] << "  *  " << chessBoard[3][3][0] << "  *  " << chessBoard[3][4][0] << "  *  " << chessBoard[3][5][0] << "  *  " << chessBoard[3][6][0] << "  *  " << chessBoard[3][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 5 *  " << chessBoard[4][0][0] << "  *  " << chessBoard[4][1][0] << "  *  " << chessBoard[4][2][0] << "  *  " << chessBoard[4][3][0] << "  *  " << chessBoard[4][4][0] << "  *  " << chessBoard[4][5][0] << "  *  " << chessBoard[4][6][0] << "  *  " << chessBoard[4][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 6 *  " << chessBoard[5][0][0] << "  *  " << chessBoard[5][1][0] << "  *  " << chessBoard[5][2][0] << "  *  " << chessBoard[5][3][0] << "  *  " << chessBoard[5][4][0] << "  *  " << chessBoard[5][5][0] << "  *  " << chessBoard[5][6][0] << "  *  " << chessBoard[5][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 7 *  " << chessBoard[6][0][0] << "  *  " << chessBoard[6][1][0] << "  *  " << chessBoard[6][2][0] << "  *  " << chessBoard[6][3][0] << "  *  " << chessBoard[6][4][0] << "  *  " << chessBoard[6][5][0] << "  *  " << chessBoard[6][6][0] << "  *  " << chessBoard[6][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< " 8 *  " << chessBoard[7][0][0] << "  *  " << chessBoard[7][1][0] << "  *  " << chessBoard[7][2][0] << "  *  " << chessBoard[7][3][0] << "  *  " << chessBoard[7][4][0] << "  *  " << chessBoard[7][5][0] << "  *  " << chessBoard[7][6][0] << "  *  " << chessBoard[7][7][0] << "  *\n"
		<< "   *     *     *     *     *     *     *     *     *\n"
		<< "   *************************************************\n";
}

//isValidPieceMovement function
//sees if a piece can be moved from (startX, startY) to (destinationX, destinationY)
bool isValidPieceMovement(int startX, int startY, int destinationX, int destinationY)
{
	bool returnValue = false;
	char pieceType = chessBoard[startY][startX][0]; //hold the char that defines the piece's type

													//make sure the destination is either empty or isn't being occupied by a friendly piece. This rule applies to ALL pieces
	if (chessBoard[destinationY][destinationX] == "" || chessBoard[destinationY][destinationX][1] != chessBoard[startY][startX][1])
	{
		//check for pawn
		if (pieceType == 'P')
		{
			//check if pawn belongs to player 1
			if (chessBoard[startY][startX][1] == '1')
			{
				//if the destination is empty
				if (chessBoard[destinationY][destinationX] == "")
				{
					//check for moving straight down 1 square
					if (destinationY == startY + 1 && destinationX == startX)
					{
						returnValue = true; //valid movement
					}
					//check for moving straight down 2 squares
					else if (destinationY == startY + 2 && destinationX == startX)
					{
						//ensure pawn is at the spawn location (row 2 aka index 1), and that there are no pieces on the square that's being passed along the way
						if (startY == 1 && chessBoard[destinationY - 1][destinationX] == "")
						{
							returnValue = true; //valid movement
						}
					}
				}
				else //destination is not empty (there's a piece there)
				{
					//check for diagonal attack on another piece, either down and to the left one square or down and to the right one square
					if (destinationY == startY + 1 && destinationX == startX - 1 || destinationY == startY + 1 && destinationX == startX + 1)
					{
						//check if there's an enemy piece there (player 2's piece)
						if (chessBoard[destinationY][destinationX][1] == '2')
						{
							returnValue = true; //valid movement
						}
					}
				}
			}
			else //pawn belongs to player 2
			{
				//if the destination is empty
				if (chessBoard[destinationY][destinationX] == "")
				{
					//check for moving straight up 1 square
					if (destinationY == startY - 1 && destinationX == startX)
					{
						returnValue = true; //valid movement
					}
					//check for moving straight up 2 squares
					else if (destinationY == startY - 2 && destinationX == startX)
					{
						//ensure pawn is at the spawn location (row 7 aka index 6, and that there are no pieces on the square that's being passed along the way
						if (startY == 6 && chessBoard[destinationY + 1][destinationX] == "")
						{
							returnValue = true; //valid movement
						}
					}
				}
				else //destination is not empty (there's a piece there)
				{
					//check for diagonal attack on another piece, either up and to the left one square or up and to the right one square
					if (destinationY == startY - 1 && destinationX == startX - 1 || destinationY == startY - 1 && destinationX == startX + 1)
					{
						//check if there's an enemy piece there (player 1's piece)
						if (chessBoard[destinationY][destinationX][1] == '1')
						{
							returnValue = true; //valid movement
						}
					}
				}
			}
		}

		//check for rook
		else if (pieceType == 'R')
		{
			//check if the move is valid horizontally/vertically
			if (isValidHorizontalOrVerticalMove(startX, startY, destinationX, destinationY))
				returnValue = true;
		}

		//check for knight
		else if (pieceType == 'N')
		{
			//check for moving up 2 left 1, up 1 left 2, up 2 right 1, or up 1 right 2
			if (startX - 2 == destinationX && startY - 1 == destinationY || startX - 1 == destinationX && startY - 2 == destinationY || startX - 2 == destinationX && startY + 1 == destinationY || startX - 1 == destinationX && startY + 2 == destinationY)
				returnValue = true;
			//check for moving down 2 left 1, down 1 left 2, down 2 right 1, or down 1 right 2
			else if (startX + 2 == destinationX && startY - 1 == destinationY || startX + 1 == destinationX && startY - 2 == destinationY || startX + 2 == destinationX && startY + 1 == destinationY || startX + 1 == destinationX && startY + 2 == destinationY)
				returnValue = true;
		}

		//check for bishop
		else if (pieceType == 'B')
		{
			//check if the move is valid diagonally
			if (isValidDiagonalMove(startX, startY, destinationX, destinationY))
				returnValue = true;
		}

		//check for queen
		else if (pieceType == 'Q')
		{
			//check if the move is valid either horizontally/vertically OR diagonally
			if (isValidHorizontalOrVerticalMove(startX, startY, destinationX, destinationY) || isValidDiagonalMove(startX, startY, destinationX, destinationY))
				returnValue = true;
		}

		else //piece is a king
		{
			//check for left, right, up, or down movement
			if (startX - 1 == destinationX && startY == destinationY || startX + 1 == destinationX && startY == destinationY || startX == destinationX && startY - 1 == destinationY || startX == destinationX && startY + 1 == destinationY)
				returnValue = true;
			//check for diagonal up to the left or right and diagonal down to the left or right
			else if (startX - 1 == destinationX && startY - 1 == destinationY || startX + 1 == destinationX && startY - 1 == destinationY || startX - 1 == destinationX && startY + 1 == destinationY || startX + 1 == destinationX && startY + 1 == destinationY)
				returnValue = true;
		}
	}

	return returnValue;
}

//isValidHorizontalOrVerticalMove function
//does movement validation for rooks and queens' up, down, left, or right movement
bool isValidHorizontalOrVerticalMove(int startX, int startY, int destinationX, int destinationY)
{
	bool returnValue = false;

	int rowDifference = startX - destinationX; //set the difference in the X coordinates
	int colDifference = startY - destinationY; //set the difference in the Y coordinates

											   //make sure there is some movement, but not on both the x and y axis
	if (!(rowDifference == 0 && colDifference == 0) && !(rowDifference != 0 && colDifference != 0))
	{
		returnValue = true;

		//the piece is being moved to the left along a row
		if (rowDifference > 0)
		{
			//iterate through each square inbetween the start location and the end location
			for (int i = 1; i < rowDifference; i++)
			{
				//if there is a piece inbetween the start and end location
				if (chessBoard[destinationY][destinationX + i] != "")
					return false; //the piece cannot move through another piece
			}
		}
		//the piece is being moved to the right along a row
		else if (rowDifference < 0)
		{
			//iterate through each square inbetween the start location and the end location
			for (int i = -1; i > rowDifference; i--)
			{
				//if there is a piece inbetween the start and end location
				if (chessBoard[destinationY][destinationX + i] != "")
					return false; //the piece cannot move through another piece
			}
		}
		//the piece is being moved up along a column
		else if (colDifference > 0)
		{
			//iterate through each square inbetween the start location and the end location
			for (int i = 1; i < colDifference; i++)
			{
				//if there is a piece inbetween the start and end location
				if (chessBoard[destinationY + i][destinationX] != "")
					return false; //the piece cannot move through another piece
			}
		}
		//the piece is being moved down along a column
		else
		{
			//iterate through each square inbetween the start location and the end location
			for (int i = -1; i > colDifference; i--)
			{
				//if there is a piece inbetween the start and end location
				if (chessBoard[destinationY + i][destinationX] != "")
					return false; //the piece cannot move through another piece
			}
		}
	}

	return returnValue;
}

//isValidDiagonalMove function
//does movement validation for bishop and queens' diagonal movement
bool isValidDiagonalMove(int startX, int startY, int destinationX, int destinationY)
{
	bool returnValue = false;

	int rowDifference = startX - destinationX; //set the difference in the X coordinates
	int colDifference = startY - destinationY; //set the difference in the Y coordinates

											   //make sure there is some movement, and that the row and col differences are the same (take absolute value to ignore the sign)
	if (!(rowDifference == 0 && colDifference == 0) && abs(rowDifference) == abs(colDifference))
	{
		returnValue = true;

		//check for moving up and to the left
		if (colDifference > 0 && rowDifference > 0)
		{
			//iterate through each square between the start and end location
			for (int i = 1; i < colDifference; i++)
			{
				//if the space isn't empty, the move is invalid
				if (chessBoard[destinationY + i][destinationX + i] != "")
					return false;
			}
		}
		//check for moving up and to the right
		else if (colDifference > 0 && rowDifference < 0)
		{
			//iterate through each square between the start and end location
			for (int i = 1; i < colDifference; i++)
			{
				//if the space isn't empty, the move is invalid
				if (chessBoard[destinationY + i][destinationX - i] != "")
					return false;
			}
		}
		//check for moving down and to the left
		else if (colDifference < 0 && rowDifference > 0)
		{
			//iterate through each square between the start and end location
			for (int i = 1; i < abs(colDifference); i++)
			{
				//if the space isn't empty, the move is invalid
				if (chessBoard[destinationY - i][destinationX + i] != "")
					return false;
			}
		}
		//move is down and to the right
		else
		{
			//iterate through each square between the start and end location
			for (int i = 1; i < abs(colDifference); i++)
			{
				//if the space isn't empty, the move is invalid
				if (chessBoard[destinationY - i][destinationX - i] != "")
					return false;
			}
		}
	}

	return returnValue;
}