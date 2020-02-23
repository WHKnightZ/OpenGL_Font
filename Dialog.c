#include "../Library/gl_texture.h"
#include "../Library/loadpng.h"

#define WIDTH 640
#define HEIGHT 480
#define W2 320.0f
#define H2 360.0f
#define START_X 135.0f
#define START_Y 340.0f
#define FULL_X 440.0f
#define LINE_SPACE 22.0f

#define INTERVAL 15

#define MAX_CHAR 128

#define FONT_OFFSET 3.0f

#define COUNT_DLG 8
#define DIALOG_0 "Hello, welcome to OpenGL! Press any key to continue!"
#define DIALOG_1 "Want to learn about OpenGL? If you want, press any key!"
#define DIALOG_2 "Open Graphics Library(OpenGL) is a cross-language..."
#define DIALOG_3 "for rendering 2D and 3D vector graphics..."
#define DIALOG_4 "The API is typically used to interact with a..."
#define DIALOG_5 "graphics processing unit(GPU) to achieve hardware..."
#define DIALOG_6 "accelerated rendering"
#define DIALOG_7 "Goodbye!"

int POS_X, POS_Y;

enum Dialog_Status {
    DLG_STT_IDLE,
    DLG_STT_ZOOM_IN,
    DLG_STT_STAND,
    DLG_STT_ZOOM_OUT
};

void (*Dlg_Process_Func[4])();

char Font_Name[] = "Zaratustra";

Image Img_Char[MAX_CHAR];
Rect Rct_Char;
float Char_Height;

Image Img_BG, Img_Dialog;
Rect Rct_BG, Rct_Dialog;
Rect Rct_Dialog_Save[20];
int Dialog_Timer, Dialog_Stt;

char Text[COUNT_DLG][200] = {DIALOG_0, DIALOG_1, DIALOG_2, DIALOG_3, DIALOG_4, DIALOG_5, DIALOG_6, DIALOG_7};
int Length[COUNT_DLG];
int Current_Dlg, Current_Char;

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

void Dlg_Process_Idle() {
    // Do nothing
}

void Dlg_Process_Zoom_In() {
    Rct_Dialog.Left = Rct_Dialog_Save[Dialog_Timer].Left + W2;
    Rct_Dialog.Right = Rct_Dialog_Save[Dialog_Timer].Right + W2;
    Rct_Dialog.Bottom = Rct_Dialog_Save[Dialog_Timer].Bottom + H2;
    Rct_Dialog.Top = Rct_Dialog_Save[Dialog_Timer].Top + H2;
    if (Dialog_Timer < 20) {
        Dialog_Timer++;
        if (Dialog_Timer == 20) {
            Dialog_Timer = 19;
            Dialog_Stt = DLG_STT_STAND;
        }
    }
}

void Dlg_Process_Stand() {
    if (Current_Char < Length[Current_Dlg])
        Current_Char++;
}

void Dlg_Process_Zoom_Out() {
    if (Dialog_Timer > -1) {
        Dialog_Timer--;
        if (Dialog_Timer == -1) {
            Dialog_Stt = DLG_STT_IDLE;
            return;
        }
    }
    Rct_Dialog.Left = Rct_Dialog_Save[Dialog_Timer].Left + W2;
    Rct_Dialog.Right = Rct_Dialog_Save[Dialog_Timer].Right + W2;
    Rct_Dialog.Bottom = Rct_Dialog_Save[Dialog_Timer].Bottom + H2;
    Rct_Dialog.Top = Rct_Dialog_Save[Dialog_Timer].Top + H2;
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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    Init_Font();
    Load_Texture(&Img_BG, "Images/BG.png");
    Load_Texture(&Img_Dialog, "Images/Dialog.png");
    Rct_BG.Left = 0.0f;
    Rct_BG.Right = WIDTH;
    Rct_BG.Bottom = 0.0f;
    Rct_BG.Top = HEIGHT;
    Dialog_Stt = DLG_STT_IDLE;
    Dialog_Timer = 0;
    int i;
    for (i = 0; i < 20; i++) {
        Rct_Dialog_Save[i].Left = -(i + 1) * 10.4f; // 10.4f = Rct_Dialog.w / 2 / 20
        Rct_Dialog_Save[i].Right = (i + 1) * 10.4f;
        Rct_Dialog_Save[i].Bottom = -(i + 1) * 1.3f - 20.0f;
        Rct_Dialog_Save[i].Top = (i + 1) * 1.3f + 20.0f;
    }
    char *c;
    for (i = 0; i < COUNT_DLG; i++) {
        c = Text[i];
        Length[i] = 0;
        while (*c != '\0') {
            c++;
            Length[i]++;
        }
    }
    Dlg_Process_Func[DLG_STT_IDLE] = Dlg_Process_Idle;
    Dlg_Process_Func[DLG_STT_ZOOM_IN] = Dlg_Process_Zoom_In;
    Dlg_Process_Func[DLG_STT_STAND] = Dlg_Process_Stand;
    Dlg_Process_Func[DLG_STT_ZOOM_OUT] = Dlg_Process_Zoom_Out;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    Map_Texture(&Img_BG);
    Draw_Rect(&Rct_BG);
    if (Dialog_Stt != DLG_STT_IDLE) {
        Map_Texture(&Img_Dialog);
        Draw_Rect(&Rct_Dialog);
        char *c = Text[Current_Dlg];
        Rct_Char.Right = START_X;
        Rct_Char.Bottom = START_Y;
        Rct_Char.Top = Rct_Char.Bottom + Char_Height;
        int i = 0;
        while (i < Current_Char) {
            Rct_Char.Left = Rct_Char.Right + FONT_OFFSET;
            Rct_Char.Right = Rct_Char.Left + Img_Char[*c].w;
            Map_Texture(&Img_Char[*c]);
            Draw_Rect(&Rct_Char);
            if (Rct_Char.Right > FULL_X && *c == 32) {
                Rct_Char.Right = START_X;
                Rct_Char.Bottom += LINE_SPACE;
                Rct_Char.Top = Rct_Char.Bottom + Char_Height;
            }
            c++;
            i++;
        }
    }
    glutSwapBuffers();
}

void Timer(int value) {
    Dlg_Process_Func[Dialog_Stt]();
    glutTimerFunc(INTERVAL, Timer, 0);
    glutPostRedisplay();
}

void Keyboard(GLubyte key, int x, int y) {
    if (Dialog_Stt == DLG_STT_IDLE) {
        Current_Char = Current_Dlg = 0;
        Dialog_Stt = DLG_STT_ZOOM_IN;
    } else if (Dialog_Stt == DLG_STT_STAND) {
        if (Current_Char < Length[Current_Dlg])
            Current_Char = Length[Current_Dlg];
        else {
            Current_Dlg++;
            Current_Char = 0;
            if (Current_Dlg == COUNT_DLG) {
                Current_Char = 0;
                Dialog_Stt = DLG_STT_ZOOM_OUT;
            }
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Dialog Test");
    Init_Global();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}
