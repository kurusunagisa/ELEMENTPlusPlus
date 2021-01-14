#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// X, Yの最大値
#define X 5
#define Y 7

// ブロックの大きさ
#define LENGTH 48

//  PNG画像を読み込むための変数
extern GLuint img[9];
extern pngInfo info[9];
extern GLuint numimg[10];
extern pngInfo numinfo[10];
extern pngInfo title_info, bg_info, exit_info, flame_info, shuffle_info, result_info, cursor_info, specialcursor_info;
extern GLuint title_png, bg_png, exit_png, flame_png, shuffle_png, result_png, cursor_png,specialcursor_png;

// マップ
extern char field[Y][X];

// スコア
extern int score;

// 移動回数
extern int move_count;

// シャッフルの残回数
extern int shuffle_limit;

//現在のステージ
extern int current_stage;

// fieldのコピー用の変数
extern char field_cpy[Y][X];
// fieldの調査済flagを立てるための変数
extern char field_flag[Y][X];

// 現在のx, y座標
extern int current_x;
extern int current_y;

// ShuffleボタンとTitleボタンにカーソルがあるかどうかのフラグ
extern int shufflecursor_flag;
extern int titlecursor_flag;