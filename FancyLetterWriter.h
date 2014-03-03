#pragma once
#include "GL\glaux.h"
#include "GL\glut.h"
#include <iostream>

class FancyLetterWriter
{
public:
	FancyLetterWriter();
	~FancyLetterWriter();
	void write(char *cp, float characterWidth, float characterHeight, float x, float y);
};

FancyLetterWriter::FancyLetterWriter()
{}

FancyLetterWriter::~FancyLetterWriter()
{}

void FancyLetterWriter::write(char *cp, float characterWidth, float characterHeight, float x, float y)
{
	int counter = 0;
	for(int i=0; cp[i] != '\0'; i++)
	{
		int cx; //Character x-position
		int cy; //Character y-position
		int charID = cp[i];
		charID -= 65;
		
		switch(charID)
		{
		case (46-65): //Dot
			charID = 36;
			break;
		case (45-65): //Dash
			charID = 37;
			break;
		case (33-65): //Exlamation
			charID = 38;
			break;
		case (63-65): //Question
			charID = 39;
			break;
		case (32-65): //Space
			charID = -1;
			break;
		case -17:	 //Zero
			charID = 35;
			break;
		default:
			if(charID >= -18 && charID <= -8)
			{
				charID += 42;
				break;
			}
			if(!(charID >= 0 && charID <= 25)) // Check valid range A-Z
			{
				continue; //Skip if invalid
			}
			break;
		}
		//Determine character position in image
		if(charID!=-1)
		{
			cx = charID % 8;
			cy = charID / 8;

			float rasterCharWidth = 1.0/8;
			float rasterCharHeight = 1.0/5;
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

			glBegin(GL_QUADS);
			glTexCoord2f(rasterCharWidth*(cx+1),			1.0 - rasterCharHeight*(cy));	// Top Left
			glVertex3f(x + (characterWidth*(counter+1)),	y + characterHeight, 0.0);
			glTexCoord2f(rasterCharWidth*(cx),				1.0 - rasterCharHeight*(cy));	// Bottom Left	
			glVertex3f(x + (characterWidth*(counter)),		y + characterHeight, 0.0);
			glTexCoord2f(rasterCharWidth*(cx),				1.0 - rasterCharHeight*(cy+1));	// Bottom Right
			glVertex3f(x + (characterWidth*(counter)),		y, 0.0);					
			glTexCoord2f(rasterCharWidth*(cx+1),			1.0 - rasterCharHeight*(cy+1));	// Top Right
			glVertex3f(x + (characterWidth*(counter+1)),	y,	0.0);
			
			glEnd();
		}
		counter++;
	}
}



	