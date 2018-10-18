// Filename:    main.cpp
// Author:      Group 8 - <T34M N4M3>: 
//               100591924 - Mathew Kostrzewa
//               100702629 - Evyn Brouwer
//               100701911 - Hersh Sheth
//               100637677 - Sherry Yang
//               100706090 - Thaidan Goguen-Bogdanis
// Date:        Oct 9, 2018
// Description: This code is used as the main cpp file for our chess game, developed for the GDW2 project in the Game Development Workshop course at UOIT.

/*
* TODO (Incomplete list)
* Basic TODOs:
*  Validate player's entry for start and end coordinates
*  Main menu screen
*  How to play guide option on main menu
*  Properly structured 2 player gameplay
*  Implement checks for check/stalemate/checkmate
*  Castling
*
* If time permits (Advanced TODOs):
*  Play vs AI option
*  Allow the user to perform moves with keyboard (or mouse, whichever is easier)
*  Time limit for turns
*  Sound FX/Music
*/

#include <iostream>
#include <string>
#include <regex>
#include <Windows.h>

// conflict max() solution find stackoverflow.com/questions/20446373/cin-ignorenumeric-limitsstreamsizemax-n-max-not-recognize-it
#undef max

std::string chessBoard[8][8]; //a 2d array to represent the chess board, standard size is 8x8


//initializeBoard function prototype
//resets the logical chess board to place pieces at starting positions
void initializeBoard();

//drawBoard function prototype
//draws out a neatly formatted chess board with pieces dynamically placed
void drawBoard();

//howTo function prototype
//displays the menu for a guide on the rules of the game and the pieces
void howTo();

//getPieceType function prototype
//takes in a char and returns the name of that piece based on the char
std::string getPieceType(char pieceChar);

//isValidPieceMovement function prototype
//figures out if a piece is allowed to be moved to the user's destination
bool isValidPieceMovement(int startX, int startY, int destinationX, int destinationY);

//isValidHorizontalOrVerticalMove function prototype
//determines if a piece can be moved to the destination horizontally or vertically (includes collision detection)
bool isValidHorizontalOrVerticalMove(int startX, int startY, int destinationX, int destinationY);

//isValidDiagonalMove function prototype
//determines if a piece can be moved to the destination diagonally (includes collision detection)
bool isValidDiagonalMove(int startX, int startY, int destinationX, int destinationY);






//isInputPattern function prototype
//check user input correct format 1-8,1-8
bool isInputPattern(const std::string& input);

//isInputValid function prototype
//check both user input is valid and ask user input another x,y
void isInputValid(std::string &userInput, std::string msg);

int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

	//used the following thread for help with resizing the console window: stackoverflow.com/questions/21238806/how-to-set-output-console-width-in-visual-studio
	HWND console = GetConsoleWindow();
	MoveWindow(console, 500, 200, 500, 720, TRUE); //startX, startY, width, height - int params for the console window

	initializeBoard(); //reset the chess board
	drawBoard();       //display the chess board to the user

	// test piece validation and moving them around the board

	std::string userInputStart; //user's input for the start location of the piece
	std::string userInputEnd;   //user's input for the desired end location of the piece
	std::string startMsg;       //message for ask user input start position
	std::string endMsg;         //message for ask user input end position
	int playerNumber = 1;       //an int that is either 1 or 2, which determines which player's move it is
	std::string previousTurnAction = "Game started."; //a small description of the previous turn's action

	while (1)
	{
		bool valid = false;		    //initialize valid to false
		std::cout << "\n " << previousTurnAction << std::endl; //output a description of the previous turn's action

		//***NOTE*** 
		//this is a demo and will need to be refined later on to make it more user friendly
		//when testing the program, enter x,y coordinate. Do not enter the letter, instead enter it's number equivalent (ex. column A would be a y value of 1, B would be 2, H would be 8, etc.)
		//example input (move second player's leftmost pawn up 2 spaces): 
		// first line  - 1,7
		// second line - 1,5

		while (valid == false) {

			// perpare messages for output
			startMsg = " Enter your piece's starting position <x,y>: ";
			endMsg = "\n Enter the desired end position <x,y>: ";

			std::cout << "\n Player " << std::to_string(playerNumber) << " enter your piece's starting position <x,y>: ";
			std::cin >> userInputStart;

			isInputValid(userInputStart, startMsg); // call isInputValid to do clear input and ask valid input

			std::cout << "\n Enter the desired end position <x,y>: ";
			std::cin >> userInputEnd;

			isInputValid(userInputEnd, endMsg);

			// user input passed the pattern check, check for the movement
			int startX = userInputStart[0] - '0' - 1; //convert the char into an int and subtract 1 so it can be used as an index value
			int startY = userInputStart[2] - '0' - 1;
			int endX = userInputEnd[0] - '0' - 1;
			int endY = userInputEnd[2] - '0' - 1;

			// validate the movement
			if (isValidPieceMovement(startX, startY, endX, endY) == false) {

				std::cout << " Invalid movement \n";
				valid = false;
			}
			else {
				valid = true;
			}

		}

		int startX = userInputStart[0] - '0' - 1; //convert the char into an int and subtract 1 so it can be used as an index value
		int startY = userInputStart[2] - '0' - 1;
		int endX = userInputEnd[0] - '0' - 1;
		int endY = userInputEnd[2] - '0' - 1;

		if (isValidPieceMovement(startX, startY, endX, endY)) //if piece movement is valid, move it
		{
			//update the previous turn's action
			previousTurnAction = "Player " + std::to_string(playerNumber) + " moved " + getPieceType(chessBoard[startY][startX][0]) +
				" from (" + std::to_string(startX + 1) + ", " + std::to_string(startY + 1) + ") to (" + std::to_string(endX + 1) + ", " + std::to_string(endY + 1) + ")";
			//check if end location has an enemy piece
			if (chessBoard[endY][endX] != "")
				previousTurnAction += "\n And took the enemy's " + getPieceType(chessBoard[endY][endX][0]);
			previousTurnAction += ".";

			//set the new position for the piece and clear the old position
			chessBoard[endY][endX] = chessBoard[startY][startX];
			chessBoard[startY][startX] = "";
		}



		drawBoard(); //update board

		//change which player's turn it is
		if (playerNumber == 1)
			playerNumber = 2;
		else
			playerNumber = 1;
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

	std::cout << "\n" << "      A     B     C     D     E     F     G     H\n";
	for (int i = 0; i < 33; i++)
	{
		if (i % 4 == 0)
		{
			std::cout << "   *************************************************\n";
		}
		if (i % 4 == 1 || i % 4 == 3)
		{
			std::cout << "   *     *     *     *     *     *     *     *     *\n";
		}
		if (i % 4 == 2)
		{
			std::cout << " " << i / 4 + 1 << " *  ";
			for (int n = 0; n < 15; n++)
			{
				if (n % 2 == 0)
				{
					if (chessBoard[i / 4][n / 2].length() == 2)
					{
						if (chessBoard[i / 4][n / 2][1] == '1')
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 241);
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244);
						}
						std::cout << chessBoard[i / 4][n / 2][0];
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
					}
					else
					{
						std::cout << chessBoard[i / 4][n / 2][0];
					}
				}
				else
				{
					std::cout << "  *  ";
				}
			}
			std::cout << "  *\n";
		}
	}
}

//howTo function
//displays the guide menu with various options to choose from
void howTo()
{
	char response; //uses char to avoid some errors that happen when the user enters invalid numbers in an int

	while (1)
	{
		system("cls"); //clear screen every time it loops

		std::cout << "Enter the number of whichever topic you would like to\nlearn about.\n\n";
		std::cout << "1. Pawns\n2. Rooks\n3. Knights\n4. Bishops\n5. Queen\n6. King\n7. General rules\n8. Return to main menu\n";
		std::cin >> response;

		system("cls"); //clear screen every time the user selects something, to look less cluttered

		//formatting for the couts in the if statements lets the program neatly display each line of text on one line

		if (response == '1')
		{
			std::cout << "Pawns, represented by a P, can move two spaces directly\n";
			std::cout << "forwards on their first turn, and otherwise can move\n";
			std::cout << "either one space forward to an empty space, or diagonally\n";
			std::cout << "forwards one space, but only to capture another piece. \nPawns may also become queens by reaching the enemy team's first row.";
		}
		else if (response == '2')
		{
			std::cout << "Rooks, represented by an R, can only move directly\n";
			std::cout << "horizontally or vertically any amount of spaces, as long\n";
			std::cout << "as there are no pieces in between the start and end.";
		}
		else if (response == '3')
		{
			std::cout << "Knights, represented by an N, can move two spaces \n";
			std::cout << "vertically or horizontally and one space perpendicular to that.";
		}
		else if (response == '4')
		{
			std::cout << "Bishops, represented by a B, can move diagonally any\n";
			std::cout << "number of spaces, as long as there are no pieces in\n";
			std::cout << "between the start and end of its move.";
		}
		else if (response == '5')
		{
			std::cout << "Queens, represented by a Q, can move any number of spaces\n";
			std::cout << "horizontally, vertically, or diagonally, as long as there\n";
			std::cout << "are no pieces in between the start and end.";
		}
		else if (response == '6')
		{
			std::cout << "The king, represented by a K, can move one space in any\n";
			std::cout << "direction. The game is won by capturing the opponent\'s\n";
			std::cout << "king, or lost when yours is captured.";
		}
		else if (response == '7')
		{
			std::cout << "Players can capture the opponent\'s game pieces by moving\n";
			std::cout << "their own piece onto the same tile as the opponent\'s\n";
			std::cout << "piece. A player wins when they capture the opponent\'s\n";
			std::cout << "king, resulting in a checkmate. The only other outcome is\n";
			std::cout << "a stalemate, where neither player can possibly capture\n";
			std::cout << "the other\'s king.";
		}
		else if (response == '8')
		{
			break; //break the loop if the user enters 8
		}
		else
		{
			std::cout << "That is not a valid number."; //displays this if the user enters anything other than the above values for response
		}

		std::cout << "\n\n\n"; //makes sure the prompt from system("pause") is on a new line

		system("pause"); //pauses the program so that the user can read their desired text before going back to the howTo menu
	}

	//currently, breaking the loop will just move to the next part of the program, but
	//I think it should probably call the main menu function when that exists.
}

//getPieceType function
//converts a char into a string to represent a piece
std::string getPieceType(char pieceChar)
{
	std::string pieceString = "";

	//determine piece name based on the char
	if (pieceChar == 'P')
		pieceString = "Pawn";
	else if (pieceChar == 'N')
		pieceString = "Knight";
	else if (pieceChar == 'B')
		pieceString = "Bishop";
	else if (pieceChar == 'Q')
		pieceString = "Queen";
	else if (pieceChar == 'K')
		pieceString = "King";

	return pieceString;
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

				//check if pawn had a valid movement and reached the end of the enemy's board
				if (returnValue == true && destinationY == 7)
					chessBoard[startY][startX][0] = 'Q'; //change pawn to a queen
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

				//check if pawn had a valid movement and reached the end of the enemy's board
				if (returnValue == true && destinationY == 0)
					chessBoard[startY][startX][0] = 'Q'; //change pawn to a queen
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

	while (isInputPattern(userInput) == false) {

		std::cout << " Invalid Input \n";
		std::cin.clear(); // reset cin for next input
		// ignore the user input, passing in the maximize size a user could input to clear
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << msg;
		std::cin >> userInput;// ask user input the valid x,y
	}

}