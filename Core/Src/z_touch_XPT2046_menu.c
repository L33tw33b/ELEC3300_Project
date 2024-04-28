



#include "main.h"
#include "synth.h"

extern int16_t _width;
extern int16_t _height;


// menus declaration
#define Menu1Size 5
sMenuItem Menu1[Menu1Size];
#define Menu2Size 5
sMenuItem Menu2[Menu2Size];





void InitMenu(){
#ifdef ILI9488
	const uint16_t eGap=30;
	const uint16_t vGap=20;
	const uint16_t vBord=20;
#endif
#ifdef ILI9341
	const uint16_t eGap=10;
	const uint16_t vGap=15;
	const uint16_t vBord=10;
#endif

// Menu1
	for (uint8_t k=0;k<Menu1Size;k++){
		Menu1[k].X=eGap;
		Menu1[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu1[k].W=_width-2*eGap;
		Menu1[k].H=Font24.Height+2*vBord;
		Menu1[k].BkgUnsel=DD_BLUE;
		Menu1[k].BorUnsel=D_CYAN;
		Menu1[k].InkUnsel=WHITE;
		Menu1[k].BkgSel=ORANGE;
		Menu1[k].BorSel=YELLOW;
		Menu1[k].InkSel=WHITE;
		Menu1[k].font=Font24;
		Menu1[k].fontSize=1;

	}
	strcpy(Menu1[0].Desc,"Wave");
	strcpy(Menu1[1].Desc,"Item2");
	strcpy(Menu1[2].Desc,"Item3");
	strcpy(Menu1[3].Desc,"MENU 2");
	strcpy(Menu1[4].Desc,"EXIT");



// Menu2
	for (uint8_t k=0;k<Menu2Size;k++){
		Menu2[k].X=eGap;
		Menu2[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu2[k].W=_width-2*eGap;
		Menu2[k].H=Font24.Height+2*vBord;
		Menu2[k].BkgUnsel=DD_BLUE;
		Menu2[k].BorUnsel=D_CYAN;
		Menu2[k].InkUnsel=WHITE;
		Menu2[k].BkgSel=ORANGE;
		Menu2[k].BorSel=YELLOW;
		Menu2[k].InkSel=WHITE;
		Menu2[k].font=Font24;
		Menu2[k].fontSize=1;
	}
	strcpy(Menu2[0].Desc,"Sine On");
	strcpy(Menu2[1].Desc,"Semi_Sine On");
	strcpy(Menu2[2].Desc,"Triangle On");
	strcpy(Menu2[3].Desc,"Square On");
	strcpy(Menu2[4].Desc,"BACK");

}




/***************************************************************************
 * @brief 	menu element drawing
 * 			function of the menu system: you shouldn't need to change it
 * @params  menuItem	menuitem structure to draw
 * 			selected	0 uses unselected colors
 * 						1 uses selected
 ***************************************************************************/
void DrawMenuItem(sMenuItem *item, uint8_t selected){
uint16_t bor,bkg,ink;
	bor=(selected ? item->BorSel : item->BorUnsel);
	bkg=(selected ? item->BkgSel : item->BkgUnsel);
	ink=(selected ? item->InkSel : item->InkUnsel);
	Displ_CString(item->X, item->Y, item->X+item->W-1, item->Y+item->H-1,item->Desc,item->font,item->fontSize,ink,bkg);
	Displ_Border(item->X, item->Y, item->W, item->H,1,bor);
}





/***************************************************************************
 * @brief 	full menu drawing
 * 			function of the menu system: you shouldn't need to change it
 * @params  menu		structure to check
 * 			menusize	number of elements into menu
 ***************************************************************************/
void DrawMenu(sMenuItem *menu,uint8_t menusize){
uint8_t k;

	Displ_CLS(BLACK);
	for (k=0;k<menusize;k++){
		DrawMenuItem(&menu[k], 0);
	}
}




/***************************************************************************
 * @brief 	verifies if there is a touch over menu returning option selected
 * 			function of the menu system: you shouldn't need to change it
 * @params  menu		structure to check
 * 			menusize	number of elements into menu
 * @return	menu item selected
 ***************************************************************************/
uint8_t CheckMenu(sMenuItem *menu,uint8_t menusize){
uint8_t k,result;

// if not registred any touch return a null choice
	if (!Touch_GotATouch(1))
		return 255;

	for (k=0;k<menusize;k++){
		if (Touch_In_XY_area(menu[k].X,menu[k].Y,menu[k].W,menu[k].H)){  //polling if touch was in the current menu item area
			result=k;
			break;
		}
	}
	DrawMenuItem(&menu[k], 1); //draw selected

	Touch_WaitForUntouch(0);
	return result;
}

void UpdateMenuItem(sMenuItem menu[], int itemIndex, int isSelected) {
    if (itemIndex < 0) return; // Basic check to avoid invalid index

    sMenuItem *item = &menu[itemIndex]; // Pointer to the menu item to be updated

    // Determine the colors based on the selection status
    uint16_t backgroundColor = isSelected ? item->BkgSel : item->BkgUnsel;
    uint16_t borderColor = isSelected ? item->BorSel : item->BorUnsel;
    uint16_t textColor = isSelected ? item->InkSel : item->InkUnsel;

    // Draw the rounded rectangle background
    Displ_fillRoundRect(item->X, item->Y, item->W, item->H, 5, backgroundColor); // Assuming a radius of 5 for rounded corners

    // Draw the border around the item
    Displ_drawRoundRect(item->X, item->Y, item->W, item->H, 5, borderColor);

    // Draw the text centered within the rectangle
    uint16_t textWidth = strlen(item->Desc) * item->font.Width * item->fontSize;
    uint16_t textX = item->X + (item->W - textWidth) / 2;
    uint16_t textY = item->Y + (item->H - item->font.Height * item->fontSize) / 2;
    Displ_WString(textX, textY, item->Desc, item->font, item->fontSize, textColor, backgroundColor);
}



/******************************************************
 * @brief actions done by Menu1 - specific funcion to implement for each menu
 * 		  the "switch case" below defines actions to perform for each menu item
 * 		  consider that result has
 * 		  0-253 	the menu item chosen
 * 		  254 		kdisplay was clicked outside items menu area
 * 		  255		ther was no touch on the menu
 * 		  after any actions repeat menu unless there is a specific "return"
 ******************************************************/
int x = 0;
int y = 0;
int z = 0;
int b = 0;
extern volatile int menu1_running;
extern volatile int menu2_running;

void RunMenu1(int menu_counter, int button_pressed){


	 static uint8_t itemSelected[5] = {0}; // Static array to keep track of the selection state
	    // Loop through all items and update their selection state
	    for (int i = 0; i < 5; i++) {
	        if (i == menu_counter) {
	            if (itemSelected[i] == 0) { // If it is not already selected
	                UpdateMenuItem(Menu1, i, 1); // Set to selected
	                itemSelected[i] = 1; // Update the state to selected
	            }
	        } else {
	            if (itemSelected[i] != 0) { // If it is not already unselected
	                UpdateMenuItem(Menu1, i, 0); // Set to unselected
	                itemSelected[i] = 0; // Update the state to unselected
	            }
	        }
	    }

	    // Handle special cases like touches outside the menu items
	    if(button_pressed == 1){
	    	button_pressed = 0;
	    	switch (menu_counter) {
	        	case 0: // Wave

	        		menu2_running = 1;
	        	    menu1_running = 0;
	        	    DrawMenu(Menu2,	(sizeof(Menu2)/sizeof(Menu2[0])));
	        	    itemSelected[0] = 0;
	        		RunMenu2(0, button_pressed);

	        		break;
	        	case 1: // Saw Wave
	        		break;
	        	case 2: // Triangle Wave
	        		break;
	        	case 3: // Square Wave
	        	default:
	        		break;
	    	}
	    }


}







/******************************************************
 * @brief actions done by Menu2 - specific funcion to implement for each menu
 * 		  the "switch case" below defines actions to perform for each menu item
 * 		  consider that result has
 * 		  0-253 	the menu item chosen
 * 		  254 		kdisplay was clicked outside items menu area
 * 		  255		ther was no touch on the menu
 * 		  after any actions repeat menu unless there is a specific "return"
 ******************************************************/
void RunMenu2(int menu2_counter, int button_pressed){
	//DrawMenu(Menu2,	(sizeof(Menu2)/sizeof(Menu2[0])));
	static uint8_t itemSelected[5] = {0}; // Static array to keep track of the selection state
	    // Loop through all items and update their selection state
	    for (int i = 0; i < 5; i++) {
	        if (i == menu2_counter) {
	            if (itemSelected[i] == 0) { // If it is not already selected
	                UpdateMenuItem(Menu2, i, 1); // Set to selected
	                itemSelected[i] = 1; // Update the state to selected
	            }
	        } else {
	            if (itemSelected[i] != 0) { // If it is not already unselected
	                UpdateMenuItem(Menu2, i, 0); // Set to unselected
	                itemSelected[i] = 0; // Update the state to unselected
	            }
	        }
	    }

	    if(button_pressed == 1){

	    	    	switch (menu2_counter) {
	    	        	case 0: // Sine Wave
	    	        		x = x%2;
	    	        		if(x==0){
	    	        			strcpy(Menu2[0].Desc,"Sine Off");
	    	        			UpdateMenuItem(Menu2, 0, 1);
	    	        			set_wave(0);
	    	        		}
	    	        		else{
	    	        			strcpy(Menu2[0].Desc,"Sine On");
	    	        			UpdateMenuItem(Menu2, 0, 1);
	    	        			set_wave(1);
	    	        		}
	    	        		x++;
	    	        		break;
	    	        	case 1: // Semi_Sine Wave
	    	        		y = y%2;
	    	        		if(y==0){
	    	        			strcpy(Menu2[1].Desc,"Semi_Sine Off");
	    	        			UpdateMenuItem(Menu2, 1, 1);
	    	        			set_wave(4);
	    	        		}
	    	        		else{
	    	        			strcpy(Menu2[1].Desc,"Semi_Sine On");
	    	        			UpdateMenuItem(Menu2, 1, 1);
	    	        			set_wave(1);
	    	        		}
	    	        		y++;
	    	        		break;
	    	        	case 2: // Triangle Wave
	    	        		z = z%2;
	    	        		if(z==0){
	    	        			strcpy(Menu2[2].Desc,"Triangle Off");
	    	        			UpdateMenuItem(Menu2, 2, 1);
	    	        			set_wave(3);
	    	        		}
	    	        		else{
								strcpy(Menu2[2].Desc,"Triangle On");
								UpdateMenuItem(Menu2, 2, 1);
								set_wave(1);
	    	        		}
	    	        		z++;
	    	        		break;
	    	        	case 3: // Square Wave
	    	        		b = b%2;
	    	        		if(b==0){
	    	        			strcpy(Menu2[3].Desc,"Square Off");
	    	        			UpdateMenuItem(Menu2, 3, 1);
	    	        			set_wave(2);
	    	        		}
	    	        		else{
	    	        			strcpy(Menu2[3].Desc,"Square On");
	    	        			UpdateMenuItem(Menu2, 3, 1);
	    	        			set_wave(1);
	    	        		}
	    	        		b++;
	    	        		break;
	    	        	case 4:
	    	        		menu2_running = 0;
	    	        	    menu1_running = 1;

	    	        	    DrawMenu(Menu1,	(sizeof(Menu1)/sizeof(Menu1[0])));
	    	        		RunMenu1(0, 0);
	    	        		break;
	    	        	default:
	    	        		break;
	    	    	}
	   }


}

// Calls drawmenu in main.c
void callDrawMenu(){

		DrawMenu(Menu1,	(sizeof(Menu1)/sizeof(Menu1[0])));




}
