/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                                   
 * User interface commands and utilities                                                                          
 */
 
struct Box{
	float* boxPoints=NULL;
	bool selected = false;
	
	
	bool isMouseOver(){
		int mouseX = esat::MousePositionX();
		int mouseY = esat::MousePositionY();
		return ((mouseX > boxPoints[0] && mouseX < boxPoints[2]) && (mouseY > boxPoints[1] && mouseY < boxPoints[5]));
	}
	
	void DrawButton(bool solid, Color color){
		esat::DrawSetStrokeColor(color.r, color.g, color.b);
		if(solid){
			esat::DrawSetFillColor(color.r, color.g, color.b);
			esat::DrawSolidPath(boxPoints, 5);
		}
		else{
			esat::DrawPath(boxPoints, 5);
		}
	}
	
	void DrawTextField(bool selected, Color color1, Color color2){
		if(selected){ esat::DrawSetFillColor(color1.r, color1.g, color1.b);}
		else{esat::DrawSetFillColor(color2.r, color2.g, color2.b);}
		esat::DrawSolidPath(boxPoints, 5);
	}
	
	void CreateBox(int posX, int posY, int width, int height, float **ptr){
		float* points;
		*ptr = (float*) calloc(10, sizeof(float));
		points = (float*) calloc(10, sizeof(float));
		points[0] = posX;
		points[1] = posY;
		points[2] = posX + width;
		points[3] = posY;
		points[4] = posX + width;
		points[5] = posY + height;
		points[6] = posX;
		points[7] = posY + height;
		points[8] = posX;
		points[9] = posY;
			
		*ptr = points;
	}
};
 
void DrawTextPlus(float x, float y, char* text, Color color1, Color color2, Box father){
	if(father.isMouseOver()) {esat::DrawSetFillColor(color1.r, color1.g, color1.b);}
	else{esat::DrawSetFillColor(color2.r, color2.g, color2.b);}
	 
	esat::DrawText(x, y, text);
}

void TextField(Box *box, std::string *referenceField, bool encrypted = false){
	(*box).DrawTextField((*box).selected, lightGrey, white);
	if((*box).isMouseOver() && esat::MouseButtonDown(0)){
		(*box).selected = true;
	}
	if((*box).selected){
		char newChar = esat::GetNextPressedKey();
		if(esat::IsSpecialKeyDown(esat::kSpecialKey_Backspace) && (*referenceField).length() > 0){
			(*referenceField).erase((*referenceField).length() - 1);
		}
		if(newChar != '\0')
		*referenceField += newChar;
	}
	if((*referenceField).length() > 0){
		char *text = new char[(*referenceField).length() + 1];
		strcpy(text, (*referenceField).c_str());
		if(!encrypted){
			DrawTextPlus((*box).boxPoints[0] + 10, (*box).boxPoints[1] + 32, text, black, black, *box);
		}
		else{
			for(int i = 0; i < (*referenceField).length(); i++){
				text[i] = '*';
			}
			DrawTextPlus((*box).boxPoints[0] + 10, (*box).boxPoints[1] + 32, text, black, black, *box);
		}
	}
}