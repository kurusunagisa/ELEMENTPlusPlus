#include "main.h"
#include "display.h"
#include "move.h"

// グローバル変数の宣言と初期化
char field[Y][X];
GLuint img[9];
pngInfo info[9];
GLuint numimg[10];
pngInfo numinfo[10];
pngInfo title_info, bg_info, exit_info, flame_info, shuffle_info, result_info, cursor_info, specialcursor_info;
GLuint title_png, bg_png, exit_png, flame_png, shuffle_png, result_png, cursor_png, specialcursor_png;
int score = 0;
int move_count = 0;
int current_stage = 0;
int shuffle_limit = 0;
char field_flag[Y][X] = {0};
char field_cpy[Y][X] = {0};
int current_x = LENGTH;
int current_y = LENGTH;
int shufflecursor_flag = 0;
int titlecursor_flag = 0;

int main(int argc, char **argv) {
    srandom(time(NULL)); // 起動時に乱数を時間で初期化
    glutInit(&argc, argv);
    glutInitWindowSize(LENGTH * 9, LENGTH * 9); // 画面サイズ
    glutCreateWindow("ELEMENT++");              // タイトルバーに表示する文字
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
    glClearColor(0.0, 0.0, 1.0, 1.0);

    // テクスチャのアルファチャネルを有効にする設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // PNG画像の読み込み
    int i;
    char fname[30];

    // 元素ブロックの読み込み
    for (i = 0; i < 9; i++) {
        sprintf(fname, "./images/element%02d.png", i + 1);
        img[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA, &info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    // Result表示のための数字読み込み
    for (i = 0; i < 10; i++) {
        sprintf(fname, "./images/number%d.png", i);
        numimg[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA, &numinfo[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    // タイトルの画像
    title_png = pngBind("./images/title.png", PNG_NOMIPMAP, PNG_ALPHA, &title_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    //背景画像
    bg_png = pngBind("./images/bg.png", PNG_NOMIPMAP, PNG_ALPHA, &bg_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    // Titleに戻るボタンの画像
    exit_png = pngBind("./images/exit.png", PNG_NOMIPMAP, PNG_ALPHA, &exit_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    // フレームの画像
    flame_png = pngBind("./images/flame.png", PNG_NOMIPMAP, PNG_ALPHA, &flame_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    // Shuffleボタンの画像
    shuffle_png = pngBind("./images/shuffle.png", PNG_NOMIPMAP, PNG_ALPHA, &shuffle_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    // 結果表示画面の画像
    result_png = pngBind("./images/result.png", PNG_NOMIPMAP, PNG_ALPHA, &result_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    //カーソルのある場所を示す画像
    cursor_png = pngBind("./images/cursor.png", PNG_NOMIPMAP, PNG_ALPHA, &cursor_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    // 特別なところにカーソルがある場合の画像
    specialcursor_png = pngBind("./images/cursor_special.png", PNG_NOMIPMAP, PNG_ALPHA, &specialcursor_info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

    //  コールバック関数の登録
    glutDisplayFunc(Display);             //画面表示
    glutReshapeFunc(Reshape);             //座標系の再設定
    glutMouseFunc(Mouse);                 //マウスのアクションを取得する
    glutPassiveMotionFunc(PassiveMotion); //マウスの位置を取得
    glutKeyboardFunc(Keyboard);           //キーボードのアクションを取得する
    glutSpecialFunc(SpecialKey);          //特殊なキーのアクションを取得する
    glutTimerFunc(1000, Timer, 0);        //タイマー割り込み

    //  イベントループ突入
    glutMainLoop();

    return 0;
}