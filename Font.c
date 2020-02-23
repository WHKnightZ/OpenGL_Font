#include "../Library/loadpng.h"

#include "../Library/process_image.h"

#include <GL/glut.h>

#include "../Library/gl_texture.h"

#define WIDTH 800
#define HEIGHT 400

#define START_X 50.0f
#define START_Y 100.0f
#define FULL_X 650.0f
#define LINE_SPACE 60.0f

#define INTERVAL 15

#define MAX_CHAR 128

#define FONT_OFFSET 0.0f

#define FONT_NAME_1 "Font1"
#define FONT_NAME_2 "Font2"
#define FONT_NAME_3 "Upheaval"
#define FONT_NAME_4 "BreamCatcher"
#define FONT_NAME_5 "iCielKermel"
#define FONT_NAME_6 "akaDylan"
#define FONT_NAME_7 "Zaratustra"
#define FONT_NAME_8 "Terbert"
#define FONT_NAME_9 "Somerset_Barnyard_Small"
#define FONT_NAME_10 "Somerset_Barnyard_Medium"
#define FONT_NAME_11 "Somerset_Barnyard_Large"

int POS_X, POS_Y;

char Font_Name[] = FONT_NAME_9;
//char Text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ! HOW DO YOU THINK ABOUT THIS FONT?";
char Text[] = "abcdef ghijkl mnopqr stuvwx yzABCD EFGHIJ KLMNOP QRSTUV WXYZ ()[],./";
int Length;

Image Img_Char[MAX_CHAR];
float Char_Height;
Rect Rct;

int Count;

void Init_Font() {
    char str[50];
    Image Img_Font, Img_Char_Tmp;
    sprintf(str, "Fonts/%s.png", Font_Name);
    Load_Texture(&Img_Font, str);
    sprintf(str, "Fonts/%s.fnt", Font_Name);
    FILE *f = fopen(str, "r");
    int i, n;
    int id, x, y, w, h, xo, yo, wf, hf;
    fscanf(f, "%d %d", &n, &hf);
    for (i = 0; i < n; i++) {
        // ID, x, y, Width, Height, x_offset, y_offset, w_full
        fscanf(f, "%d %d %d %d %d %d %d %d", &id, &x, &y, &w, &h, &xo, &yo, &wf);
        Crop_Image(&Img_Font, &Img_Char_Tmp, x, y, w, h);
        Create_Image(&Img_Char[id], wf, hf);
        Mix_Image(&Img_Char[id], &Img_Char_Tmp, xo, yo);
        Delete_Image(&Img_Char_Tmp);
    }
    Char_Height = hf;
    Delete_Image(&Img_Font);
}

void Init_Global() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Init_Font();
    Count = Length = 0;
    char *c = Text;
    while (*c != '\0') {
        c++;
        Length++;
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    char *c = Text;
    Rct.Right = START_X;
    Rct.Bottom = START_Y;
    Rct.Top = Rct.Bottom + Char_Height;
    int i = 0;
    while (i < Count) {
        Rct.Left = Rct.Right + FONT_OFFSET;
        Rct.Right = Rct.Left + Img_Char[*c].w;
        Map_Texture(&Img_Char[*c]);
        Draw_Rect(&Rct);
        if (Rct.Right > FULL_X && *c == 32) {
            Rct.Right = START_X;
            Rct.Bottom += LINE_SPACE;
            Rct.Top = Rct.Bottom + Char_Height;
        }
        c++;
        i++;
    }
    glutSwapBuffers();
}

void Timer(int value) {
    if (Count < Length) {
        Count++;
        glutTimerFunc(INTERVAL, Timer, 0);
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Font Test");
    Init_Global();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
