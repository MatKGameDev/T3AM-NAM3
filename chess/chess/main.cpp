// Filename:    main.cpp
// Author:      Group 8 - <T34M N4M3>: 
//               100591924 - Mathew Kostrzewa
//               100702629 - Evyn Brouwer
//               100701911 - Hersh Sheth
//               100637677 - Sherry Yang
//               100706090 - Thaidan Goguen-Bogdanis
// Date:        Oct 9, 2018
// Description: This code is used as the main cpp file for our chess game, developed for the GDW2 project in the Game Development Workshop course at UOIT.
//References: https://www.youtube.com/watch?v=8fCwUwI31Dk,

/*
* TODO (Incomplete list)
* Basic TODOs:
*  Add right click to move in the how-to
*  Implement checks for check/stalemate/checkmate
*  Castling
*
* If time permits (Advanced TODOs):
*  Sound FX/Music
*/

#include <iostream>
#include <string>
#include <Windows.h>
#include <regex>
#include <cwchar>
#include <cstdlib>

// conflict max() solution find stackoverflow.com/questions/20446373/cin-ignorenumeric-limitsstreamsizemax-n-max-not-recognize-it
#undef max

std::string chessBoard[8][8]; //a 2d array to represent the chess board, standard size is 8x8


//initializeBoard function prototype
//resets the logical chess board to place pieces at starting positions
void initializeBoard();

//drawBoard function prototype
//draws out a neatly formatted chess board with pieces dynamically placed
void drawBoard(bool validMoves[64]);

//howTo function prototype
//displays the menu for a guide on the rules of the game and the pieces
void howTo();

//getPieceType function prototype
//takes in a char and returns the name of that piece based on the char
std::string getPieceName(char pieceChar);

//convertNumberToLetterCoordinate function prototype
//takes in a number to be converted and changes it to the letter equivalent
char convertNumberToLetterCoordinate(int numberToConvert);

//isValidPieceMovement function prototype
//figures out if a piece is allowed to be moved to the user's destination
bool isValidPieceMovement(int startX, int startY, int destinationX, int destinationY);

//isValidHorizontalOrVerticalMove function prototype
//determines if a piece can be moved to the destination horizontally or vertically (includes collision detection)
bool isValidHorizontalOrVerticalMove(int startX, int startY, int destinationX, int destinationY);

//isValidDiagonalMove function prototype
//determines if a piece can be moved to the destination diagonally (includes collision detection)
bool isValidDiagonalMove(int startX, int startY, int destinationX, int destinationY);

//setKingCoordinates function prototype
//sets the x and y values for the king's position
void setKingCoordinates(char playerNum, int &kingX, int &kingY);

//isInCheck function prototype
//determines if the player's king is in check
bool isInCheck(char playerNum);

//isCheckmate function prototype
//checks if a player has been checkmate'd (or however you say that)
bool isCheckmate(char playerNum);

//canSaveKingMove function prototype
//checks if a player's king can move or be saved
bool canSaveKingMove(char playerNum);

//isInputPattern function prototype
//check user input correct format 1-8,1-8
bool isInputPattern(const std::string& input);

//isInputValid function prototype
//check both user input is valid and ask user input another x,y
void isInputValid(std::string &userInput, std::string msg);

//isValidStartP1/P2 function prototype
//check the start point for player one and two
void isValidStartP1(std::string &userInput, std::string msg);
void isValidStartP2(std::string &userInput, std::string msg);

//playGame function prototype
//allows two players to play chess against each other, or a player can choose to play against a computer
void playGame(bool isVersusComputer = false);

//performComputerTurn function prototype
//determines logic for a computer player's turn
void performComputerTurn(std::string &previousTurnAction);

//movePiece function prototype
//moves a piece from the start location to the end location
void movePiece(int startX, int startY, int destinationX, int destinationY);

//highlightValidMoves function prototype
//used for highlighting any squares that the player can move their piece, returns an array that corresponds with valid potential moves for the piece
void highlightValidMoves(bool *validSquares, int startX, int startY);

//toggleMusic function prototype
//toggles music on or off
//music taken from royalty free website: www.purple-planet.com/gentle
void toggleMusic();

//showMainMenu function prototype
//displays the main menu
void showMainMenu();

int main()
{
	srand(time(NULL)); //seed the random number generator

	showMainMenu(); //open the main menu

	return 0;
}

//initializeBoard function
//fills the chessBoard 2d array with the pieces at their starting positions
void initializeBoard()
{
	//######## STRING FORMAT FOR GAME PIECES ########
	//2 chars: [Char signifying piece's type] [Player number that owns the piece]

	//player 2 pieces
	chessBoard[0][0] = "R2";
	chessBoard[0][1] = "N2";
	chessBoard[0][2] = "B2";
	chessBoard[0][3] = "Q2";
	chessBoard[0][4] = "K2";
	chessBoard[0][5] = "B2";
	chessBoard[0][6] = "N2";
	chessBoard[0][7] = "R2";
	//player 2 pawns
	chessBoard[1][0] = "P2";
	chessBoard[1][1] = "P2";
	chessBoard[1][2] = "P2";
	chessBoard[1][3] = "P2";
	chessBoard[1][4] = "P2";
	chessBoard[1][5] = "P2";
	chessBoard[1][6] = "P2";
	chessBoard[1][7] = "P2";

	//reset all pieces inbetween (row index 2 to row index 5)
	for (int i = 2; i <= 5; i++)
	{
		for (int j = 0; j <= 7; j++)
			chessBoard[i][j] = "";
	}

	//player 1 pawns
	chessBoard[6][0] = "P1";
	chessBoard[6][1] = "P1";
	chessBoard[6][2] = "P1";
	chessBoard[6][3] = "P1";
	chessBoard[6][4] = "P1";
	chessBoard[6][5] = "P1";
	chessBoard[6][6] = "P1";
	chessBoard[6][7] = "P1";
	//player 1 pieces	 
	chessBoard[7][0] = "R1";
	chessBoard[7][1] = "N1";
	chessBoard[7][2] = "B1";
	chessBoard[7][3] = "Q1";
	chessBoard[7][4] = "K1";
	chessBoard[7][5] = "B1";
	chessBoard[7][6] = "N1";
	chessBoard[7][7] = "R1";
}

//drawBoard function
//outputs the boardgame with neat formatting to the console window
void drawBoard(bool validMoves[64])
{
	system("cls"); //clear the screen

	//identify each column with a letter and each row with a number
	//uses * chars to define boarders
	//leave 1 block space in every direction around the pieces
	//uses chessBoard[x][y][char in the string to be accessed] to get the letter that represents the chess piece

	std::cout << "\n" << "      A     B     C     D     E     F     G     H\n";
	for (int i = 0; i < 33; i++)//for the size of the board
	{
		if (i % 4 == 0) //prints out the horiztonal border
		{
			std::cout << "   *************************************************\n";
		}
		if (i % 4 == 1 || i % 4 == 3) //print out the vertical border
		{
			std::cout << "   *";
				//<<"     *     *     *     *     *     *     *     *\n";
			for (int n = 0;n < 8;n++)
			{
				if (validMoves[((i / 4) * 8) + n])
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
				}
				std::cout << "     ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
				std::cout << "*";
			}
			std::cout << "\n";
		}
		if (i % 4 == 2)//print out the pieces 
		{
			std::cout << " "<< i / 4 + 1 << " *";//print the row identifier
			for (int n = 0; n < 15; n++)//print out the row of pieces
			{
				if (n % 2 == 0)//if a piece belongs here
				{
					if (validMoves[((i / 4) * 8) + n / 2])
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
					}
					std::cout << "  ";
					if (chessBoard[i / 4][n / 2] != "")//validation to make sure the piece exists
					{
						//check if piece belongs to player 1
						if (chessBoard[i / 4][n / 2][1] == '1')
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 241);//set the text to blue
						}
						//piece belongs to player 2
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244);//set the text red
						}
						if (validMoves[((i / 4) * 8) + n/2])
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
						}
						std::cout << chessBoard[i / 4][n / 2][0];//print out the piece type
					}
					
					else//if empty
					{
						if (validMoves[((i / 4) * 8) + n / 2])
						{
							//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
						}
						std::cout << chessBoard[i / 4][n / 2][0];//print out a space
					}
				}
				else
				{
					std::cout << "  "; //print a piece of the vertical border
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
					std::cout << "*";
				}
				if (n == 14)
				{
					if (validMoves[((i / 4) * 8) + n / 2])
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
					}
					std::cout << "  ";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
					std::cout << "*";
				}
			}
			std::cout << "\n"; //the end of the row
		}
	}
}

bool isEnd = false;

//howTo function
//displays the guide menu with various options to choose from
void howTo()
{
	std::string response; //uses char to avoid some errors that happen when the user enters invalid numbers in an int

	while (!isEnd)
	{
		system("cls"); //clear screen every time it loops

		//used the following thread for help with resizing the console window: stackoverflow.com/questions/21238806/how-to-set-output-console-width-in-visual-studio
		HWND console = GetConsoleWindow();
		MoveWindow(console, 500, 200, 520, 500, TRUE); //startX, startY, width, height - int params for the console window

		std::cout << "Enter the number of whichever topic you would like to\nlearn about.\n\n"
				  << "1.  Terminology\n2.  Pawns\n3.  Rooks\n4.  Knights\n5.  Bishops\n6.  Queen\n7.  King\n8.  General rules\n9.  Player 1 & 2 rules\n10. Castling\n11. Return to main menu.\n";
		std::cout << "\n\nEnter your selection: ";
		std::cin >> response;

		system("cls"); //clear screen every time the user selects something, to look less cluttered

		//formatting for the couts in the if statements lets the program neatly display each line of text on one line

		if (response == "1")
		{
			std::cout << "Piece:     a piece is any of the letters on the board.\n";
			std::cout << "\nTile:      a tile is a space where a piece can move.\n";
			std::cout << "\nCapture:   a piece is captured when another piece lands on\n";
			std::cout << "           its tile. Can also be called \'taking.\'\n";
			std::cout << "\nAttack:    a tile is being attacked if another piece could\n";
			std::cout << "           capture a piece on that tile.\n";
			std::cout << "\nCheck:     the king is in check if it can be captured on\n";
			std::cout << "           the next turn.\n";
			std::cout << "\nCheckmate: the game results in a checkmate if the king\n";
			std::cout << "           cannot move out of check, and the piece keeping\n";
			std::cout << "           it in check cannot be captured.\n";
			std::cout << "\nStalemate: the game results in a stalemate if the player\n";
			std::cout << "           whose turn it is to move is not in check but\n";
			std::cout << "           cannot move without moving into check.";
		}
		else if (response == "2")
		{
			std::cout << "Pawns, represented by a P, can move two spaces directly\n";
			std::cout << "forwards on their first turn, and otherwise can move\n";
			std::cout << "either one space forward to an empty space, or diagonally\n";
			std::cout << "forwards one space, but only to capture another piece.\n";
			std::cout << "Pawns may also become queens by reaching the enemy team's\n";
			std::cout << "row.";
		}
		else if (response == "3")
		{
			std::cout << "Rooks, represented by an R, can only move directly\n";
			std::cout << "horizontally or vertically any amount of spaces, as long\n";
			std::cout << "as there are no pieces in between the start and end.";
		}
		else if (response == "4")
		{
			std::cout << "Knights, represented by an N, can move two spaces \n";
			std::cout << "vertically or horizontally and one space perpendicular\n";
			std::cout << "to that.";
		}
		else if (response == "5")
		{
			std::cout << "Bishops, represented by a B, can move diagonally any\n";
			std::cout << "number of spaces, as long as there are no pieces in\n";
			std::cout << "between the start and end of its move.";
		}
		else if (response == "6")
		{
			std::cout << "Queens, represented by a Q, can move any number of spaces\n";
			std::cout << "horizontally, vertically, or diagonally, as long as there\n";
			std::cout << "are no pieces in between the start and end.";
		}
		else if (response == "7")
		{
			std::cout << "The king, represented by a K, can move one space in any\n";
			std::cout << "direction. The game is won by capturing the opponent\'s\n";
			std::cout << "king, or lost when yours is captured.";
		}
		else if (response == "8")
		{
			std::cout << "Players can capture the opponent\'s game pieces by moving\n";
			std::cout << "their own piece onto the same tile as the opponent\'s\n";
			std::cout << "piece. A player wins when they capture the opponent\'s\n";
			std::cout << "king, resulting in a checkmate. The only other outcome is\n";
			std::cout << "a stalemate, where neither player can possibly capture\n";
			std::cout << "the other\'s king.";
		}
		else if (response == "9")
		{
			std::cout << "Player 1\'s pieces are represented by the blue letters.\n";
			std::cout << "Player 2\'s pieces are represented by the red letters.\n";
			std::cout << "Player 1 starts at the top, and always moves first.";
		}
		else if (response == "10")
		{
			std::cout << "Castling is a special move that a king can do.\n";
			std::cout << "\nThe king can castle to the left or right. To castle, the\n";
			std::cout << "player moves their king two spaces to the right or left,\n";
			std::cout << "and the rook is moved to the opposite side of the king.\n";
			std::cout << "\n\nRestrictions:\n";
			std::cout << "\nThe king cannot castle if it has moved, or if the rook it\n";
			std::cout << "is castling has moved.\n";
			std::cout << "\nThe king cannot castle out of check.\n";
			std::cout << "\nThe king cannot castle into check.\n";
			std::cout << "\nThe king cannot castle if their are any pieces between it\n";
			std::cout << "and the rook it is using to castle.\n";
			std::cout << "\nThe king cannot castle  across a space that is being\n";
			std::cout << "attacked.";
		}
		else if (response == "11")
		{
			isEnd = true;
			showMainMenu();
		}
		else
		{
			std::cout << "That is not a valid number."; //displays this if the user enters anything other than the above values for response
		}

		std::cout << "\n\n\n"; //makes sure the prompt from system("pause") is on a new line

		system("pause"); //pauses the program so that the user can read their desired text before going back to the howTo menu
	}

	showMainMenu();
}

//getPieceName function
//converts a char into a string to represent a piece
std::string getPieceName(char pieceChar)
{
	std::string pieceName = ""; //name of the piece to return

	//determine piece name based on the char
	if (pieceChar == 'P')
		pieceName = "Pawn";
	else if (pieceChar == 'R')
		pieceName = "Rook";
	else if (pieceChar == 'N')
		pieceName = "Knight";
	else if (pieceChar == 'B')
		pieceName = "Bishop";
	else if (pieceChar == 'Q')
		pieceName = "Queen";
	else if (pieceChar == 'K')
		pieceName = "King";

	return pieceName;
}

//convertNumberToLetterCoordinate function
//takes in a number to be converted and changes it to the letter equivalent
char convertNumberToLetterCoordinate(int numberToConvert)
{
	char convertedNumber = 'Z'; //the number to be converted, initialized to something invalid for error checking

	//convert 1-8 to A-H
	if (numberToConvert == 1)
		convertedNumber = 'A';
	else if (numberToConvert == 2)
		convertedNumber = 'B';
	else if (numberToConvert == 3)
		convertedNumber = 'C';
	else if (numberToConvert == 4)
		convertedNumber = 'D';
	else if (numberToConvert == 5)
		convertedNumber = 'E';
	else if (numberToConvert == 6)
		convertedNumber = 'F';
	else if (numberToConvert == 7)
		convertedNumber = 'G';
	else if (numberToConvert == 8)
		convertedNumber = 'H';

	return convertedNumber; //return the converted number as a char
}

//isValidPieceMovement function
//sees if a piece can be moved from (startX, startY) to (destinationX, destinationY)
bool isValidPieceMovement(int startX, int startY, int destinationX, int destinationY)
{
	//if the start position isn't a piece
	if (chessBoard[startY][startX] == "")
		return false;
	//if the destination is off the board
	if (destinationX > 7 || destinationX < 0 || destinationY > 7 || destinationY < 0)
		return false;

	bool returnValue = false;
	char pieceType = chessBoard[startY][startX][0]; //hold the char that defines the piece's type

	//make sure start and end coordinates are in valid range
	if (startX > -1 && startX < 8 && startY > -1 && startY < 8 && destinationX > -1 && destinationX < 8 && destinationY > -1 && destinationY < 8)
	{
		//make sure the destination is either empty or isn't being occupied by a friendly piece. This rule applies to ALL pieces
		if (chessBoard[destinationY][destinationX] == "" || chessBoard[destinationY][destinationX][1] != chessBoard[startY][startX][1])
		{
			//check for pawn
			if (pieceType == 'P')
			{
				//check if pawn belongs to player 2
				if (chessBoard[startY][startX][1] == '2')
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
							//check if there's an enemy piece there (player 1's piece)
							if (chessBoard[destinationY][destinationX][1] == '1')
							{
								returnValue = true; //valid movement
							}
						}
					}
				}

				else //pawn belongs to player 1
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
							//check if there's an enemy piece there (player 2's piece)
							if (chessBoard[destinationY][destinationX][1] == '2')
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
	}

	//see if the king is in check first
	if (returnValue)
	{
		std::string desinationPiece = chessBoard[destinationY][destinationX]; //stores the piece at the destination

		chessBoard[destinationY][destinationX] = chessBoard[startY][startX]; //move piece to the end position
		chessBoard[startY][startX] = ""; //clear the old start position

		//check if the move will put the player into check
		if (isInCheck(chessBoard[destinationY][destinationX][1]))
			returnValue = false; //invalid movement

		chessBoard[startY][startX] = chessBoard[destinationY][destinationX]; //move piece back to start position
		chessBoard[destinationY][destinationX] = desinationPiece; //reset the old desination
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

//setKingCoordinates function prototype
//sets the x and y values for the king's position
void setKingCoordinates(char playerNum, int &kingX, int &kingY)
{
	//loop through each row
	for (int i = 0; i < 8; i++)
	{
		//loop through each column
		for (int j = 0; j < 8; j++)
		{
			//if the king is at the current square
			if (chessBoard[i][j] == ("K" + std::string(1, playerNum)))
			{
				//set the x and y values for the king
				kingX = j;
				kingY = i;
				//stop looping
				j = 8;
				i = 8;
			}
		}
	}
}

//isInCheck function prototype
//determines if the player's king is in check
bool isInCheck(char playerNum)
{
	//set the king's x and y coordinates
	int kingX;
	int kingY;
	setKingCoordinates(playerNum, kingX, kingY);

	//loop through each row
	for (int k = 0; k < 8; k++)
	{
		//loop through each column
		for (int m = 0; m < 8; m++)
		{
			//check if any pieces on the board can attack the king
			if (isValidPieceMovement(k, m, kingX, kingY))
				return true;
		}
	}

	return false;
}

//isCheckmate function
//checks if a player has been checkmate'd (or however you say that)
bool isCheckmate(char playerNum)
{
	//if the king is in check and cannot move anywhere
	if (isInCheck(playerNum) && !canSaveKingMove(playerNum))
		return true;
	else
		return false;
}

//canSaveKingMove function
//checks if a player's king can move or be saved
bool canSaveKingMove(char playerNum)
{
	//set the king's x and y coordinates
	int kingX;
	int kingY;
	setKingCoordinates(playerNum, kingX, kingY);

	//check if king can move up and to the left, straight up, or up and to the right
	if (isValidPieceMovement(kingX, kingY, kingX - 1, kingY + 1) || isValidPieceMovement(kingX, kingY, kingX, kingY + 1) || isValidPieceMovement(kingX, kingY, kingX + 1, kingY + 1))
		return true;
	//check if king can move down and to the left, straight down, or down and to the right
	else if (isValidPieceMovement(kingX, kingY, kingX - 1, kingY - 1) || isValidPieceMovement(kingX, kingY, kingX, kingY - 1) || isValidPieceMovement(kingX, kingY, kingX + 1, kingY - 1))
		return true;
	//check if king can move left or right
	else if (isValidPieceMovement(kingX, kingY, kingX - 1, kingY) || isValidPieceMovement(kingX, kingY, kingX + 1, kingY))
		return true;
	else //no valid movement options
	{
		//loop through each row
		for (int i = 0; i < 8; i++)
		{
			//loop through each column
			for (int j = 0; j < 8; j++)
			{
				//if there is a friendly piece at the square, check if it has any valid movement options to protect the king
				if (chessBoard[i][j] != "" && chessBoard[i][j][1] == playerNum)
				{
					//loop through each row
					for (int k = 0; k < 8; k++)
					{
						//loop through each column
						for (int m = 0; m < 8; m++)
						{
							//if the piece has any valid moves to get the king out of check
							if (isValidPieceMovement(j, i, m, k))
								return true;
						}
					}
				}
			}
		}
	}

	return false; //no valid options to save king
}

//isInputPattern function use regular expression to 
//determine the user input is correct format/pattern reference: www.newthinktank.com/2018/06/c-tutorial-19-2/
bool isInputPattern(const std::string& input)
{
	// define a regular expression
	const std::regex pattern
	("\\b[1-8],[1-8]\\b");

	// try to match the string with the regular expression
	return std::regex_match(input, pattern);
}

//isInputValid function run while loop to clear user input 
//and ask for valid Input from user
void isInputValid(std::string &userInput, std::string msg)
{
	while (isInputPattern(userInput) == false) 
	{
		std::cout << " Invalid Input \n";
		std::cin.clear(); // reset cin for next input
						  // ignore the user input, passing in the maximize size a user could input to clear
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << msg;
		std::cin >> userInput;// ask user input the valid x,y
	}
}

// check there is piece for player one to start
void isValidStartP1(std::string &userInput, std::string msg)
{
	bool valid = false;

	while (valid == false) 
	{
		// check for start point
		int startX = userInput[0] - '0' - 1; //convert the char into an int and subtract 1 so it can be used as an index value
		int startY = userInput[2] - '0' - 1;

		// the start piece is not empty
		if (chessBoard[startY][startX] != "" && chessBoard[startY][startX][1] == '1') 
			valid = false;
		else 
			valid = false;

		if (valid == false) 
		{
			std::cout << " Invalid Start Piece \n";
			std::cin.clear(); // reset cin for next input
							  // ignore the user input, passing in the maximize size a user could input to clear
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << msg;
			std::cin >> userInput;// ask user input the valid x,y
		}
	}
}

// check there is piece for player two to start
void isValidStartP2(std::string &userInput, std::string msg)
{
	bool valid = false;

	while (valid == false) 
	{
		// check for start point
		int startX = userInput[0] - '0' - 1; //convert the char into an int and subtract 1 so it can be used as an index value
		int startY = userInput[2] - '0' - 1;

		// the start piece is not empty
		if (chessBoard[startY][startX] != "" && chessBoard[startY][startX][1] == '2')
			valid = false;
		else
			valid = false;

		if (valid == false) 
		{
			std::cout << " Invalid Start Piece \n";
			std::cin.clear(); // reset cin for next input
							  // ignore the user input, passing in the maximize size a user could input to clear
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << msg;
			std::cin >> userInput;// ask user input the valid x,y
		}
	}
}

//playGame function
//performs all actions that allow a user to play against another player, or against a computer player
void playGame(bool isVersusComputer)
{
	//used the following thread for help with resizing the console window: stackoverflow.com/questions/21238806/how-to-set-output-console-width-in-visual-studio
	HWND console = GetConsoleWindow();
	MoveWindow(console, 600, 200, 500, 720, TRUE); //startX, startY, width, height - int params for the console 

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

	std::string userInputStart;      //user's input for the start location of the piece
	std::string userInputEnd;        //user's input for the desired end location of the piece
	int playerNumber = 1;            //an int that is either 1 or 2, which determines which player's move it is
	int defendingPlayer;             //holds the number of the defending player
	bool validMoves[64] = { false }; //an array where each true value determines a spot that a selected piece can be moved
	std::string previousTurnAction = " Game started."; //a small description of the previous turn's action

	initializeBoard();     //reset the chess board
	drawBoard(validMoves); //display the chess board to the user

	//declare all variables for holding x and y values
	int tempX;
	int tempY;
	int startX = -1;
	int startY = -1;
	int endX;
	int endY;

	std::cout << "\n" << previousTurnAction << "\n\n"; //output a description of the previous turn's action

	//display whos turn it is
	if (isVersusComputer)
		std::cout << " Your turn.";
	else
		std::cout << " Player " << std::to_string(playerNumber) << "'s turn.";

	while (1)
	{
		//declare and set the cursor position to get the x and y values (in pixels)
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(GetConsoleWindow(), &cursorPos);

		//if a spot on the chess board was clicked
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			//store the x and y coordinates of the position on the chess board that was clicked
			//convert the x and y from pixles to coordinates, using the width as x and height as y of each square
			tempX = (cursorPos.x - 26) / 48; //26 is the pixels between the left side of console window and the left side of the board. 48 is the width of each square
			tempY = (cursorPos.y - 46) / 63; //48 is the pixels between the top of the console window and the top of the board. 63 is the height of each square

			//if chess board coordinates' start positions arent set OR the player clicked on a friendly piece (convert player number to char to compare)
			if (chessBoard[tempY][tempX] != "" && chessBoard[tempY][tempX][1] == ('0' + playerNumber))
			{
				//set x and y coordinates for chess board start positions based on the cursor x and y positions
				startX = tempX;
				startY = tempY;

				//update the board to show the valid movement options for the selected piece
				highlightValidMoves(validMoves, startX, startY);

				drawBoard(validMoves);
				std::cout << "\n" << previousTurnAction << "\n"; //output a description of the previous turn's action
   				//if the player is against the computer
				if (isVersusComputer)
				{
					std::cout << " Your turn.";
				}
				else //player vs player
				{
					std::cout << " Player " << std::to_string(playerNumber) << "'s turn."; //display which player's turn it is
				}
				//display if the player is in check
				if (isInCheck('0' + playerNumber))
					std::cout << "\n Your king is in check!\n";
			}
			//if chess board coordinates' start positions are set
			else if (startX >= 0 && startY >= 0)
			{
				//set x and y coordinates for chess board end positions based on the cursor x and y positions
				endX = tempX;
				endY = tempY;

				//if piece movement is valid, move it
				if (isValidPieceMovement(startX, startY, endX, endY))
				{
					//reset all squares to invalid movement options till next turn
					for (int i = 0; i < 64; i++)
						validMoves[i] = false;

					//update the previous turn's action
					if (isVersusComputer)
						previousTurnAction = " You";
					else
						previousTurnAction = " Player " + std::to_string(playerNumber);

					previousTurnAction += " moved " + getPieceName(chessBoard[startY][startX][0]) +
						" from " + convertNumberToLetterCoordinate(startX + 1) + std::to_string(startY + 1) + " to " +
						convertNumberToLetterCoordinate(endX + 1) + std::to_string(endY + 1);

					//check if end location has an enemy piece
					if (chessBoard[endY][endX] != "")
						previousTurnAction += "\n And took the enemy's " + getPieceName(chessBoard[endY][endX][0]);
					previousTurnAction += ".";

					//move the piece
					movePiece(startX, startY, endX, endY);
					drawBoard(validMoves); //update board
					std::cout << "\n" << previousTurnAction << "\n"; //output a description of the previous turn's action

					//check which player is defending
					if (playerNumber == 1)
						defendingPlayer = 2;
					else
						defendingPlayer = 1;
					//check for checkmate
					if (isCheckmate('0' + defendingPlayer))
					{
						//display win message and exit the game
						if (isVersusComputer)
							std::cout << "\n CHECKMATE! You won the game!\n ";
						else
							std::cout << "\n CHECKMATE! Player " << playerNumber << " won the game!\n ";
						system("pause");
						break;
					}

					//if the player is against the computer
					if (isVersusComputer)
					{
						//display computer player messages
						std::cout << " Computer player's turn.";
						if (isInCheck('0' + defendingPlayer))
							std::cout << "\n The computer's king is in check!\n";

						//pause and then do the computer's move
						Sleep(1500);
						performComputerTurn(previousTurnAction); //determine the computer player's action
						drawBoard(validMoves); //update board
						std::cout << "\n" << previousTurnAction << "\n"; //output a description of the previous turn's action
						std::cout << " Your turn."; //tell the user it's their turn

						//check for checkmate
						if (isCheckmate('0' + playerNumber))
						{
							std::cout << "\n CHECKMATE! Computer player won the game!\n ";
							system("pause");
							break;
						}
					}
					else //player vs player
					{
						//change which player's turn it is
						if (playerNumber == 1)
							playerNumber = 2;
						else
							playerNumber = 1;

						std::cout << " Player " << std::to_string(playerNumber) << "'s turn."; //display which player's turn it is
					}

					if (isInCheck('0' + playerNumber))
						std::cout << "\n Your king is in check!\n";
						
					//reset start x and y
					startX = -1;
					startY = -1;
				}
			}

			Sleep(50); //pause for 50ms (run at 20fps)
		}
	}

	showMainMenu();
}

//performComputerTurn function
//determines an action for the computer to perform on its turn
void performComputerTurn(std::string &previousTurnAction)
{
	const int MAXIMUM_CHECKS = 20;

	bool validMoveSelected;  //loop condition for the entire computer's turn
	bool validPieceSelected; //loop condition for selecting a starting position for a piece

	int startXIndex;   //starting x value for the piece to be moved
	int startYIndex;   //starting y value for the piece to be moved
	int tempEndXIndex; //stores a temporary ending x value before checking if the move is valid
	int tempEndYIndex; //stores a temporary ending y value before checking if the move is valid
	int endXIndex;	   //ending x value for the piece to move to
	int endYIndex;	   //ending y value for the piece to move to

	for (int i = 0; i < MAXIMUM_CHECKS; i++)
	{
		validMoveSelected = false;
		//loop until there is a valid move generated
		while (!validMoveSelected)
		{
			validPieceSelected = false;
			//loop until a valid piece is selected
			while (!validPieceSelected)
			{
				//get random start x index between 0 and 7
				startXIndex = rand() % 8;
				//get random start y index between 0 and 7
				startYIndex = rand() % 8;

				//check if the piece belongs to the computer player
				if (chessBoard[startYIndex][startXIndex] != "" && chessBoard[startYIndex][startXIndex][1] == '2')
					validPieceSelected = true;
			}

			//set the indexes to -1 temporarily so we can check if they were set later
			endXIndex = -1;
			endYIndex = -1;

			//if a pawn was selected
			if (chessBoard[startYIndex][startXIndex][0] == 'P')
			{
				//check if it can attack down and to the left
				if (isValidPieceMovement(startXIndex, startYIndex, startXIndex - 1, startYIndex + 1))
				{
					endXIndex = startXIndex - 1;
					endYIndex = startYIndex + 1;
					validMoveSelected = true;
					i = MAXIMUM_CHECKS; //prioritize attacking
				}
				//check if it can attack down and to the right
				else if (isValidPieceMovement(startXIndex, startYIndex, startXIndex + 1, startYIndex + 1))
				{
					endXIndex = startXIndex + 1;
					endYIndex = startYIndex + 1;
					validMoveSelected = true;
					i = MAXIMUM_CHECKS; //prioritize attacking
				}
				//can't attack, find a spot to move it
				else
				{
					//try to move it 2 spaces down
					if (isValidPieceMovement(startXIndex, startYIndex, startXIndex, startYIndex + 2))
					{
						endXIndex = startXIndex;
						endYIndex = startYIndex + 2;
						validMoveSelected = true;
					}
					//try to move it 1 space down
					else if (isValidPieceMovement(startXIndex, startYIndex, startXIndex, startYIndex + 1))
					{
						endXIndex = startXIndex;
						endYIndex = startYIndex + 1;
						validMoveSelected = true;
					}
				}
			}
			//piece is not a pawn, try to move it randomly, prioritizing attacking
			else
			{
				//loop 80 times checking for a valid move
				for (int j = 0; j < 80; j++)
				{
					//set a random x and y index between 0 and 7
					tempEndXIndex = rand() % 8;
					tempEndYIndex = rand() % 8;

					//if the randomly selected destination is a valid move
					if (isValidPieceMovement(startXIndex, startYIndex, tempEndXIndex, tempEndYIndex))
					{
						//set the end indexes
						endXIndex = tempEndXIndex;
						endYIndex = tempEndYIndex;

						//if the move is also attacking another piece
						if (chessBoard[endYIndex][endXIndex] != "")
						{
							i = MAXIMUM_CHECKS; //do not check for any more pieces
							break; //do not look for any more destination possibilities, we want to prioritize attacking
						}
					}
				}
			}

			//if there was a valid move selected
			if (endXIndex > -1 && endYIndex > -1)
			{
				validMoveSelected = true; //do not loop anymore
			}
		}
	}

	//update the previous turn's action
	previousTurnAction = " Computer player moved " + getPieceName(chessBoard[startYIndex][startXIndex][0]) +
		" from " + convertNumberToLetterCoordinate(startXIndex + 1) + std::to_string(startYIndex + 1) + " to " +
		convertNumberToLetterCoordinate(endXIndex + 1) + std::to_string(endYIndex + 1);
	//check if end location has an enemy piece
	if (chessBoard[endYIndex][endXIndex] != "")
		previousTurnAction += "\n And took your " + getPieceName(chessBoard[endYIndex][endXIndex][0]);
	previousTurnAction += ".";

	//perform the move
	movePiece(startXIndex, startYIndex, endXIndex, endYIndex);
}

//movePiece function
//moves a piece from the start to the end location and clears the old start location
void movePiece(int startX, int startY, int destinationX, int destinationY)
{
	chessBoard[destinationY][destinationX] = chessBoard[startY][startX]; //move piece to the end position
	chessBoard[startY][startX] = ""; //clear the old start position

	//check if pawn reached the end of the enemy's board
	//check for player 1's pawn
	if (destinationY == 0 && chessBoard[destinationY][destinationX][0] == 'P' && chessBoard[destinationY][destinationX][1] == '1')
		chessBoard[destinationY][destinationX][0] = 'Q'; //change pawn to a queen
	//check for player 2's pawn
	else if (destinationY == 7 && chessBoard[destinationY][destinationX][0] == 'P' && chessBoard[destinationY][destinationX][1] == '2')
		chessBoard[destinationY][destinationX][0] = 'Q'; //change pawn to a queen
}

//highlightValidMoves function
//used for highlighting any squares that the player can move their piece returns an array that corresponds with valid potential moves for the piece
void highlightValidMoves(bool *validSquares, int startX, int startY)
{
	//loop through each row
	for (int i = 0; i < 8; i++)
	{
		//loop through each column
		for (int j = 0; j < 8; j++)
		{
			//check if the spot is a valid position for the piece to move
			if (isValidPieceMovement(startX, startY, j, i))
				validSquares[i * 8 + j] = true; //set the square to be a valid movement 
			else //spot is not a valid position
				validSquares[i * 8 + j] = false;
		}
	}
}

//toggleMusic function
//toggles music on or off
void toggleMusic()
{
	//mciSendString("open \"*.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	//mciSendString("play mp3 repeat", NULL, 0, NULL);
}

//showMainMenu function prototype
//displays the main menu
void showMainMenu()
{
	system("cls");
	bool isDone = false;

	//used the following thread for help with resizing the console window: stackoverflow.com/questions/21238806/how-to-set-output-console-width-in-visual-studio
	HWND console = GetConsoleWindow();
	MoveWindow(console, 250, 80, 1300, 880, TRUE); //startX, startY, width, height - int params for the console window

	//display "MAIN MENU"
	std::cout.width(89);
	std::cout << "__    _           _             __   _ " << std::endl;
	std::cout.width(90);
	std::cout << "| \\  / |         (_)           |  \\ / | " << std::endl;
	std::cout.width(112);
	std::cout << "| \\  / |   __ _   _   _ __     | \\  / |   ___   _ __    _   _ " << std::endl;
	std::cout.width(113);
	std::cout << "| |\\/| |  / _` | | | | '_ \\    | |\\/| |  / _ \\ | '_ \\  | | | | " << std::endl;
	std::cout.width(113);
	std::cout << "| |  | | | (_| | | | | | | |   | |  | | |  __/ | | | | | |_| | " << std::endl;
	std::cout.width(116);
	std::cout << "|_|  |_| \\__, _| |_| |_| |_|   |_|  |_| \\___|  |_| |_| \\__, _| \n\n\n" << std::endl;

	//display "CHESS"
	std::cout.width(130);
	std::cout << ".----------------.  .----------------.  .----------------.  .----------------.  .----------------." << std::endl;
	std::cout.width(130);
	std::cout << "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |" << std::endl;
	std::cout.width(130);
	std::cout << "| |     ______   | || |  ____  ____  | || |  _________   | || |    _______   | || |    _______   | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |   .' ___  |  | || | |_   ||   _| | || | |_   ___  |  | || |   /  ___  |  | || |   /  ___  |  | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |  / .'   \\_|  | || |   | |__| |   | || |   | |_  \\_|  | || |  |  (__ \\_|  | || |  |  (__ \\_|  | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |  | |         | || |   |  __  |   | || |   |  _ | _   | || |   '.___`-.   | || |   '.___`-.   | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |  \\ `.___.'\\  | || |  _| |  | |_  | || |  _| |___/ |  | || |  |`\\____) |  | || |  |`\\____) |  | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |   `._____.'  | || | |____||____| | || | |_________|  | || |  |_______.'  | || |  |_______.'  | |" << std::endl;
	std::cout.width(130);
	std::cout << "| |              | || |              | || |              | || |              | || |              | |" << std::endl;
	std::cout.width(130);
	std::cout << "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |" << std::endl;
	std::cout.width(132);
	std::cout << "'----------------'  '----------------'  '----------------'  '----------------'  '----------------' \n\n" << std::endl;

	//display all menu options
	std::cout.width(98);
	std::cout << "Enter one of the following options:\n\n\n";
	std::cout.width(90);
	std::cout << "|| [1] How To Play || \n\n\n" << std::endl;
	std::cout.width(93);
	std::cout << " || [2] Player vs Player || \n\n\n" << std::endl;
	std::cout.width(94);
	std::cout << " || [3] Player vs Computer || \n\n\n" << std::endl;
	std::cout.width(90);
	std::cout << "|| [4] Quit || \n\n\n\n\n" << std::endl;

	//display our names :D
	std::cout.width(130);
	std::cout << "Hersh Sheth, 100701911" << std::endl;
	std::cout.width(130);
	std::cout << "Evyn Brouwer, 100702629" << std::endl;
	std::cout.width(130);
	std::cout << "Sherry Yang, 100637677" << std::endl;
	std::cout.width(130);
	std::cout << "Thaidan Goguen-Bogdanis, 100706090" << std::endl;
	std::cout.width(130);
	std::cout << "Mathew Kostrzewa, 100591924" << std::endl;

	system("color 6");

	char key;
	
	while (!isDone)
	{
		//GetAsyncKeyState checks for a key press and the "0x8000" checks if the key is being pressed down (otherwise it would always return true after the first keypress)
		//thanks to this thread for help: https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
		//check for 1 (how to play)
		if (isEvent(Events::One)) {
			std::cin >> key;
			if (key == '1') {
				isEnd = false;
			}
			howTo();
		}
		//check for 2 (player vs player)
		else if (GetAsyncKeyState('2') & 0x8000)
			playGame();

		//check for 3 (player vs computer)
		else if (GetAsyncKeyState('3') & 0x8000)
			playGame(true);

		//check for 4 (player wants to exit)
		else if (GetAsyncKeyState('4') & 0x8000)
			isDone = true;

		else
			Sleep(50);
	}
}