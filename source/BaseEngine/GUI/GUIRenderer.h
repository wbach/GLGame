#ifndef GUI_RENDERER_H
#define  GUI_RENDERER_H
#include <vector>
#include <GL/glew.h>
#include "../Loader/Loader.h"
#include "GUIShader.h"
#include "GUITexture.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "GUI.h"
using namespace std;
class CGUIRenderer{
	GLuint quadVao, indVbo,vertexVbo ;

	GUIShader shader;
	glm::mat4 transformationMatrix;

	shared_ptr<CGUITexture>cursor ;
	glm::vec2 windowSize ;

	freetype::font_data textFont;
    FontShader fontshader;
	string fontName ;
public:
	void init(int windowW, int windowH){
		vector<float>postions= {
			-1,1,
			-1,-1,
			1,1,
			1,-1 };
		quadVao = createVAO();
		vertexVbo = storeDataInAttributesList(0,2,postions);
		unbindVAO();

		shader.init();
		fontshader.init();
		textFont.init("Data/GUI/CRYSISB.ttf", 50);
		//textFont.init("CRYSISB.ttf", 50);//"bgothm.ttf"
		cursor = nullptr ;
		windowSize.x = windowW ;
		windowSize.y = windowH ;
	}
	void setFont(string filename){
		textFont.clean();
		textFont.init(filename.c_str(), 50) ;
	}
	void loadCursor(shared_ptr<CGUITexture>cur){
		cursor = cur;
	}

	void render(CGUI &gui) {		
		renderTextures(gui.guiTextures);
		renderButtons(gui.guiButtons);
		renderText(gui.guiTexts);
	}
	void renderText(vector<CGUIText> &texts) {
		for (CGUIText text : texts) {
			text.drawText(&fontshader, &textFont);
		}
	}
	void renderButtons(vector<CGUIButton>&buttons) {
		if (buttons.size() <= 0) return;
		shader.start();
		glBindVertexArray(quadVao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		for (CGUIButton button : buttons) {
			if (!button.buttonTexture.loaded) continue;


			glActiveTexture(GL_TEXTURE0);
			switch (button.state) {
			case NORMAL_BUTTON_STATE:  glBindTexture(GL_TEXTURE_2D, button.normalTexture.getTextureId()); break;
			case ACTIVE_BUTTON_STATE:  glBindTexture(GL_TEXTURE_2D, button.activeTexture.getTextureId());   break;
			case HOVER_BUTTON_STATE:  glBindTexture(GL_TEXTURE_2D, button.hoverTexture.getTextureId());  break;
			}
			
			transformationMatrix = createTransformationMatrix(button.position, button.size);
			shader.loadTransformMatrix(transformationMatrix);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();
	}
	void renderTextures(vector<CGUITexture> &guis){
		shader.start();
		glBindVertexArray(quadVao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		for(CGUITexture gui : guis){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gui.getTextureId());
			transformationMatrix = createTransformationMatrix(gui.getPosition(), gui.getScale());
			shader.loadTransformMatrix(transformationMatrix);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();


		shader.start();


		if(cursor!= nullptr){
		glBindVertexArray(quadVao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		float x = (((float)tmpx)/(float)windowSize.x)*2 - 1 ;
		float y = (1-(((float)tmpy)/(float)windowSize.y))*2 - 1 ;
		cursor->setPosition(glm::vec2(x + cursor->getScale().x/2,y-cursor->getScale().y/2));
		SDL_ShowCursor(SDL_DISABLE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cursor->getTextureId());
		transformationMatrix = createTransformationMatrix(cursor->getPosition(), cursor->getScale());
		shader.loadTransformMatrix(transformationMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();
		}
	}
	void cleanUP(){
		if (cursor != nullptr) {
			cursor->cleanUp();
		}
		shader.cleanUp();
		fontshader.cleanUp();

		glDeleteBuffers(1, &vertexVbo);
		glDeleteVertexArrays(1, &quadVao);
		textFont.clean();
	}
};
#endif
