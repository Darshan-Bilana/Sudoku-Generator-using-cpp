#include <bits/stdc++.h>

#define UNASSIGNED 0

using namespace std;

class Sudoku {
private:
  int grid[9][9];
  int solnGrid[9][9];
  int guessNum[9];
  int gridPos[81];

public:
  Sudoku ();
  void fillEmptyDiagonalBox(int);
  void createSeed();
  void printGrid();
  void printSoln();
  bool solveGrid();
  void genPuzzle();
  int unique_solution();
  
};


int genRandNum(int maxLimit)
{
  return rand()%maxLimit;
}

// Helper functions for solving grid
bool FindUnassignedLocation(int grid[9][9], int &row, int &col)
{
    for (row = 0; row < 9; row++)
    {
        for (col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
                return true;
        }
    }

    return false;
}

bool FindUnassignedLocation2(int grid[9][9], int &row, int &col)
{
    for (row; row < 9; row++)
    {
        for (col; col < 9; col++)
        {
            if (grid[row][col] == 0)
                return true;
        }
    }

    return false;
}

bool UsedInRow(int grid[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
    {
        if (grid[row][col] == num)
            return true;
    }

    return false;
}

bool UsedInCol(int grid[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
    {
        if (grid[row][col] == num)
            return true;
    }

    return false;
}

bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
        }
    }

    return false;
}

bool isSafe(int grid[9][9], int row, int col, int num)
{
    return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) && !UsedInBox(grid, row - row%3 , col - col%3, num);
}
// END: Helper functions for solving grid


// START: Create seed grid
void Sudoku::fillEmptyDiagonalBox(int idx)
{
  int start = idx*3;
  random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      this->grid[start+i][start+j] = guessNum[i*3+j];
    }
  }
}

void Sudoku::createSeed()
{
  this->fillEmptyDiagonalBox(0);
  this->fillEmptyDiagonalBox(1);
  this->fillEmptyDiagonalBox(2);

  this->solveGrid(); 

  // Saving the solution grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->solnGrid[i][j] = this->grid[i][j];
    }
  }
  
  
}

// Constructor
Sudoku::Sudoku()
{

  // Randomly shuffling the array of removing grid positions
  for(int i=0;i<81;i++)
  {
    this->gridPos[i] = i;
  }

  random_shuffle(this->gridPos, (this->gridPos) + 81, genRandNum);

  // Randomly shuffling the guessing number array
  for(int i=0;i<9;i++)
  {
    this->guessNum[i]=i+1;
  }

  random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);

  // Initialising the grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->grid[i][j]=0;
    }
  }

}

//Printing the grid
void Sudoku::printGrid()
{
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      if(grid[i][j] == 0)
	cout<<"_";
      else
	cout<<grid[i][j];
      cout<<"|";
    }
    cout<<endl;
  }

  
  cout<<endl;
}


// Sudoku solver
bool Sudoku::solveGrid()
{
    int row, col;

    // If there is no unassigned location, we are done
    if (!FindUnassignedLocation(this->grid, row, col))
       return true; // success!

    // Consider digits 1 to 9
    for (int num = 0; num < 9; num++)
    {
        // if looks promising
        if (isSafe(this->grid, row, col, this->guessNum[num]))
        {
            // make tentative assignment
            this->grid[row][col] = this->guessNum[num];

            // return, if success 
            if (solveGrid())
                return true;

            // failure, unmake & try again
            this->grid[row][col] = UNASSIGNED;
        }
    }

    return false; //backtracking

}

//function to solve for temperory grids
int solvebox(int temp[9][9],int guess[9])
{
    int row, col;

    // If there is no unassigned location, we are done
    if (!FindUnassignedLocation(temp, row, col))
       return true; // success!

    // Consider digits 1 to 9
    for (int num = 0; num < 9; num++)
    {
        // if looks promising
        if (isSafe(temp, row, col, guess[num]))
        {
            // make tentative assignment
            temp[row][col] = guess[num];

            // return, if success 
            if (solvebox(temp,guess))
                return true;

            // failure, unmake & try again
            temp[row][col] = UNASSIGNED;
        }
    }

    return false; //backtracking

}
//check if two grids are equal to each other
int duplicate_grid(int box1[9][9],int box2[9][9]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(box1[i][j]!=box2[i][j]) return 0;
        }
    }
    return 1; //if all elements are equal then both are duplicates
}

//check if puzzle is uniquely solvable
int Sudoku :: unique_solution(){
     Sudoku *tempgrid1 = new Sudoku();
     Sudoku *tempgrid2 = new Sudoku();
     int tempbox1[9][9];
     int tempbox2[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            tempbox1[i][j]=this->grid[i][j];
            tempbox2[i][j]=this->grid[i][j];
        }
    }
    int guess1[] = {1,2,3,4,5,6,7,8,9};
    int guess2[] = {9,8,7,6,5,4,3,2,1};
    solvebox(tempbox1,guess1);
    solvebox(tempbox2,guess2);
    if(duplicate_grid(tempbox1,tempbox2)) return 1; //if both solutions are equal, that means unique solution
    return 0;
}

void Sudoku:: printSoln(){
    for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      cout<<this->solnGrid[i][j]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}


void Sudoku::genPuzzle()
{
  for(int i=0;i<81;i++)
  {
    int x = (this->gridPos[i])/9;
    int y = (this->gridPos[i])%9;
    int temp = this->grid[x][y];
    this->grid[x][y] = 0;

    // If now more than 1 solution , replace the removed cell back.
    int check=this->unique_solution();
   
    if(check!=1)
    {
      this->grid[x][y] = temp;
    }
  }
}


int main()
{ 
 
  srand(time(NULL));
  Sudoku *puzzle = new Sudoku();
  
  puzzle->createSeed();
  puzzle->genPuzzle();
  puzzle->printGrid();
  cout<<endl;
  cout<<"Solution for above sudoku : "<<endl<<endl;
  puzzle->printSoln();
  cout<<endl;
  delete puzzle;

  return 0;
}
