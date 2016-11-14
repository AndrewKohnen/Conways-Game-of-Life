#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

/*
SUMMARY: 

This program simulates Conways Game of Life
https://en.wikipedia.org/wiki/Conway%27s_game_of_life

We do this with 3 key arrays:
-Living, referred to as the "Living" grid, stores which cells are living and dead
- xLines and yLines are used for drawing our grid

Living cells are red and dead cells are black, or empty 

The program also comes with some functionalities listed to the user on startup
or when the 'h' key is hit. 

Compile the program with "make conway" on either linux or Windows Powershell

Be sure to have the GLUT library installed. 
*/

//Global Values
int **living; 	// Shows which cells are living and which are dead. 
int **temp;		// Stores temporary values used in checkCell
int *xLines;	// Stores the x values for our grid
int *yLines;	// Stores the y values for our grid
int xNum;		// Stores the Number of x values in our grid, including the extra point
int yNum;		// Stores the Number of x values in our grid, including the extra point
int sq;			// The pixel size of a square in our grid 
int speed;		// Stores the refresh rate for our graphics buffer
bool isPaused(true); //displays whether or not the program is paused 

//sets the Global Values of all relavent globals for the program. Grid is empty by default
void setGlobals(int xInit, int yInit, int xLoc, int yLoc, int squareSize){
	xNum = xInit;
	yNum = yInit;
	sq = squareSize;
	speed = 250;
	xLines = (int *) calloc (xNum, sizeof(int));
	yLines = (int *) calloc (yNum, sizeof(int));
	living = (int **) calloc (xNum, sizeof(int *));
	temp = (int **) calloc (xNum, sizeof(int *));

	for(int i = 0; i < xNum; i++){
		living[i] = (int*) calloc(yNum, sizeof(int));
		temp[i] = (int*) calloc(yNum, sizeof(int));
	}
	
	for(int x = 0; x < xNum; x++){
		for(int y = 0; y < yNum; y++){
			living[x][y] = 0;
			temp[x][y] = 0;
		}
	}
	
	for(int j = 0; j <xNum; j++){
		xLines[j] = xLoc + j*sq;
	}
	
	for (int k = 0; k < yNum; k++){
		yLines[k] = yLoc + k*sq;
	}
	
	
}

//Checks if a click made by the user is valid, and fills the grid cell if it is
//The commented lines can be used in debugging
void checkClick(int x, int y){
	//printf("Matching for %i, %i\n", x, y);
	if(!isPaused){
		printf("Pause error: Please pause simulation before clicking\n");
		return;
	}
	if (x < xLines[0] || x > xLines[xNum-1] ||  y < yLines[0] ||  y >yLines[yNum-1]){
		printf("Loc Error: %i- %i, %i - %i\n",xLines[0],xLines[xNum-1],yLines[0], yLines[yNum-1]);
		return;
	}
	//printf("Got to loop\n");
	for(int i = 0; i < xNum-1; i++){
		for(int j = 0; j < yNum-1; j++){
			//printf("Pairing for %i, %i\n", xLines[i], yLines[j]);
			if((x > xLines[i] && xLines[i+1] > x) && (y > yLines[j] && yLines[j+1] > y))
				living[i][j] = (living[i][j] + 1) % 2;
		}
	}
}

//prints a given 2D array. 
void printArray(int ** array, int lx, int ly){
	printf("------------------------------------------\n");
	for(int i = ly -1; i >= 0; i--){
		for(int j = 0; j < lx; j++){
			printf("%i ",array[j][i]);
		}
		printf("\n");
	}
	printf("------------------------------------------\n");
}

//Copies 2D array ar1 to 2D array ar2 to another, both arrays must be the same size
void copyArray(int **ar1, int **ar2, int lx, int ly){
	for(int x = 0; x < lx; x++){
		for(int y = 0; y < ly; y++){
			ar2[x][y] = ar1[x][y];
		}
	}
}

//sets all values of a given 2D array to 0
void clear(int **ar1, int lx, int ly){
	for(int x = 0; x < lx; x++){
		for(int y = 0; y < ly; y++){
			ar1[x][y] = 0;
		}
	}
}

//Fills a given 2D array with 1s and 0s
void randomize(int **ar1, int lx, int ly){
	srand(time(0));
	for(int x = 0; x < lx; x++){
		for(int y = 0; y < ly; y++){
			ar1[x][y] = rand()%2;
		}
	}
}

//checks if the cell specified by the X and Y co-ordinates survives to the next iteration
//the commented lines can be used for debugging 
void checkCell(int ** orig, int **copy, int x, int y, int lx, int ly){
	int life = 0;
	//printf("checking %i, %i\n", x, y);
	if (x-1 >= 0){
		//printf("x check less pass\n");
		life = life + orig[x-1][y];
		if (y-1 >= 0)
			life = life + orig[x-1][y-1];
		if (y + 1 < ly)
			life = life + orig[x-1][y+1];
		//printf("checking life %i\n", life);
	}
	if (x + 1 < lx){
		//printf("x check great pass\n");
		life = life + orig[x+1][y];
		if (y-1 >= 0)
			life = life + orig[x+1][y-1];
		if (y + 1 < ly)
			life = life + orig[x+1][y+1];
		//printf("checking life %i\n", life);
	}
	if (y-1 >= 0){
		//printf("y check less pass\n");
		life = life + orig[x][y-1];	
		//printf("checking life %i\n", life);
	}
	if (y + 1 < ly){
		//printf("y check great pass\n");
		life = life + orig[x][y+1];
		//printf("checking life %i\n", life);
	}
	//printf("checking life %i, %i\n", life, orig[x][y]);
	
	if(life == 3)
		copy[x][y] = 1;
	else if (life + orig[x][y] == 3)
		copy[x][y] = 1;
	else 
		copy[x][y] = 0;
}

//Performs CheckCell on all cells in the "Living" grid 
//Can be updated for improved runtime. 
void checkGrid(int ** orig, int **copy, int lx, int ly){
	for(int x = 0; x < lx; x++){
		for(int y = 0; y < ly; y++){
			checkCell(orig, copy, x, y, lx, ly);
		}
	}
	copyArray(copy, orig, lx, ly);
}

//draws a square at the specified location 
drawSquare(int x, int y){
	glBegin(GL_POLYGON);
		glVertex2i(x,y);
		glVertex2i(x+sq,y);
		glVertex2i(x+sq,y+sq);
		glVertex2i(x,y+sq);
	glEnd();
}

//The Display Function for GLUT
//Commented Lines can be used for Debugging 
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//temp[1][2] = 1;
	//temp[1][3] = 1;
	//temp[1][4] = 1;
	//temp[xNum-2][xNum-2] = 1;
	//temp[xNum-2][xNum-3] = 1;
	//temp[xNum-3][xNum-3] = 1;
	//temp[xNum-3][xNum-2] = 1;
	//checkCell(temp, living, xNum-2, 1, xNum, yNum);
	//checkCell(temp, living, xNum-3, 1, xNum, yNum);
	//copyArray(temp, living, xNum, yNum);
	//printArray(temp, xNum, yNum);
	//printArray(living, xNum, yNum);
	//checkGrid(temp, xNum, yNum);
	
	//Checks if the program is paused. If unpaused then we can update the "Living" grid
	if(!isPaused){
		//printArray(temp, xNum, yNum);
		//printArray(living, xNum, yNum);
		//printf("Is not paused\n");
		checkGrid(living, temp, xNum-1, yNum-1);
	}	
	
	//Draws all squares in the grid
	glColor3f(1,0,0);
	for(int x = 0; x < xNum; x++){
		for(int y = 0; y < yNum; y++){
			if(living[x][y] == 1){
				drawSquare(xLines[x], yLines[y]);
			}
		}
	}
	
	//draws the grid itself 
	glColor3f(1,1,1);
	glBegin(GL_LINES);
		for (int x = 0; x < xNum; x++){
			glVertex2i(xLines[x],yLines[0]);
			glVertex2i(xLines[x],yLines[yNum-1]);
		}
		for (int y = 0; y < xNum; y++){
			glVertex2i(xLines[0],yLines[y]);
			glVertex2i(xLines[xNum-1],yLines[y]);
		}
	glEnd();
	glFlush();
}

//Deals with mouse clicks for GLUT  
void mouse(int btn, int state, int x, int y){
	if(btn == GLUT_LEFT_BUTTON){
			//printf("LeftButton\n");
			if (state == GLUT_DOWN){
				//printArray(living, xNum, yNum);
				//printf("Pressed\n");
				//printf("MouseFunc coords: %i, %i\n", , );
				checkClick(x,600-y);
				//printArray(living, xNum, yNum);
			}
	}
}

//Prints the help interface 
void printHelp(){
	isPaused = true;
	printf("-------------------------------------------------------------------\n");
	printf("Welcome to Conways Game of Life!\n");
	printf("The game is currently paused. The game MUST BE PAUSED to do the following:\n");
	printf("	Click cells to fill them in.\n");
	printf("	Randomize the grid by pressing 'r' \n");
	printf("	Clear the board with 'c'\n");
	printf("Pressing 'p' will start and stop the simulation \n");
	printf("Press 's' to alter and print out the speed (max is 1250 mil) \n");
	printf("Pressing 'h' will redisplay help and pause the simulation\n");
	printf("Press 'q' to quit\n");
	printf("Enjoy!\n");
	printf("--------------------------------------------------------------------\n");
}

//Deals with keyboard presses for GLUT 
void keyboard(unsigned char key, int xIn, int yIn){
	switch (key){
		case 'c':
			if (isPaused)
				clear(living, xNum, yNum);
			break;
		case 'r':
			if (isPaused)
				randomize(living, xNum-1, yNum-1);
			break;
		case 's':
			speed = (speed+200)%1400; 
			printf("current speed = %i milliseconds\n",speed);
			break;
		case 'h':
			printHelp();
			break;
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		
		case 'p':
			isPaused = !isPaused; 
			if(isPaused)
				printf("Simulation is paused\n");
			else
				printf("Simulation is now running\n");
			break;
	}
}

//Deals with refresh for GLUT 
void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(speed, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

//Binds our interaction functions to the GLUT library 
void callBackInit(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);
}

// The main Method 
int main(int argc, char** argv){
	glutInit(&argc, argv);		//starts up GLUT
	int x = 40;
	int y = 40;
	int squareSize = 10;
	int x0 = 300 - (x*squareSize)/2;
	int y0 = 300 - (y*squareSize)/2;
	setGlobals(x+1,y+1, x0, y0, squareSize);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200,200);
	glutCreateWindow("Conways Game of Life: The Living Square Game");	//creates the window
	printHelp();
	callBackInit();	//registers "display" as the display callback function
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0 ,600);
	
	//createOurMenu();
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}