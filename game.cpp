#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_
#include <iostream>
#include<string>
#include<cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <sstream>
#include<vector>
#include"colors.h"
using namespace std;
#define KEY_ESC 27
////////////////////////////////////////////////////////////////////////////////////////////////////////////
float multiplier=1;
int turns=20;
int tot_turns=20;
int playcheck=0;
int timecheck=0;
int normalcheck=0;
int levelcheck;
int boardcheck=0;
int level=1;
int playerscore=0;
int totalScore=500;
float width=0.0;
int board[9][9]={0};
int i;
string name;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int nvertices = 722;
GLfloat vertices[nvertices][2];
float MIN(int A,int B)
{
	if(A<B)
	{
		return A;
	}
	else
	{
		return B;
	}
	
}
void InitCircleVertices(float radius) {

	float hdegree = M_PI / 360.0;
	float angle = 0;
	for (int i = 0; i < nvertices; ++i) {
		vertices[i][0] = radius * cos(angle);
		vertices[i][1] = radius * sin(angle);
		angle += hdegree;
	}
}
void DrawCircle(float sx, float sy, float radius, float*color) 
{
	glColor3fv(color); // set the circle color
	InitCircleVertices(radius);
	glBegin(GL_TRIANGLE_FAN);
	glVertex4f(sx, sy, 0, 1);
	for (int i = 0; i < nvertices; ++i)
		glVertex4f(sx + vertices[i][0], sy + vertices[i][1], 0, 1);
	glEnd();
}
void DrawSquare(int sx, int sy, int size,float color[]) 
{
	int mx=size,my=size;
	glColor3fv(color); // specify the color of the brick
	glBegin(GL_TRIANGLES);
	glVertex4f(sx, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy + my - 1, 0, 1);
	glVertex4f(sx, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy + my - 1, 0, 1);
	glVertex4f(sx, sy + my - 1, 0, 1);
	glEnd();
}
void DrawString(float x, float y, const string& score, float * color) 
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	if (color)
		glColor3fv(color);
	glRasterPos3f(x, y, 1);
	//  Draw the characters one by one
	for (int i = 0; i < score.size(); i++)
		glutBitmapCharacter(font_style, score[i]);
	glPopMatrix();
}
void DrawString(int x, int y, int width, int height, const string &score,float*color) 
{
	float fx = (float)x / width * 2 - 1, fy = (float)y / height * 2 - 1;
	DrawString(fx, fy, score, color);
}
void InitRandomizer() 
{
	srand((unsigned int)time(0));
}
long GetRandInRange(const long &rmin, const long &rmax) 
{
	long range = rmax - rmin;
	long value = (rand() % (long)range) + rmin;
	return value;
}void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, float *color);
void DrawRectangle(int sx, int sy, int mx, int my, float *color) 
{
	glColor3fv(color); // specify the color of the rectangle
	glBegin(GL_TRIANGLES);
	glVertex4f(sx, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy + my - 1, 0, 1);
	glVertex4f(sx, sy, 0, 1);
	glVertex4f(sx + mx - 1, sy + my - 1, 0, 1);
	glVertex4f(sx, sy + my - 1, 0, 1);
	glEnd();
}
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,float color[])
{
	glColor3fv(color); // Set the triangle colour
	// ask library to draw triangle at given position...
	glBegin(GL_TRIANGLES);
	/*Draw triangle using given three vertices...*/
	glVertex4i(x1, y1, 0, 1);
	glVertex4i(x2, y2, 0, 1);
	glVertex4i(x3, y3, 0, 1);
	glEnd();
}
void DrawLine(int x1, int y1, int x2, int y2, int lwidth, float *color) 
{
	glLineWidth(lwidth);
	if (color)
		glColor3fv(color);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0);
	glEnd();
}
//http://stackoverflow.com/questions/5369507/opengles-1-0-2d-rounded-rectangle
#define ROUNDING_POINT_COUNT 16
typedef struct {
	float x;
	float y;

} Vector2f;
void DrawRoundRect(float x, float y, float width, float height, float* color,float radius) {
	y = y + height;
	Vector2f top_left[ROUNDING_POINT_COUNT];
	Vector2f bottom_left[ROUNDING_POINT_COUNT];
	Vector2f top_right[ROUNDING_POINT_COUNT];
	Vector2f bottom_right[ROUNDING_POINT_COUNT];
	if (radius == 0.0) 
	{
		radius = MIN(width, height);
		radius *= 0.10; // 10%
	}
	int i = 0;
	float x_offset, y_offset;
	float step = (2.0f * M_PI) / (ROUNDING_POINT_COUNT * 4), angle = 0.0f;
	unsigned int index = 0, segment_count = ROUNDING_POINT_COUNT;
	Vector2f bottom_left_corner = { x + radius, y - height + radius };
	while (i != segment_count) 
	{
		x_offset = cosf(angle);
		y_offset = sinf(angle);
		top_left[index].x = bottom_left_corner.x - (x_offset * radius);
		top_left[index].y = (height - (radius * 2.0f)) + bottom_left_corner.y - (y_offset * radius);
		top_right[index].x = (width - (radius * 2.0f)) + bottom_left_corner.x + (x_offset * radius);
		top_right[index].y = (height - (radius * 2.0f)) + bottom_left_corner.y - (y_offset * radius);
		bottom_right[index].x = (width - (radius * 2.0f)) + bottom_left_corner.x + (x_offset * radius);
		bottom_right[index].y = bottom_left_corner.y + (y_offset * radius);
		bottom_left[index].x = bottom_left_corner.x - (x_offset * radius);
		bottom_left[index].y = bottom_left_corner.y + (y_offset * radius);
		angle -= step;
		++index;
		++i;
	}
	static GLubyte clr[] = { 156, 207, 255, 128 }; // Light blue, 50% transparent.
	if (color)
		glColor4fv(color);
	else
		glColor4ubv(clr);
	glBegin(GL_TRIANGLE_STRIP);
	{
		// Top
		for (i = segment_count - 1; i >= 0; i--) 
		{
			glVertex2f(top_left[i].x, top_left[i].y);
			glVertex2f(top_right[i].x, top_right[i].y);
		}

		// In order to stop and restart the strip.
		glVertex2f(top_right[0].x, top_right[0].y);
		glVertex2f(top_right[0].x, top_right[0].y);

		// Center
		glVertex2f(top_right[0].x, top_right[0].y);
		glVertex2f(top_left[0].x, top_left[0].y);
		glVertex2f(bottom_right[0].x, bottom_right[0].y);
		glVertex2f(bottom_left[0].x, bottom_left[0].y);

		// Bottom
		for (i = 0; i != segment_count; i++) 
		{
			glVertex2f(bottom_right[i].x, bottom_right[i].y);
			glVertex2f(bottom_left[i].x, bottom_left[i].y);
		}
	}
	glEnd();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void increasemultiplyer()
{
    multiplier+=0.01;
}
void decreaseturns()
{
	turns-=1;     //turns are decremented 
}
void increasescore(int n)
{
    playerscore+=(n*10*multiplier);    //player score increases
}
void increasewidth(int n)
{
    width+=((n*10*multiplier)*390)/totalScore;
}
void LoadMenu()     //Initial start screen
{
    
    DrawRectangle(0,0,400,600,colors[DARK_RED]);
    int xaxis=0;
    int yaxis=0;
    for (int i = 0; i < 100; i++)
	{
		
		DrawLine( xaxis, 0 , xaxis, 600 , 1 , colors[ BLACKO]);
		xaxis=xaxis+10;
		
		DrawLine( 0 , yaxis,  400 , yaxis, 1 , colors[BLACKO]);
		yaxis=yaxis+10;
	}
    
    DrawRoundRect(153,273,100,50,colors[MEDIUM_TURQUOISE],10);
    DrawString(170,290,"PLAY",colors[DARK_VIOLET]);
    DrawRectangle(0,0,600,30,colors[WHITE]);
    DrawString(70,6,"PRESS ESCAPE TO EXIT",colors[BLACK]);
}
void LoadSubMenu()   //Click to start the game 
//Initial start screen before game 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
    glColor3f(0.66, 0.12, 0.25);
    glVertex2f(400, 600);
    glVertex2f(0, 600);
	glColor3f(0.88,0.12,0.25);
	glVertex2f(0, 0); 
    glVertex2f(400, 0);       
    glEnd();
    int xaxis=0;
    int yaxis=0;
    for (int i = 0; i < 100; i++)
	{
		
		DrawLine( xaxis, 0 , xaxis, 600 , 1 , colors[ BLACKO]);
		xaxis=xaxis+10;
		
		DrawLine( 0 , yaxis,  400 , yaxis, 1 , colors[BLACKO]);
		yaxis=yaxis+10;
	}
    DrawString(120,305,"NOW PLAYING",colors[WHITE]);
    DrawString(110,270,"NORMAL MODE",colors[WHITE]);  
}
void LoadSubMenu1()   //incase player wins 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
    glColor3f(0.66, 0.12, 0.25);
    glVertex2f(400, 600);
    glVertex2f(0, 600);
	glColor3f(0.88,0.12,0.25);
	glVertex2f(0, 0); 
    glVertex2f(400, 0);       
    glEnd();
    int xaxis=0;
    int yaxis=0;
    for (int i = 0; i < 100; i++)
	{
		
		DrawLine( xaxis, 0 , xaxis, 600 , 1 , colors[ BLACKO]);
		xaxis=xaxis+10;
		
		DrawLine( 0 , yaxis,  400 , yaxis, 1 , colors[BLACKO]);
		yaxis=yaxis+10;
	}
    DrawString(150,305,"WINNER",colors[WHITE]);
    DrawString( 40, 520, "CLICK HERE TO CONTINUE ", colors[WHITE]);
}
void LoadSubMenu2()   //incase player loses
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
    glColor3f(0.66, 0.12, 0.25);
    glVertex2f(400, 600);
    glVertex2f(0, 600);
	glColor3f(0.88,0.12,0.25);
	glVertex2f(0, 0); 
    glVertex2f(400, 0);       
    glEnd();
    int xaxis=0;
    int yaxis=0;
    for (int i = 0; i < 100; i++)
	{
		
		DrawLine( xaxis, 0 , xaxis, 600 , 1 , colors[ BLACKO]);
		xaxis=xaxis+10;
		
		DrawLine( 0 , yaxis,  400 , yaxis, 1 , colors[BLACKO]);
		yaxis=yaxis+10;
	}
    DrawString(150,305,"LOSER!",colors[WHITE]);
    DrawString( 40, 520, "CLICK HERE TO RESTART", colors[WHITE]);
}
void setplaycheck(int n)
{
    playcheck=n;
}
void settimecheck(int n)
{
    timecheck=n;
}
void setnormalcheck(int n)
{
    normalcheck=n;
}
void setBoardcheck(int n)
{
    boardcheck=n;
}
void BoardRandomizer()   //places random candies in the grid
{
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				board[i][j]=GetRandInRange(0,6);
			}
		}
}
void BoardDrawer()   //Main brain of the program
{	DrawRectangle(0,300,400,300,colors[BROWN]);
    DrawRectangle(0,0,400,300,colors[SANDY_BROWN]);
	int xaxis1=0;
    int yaxis1=0;
    for (int i = 0; i < 100; i++)
	{
		
		DrawLine( xaxis1, 0 , xaxis1, 600 , 1 , colors[ BLACKO]);
		xaxis1=xaxis1+10;
		
		DrawLine( 0 , yaxis1,  400 , yaxis1, 1 , colors[BLACKO]);
		yaxis1=yaxis1+10;
	}
	DrawRoundRect(35,139,335,332,colors[TAN],5);
	DrawRoundRect(43,146,318,318,colors[SKY_BLUE],5);
	int xaxis=46;
	int yaxis=150;
	for (int i = 0; i < 10; i++)
	{	
		DrawLine( xaxis-1, 465 , xaxis-1 , 147 , 1 , colors[MISTY_ROSE]);
		xaxis=xaxis+35;
		DrawLine( 43 , yaxis+312,  361 , yaxis+312 , 1 , colors[MISTY_ROSE]);
		yaxis=yaxis-35;
	}
	DrawCircle(55,543,47,colors[GRAY]);
	DrawCircle(55,543,41,colors[WHITE]);
	DrawCircle(55,560,20,colors[BLACK]);
	DrawRectangle(48,515,15,30,colors[BLACK]);
	DrawRoundRect(32,511,45,25,colors[BLACK],5);
	for(int i =0,x=47; i<9;x+=35,i++)
	{  
  		for(int y=150,j=0;j<9;j++,y+=35)
    	{
 			if(board[i][j]==0)   //plain red candy
			{
				DrawSquare(x,y,30,colors[RED]);
			}
		   	if(board[i][j]==1)  //plain yellow candy
			{
            	DrawCircle(x+15,y+15,15,colors[YELLOW]);
			}
			if(board[i][j]==2)   //plain green candy
			{
            	DrawRoundRect(x,y,30,30,colors[GREEN],7);
			}
        	if(board[i][j]==3)  //plain blue candy
			{
				DrawTriangle(x,y,x+30,y,x+15,y+30,colors[BLUE]);  
			} 
			if(board[i][j]==4)   //plain candy gold 
			{ 
				DrawTriangle( x+5, y+15 , x+25 , y+15 , x+15 , y+30 , colors[GOLD] );
				DrawTriangle( x+5, y+15 , x+25 , y+15 , x+15 , y+0 , colors[GOLD] );  
			} 
			if(board[i][j]==5)    //plain candy brown 
			{
				DrawRoundRect(x,y+7.5,30,15,colors[BROWN],7);
				DrawRoundRect(x+7.5,y,15,30,colors[BROWN],7);
			}
			if(board[i][j]==6)   //stripped candy 
			{
				DrawSquare(x,y,30,colors[RED]);
				DrawSquare(x+5,y+5,20,colors[BLUE]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);

			}
			if(board[i][j]==7)    //stripped candy 
			{
				DrawSquare(x,y,30,colors[GREEN]);
				DrawSquare(x+5,y+5,20,colors[BLACK]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);
			}
			if(board[i][j]==8)    //stripped candy 
			{
				DrawSquare(x,y,30,colors[GREEN]);
				DrawSquare(x+5,y+5,20,colors[BLACK]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);
			}
			if(board[i][j]==9)   //stripped candy 
			{
				DrawSquare(x,y,30,colors[GREEN]);
				DrawSquare(x+5,y+5,20,colors[BLACK]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);
			}
			if(board[i][j]==10)  //stripped candy 
			{
				DrawSquare(x,y,30,colors[GREEN]);
				DrawSquare(x+5,y+5,20,colors[BLACK]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);
			} 
			if(board[i][j]==11)    //stripped candy 
			{
				DrawSquare(x,y,30,colors[GREEN]);
				DrawSquare(x+5,y+5,20,colors[BLACK]);
				DrawSquare(x+10,y+10,10,colors[GREEN]);
			}
			if(board[i][j]==12)  //color bomb
			{
				DrawCircle(x+15,y+15,15,colors[YELLOW]);
				DrawCircle(x+15,y+15,10,colors[RED]);
			}
			if(board[i][j] == 13)  //wrapped candy 
			{
				DrawCircle(x+15,y+15,15,colors[BLACK]);
				DrawCircle(x+15,y+15,10,colors[RED]);
			}
    	}
	}

	DrawString(105,530,name,colors[WHITE]);
	DrawString(290,570,"LEVEL: "+std::to_string(level),colors[WHITE]);
	DrawString(3,60,"TARGET: "+ std::to_string(totalScore),colors[BLACK]);
	DrawString(340,60,"X"+std::to_string(multiplier),colors[BLACK]);
	DrawString(5,30,"YOUR SCORE: "+std::to_string(playerscore),colors[BLACK]);
	DrawString(280,550,"TURNS: "+std::to_string(turns),colors[WHITE]);
	DrawRectangle(5,5,395,20,colors[WHITE]);
	DrawRectangle(10,8,width,14,colors[RED]);
}
void deletion(int y1, int x1)   //deletes the candies from the grid accordingly
{

	if (board[y1][x1]==board[y1][x1+1] && board[y1][x1]==board[y1][x1+2] && board[y1][x1]==board[y1][x1+3] && board[y1][x1]==board[y1][x1+4])
	{
		decreaseturns();
		increasewidth(5);
		increasescore(5);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1+1]=(rand() % 6);
		board[y1][x1+2]=(rand() % 6);
		board[y1][x1+3]=(rand() % 6);
		board[y1][x1+4]=12;
	}
	if (board[y1][x1]==board[y1][x1+1] && board[y1][x1]==board[y1][x1+2] && board[y1][x1]==board[y1][x1+3])
	{
		decreaseturns();
		increasewidth(4);
		increasescore(4);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1+1]=(rand() % 6);
		board[y1][x1+2]=(rand() % 6);
		int y;
		srand( (unsigned)time( NULL ) );
		y= 1 + (rand() % 6);
		if(y==1)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=6;
			}
		}
		if(y==2)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=7;
			}
		}
		if(y==3)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=8;
			}
		}
		if(y==4)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=9;
			}
		}
		if(y==5)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=10;
			}
		}
		if(y==6)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1+3]=11;
			}
		}

	}
	if (board[y1][x1]==board[y1][x1+1] && board[y1][x1]==board[y1][x1+2])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1+1]=(rand() % 6);
		board[y1][x1+2]=(rand() % 6);
	}
	if (board[y1][x1]==board[y1][x1-1] && board[y1][x1]==board[y1][x1-2] && board[y1][x1]==board[y1][x1-3] && board[y1][x1]==board[y1][x1-4])
	{
		decreaseturns();
		increasewidth(5);
		increasescore(5);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1-1]=(rand() % 6);
		board[y1][x1-2]=(rand() % 6);
		board[y1][x1-3]=(rand() % 6);
		board[y1][x1-4]=12;
	}
	if (board[y1][x1]==board[y1][x1-1] && board[y1][x1]==board[y1][x1-2] && board[y1][x1]==board[y1][x1-3])
	{
		decreaseturns();
		increasewidth(4);
		increasescore(4);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1-1]=(rand() % 6);
		board[y1][x1-2]=(rand() % 6);
		int y;
		srand( (unsigned)time( NULL ) );
		y= 1 + (rand() % 6);
		if(y==1)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=6;
			}
		}
		if(y==2)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=7;
			}
		}
		if(y==3)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=8;
			}
		}
		if(y==4)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=9;
			}
		}
		if(y==5)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=10;
			}
		}
		if(y==6)
		{
			for(int i=0;i<9;i++)
			{
				board[y1][x1-3]=11;
			}
		}
	}
	if (board[y1][x1]==board[y1][x1-1] && board[y1][x1]==board[y1][x1-2])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1-1]=(rand() % 6);
		board[y1][x1-2]=(rand() % 6);
	}
	if (board[y1][x1]==board[y1][x1-1] && board[y1][x1]==board[y1][x1+1])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1][x1-1]=(rand() % 6);
		board[y1][x1+1]=(rand() % 6);
	}
	

	//Yaxis
	if (board[y1][x1]==board[y1+1][x1] && board[y1][x1]==board[y1+2][x1] && board[y1][x1]==board[y1+3][x1] && board[y1][x1]==board[y1+4][x1])
	{
		decreaseturns();
		increasewidth(5);
		increasescore(5);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1+1][x1]=(rand() % 6);
		board[y1+2][x1]=(rand() % 6);
		board[y1+3][x1]=(rand() % 6);
		board[y1+4][x1]=12;
	}
	if (board[y1][x1]==board[y1+1][x1] && board[y1][x1]==board[y1+2][x1] && board[y1][x1]==board[y1+3][x1])
	{
		decreaseturns();
		increasewidth(4);
		increasescore(4);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1+1][x1]=(rand() % 6);
		board[y1+2][x1]=(rand() % 6);
		int y;
		srand( (unsigned)time( NULL ) );
		y= 1 + (rand() % 6);
		if(y==1)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=6;
			}
		}
		if(y==2)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=7;
			}
		}
		if(y==3)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=8;
			}
		}
		if(y==4)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=9;
			}
		}
		if(y==5)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=10;
			}
		}
		if(y==6)
		{
			for(int i=0;i<9;i++)
			{
				board[y1+3][x1]=11;
			}
		}
	}
	if (board[y1][x1]==board[y1+1][x1] && board[y1][x1]==board[y1+2][x1])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1+1][x1]=(rand() % 6);
		board[y1+2][x1]=(rand() % 6);
	}
	if (board[y1][x1]==board[y1-1][x1] && board[y1][x1]==board[y1-2][x1] && board[y1][x1]==board[y1-3][x1] && board[y1][x1]==board[y1-4][x1])
	{
		decreaseturns();
		increasewidth(5);
		increasescore(5);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1-1][x1]=(rand() % 6);
		board[y1-2][x1]=(rand() % 6);
		board[y1-3][x1]=(rand() % 6);
		board[y1-4][x1]=12;
	}
	if (board[y1][x1]==board[y1-1][x1] && board[y1][x1]==board[y1-2][x1] && board[y1][x1]==board[y1-3][x1])
	{
		decreaseturns();
		increasewidth(4);
		increasescore(4);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1-1][x1]=(rand() % 6);
		board[y1-2][x1]=(rand() % 6);
		int y;
		srand( (unsigned)time( NULL ) );
		y= 1 + (rand() % 6);
		if(y==1)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=6;
			}
		}
		if(y==2)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=7;
			}
		}
		if(y==3)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=8;
			}
		}
		if(y==4)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=9;
			}
		}
		if(y==5)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=10;
			}
		}
		if(y==6)
		{
			for(int i=0;i<9;i++)
			{
				board[y1-3][x1]=11;
			}
		}
	}
	if (board[y1][x1]==board[y1-1][x1] && board[y1][x1]==board[y1-2][x1])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1-1][x1]=(rand() % 6);
		board[y1-2][x1]=(rand() % 6);
	}
	if (board[y1][x1]==board[y1-1][x1] && board[y1][x1]==board[y1+1][x1])
	{
		decreaseturns();
		increasewidth(3);
		increasescore(3);
		increasemultiplyer();
		board[y1][x1]=(rand() % 6);
		board[y1-1][x1]=(rand() % 6);
		board[y1+1][x1]=(rand() % 6);
	}
}
void SwipeCoSend(int oldi,int oldj,int newi,int newj)
{
	if(board[oldi][oldj] == 13 && board[newi][newj] != board[oldi][oldj])  //wrapped candy code 
	{
	swap(board[oldi][oldj],board[newi][newj]);
	swap(board[oldi][oldj],board[newi][newj]);
	board[newi][newj] =(rand() % 6);
	board[newi][newj+1] = (rand() % 6);
	board[newi][newj-1] = (rand() % 6);
	board[newi+1][newj] = (rand() % 6);
	board[newi-1][newj] = (rand() % 6);
	board[newi+1][newj+1] = (rand() % 6);
	board[newi-1][newj+1] = (rand() % 6);
	board[newi-1][newj-1] = (rand() % 6);
	board[newi+1][newj-1] = (rand() % 6);
	
	}
	if(board[oldi][oldj]==12 && board[newi][newj]==12)   //color bomb with color bomb 
	{
		--turns;
		BoardRandomizer();
		increasewidth(81);
		increasescore(81);
		increasemultiplyer();
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==6) ||(board[oldi][oldj]==6 && board[newi][newj]==12) )  //color bomb with stripped candy
	{
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==0)
				{
					board[i][j]=6;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==6)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[newi][i]=(rand() % 6);
					}
				}
			}
		}
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==7) ||(board[oldi][oldj]==7 && board[newi][newj]==12) )   //color bomb with stripped candy 
	{ 
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==1)
				{
					board[i][j]=7;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==7)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[i][newj]=(rand() % 6);
					}
				}
			}
		}
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==8) ||(board[oldi][oldj]==8 && board[newi][newj]==12) )   //color bomb with stripped candy 
	{
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==2)
				{
					board[i][j]=8;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==8)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[newi][i]=(rand() % 6);
					}
				}
			}
		}
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==9) ||(board[oldi][oldj]==9 && board[newi][newj]==12) )   //color bomb with stripped candy 
	{
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==3)
				{
					board[i][j]=9;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==9)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[i][newj]=(rand() % 6);
					}
				}
			}
		}
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==10) ||(board[oldi][oldj]==10 && board[newi][newj]==12) )   //color bomb with stripped candy 
	{
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==4)
				{
					board[i][j]=10;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==10)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[newi][i]=(rand() % 6);
					}
				}
			}
		}
	}
	if((board[oldi][oldj]==12 && board[newi][newj]==11) ||(board[oldi][oldj]==11 && board[newi][newj]==12) )   //color bomb with stripped candy 
	{
		--turns;
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==5)
				{
					board[i][j]=11;
				}
			}
		}
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j]==11)
				{
					increasewidth(9);
					increasescore(9);
					increasemultiplyer();
					for(int i=0;i<9;i++)
					{
						board[i][newj]=(rand() % 6);
					}
				}
			}
		}
	}
	
	if(board[oldi][oldj]==12 && board[oldi][oldj]!=board[newi][newj])
	{  //color bomb code randomiser 
		--turns;
		int x=board[newi][newj];
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				//srand( (unsigned)time( 0 ) );
				int k=GetRandInRange(0,6);
				if(board[i][j]==x)
				{
					board[i][j]=k;
					increasewidth(1);
					increasescore(1);
					for(int i = 0; i < 6; i++)
					{
						srand( (unsigned)time( 0 ) );
						board[newi][newj]=(rand() % 6);
					}
				}
			}
		}
		increasemultiplyer();
		//board[newi][newj]=(rand() % 6);
	}
	if(board[oldi][oldj]==6 && board[oldi][oldj]!=board[newi][newj])
	{ //stripped candy swipe vertical or horizontal 
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[newi][i]=(rand() % 6);
		}
	}
	if(board[oldi][oldj]==7 && board[oldi][oldj]!=board[newi][newj])
	{  //stripped candy swipe vertical or horizontal 
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[i][newj]=(rand() % 6);
		}
	}

	if(board[oldi][oldj]==8 && board[oldi][oldj]!=board[newi][newj])
	{   //stripped candy swipe vertical or horizontal  
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[newi][i]=(rand() % 6);
		}
	}

	if(board[oldi][oldj]==9 && board[oldi][oldj]!=board[newi][newj])
	{   //stripped candy swipe vertical or horizontal 
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[i][newj]=(rand() % 6);
		}
	}
	if(board[oldi][oldj]==10 && board[oldi][oldj]!=board[newi][newj])
	{   //stripped candy swipe vertical or horizontal  
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[newi][i]=(rand() % 6);
		}
	}
	if(board[oldi][oldj]==11 && board[oldi][oldj]!=board[newi][newj])
	{   //stripped candy swipe vertical or horizontal 
		--turns;
		increasewidth(9);
		increasescore(9);
		increasemultiplyer();
		swap(board[oldi][oldj],board[newi][newj]);
		for(int i=0;i<9;i++)
		{
			board[i][newj]=(rand() % 6);
		}
	}
	if (board[oldi][oldj]== board[oldi][oldj+2] && board[oldi][oldj]== board[oldi][oldj+3])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi][oldj-2] && board[oldi][oldj]== board[oldi][oldj-3])
	{
		if (board[oldi][oldj]== board[newi][newj-1] && board[oldi][oldj]== board[newi][newj-2])
		{
		swap(board[oldi][oldj],board[newi][newj]);
		deletion(newi,newj);
		}
	}

   	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi+1][oldj+2])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi+1][oldj-1] && board[oldi][oldj]== board[oldi+1][oldj-2])
	{
		if (board[oldi][oldj]== board[newi][newj-1] && board[oldi][oldj]== board[newi][newj-2])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
				
	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi+1][oldj-1])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj-1])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-1][oldj-1])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj-1])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
			}
	}

	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-1][oldj+2])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi-1][oldj-1] && board[oldi][oldj]== board[oldi-1][oldj-2])
	{
		if (board[oldi][oldj]== board[newi][newj-1] && board[oldi][oldj]== board[newi][newj-2])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-1][oldj+2])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi+2][oldj] && board[oldi][oldj]== board[oldi+3][oldj])
	{
		if (board[oldi][oldj]== board[newi+1][newj] && board[oldi][oldj]== board[newi+2][newj])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);		
		}
	}

	if (board[oldi][oldj]== board[oldi-2][oldj] && board[oldi][oldj]== board[oldi-3][oldj])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi-2][newj])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);		
		}
	}

   	if (board[oldi][oldj]== board[oldi-1][oldj-1] && board[oldi][oldj]== board[oldi-2][oldj-1])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi-2][newj])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
		deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-2][oldj+1])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi-2][newj])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-2][oldj+1])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi-2][newj])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi+2][oldj+1])
	{
		if (board[oldi][oldj]== board[newi+1][newj] && board[oldi][oldj]== board[newi+2][newj])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
}

	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi-1][oldj+1])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi+1][newj])
		{	
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}

	if (board[oldi][oldj]== board[oldi+1][oldj-1] && board[oldi][oldj]== board[oldi-1][oldj-1])
	{
		if (board[oldi][oldj]== board[newi-1][newj] && board[oldi][oldj]== board[newi+1][newj])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi][oldj+2] && board[oldi][oldj]== board[oldi][oldj+3] && board[oldi][oldj] == board[oldi+1][oldj -1] && board[oldi][oldj] == board[oldi+ 1][oldj-2])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	
	if (board[oldi][oldj]== board[oldi-1][oldj+2] && board[oldi][oldj]== board[oldi-1][oldj+3] && board[oldi][oldj] == board[oldi][oldj -2] && board[oldi][oldj] == board[oldi][oldj-3])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	
	if (board[oldi][oldj]== board[oldi+1][oldj-1] && board[oldi][oldj]== board[oldi-1][oldj-1] && board[oldi][oldj] == board[oldi-1][oldj -2] && board[oldi][oldj] == board[oldi-1][oldj-3])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	
	if (board[oldi][oldj]== board[oldi-1][oldj-1] && board[oldi][oldj]== board[oldi-2][oldj-1] && board[oldi][oldj] == board[oldi-2][oldj -2] && board[oldi][oldj] == board[oldi-2][oldj-3])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj]== board[oldi-2][oldj+1] && board[oldi][oldj] == board[oldi-2][oldj] && board[oldi][oldj] == board[oldi-2][oldj-1])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi-1][oldj+1] && board[oldi][oldj] == board[oldi-1][oldj] && board[oldi][oldj] == board[oldi-1][oldj-1])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi+1][oldj-1] && board[oldi][oldj] == board[oldi+2][oldj+1] && board[oldi][oldj] == board[oldi+3][oldj+1])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	if (board[oldi][oldj]== board[oldi+1][oldj+1] && board[oldi][oldj]== board[oldi+1][oldj+2] && board[oldi][oldj] == board[oldi+2][oldj+2] && board[oldi][oldj] == board[oldi+3][oldj+2])
	{
		if (board[oldi][oldj]== board[newi][newj+1] && board[oldi][oldj]== board[newi][newj+2])
		{		
			swap(board[oldi][oldj],board[newi][newj]);
			deletion(newi,newj);
		}
	}
	

}
bool coordinatechecker(int oi,int oj,int X1,int X2,int Y1,int Y2)
{
    if(oi>X1 && oj<X2 && oj>Y1 && oi<Y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameDisplay(){
	glClearColor(0.0,0.0,0.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT);
	LoadMenu();
	if(playcheck==1)
	{
		LoadSubMenu();
	}
	if(timecheck==1 && normalcheck==0)
	{
		DrawRectangle(0,0,400,600,colors[BLACK]);
	}
	if(normalcheck==1 && timecheck==0)
	{}
	if(boardcheck==1)
	{
		BoardDrawer();
	}
	if(turns==0 && playerscore<totalScore)   //Code if the number of moves are finished and the player loses 
	{
		tot_turns=20;
		playcheck=0;
		timecheck=0;
		normalcheck=0;
		turns=20;
		level=1;
		totalScore=500;
		playerscore=0;
		multiplier=1;
		width=0;
		LoadMenu();
		LoadSubMenu2();
		BoardRandomizer();
	}
	if(playerscore>=totalScore)  //Code block if the player attains target score within 20 moves 
	{
		LoadSubMenu1();
		tot_turns = 20;
		turns=tot_turns;
		level++;
		totalScore+=200;
		playerscore=0;
		multiplier=1;
		width=0;
		BoardRandomizer();
	}
	glutSwapBuffers();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27)
	{
		exit(1);
	}
	glutPostRedisplay();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer(int m) 
{
	glutTimerFunc(1000.0, Timer, 0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseClicked(int button, int state, int x, int y) 
{
	int static mx1, my1,mx2, my2;
	if (button == GLUT_LEFT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{
			mx1 =x;
			my1 =y;
		}
		else if(state== GLUT_UP)
		{ 
			mx2 =x;
			my2=y;
         	
			int oi =(( mx1-35 )/35);
			int oj = ((  600 -my1-140)/35);
			int ni = (( mx2-35)/35);
			int nj = ((  600 -my2-140)/35);

			int static pcheck;
			int static normalcheck;
			int static timecheck;
			int static levelcheck;
			int static boardcheck;
			pcheck=0;
			normalcheck=0;
			timecheck=0;
			boardcheck=0;
			if(levelcheck<1)
			{
				if(coordinatechecker(oi,oj,3,6,3,6))
				{
					pcheck++;
					setplaycheck(pcheck);
				}
				if (coordinatechecker(oi,oj,0,10,5,12))
				{
					timecheck++;
					settimecheck(timecheck);
				}
				if (coordinatechecker(oi,oj,0,10,-3,3))
				{
					normalcheck++;
					setnormalcheck(normalcheck);
				}
			}
			if( (normalcheck>0 || timecheck>0) && coordinatechecker(oi,oj,0,10,-3,12))
			{
				boardcheck++;
				setBoardcheck(boardcheck);
			}
			
			if (oi> -1 && oi< 9 && oj> -1 && oj< 9)
			{
				SwipeCoSend(oi,oj,ni,nj);
			}
		}
	} 
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
	}
	glutPostRedisplay();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char*argv[]) {
	string str;
	cout<<"Enter name: ";
	cin>>name;
	int width = 400, height = 600;
	InitRandomizer();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(400, 50);
	glutInitWindowSize(width, height);
	glutCreateWindow("OOP CRUSH");
	SetCanvasSize(width, height);
	BoardRandomizer();
	glutDisplayFunc(GameDisplay);
	glutKeyboardFunc(PrintableKeys);
	glutTimerFunc(1000.0, Timer, 0);
	glutMouseFunc(MouseClicked);
	glutMainLoop();
	return 1;
}
#endif