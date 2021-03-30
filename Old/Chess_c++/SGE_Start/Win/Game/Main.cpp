//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
#include "GUI.h"
//---------------------------------------------------------------------
GUI G;
//---------------------------------------------------------------------
//GLUT tips: http://www.lighthouse3d.com/opengl/glut/index.php?1
void Display(void){G.Display();}
void Idle(void){G.Idle();}
void ChangeSize(int w, int h){G.ChangeSize(w,h);}
void Keyboard_NormalKeys(unsigned char key, int x, int y){
	G.Keyboard_NormalKeys(key,x,y);}
void Keyboard_NormalKeysUp(unsigned char key, int x, int y){
	G.Keyboard_NormalKeysUp(key,x,y);}
void Keyboard_SpecialKeys(int key, int x, int y){
	G.Keyboard_SpecialKeys(key,x,y);}
void Mouse(int button, int state, int x, int y){
	G.Mouse(button,state,x,y);}
void Mouse_Motion(int x, int y){G.Mouse_Motion(x,y);}
void Mouse_PassiveMotion(int x, int y){G.Mouse_PassiveMotion(x,y);}
//---------------------------------------------------------------------
int main(int argc, char **argv){
    //----------------------Init1
	glutInitWindowPosition(G.mWinPosX,G.mWinPosY);
	glutInitWindowSize(G.mW,G.mH);
    glutInitDisplayMode(GLUT_RGBA  | GLUT_DOUBLE |
						GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInit(&argc, argv);
        
	//----------------------Window
	glutCreateWindow("Player - Computer");
	G.mFullScreen = false;
	if (G.mFullScreen) glutFullScreen();
    glutReshapeFunc(ChangeSize);

	//----------------------Display
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard_NormalKeys);
    glutKeyboardUpFunc(Keyboard_NormalKeysUp);
	glutSpecialFunc(Keyboard_SpecialKeys);
	glutMouseFunc(Mouse);
	glutMotionFunc(Mouse_Motion);
    glutPassiveMotionFunc(Mouse_PassiveMotion);
    
	//----------------------Init2
    G.Init(argc, argv);

	//----------------------Main
	glutMainLoop();
    return 0;
}
//---------------------------------------------------------------------
