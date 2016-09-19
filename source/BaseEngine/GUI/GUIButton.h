#ifndef GUIBUTTON_H
#define GUIBUTTON_H
#include "GUITexture.h"
#include "FreeType.h"
#include "FontShader.h"
enum buttonState{
	NORMAL_BUTTON_STATE = 0,
	HOVER_BUTTON_STATE,
	ACTIVE_BUTTON_STATE
};
class CGUIButton {

	string text;
	float fontSize;	

public:
	CGUITexture buttonTexture;
	CGUITexture normalTexture;
	CGUITexture hoverTexture;
	CGUITexture activeTexture;
	int state;
	glm::vec2 position,size;
	glm::vec3 colour;
	CGUIButton() {}
	CGUIButton(string text, glm::vec2 position, float fontSize, glm::vec3 colour, glm::vec2 size) :size(size) {
		this->position = position;
		this->colour = colour;
		this->text = text;
		this->fontSize = fontSize;
		state = NORMAL_BUTTON_STATE;
	}
	CGUIButton(GLuint normalTextureID, GLuint hoverTextureID, GLuint activeTextureID,string text, glm::vec2 position, float fontSize, glm::vec3 colour,glm::vec2 size):size(size),		
		normalTexture(normalTextureID,position, size),
		hoverTexture(hoverTextureID, position, size),
		activeTexture(activeTextureID,position, size) {

		buttonTexture = normalTexture;

		this->position = position;
		this->colour = colour;
		this->text = text;
		this->fontSize = fontSize;
	}
	void updateText(string text) { this->text = text; }
	int isPressedButton(glm::vec2 windowSize) {
		state = NORMAL_BUTTON_STATE;
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);
		float x = (((float)tmpx) / (float)windowSize.x) * 2 - 1;
		float y = (1 - (((float)tmpy) / (float)windowSize.y)) * 2 - 1;
		int button = 0;
		/*if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
			button = 1;
		else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			button = -1;*/

		

		/*	cout << "Button : "<<endl << x << " | " << position.x << " | " << position.x + size.x << endl
				<< y << " | " << position.y << " | " << position.y + size.y << endl;*/

			if( x >= position.x - size.x  && x < position.x + size.x ){
				if (y >= position.y - size.y  && y < position.y + size.y) {
					if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						cout << "BUTTON PRESSED"<< endl;
						state = ACTIVE_BUTTON_STATE;
						return 1;
					}
					state = HOVER_BUTTON_STATE;
				}
			}
		
		
		//cout << "Pressed mose, button : " << button << ",x,y: " << x << ", " << y << endl;

		return 0;
	}

	void drawText(FontShader *shader, freetype::font_data *font)
	{
		//glUseProgram(0);
		shader->Start();
		glActiveTexture(GL_TEXTURE0);
		GLfloat ActiveColor[] = { colour.x,colour.y,colour.z,1 };
		glPushMatrix();
		glLoadIdentity();
		glColor4fv(ActiveColor);
		shader->loadTranslation(position);
		glScalef(fontSize, fontSize, fontSize);
		freetype::print(*font, position.x, position.y, text.c_str());
		glPopMatrix();
		shader->Stop();
	}

};
#endif