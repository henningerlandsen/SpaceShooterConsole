#pragma once

#include "GL\glaux.h"
#include "GL\glut.h"
#include <stdio.h>
#include <iostream>
#include "FancyLetterWriter.h"

using namespace std;

//#define MAX_TEXTURES 14

class DrawingAssistant : public FancyLetterWriter
{
public:

	typedef struct Drawing
	{
		float x;
		float y;
		float width;
		float height;
		float rot;
	};

	enum TextureNum { 
		T_SPACESHIP, 
		T_ROCKETPARTICLE, 
		T_STAR, 
		T_PARTICLE, 
		T_PARTICLE_RGB,
		T_PARTICLE_CLOUD,
		T_PARTICLE_CONSETRATE,
		T_NME_SAW_INNER, 
		T_NME_SAW_BLADE, 
		T_NME_MINE,
		T_NME_MINE_SPIKE,
		T_EM_UPPER, 
		T_EM_LOWER, 
		T_EM_LASER, 
		T_EM_TUBE, 
		T_XPL_RING, 
		T_XPL_STAR, 
		T_XPL_BALL,
		T_UPGRADE_PANEL,
		T_UPGRADE_TOP,
		T_UP_SHIELD,
		T_UP_MOVE,
		T_UP_WEAPONS,
		T_UP_ON,
		T_UP_OFF,
		T_TEXT,
		MAX_TEXTURES};
	typedef struct									// Create A Structure
	{
		GLubyte	*imageData;							// Image Data (Up To 32 Bits)
		GLuint	bpp;								// Image Color Depth In Bits Per Pixel.
		GLuint	width;								// Image Width
		GLuint	height;								// Image Height
		GLuint	texID;								// Texture ID Used To Select A Texture
	} TextureImage;	

	TextureImage textures[MAX_TEXTURES];

	//MEMBER-FUNCTIONS--------------------------------------------------------------
	inline void drawTextured2dQuadCenter(float x, float y, float width, float height);
	inline void drawTextured2dQuadCenter(Drawing d);
	inline void drawTextured2dQuadCenter(float x, float y, float z, float width, float height);
	inline void drawTextured2dQuadBottomLeft(float x, float y, float width, float height);
	inline void drawRotatingQuad(float x, float y, float rad, float rotation);
	inline void drawRotatingQuad(Drawing d);
	inline void setTexture(TextureNum texnum);
	bool loadTextures();
	bool LoadTGA(TextureImage *texture, char *filename);
};

inline void DrawingAssistant::drawTextured2dQuadCenter(float x, float y, float width, float height)
{
	glBegin(GL_QUADS);
	glVertex3f(x + width, y + height, 0.0); glTexCoord2f(0.0, 1.0);
	glVertex3f(x - width, y + height, 0.0); glTexCoord2f(0.0, 0.0);
	glVertex3f(x - width, y - height, 0.0); glTexCoord2f(1.0, 0.0);
	glVertex3f(x + width, y - height, 0.0); glTexCoord2f(1.0, 1.0);
	glEnd();
}

inline void DrawingAssistant::drawTextured2dQuadCenter(Drawing d)
{
	drawTextured2dQuadCenter(d.x, d.y, d.width, d.height);
}

inline void DrawingAssistant::drawTextured2dQuadCenter(float x, float y, float z, float width, float height)
{
	glBegin(GL_QUADS);
	glVertex3f(x + width, y + height, z); glTexCoord2f(0.0, 1.0);
	glVertex3f(x - width, y + height, z); glTexCoord2f(0.0, 0.0);
	glVertex3f(x - width, y - height, z); glTexCoord2f(1.0, 0.0);
	glVertex3f(x + width, y - height, z); glTexCoord2f(1.0, 1.0);
	glEnd();
}

inline void DrawingAssistant::drawTextured2dQuadBottomLeft(float x, float y, float width, float height)
{
	glBegin(GL_QUADS);
	glVertex3f(x + width,	y + height, 0.0); glTexCoord2f(0.0, 1.0);
	glVertex3f(x,			y + height, 0.0); glTexCoord2f(0.0, 0.0);
	glVertex3f(x,			y,			0.0); glTexCoord2f(1.0, 0.0);
	glVertex3f(x + width,	y,			0.0); glTexCoord2f(1.0, 1.0);
	glEnd();
}

inline void DrawingAssistant::drawRotatingQuad(float x, float y, float rad, float rotation)
{
	glTranslatef(x, y, 0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	drawTextured2dQuadCenter(0, 0, rad, rad);
	glRotatef(-rotation, 0.0, 0.0, 1.0);
	glTranslatef(-x, -y, 0);
}

inline void DrawingAssistant::drawRotatingQuad(Drawing d)
{
	drawRotatingQuad(d.x, d.y, d.width, d.rot);
}

inline void DrawingAssistant::setTexture(DrawingAssistant::TextureNum texnum)
{
	glBindTexture(GL_TEXTURE_2D, textures[texnum].texID);
}


// LOAD TGA -----------------------------------------------------------------------

bool DrawingAssistant::LoadTGA(TextureImage *texture, char *filename)				// Loads A TGA File Into Memory
{
	cout << ".";
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))	// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Does The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{					
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0 ||									// Is The Width Less Than Or Equal To Zero
		texture->height	<=0 ||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}
//------------------------------------------------------------------------ LOAD TGA

bool DrawingAssistant::loadTextures()
{
	cout << "Loading textures";
	return (
	LoadTGA(&textures[T_SPACESHIP],			"images/spaceship.tga") &&
	LoadTGA(&textures[T_ROCKETPARTICLE],	"images/particle.tga") &&
	LoadTGA(&textures[T_STAR],				"images/star.tga") &&
	LoadTGA(&textures[T_PARTICLE],			"images/particlewhite.tga") &&
	LoadTGA(&textures[T_PARTICLE_RGB],		"images/particlewhitergb.tga") &&
	LoadTGA(&textures[T_PARTICLE_CLOUD],	"images/particlecloud.tga") &&
	LoadTGA(&textures[T_PARTICLE_CONSETRATE],"images/particleconsentrate.tga") &&
	LoadTGA(&textures[T_NME_SAW_BLADE],		"images/saw_blade.tga") &&
	LoadTGA(&textures[T_NME_SAW_INNER],		"images/saw_inner.tga") &&
	LoadTGA(&textures[T_NME_MINE],			"images/trimine.tga") &&
	LoadTGA(&textures[T_NME_MINE_SPIKE],	"images/trispike.tga") &&
	LoadTGA(&textures[T_EM_UPPER],			"images/emupper.tga") &&
	LoadTGA(&textures[T_EM_LOWER],			"images/emlower.tga") &&
	LoadTGA(&textures[T_EM_LASER],			"images/laser.tga") &&
	LoadTGA(&textures[T_EM_TUBE],			"images/emtube.tga") &&
	LoadTGA(&textures[T_XPL_BALL],			"images/ExplodeBall.tga") &&
	LoadTGA(&textures[T_XPL_RING],			"images/explodering.tga") &&
	LoadTGA(&textures[T_UPGRADE_PANEL],		"images/upgradepanel.tga") &&
	LoadTGA(&textures[T_UPGRADE_TOP],		"images/upgrademetertop.tga") &&
	LoadTGA(&textures[T_UP_SHIELD],			"images/upgradeshield.tga") &&
	LoadTGA(&textures[T_UP_MOVE],			"images/upgrademovement.tga") &&
	LoadTGA(&textures[T_UP_WEAPONS],		"images/upgradeweapons.tga") &&
	LoadTGA(&textures[T_UP_ON],				"images/UpgradeLevelOn.tga") &&
	LoadTGA(&textures[T_UP_OFF],			"images/UpgradeLevelOff.tga") &&
	LoadTGA(&textures[T_TEXT],				"images/Letters.tga") &&
	LoadTGA(&textures[T_XPL_STAR],			"images/explodestar.tga") );
}