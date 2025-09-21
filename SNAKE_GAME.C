
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dos.h>      // for delay()
#include <process.h>  // for exit()

#define ROWS 20
#define COLS 40
#define MAX_SNAKE 100

struct SnakePart {
    int x, y;
    char value;
};

struct SnakePart snake[MAX_SNAKE];
int length = 3;
int foodX, foodY;
char board[ROWS][COLS];
char direction = 'd'; // initial direction

// Function to place food randomly
void placeFood() {
    foodX = rand() % (ROWS-2) + 1;
    foodY = rand() % (COLS-2) + 1;
}

// Initialize snake
void createSnake() {
    int i;
    snake[0].x = ROWS/2;
    snake[0].y = COLS/2;
    snake[0].value = 'O'; // head

    for(i=1; i<length; i++) {
	snake[i].x = ROWS/2;
	snake[i].y = COLS/2 - i;
	snake[i].value = '+';
    }
}

// Create board with borders
void createBoard() {
    int i, j;
    for(i=0;i<ROWS;i++) {
	for(j=0;j<COLS;j++) {
	    if(i==0 || i==ROWS-1 || j==0 || j==COLS-1)
		board[i][j] = '*'; // border
	    else
		board[i][j] = ' ';
	}
    }
    board[foodX][foodY] = '@'; // place food
}

// Merge snake into board
void merge() {
    int i;
    for(i=0; i<length; i++) {
	board[snake[i].x][snake[i].y] = snake[i].value;
    }
}

// Display board
void display() {
    int i, j;
    clrscr(); // clear screen (Turbo C)
    for(i=0;i<ROWS;i++) {
	for(j=0;j<COLS;j++) {
	    printf("%c", board[i][j]);
	}
	printf("\n");
    }
    printf("Score: %d\n", length-3);
    printf("Follow Instructions : W for up | A for Left | S for Down | D for Right | X For Exit\n");
}

// Move snake
void moveSnake() {
    int i;
    // shift body
    for(i=length-1; i>0; i--) {
	snake[i].x = snake[i-1].x;
	snake[i].y = snake[i-1].y;
    }

    // move head
    if(direction=='w') snake[0].x--;
    else if(direction=='s') snake[0].x++;
    else if(direction=='a') snake[0].y--;
    else if(direction=='d') snake[0].y++;

    // collision with wall
    if(snake[0].x==0 || snake[0].x==ROWS-1 ||
       snake[0].y==0 || snake[0].y==COLS-1) {
	printf("Game Over! You hit the wall.\n");
	getch();
	exit(0);
    }

    // collision with self
    for(i=1; i<length; i++) {
	if(snake[0].x==snake[i].x && snake[0].y==snake[i].y) {
	    printf("Game Over! You hit yourself.\n");
	    getch();
	    exit(0);
	}
    }

    // eating food
    if(snake[0].x==foodX && snake[0].y==foodY) {
	length++;
	snake[length-1].x = snake[length-2].x;
	snake[length-1].y = snake[length-2].y;
	snake[length-1].value = '+';
	placeFood();
    }
}

int main() {
    srand(time(NULL));
    createSnake();
    placeFood();

    while(1) {
        if(kbhit()) { // check if key pressed
            char c = getch();
            if((c=='w' && direction!='s') ||
               (c=='s' && direction!='w') ||
               (c=='a' && direction!='d') ||
               (c=='d' && direction!='a')) {
                direction = c;
            }
            if(c=='x') { // exit
                break;
            }
        }

        createBoard();
        moveSnake();
        merge();
        display();
        delay(150); // control speed
    }

    return 0;
}
