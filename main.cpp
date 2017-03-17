#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>

#define MAX 10
#define UNMARKED 0

using namespace std;

int grid[MAX][MAX];

// This function inputs the sudoku from the given file name.
// It return the size of the grid(int).
int inputPuzzle()
{
    ifstream input;
    input.open("Puzzle-5.txt",ios::in);

    if(!input.is_open())
    {
        cout<<"Sorry could not open your Puzzle file.!!"<<endl;
        return -1;
    }
    int grid_size;
    input>>grid_size;

    grid_size*=grid_size;

    for(int i=0; i<grid_size; i++)
    {
        for(int j=0; j<grid_size; j++)
        {
            input>>grid[i][j];
        }
    }

    input.close();
    return grid_size;
}

// Finds a empty position on the grid and returns them via the pointers.
// Returns: true - if it is Empty.
//          false - if it is not Empty.
// Parameters :
//          int grid_size  -- Size of the Sudoku that u are going to solve.
//          int *row -- Pointer that will point to the empty x coordinate.
//          int *col -- Pointer that will point to the empty y coordinate.
bool findEmptyPosition(int grid_size,int &row, int&col)
{
    for(row=0; row<grid_size; row++)
    {
        for(col=0; col<grid_size; col++)
        {
            if(grid[row][col] == UNMARKED)
            {
                return true;
            }
        }
    }
    return false;
}

// Checks whether the Row is safe for that number to be put on.
// Returns: true - if it is Safe.
//          false - if it is not Safe.
// Parameters :
//              int grid_size  -- Size of the Sudoku that u are going to solve.
//              int row --  The Row Number where you want to check whether a particular value is safe in that row.
//              int num --  The Number that you want to check for being safe in that Row
bool isSafeRow(int grid_size,int row,int num)
{
    for(int i=0; i<grid_size; i++)
    {
        if(grid[row][i]==num)
        {
            return false;
        }
    }
    return true;
}

// Checks whether the Column is safe for that number to be put on.
// Returns: true - if it is Safe.
//          false - if it is not Safe.
// Parameters :
//              int grid_size  -- Size of the Sudoku that u are going to solve.
//              int col --  The Col Number where you want to check whether a particular value is safe in that row.
//              int num --  The Number that you want to check for being safe in that Column.
bool isSafeCol(int grid_size,int col,int num)
{
    for(int i=0; i<grid_size; i++)
    {
        if(grid[i][col]==num)
        {
            return false;
        }
    }
    return true;
}

// Checks whether the Box is safe for that number to be put on.
// Returns: true - if it is Safe.
//               false - if it is not Safe
// Parameters :
//              int grid_size  -- Size of the Sudoku that u are going to solve.
//              int row  --  The Row Number where the number is placed.
//              int col  --  The Column Number where the number is placed
//              int num  --  The Number that you want to check for being safe in the Box.
bool isSafeBox(int grid_size,int row,int col,int num)
{
    int box_size=sqrt(grid_size);

    int box_start_row=row-row%box_size;
    int box_start_col=col-col%box_size;

    for(int i=0; i<box_size; i++)
    {
        for(int j=0; j<box_size; j++)
        {
            //cout<<"I="<<i+box_start_row<<"--J="<<j+box_start_col<<endl;
            if( grid[i+box_start_row][j+box_start_col] == num)
            {
                return false;
            }
        }
    }
    return true;
}

// Checks whether the Row & Column & the Box is safe for that number to be put on.
// Returns: true - if it is Safe.
//               false - if it is not Safe
// Parameters :
//              int grid_size  -- Size of the Sudoku that u are going to solve.
//              int row  --  The Row Number where the number is placed.
//              int col  --  The Column Number where the number is placed
//              int num  --  The Number that you want to check for being safe in the Row , Column and the Box.
bool isSafe(int grid_size,int row,int col,int num)
{
    if( isSafeRow(grid_size,row,num) && isSafeCol(grid_size,col,num) && isSafeBox(grid_size,row,col,num) )
    {
        return true;
    }
    return false;
}

// The main solving function which finds for a solution for the given Sudoku.
bool solveSudoku(int grid_size)
{
    int row, col;

    if(!findEmptyPosition(grid_size,row,col))
    {
        return true;//No more positions left to fill. The Sudoku is Solved.
    }

    for(int num=1; num<=grid_size; num++)
    {
        if(isSafe(grid_size,row,col,num))
        {
            //cout<<"FILLED Row= "<<row<<" --Col= "<<col<<"--Num="<<num<<endl;
            grid[row][col] = num;
            //print_Grid(grid_size);
            if(solveSudoku(grid_size))
                return true;

            grid[row][col] = UNMARKED;
            //cout<<"BACKTRACK Row= "<<row<<" --Col= "<<col<<endl;
            //print_Grid(grid_size);
        }
    }
    return false;
}

// Prints the Sudoku Output in a stylish manner.
void printInStyle(int n)
{
    int box_size=sqrt(n);
    for(int i=0; i<n+3; i++)
        cout<<" -";
    cout<<endl;

    for(int i=0; i<n; i++)
    {
        cout<<"| ";
        for(int j=0; j<n; j++)
        {
            if(i%box_size==0 && j%box_size==0 && i!=0 && j!=0)
                cout<<"+ ";
            if(i%box_size==0 && i!=0)
            {
                cout<<"- ";
                continue;
            }
            if(j%box_size==0 && j!=0)
            {
                cout<<"| "<<grid[i][j]<<" ";
            }
            else
            {
                cout<<grid[i][j]<<" ";
            }
        }
        cout<<"|"<<endl;

        if(i%box_size==0 && i!=0)
        {
            cout<<"| ";
            for(int j=0; j<n; j++)
            {
                if(j%box_size==0 && j!=0)
                {
                    cout<<"| "<<grid[i][j]<<" ";
                }
                else
                {
                    cout<<grid[i][j]<<" ";
                }
            }
            cout<<"|"<<endl;
        }
    }

    for(int i=0; i<n+3; i++)
        cout<<" -";
    cout<<endl;
}

int main()
{
    int grid_size=inputPuzzle();

    // If the file does not exists.
    if(grid_size==-1)
    {
        return 0;
    }

    cout<<"Entered Sudoku is :"<<endl;
    cout<<"0 is for empty places that are to be filled..!!\n"<<endl;
    printInStyle(grid_size);
    cout<<endl;

    if(solveSudoku(grid_size))
    {
        cout<<"Solution is : \n"<<endl;
        printInStyle(grid_size);
    }
    else
    {
        cout<<"Sorry The Answer does not exists..!!"<<endl;
    }
    return 0;
}
