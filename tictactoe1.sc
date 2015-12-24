// require multi-dim array
// require formatted output
// require identification of char ' '

buildboard(size)
{
    for (i = 0; i < size; i = i + 1)
    {
        for (j = 0; j < size; j = j + 1)
        {
            arr[i * 3 + j] = ' ';
        }
    }
}

printboard(size)
{
    putc(' ');
    for (i = 0; i < size; i = i + 1)
    {
        putc(' '); puti(i + 1); putc(' ');
    }
    putc(' ');
    
    for (i = 0; i < size; i = i + 1)
    {
        puti(i + 1);
        for (j = 0; j < size; j = j + 1)
        {
            putc(' '); putc(arr[i * 3 + j]); putc(' ');
        }
        putc(' ');
    }
    putc(' ');
}

checkrow(size)
{
    for (i = 0; i < size; i++)
    {
        head = arr[i * 3];
        if (head != ' ')
        {
            for (j = 1; j < size; j++)
            {
                if (head != arr[i * 3 + j])
                    break;
                else if (j == size - 1) // found a winning row
                    return 1;
            }
        }
    }
    return 0;
}

checkcol(size)
{
    for ( i = 0; i < size; i++)
    {
        head = arr[i];
        if (head != ' ')
        {
            for ( j = 1; j < size; j++)
            {
                if (head != arr[j * 3 + i])
                    break;
                else if (j == size - 1) // found a winning row
                    return 1;
            }
        }
    }
    return 0;
}

checkdia(size)
{
    for (i = 1; i < size; i++)
    {
        if (arr[0] != ' ')
        {
            if (arr[0] != arr[i * 3 + i])
                break;
            else if (i == size - 1)
                return 1;
        }
    }
    
    for (i = 1; i < size; i++)
    {
        if (arr[size - 1] != ' ')
        {
            if (arr[size - 1] != arr[i * 3 + size - 1 - i])
                break;
            else if (i == size - 1)
                return 1;
        }
    }
    
    return 0;
}

checkwin(size)
{
    if (checkrow(arr, size) || checkcol(arr, size) || checkdia(arr, size))
        return 1;
    return 0;
}

move(size, player)
{
    puts("Player %d moves: ", player + 1); 
    geti(x); 
    geti(y);

    while ((1 > x) || (x > SIZE) || (y < 1) || (y > SIZE) || arr[(x - 1) * 3 + y - 1] == 'o' || arr[(x - 1) * 3 + y - 1] == 'x')
    {
        puts("Invalid coordinate. Try again: ");
        geti(x); 
        geti(y);
    }
    if (player == 0)
        arr[(x - 1) * 3 + y - 1] = 'o';
    else
        arr[(x - 1) * 3 + y - 1] = 'x';
}

checktie(size)
{
    for (i = 0; i < size; i++)
    {
        for ( j = 0; j < size; j++)
        {
            if (arr[i * 3 + j] == ' ')
                return 0;
        }
    }
    return 1;
}

SIZE = 4; // initialize the size to get the size of the board

count = 0;
puts("Welcome to Size %d Tic-Tac-Toe!\n", SIZE);
puts("Usage: <x-coordinate> <y-coordinate>\n");

array arr[SIZE * SIZE];
buildboard(SIZE);
printboard(SIZE);

while (1)
{
    player = count % 2;
    
    if (count % 2 == 0)
        puts("Round %d\n", count/2 + 1);

    
    move(SIZE, player);
    printboard(SIZE);
    if (checkwin(SIZE))
    {
        puts("Congratulations! Player %d is the winner!\n", player + 1);
        return 0;
    }
    else if (checktie(SIZE))
    {
        puts("Game end. Player 2 is tied with Player 1.\n");
        return 0;
    }
    
    count = count + 1;
}
