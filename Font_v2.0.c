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

#define FONT_COUNT 3
#define MAX_CHAR 128

int POS_X, POS_Y;

typedef struct s_Font {
    int x, y, w;
    Image Img;
} s_Font;

char Font_Name[][14] = {"courier_new", "comic_sans_ms", "iciel_kermel"};

//char Text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ! HOW DO YOU THINK ABOUT THIS FONT?";
char Text[] = "The quick brown fox jumps";
int Length;

s_Font Font_Save[FONT_COUNT][MAX_CHAR];
s_Font *Font;
Rect Rct;

int Count;

void Font_Init() {
    char Str[50];
    Image Img_Font;
    FILE *f;
    int i, j, n;
    int Id, x, y, w, h, xo, yo, wf;
    s_Font *p;
    for (i = 0; i < FONT_COUNT; i++) {
        sprintf(Str, "Fonts_v2.0/%s.png", Font_Name[i]);
        Load_Texture(&Img_Font, Str);
        sprintf(Str, "Fonts_v2.0/%s.fnt", Font_Name[i]);
        f = fopen(Str, "r");
        fscanf(f, "%d", &n);
        for (j = 0; j < n; j++) {
            // ID, x, y, Width, Height, x_offset, y_offset, w_full
            fscanf(f, "%d %d %d %d %d %d %d %d", &Id, &x, &y, &w, &h, &xo, &yo, &wf);
            p = &Font_Save[i][Id];
            Crop_Image(&Img_Font, &p->Img, x, y, w, h);
            p->x = xo;
            p->y = yo;
            p->w = wf;
        }
        Delete_Image(&Img_Font);
    }
}

void Font_Set(int Type) {
    Font = &Font_Save[Type][0];
}

void Font_Draw(char *c, float x, float y) {
    s_Font *p;
    Rect Rct;
    while (*c != '\0') {
        p = &Font[*c];
        Rct.Left = x + p->x;
        Rct.Right = Rct.Left + p->Img.w;
        Rct.Bottom = y + p->y;
        Rct.Top = Rct.Bottom + p->Img.h;
        Map_Texture(&p->Img);
        Draw_Rect(&Rct);
        x += p->w;
        c++;
    }
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
    Font_Init();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    Font_Set(0);
    Font_Draw(Text, 100, 50);
    Font_Set(1);
    Font_Draw(Text, 100, 150);
    Font_Set(2);
    Font_Draw(Text, 100, 250);
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
