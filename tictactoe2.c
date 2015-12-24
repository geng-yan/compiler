// for reference only

#include <stdio.h>
#include <stdlib.h>

#define SIZE 4

void build_board(char array[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = ' ';
        }
    }
}

void print_board(char array[SIZE][SIZE], int size)
{
    printf(" ");
    for (int i = 0; i < size; i = i + 1)
    {
        printf(" %d ", i + 1);
    }
    printf("\n");
    
    for (int i = 0; i < size; i = i + 1)
    {
        printf("%d", i + 1);
        for (int j = 0; j < size; j = j + 1)
        {
            printf(" %c ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

int check_row(char array[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        char head = array[i][0];
        if (head != ' ')
        {
            for (int j = 1; j < size; j++)
            {
                if (head != array[i][j])
                    break;
                else if (j == size - 1) // found a winning row
                    return 1;
            }
        }
    }
    return 0;
}

int check_col(char array[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        char head = array[0][i];
        if (head != ' ')
        {
            for (int j = 1; j < size; j++)
            {
                if (head != array[j][i])
                    break;
                else if (j == size - 1) // found a winning row
                    return 1;
            }
        }
    }
    return 0;
}

int check_dia(char array[SIZE][SIZE], int size)
{
    for (int i = 1; i < size; i++)
    {
        if (array[0][0] != ' ')
        {
            if (array[0][0] != array[i][i])
                break;
            else if (i == size - 1)
                return 1;
        }
    }
    
    for (int i = 1; i < size; i++)
    {
        if (array[0][size - 1] != ' ')
        {
            if (array[0][size - 1] != array[i][size - 1 - i])
                break;
            else if (i == size - 1)
                return 1;
        }
    }
    
    return 0;
}

int check_win(char array[SIZE][SIZE], int size)
{
    if (check_row(array, size) || check_col(array, size) || check_dia(array, size))
        return 1;
    return 0;
}

void move(char array[SIZE][SIZE], int size, int player)
{
    int x, y;
    printf("Player %d moves: ", player + 1); // 1 3; no 0
    scanf("%d", &x);
    scanf("%d", &y);
    while ((1 > x) || (x > SIZE) || (y < 1) || (y > SIZE) || array[x - 1][y - 1] == 'o' || array[x - 1][y - 1] == 'x')
    {
        printf("Invalid coordinate. Try again: ");
        scanf("%d", &x);
        scanf("%d", &y);
    }
    if (player == 0)
        array[x - 1][y - 1] = 'o';
    else
        array[x - 1][y - 1] = 'x';
}

int check_tie(char array[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (array[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

int main()
{
    int count = 0;
    printf("Welcome to Size %d Tic-Tac-Toe!\n", SIZE);
    printf("Usage: <x-coordinate> <y-coordinate>\n");
    
    char array[SIZE][SIZE];
    build_board(array, SIZE);
    print_board(array, SIZE);
    
    while (1)
    {
        int player = count % 2;
        
        if (count % 2 == 0)
            printf("Round %d\n", count/2 + 1);

        
        move(array, SIZE, player);
        print_board(array, SIZE);
        if (check_win(array, SIZE))
        {
            printf("Congratulations! Player %d is the winner!\n", player + 1);
            return 0;
        }
        else if (check_tie(array, SIZE))
        {
            printf("Game end. Player 2 is tied with Player 1.\n");
            return 0;
        }
        
        count++;
    }
}