/*
 * Exercise 7 - hardware accelerated graphics.  Some library drawing functions are included below
 * like functionality for drawing rectangles, circles, lines, and text.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// #defined constants representing values we write to the graphics 'command' register to get
// it to do something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these

#define DrawHLine				1
#define DrawVLine				2
#define DrawLine				3
#define PutAPixel				0xA
#define GetAPixel				0xB
#define ProgramPaletteColour   	0x10

/*******************************************************************************************
** This macro pauses until the graphics chip status register indicates that it is idle
*******************************************************************************************/

#define WAIT_FOR_GRAPHICS		while((GraphicsStatusReg & 0x0001) != 0x0001);

//Predefined Colour Values
//Use the symbolic constants below as the values to write to the Colour Register
//When you ask the graphics chip to draw something. These values are also returned
//By the read pixel command

// the header file "Colours.h" contains symbolic names for all 256 colours e.g. RED
// while the source file ColourPaletteData.c contains the 24 bit RGB data
// that is pre-programmed into each of the 256 palettes

#define	BLACK		0
#define	WHITE		1
#define	RED			2
#define	LIME		3
#define	BLUE		4
#define	YELLOW		5
#define	CYAN		6
#define	MAGENTA		7

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480

#define GraphicsCommandReg   			(*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg   			(*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg   				(*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg   				(*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg   				(*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg					(*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg				(*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackGroundColourReg   	(*(volatile unsigned short int *)(0x84000010))

extern const unsigned int ColourPalletteData[256];

/**********************************************************************
* This function writes a single pixel to the x,y coords specified in the specified colour
* Note colour is a palette number (0-255) not a 24 bit RGB value
**********************************************************************/
void WriteAPixel (unsigned int x, unsigned int y, unsigned int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = PutAPixel;		// give graphics a "write pixel" command
}

/**********************************************************************
* This function writes a single pixel to the x,y coords specified in the specified colour
* Note colour is a palette number (0-255) not a 24 bit RGB value
* precondition: x2 > x1
**********************************************************************/
void WriteAHorzLine (unsigned int x1, unsigned int x2, unsigned int y, unsigned int Colour)
{
	if (x2 <= x1) return;               // assert precondition

	WAIT_FOR_GRAPHICS;			        // is graphics ready for new command

	GraphicsY1Reg = y;
	GraphicsX1Reg = x1;
	GraphicsX2Reg = x2;
	GraphicsColourReg = Colour;		    // set pixel colour with a palette number
	GraphicsCommandReg = DrawHLine;		// give graphics a "write pixel" command
}

/**********************************************************************
* This function writes a single pixel to the x,y coords specified in the specified colour
* Note colour is a palette number (0-255) not a 24 bit RGB value
* precondition: y2 > y1
**********************************************************************/
void WriteAVertLine (unsigned int y1, unsigned int y2, unsigned int x, unsigned int Colour)
{
	if (y2 <= y1) return;               // assert precondition

	WAIT_FOR_GRAPHICS;			   	    // is graphics ready for new command

	GraphicsX1Reg = x;
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;
	GraphicsColourReg = Colour;		    // set pixel colour with a palette number
	GraphicsCommandReg = DrawVLine;		// give graphics a "write pixel" command
}

/**
 * Function to draw a general direction line using Bresenhams line drawing algorithm.
 * precondition: y2 > y1
 * 				 x2 > x1
 */
void WriteALine(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, unsigned int Colour){
	if ((x2 <= x1) || (y2 <= y1)) return;               // assert precondition

	WAIT_FOR_GRAPHICS;			        // is graphics ready for new command

	GraphicsX1Reg = x1;
	GraphicsX2Reg = x2;
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;
	GraphicsColourReg = Colour;		    // set pixel colour with a palette number
	GraphicsCommandReg = DrawLine;		// give graphics a "write pixel" command
}

/*****************************************************************************************
* This function read a single pixel from x,y coords specified and returns its colour
* Note returned colour is a palette number (0-255) not a 24 bit RGB value
******************************************************************************************/
int ReadAPixel (unsigned int x, unsigned int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;		// return the palette number (colour)
}

/****************************************************************************************************
** subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
****************************************************************************************************/

void ProgramPalette(unsigned int PaletteNumber, unsigned int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16   ;          // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB ;                	 // program green and blue into 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour;	// issue command
}

/**
 * Initializes screen by programming colour palette and clearing screen.
 */
void InitScreen(){
	int i;
	srand(time(NULL));
	printf("Hello from Nios II!\n");
	for (i = 0; i < 10; i++){
		ProgramPalette(i, ColourPalletteData[i]);
	}
	printf("Programming the first nine enum'd colours into the colour palette.. Done\n");
	ClearScreen();
	printf("Clearing screen... Done\n");
}


/***************************************************************************
 * FUNCTIONS FOR DRAWING HIGHER ORDER SHAPES LIKE RECTANGLES AND CIRCLES
 ***************************************************************************/

/**
 * Clears the screen by fully writing black pixels.
 */
void ClearScreen(){
	int i;
	for(i = 0; i < 479; i++){
		WriteAHorzLine(0, 799, i, BLACK);
	}
}

/*
 * Function for drawing a rectangle to the screen.
 */
void WriteRectangle(unsigned int x, unsigned int y, unsigned int height, unsigned int width, unsigned int borderColor){
	WriteAHorzLine(x, x + width, y, borderColor);                  // top line
	WriteAHorzLine(x, x + width, y + height, borderColor);         // bottom line
	WriteAVertLine(y + 1, y + height - 1, x, borderColor);         // left line
	WriteAVertLine(y + 1, y + height - 1, x + width, borderColor); // right line
}

/**
 * Function for drawing a circle to the screen.
 * Taken from https://en.wikipedia.org/wiki/Midpoint_circle_algorithm.
 */
void WriteCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int colour){
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y){
        WriteAPixel(x0 + x, y0 + y, colour);
        WriteAPixel(x0 + y, y0 + x, colour);
        WriteAPixel(x0 - y, y0 + x, colour);
        WriteAPixel(x0 - x, y0 + y, colour);
        WriteAPixel(x0 - x, y0 - y, colour);
        WriteAPixel(x0 - y, y0 - x, colour);
        WriteAPixel(x0 + y, y0 - x, colour);
        WriteAPixel(x0 + x, y0 - y, colour);

        if (err <= 0){
            y++;
            err += dy;
            dy += 2;
        }else{
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

/**
 * Function for drawing random horizontal, vertical, and diagonal lines of random colors
 * to prove that they work.
 */
void WriteRandomLinesTest(int numLines){
	int randLineType, randColor, randX1, randX2, randY1, randY2, i;
	for (i = 0; i < numLines; i++){
		randLineType = rand() % 3; // there are three types of lines
		randColor = rand() % 10;    // choose randomly from the first 10 colors in enumeration
		randX1 = rand() % SCREEN_WIDTH;
		randX2 = rand() % SCREEN_WIDTH;
		randY1 = rand() % SCREEN_HEIGHT;
		randY2 = rand() % SCREEN_HEIGHT;

		switch(randLineType){
		case 0:
			WriteAHorzLine(randX1, randX2, randY1, randColor);
			break;
		case 1:
			WriteAVertLine(randY1, randY2, randX1, randColor);
			break;
		case 2:
			WriteALine(randX1, randX2, randY1, randY2, randColor);
			break;
		}
	}
	printf("Beginning random lines test... Done\n");
}

void WriteAString(int x, int y, char str[], int color, int big){
	int i = 0;
	while(str[i] != '\0'){
		if(big){
			OutGraphicsCharFont2(x + (i * 11), y, color, BLACK, str[i], 0);
		} else {
			OutGraphicsCharFont1(x + (i * 10), y, color, BLACK, str[i], 0);
		}
		i++;
	}
}

/**
 * Does a dumb test drawing some straight red lines.
 */
void WriteScriptedLinesTest(){
	int i;
	// draw a line across the screen in RED at y coord 100 and from x = 0 to 799
	for(i = 0; i < 100; i ++)
		WriteAPixel(i, 100, RED);

	// read the pixels back and make sure we read 2 (RED) to prove it's working
	for(i = 0; i < 100; i ++)
		printf("Colour value (i.e. pallette number) = %d at [%d, 100]\n", ReadAPixel(i, 100), i);

	// try and draw a hardware accelerated horizontal line
	WriteAHorzLine (0, 100, 200, RED);

	// check result of draw horz line
	for(i = 0; i < 100; i ++)
		printf("Colour value (i.e. pallette number) = %d at [%d, 200]\n", ReadAPixel(i, 200), i);

	// try and draw a hardware accelerated vertical line
	WriteAVertLine(0, 100, 100, RED);

	// check result of draw vert line
	for(i = 0; i < 100; i ++)
		printf("Colour value (i.e. pallette number) = %d at [0, %d]\n", ReadAPixel(0, i), i);

	printf("Beginning scripted lines test... Done\n");
}

int main()
{
	InitScreen();
	WriteRectangle(100, 100, 200, 400, CYAN);
	WriteCircle(200, 300, 100, MAGENTA);
	Fill(200, 300, RED, MAGENTA);

	char hello_string[] = {'H', 'e', 'l', 'l', 'o', '!', '\0'};
	WriteAString(400, 400, hello_string, YELLOW, 1);

	return 0;
}
