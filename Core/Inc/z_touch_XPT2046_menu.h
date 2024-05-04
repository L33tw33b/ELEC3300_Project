

#ifndef INC_Z_TOUCH_XPT2046_MENU_H_
#define INC_Z_TOUCH_XPT2046_MENU_H_



// Menu data definitions and declarations


typedef struct {
	uint16_t X;			//position and size of the menu element
	uint16_t Y;
	uint16_t W;
	uint16_t H;
	char Desc[20];		//text of the menu item
	sFONT font;			//font to be used
	uint8_t fontSize;
	uint16_t BkgUnsel;   //background color when item is not selected
	uint16_t BorUnsel;   //border color when item is not selected
	uint16_t InkUnsel;   //text color when item is not selected
	uint16_t BkgSel;     //background color with item selected
	uint16_t BorSel;     //border color with item selected
	uint16_t InkSel;     //text color with item selected
}sMenuItem;


void InitMenu();
void DrawMenu(sMenuItem *menu,uint8_t menusize);
uint8_t CheckMenu(sMenuItem *menu,uint8_t menusize);
void RunMenu2(int menu2_counter, int button_pressed);
void RunMenu1(int menu_counter, int button_pressed);
void RunMenu3(int menu3_counter, int button_pressed);
void callDrawMenu(int menu);
void UpdateMenuItem(sMenuItem menu[], int itemIndex, int isSelected);







#endif /* INC_Z_TOUCH_XPT2046_MENU_H_ */
