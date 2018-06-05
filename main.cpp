#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>


using namespace std;
using std::string;


const int mapWidth = 16;
string board[mapWidth][mapWidth];


bool gameOver;
int score;
HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//class containing elements of snake
class SnakePart
{
    public:
        int x;
        int y;
        int direction; // 0-left 1-down 2-right 3-up
};

vector<SnakePart> snakeParts;


class Portal
{
    public:
        bool exists;
        int x;
        int y;
};

Portal portals[2];

//same as above but for coin
class Coin
{
    public:
        int x;
        int y;
};

Coin actualCoin;



//filling the board with single character, used also to clear the memory
void PointZero()
{
    for(int i = 0; i < mapWidth; i++)
    {
       for(int k = 0; k < mapWidth; k++)
        {
            board[i][k] = "x";
        }
    }
}



//adding characters that are going to be the snake to the board on specific coordinates
void SnakeFill()
{
    for(int z = 0; z < snakeParts.size(); z++)
    {
        board[snakeParts[z].x][snakeParts[z].y] = "@";
    }
}



//placing coin
void CoinPlacement()
{
    board[actualCoin.x][actualCoin.y] = "$";
}



void CoinCollect()
{
    if(snakeParts[0].x == actualCoin.x && snakeParts[0].y == actualCoin.y) //'collision' between coin and snake's head
    {
        int x = 0;
        int y = 0;
        int direction = snakeParts.back().direction;

        //check in with direction last element of snake were moving, this determine on with coordinates new part will be placed
        if(0 == direction)
        {
            x = snakeParts.back().x + 1; //placed on right
            y = snakeParts.back().y;
        }
        if(1 == direction)
        {
           y = snakeParts.back().y -1; //placed above
           x = snakeParts.back().x;
        }
        if(2 == direction)
        {
            x = snakeParts.back().x - 1; //placed on left
            y = snakeParts.back().y;
        }
        if(3 == direction)
        {
           y = snakeParts.back().y + 1; //placed below
           x = snakeParts.back().x;
        }
        snakeParts.push_back(SnakePart()); //push new part of snake to vector
        snakeParts.back().direction = direction; //fill empty variables of new element
        snakeParts.back().x = x;
        snakeParts.back().y = y;

        //place coin in new position
        actualCoin.x = rand() % mapWidth;
        actualCoin.y = rand() % mapWidth;
        score++;
    }
}



void PortalPlacement()
{
    if(portals[0].exists)
    {
        board[portals[0].x][portals[0].y] = "O";
    }
    if(portals[1].exists)
    {
        board[portals[1].x][portals[1].y] = "P";
    }
}



//map refresh
void FrameUpdate()
{
    system("cls");
    PointZero(); //reset map
    CoinPlacement(); //place coin
    PortalPlacement();
    SnakeFill(); // place snake

    for(int y = 0; y< mapWidth; y++) //draw map in console
    {
       for(int x = 0; x < mapWidth; x++)
        {
            if(board[x][y] == "@")
            {
                SetConsoleTextAttribute(hConsole, 5);
            }
            else if(board[x][y] == "$")
            {
                SetConsoleTextAttribute(hConsole, 2);
            }
            else if(board[x][y] == "O")
            {
                SetConsoleTextAttribute(hConsole, 0xF);
            }
            else if(board[x][y] == "P")
            {
                SetConsoleTextAttribute(hConsole, 0xC);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 6);
            }
            cout << board[x][y];
        }
        cout << endl;
    }
}



//moving snake by changing his coordinates in array
void Movement()
{
    for(int i = 0; i < snakeParts.size(); i++) //x & y are changed depending on direction of snake's parts movement direction
    {
        if(0 == snakeParts[i].direction && snakeParts[i].x >= 0)
        {
            snakeParts[i].x--;
        }
        else if(1 == snakeParts[i].direction && snakeParts[i].y <= mapWidth)
        {
           snakeParts[i].y++;
        }
        else if(2 == snakeParts[i].direction && snakeParts[i].x <= mapWidth)
        {
            snakeParts[i].x++;
        }
        else if(3 == snakeParts[i].direction && snakeParts[i].y >= 0)
        {
            snakeParts[i].y--;
        }
        if(i != 0 && snakeParts[i].x == snakeParts[0].x && snakeParts[i].y == snakeParts[0].y) //collision between snake's hear and rest of his body
        {
             gameOver = true;
        }
    }


    //updating every snake's part movement direction
    //from last to first, but without the head
    for(int i = snakeParts.size(); i <= snakeParts.size(); i--)
    {
        if(i != 0 && snakeParts[i].x == snakeParts[i-1].x)
        {
            snakeParts[i].direction = snakeParts[i-1].direction;
        }
        if(i != 0 && snakeParts[i].y == snakeParts[i-1].y)
        {
            snakeParts[i].direction = snakeParts[i-1].direction;
        }
    }
}


//input
void Controls()
{
    if(GetKeyState('A') & 0x80)
    {
        snakeParts[0].direction = 0;
    }
    if(GetKeyState('S') & 0x80 )
    {
        snakeParts[0].direction = 1;
    }
    if(GetKeyState('D') & 0x80)
    {
        snakeParts[0].direction = 2;
    }
    if(GetKeyState('W') & 0x80)
    {
        snakeParts[0].direction = 3;
    }



    if(GetKeyState('O') & 0x80)
    {
        if(snakeParts[0].direction == 0)
        {
            portals[0].x = 0;
            portals[0].y = snakeParts[0].y;
            portals[0].exists = true;
        }
        else if(snakeParts[0].direction == 1)
        {
            portals[0].x = snakeParts[0].x;
            portals[0].y = mapWidth - 1;
            portals[0].exists = true;
        }
        else if(snakeParts[0].direction == 2)
        {
            portals[0].x = mapWidth - 1;
            portals[0].y = snakeParts[0].y;
            portals[0].exists = true;
        }
        else if(snakeParts[0].direction == 3)
        {
            portals[0].x = snakeParts[0].x;
            portals[0].y = 0;
            portals[0].exists = true;
        }
    }
    if(GetKeyState('P') & 0x80)
    {
        if(snakeParts[0].direction == 0)
        {
            portals[1].x = 0;
            portals[1].y = snakeParts[0].y;
            portals[1].exists = true;
        }
        else if(snakeParts[0].direction == 1)
        {
            portals[1].x = snakeParts[0].x;
            portals[1].y = mapWidth - 1;
            portals[1].exists = true;
        }
        else if(snakeParts[0].direction == 2)
        {
            portals[1].x = mapWidth - 1;
            portals[1].y = snakeParts[0].y;
            portals[1].exists = true;
        }
        else if(snakeParts[0].direction == 3)
        {
            portals[1].x = snakeParts[0].x;
            portals[1].y = 0;
            portals[1].exists = true;
        }
    }
    if(GetKeyState('B') & 0x80)
    {
        portals[0].exists = false;
        portals[1].exists = false;
    }
}



void PortalLogic()
{
    if(portals[0].exists && portals[1].exists)
    {
        if(snakeParts[0].x == portals[0].x && snakeParts[0].y == portals[0].y)
        {
            snakeParts[0].x = portals[1].x;
            snakeParts[0].y = portals[1].y;
        }else if(snakeParts[0].x == portals[1].x && snakeParts[0].y == portals[1].y)
        {
            snakeParts[0].x = portals[0].x;
            snakeParts[0].y = portals[0].y;
        }
    }

}



int main()
{
    SetConsoleTextAttribute(hConsole, 4); //change text color (red)
    cout << "Welcome to snake" << endl;
    cout << "Press any key to start...";
    getch(); //wait for user input
    PointZero(); //firs fill of board array


    //adding snake's head to vector
    snakeParts.push_back(SnakePart());
    snakeParts[0].x = 8;
    snakeParts[0].y = 8;
    snakeParts[0].direction = 0;


    actualCoin.x = 10;
    actualCoin.y = 10;


    double startTime = GetTickCount(); //part of simple timer
    while (!gameOver)
    {
        Controls();
        PortalLogic();
        double timer = GetTickCount() - startTime; //delta time

        if( timer >= 200 ) //our simple timer that will keep game running in 5fps
		{
			FrameUpdate();
			Movement();
			CoinCollect();
			SetConsoleTextAttribute(hConsole, 2); //change text color (green)
			cout << endl << "Score: " << score;
			startTime = GetTickCount(); //update time used in our timer
		}

        if(snakeParts[0].y < 0 || snakeParts[0].x == mapWidth || snakeParts[0].y == mapWidth || snakeParts[0].x < 0) //collision with map edge
        {
            gameOver = true;
        }
    }
    SetConsoleTextAttribute(hConsole, 4); //change text color (red)
    cout << endl << endl << "YOU LOSE" << endl;
    system("pause"); //waiting for user to press any key
    return 0;
}
