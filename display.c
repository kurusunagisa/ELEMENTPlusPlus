#include "main.h"
#include "display.h"

//
// 画面再描画時間を制御する関数
//
void Timer(int t) {
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
}

//
// 座標系を再設定する関数
//
void Reshape(int w, int h) {
    glutReshapeWindow(LENGTH * 9, LENGTH * 9); // ウィンドウサイズは固定する
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

//
// ブロックが繋がっている数を数える関数
//
void CheckChain(int x, int y, int *chain) {
    field_flag[y][x] = 1; // 調査済みフラグを立てて調査の重複を防ぐ

    // 上方向に同じ種類のブロックがあるとき
    if (y > 0 && field_flag[y - 1][x] == 0 && field[y - 1][x] == field[y][x]) {
        (*chain)++;
        // 上方向のブロックから再帰的に調査する
        CheckChain(x, y - 1, chain);
    }
    // 下方向に同じ種類のブロックがあるとき
    if (y < Y - 1 && field_flag[y + 1][x] == 0 && field[y + 1][x] == field[y][x]) {
        (*chain)++;
        // 下方向のブロックから再帰的に調査する
        CheckChain(x, y + 1, chain);
    }
    // 左方向に同じ種類のブロックがあるとき
    if (x > 0 && field_flag[y][x - 1] == 0 && field[y][x - 1] == field[y][x]) {
        (*chain)++;
        // 左方向のブロックから再帰的に調査する
        CheckChain(x - 1, y, chain);
    }
    // 右方向に同じ種類のブロックがあるとき
    if (x < X - 1 && field_flag[y][x + 1] == 0 && field[y][x + 1] == field[y][x]) {
        (*chain)++;
        // 右方向のブロックから再帰的に調査する
        CheckChain(x + 1, y, chain);
    }
}

//
// タイトル画面を描画する関数
//
void DisplayTitle(void) {
    //タイトル画像を表示
    PutSprite(title_png, -40, -40, &title_info);
}

//
// ゲーム画面を描画する関数
//
void DisplayGame(void) {
    GLint bit_image;
    pngInfo bit_info;

    int i, j, k;

    // 背景画像を表示
    PutSprite(bg_png, 0, 0, &bg_info);

    //元素記号ブロックを表示する
    // '1': H
    // '2': He
    // '3': Li
    // '4': Be
    // '5': B
    // '6': C
    // '7': N
    // '8': O
    // '9': F
    for (j = 0; j < Y; j++) {
        for (k = 0; k < X; k++) {
            switch (field[j][k]) {
            case '1':
                bit_image = img[0];
                bit_info = info[0];
                break;
            case '2':
                bit_image = img[1];
                bit_info = info[1];
                break;
            case '3':
                bit_image = img[2];
                bit_info = info[2];
                break;
            case '4':
                bit_image = img[3];
                bit_info = info[3];
                break;
            case '5':
                bit_image = img[4];
                bit_info = info[4];
                break;
            case '6':
                bit_image = img[5];
                bit_info = info[5];
                break;
            case '7':
                bit_image = img[6];
                bit_info = info[6];
                break;
            case '8':
                bit_image = img[7];
                bit_info = info[7];
                break;
            case '9':
                bit_image = img[8];
                bit_info = info[8];
                break;
            default:
                break;
            }
            PutSprite(bit_image, (k + 1) * LENGTH, (j + 1) * LENGTH, &bit_info);
        }
    }
    if (titlecursor_flag == 0 && shufflecursor_flag == 0 && current_x >= LENGTH && current_x < (X + 1) * LENGTH && current_y >= LENGTH && current_y < (Y + 1) * LENGTH) {
        // 選択しているブロックに枠を表示
        PutSprite(cursor_png, current_x - (current_x % LENGTH) - 1, current_y - (current_y % LENGTH) - 1, &cursor_info);
    }
    // スコアの枠を表示
    PutSprite(flame_png, X * LENGTH + 72, LENGTH, &flame_info);
    // Shuffleボタンを表示
    PutSprite(shuffle_png, X * LENGTH + 72, (Y - 2) * LENGTH, &shuffle_info);
    // Titleボタンを表示
    PutSprite(exit_png, X * LENGTH + 72, Y * LENGTH, &exit_info);

    // Shuffleボタンにカーソルがあるとき
    if (shufflecursor_flag == 1) {
        PutSprite(specialcursor_png, X * LENGTH + 71, (Y - 2) * LENGTH - 1, &specialcursor_info);
    }
    // Titleボタンにカーソルがあるとき
    if (titlecursor_flag == 1) {
        PutSprite(specialcursor_png, X * LENGTH + 71, Y * LENGTH - 1, &specialcursor_info);
    }

    // スコアの表示
    char s[20];
    sprintf(s, "%06d", score);
    glColor3ub(0, 0, 0);
    glRasterPos2i(X * LENGTH + 90, LENGTH + 45);
    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }

    // シャッフルの残回数を表示
    char t[20];
    if (shuffle_limit > 1) {
        sprintf(t, "%d more times", shuffle_limit);
    } else if (shuffle_limit == 1) {
        sprintf(t, "%d more time", shuffle_limit);
    } else {
        sprintf(t, "Can't shuffle");
    }
    glColor3ub(255, 0, 0);
    glRasterPos2i(X * LENGTH + 70, (Y - 2) * LENGTH - 10);
    for (i = 0; i < strlen(t); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, t[i]);
    }
}

//
// 結果発表画面を表示する関数
//
void DisplayResult(void) {
    int i;
    int x = 50;
    int y = 120;
    GLint bit_image;
    pngInfo bit_info;
    char s[20];

    // 結果発表画像を表示
    PutSprite(result_png, -40, -40, &result_info);

    // スコアを6桁で表示する
    sprintf(s, "%06d", score);
    // 文字の画像を表示する
    for (i = 0; i < strlen(s); i++) {
        switch (s[i]) {
        case '0':
            bit_image = numimg[0];
            bit_info = numinfo[0];
            break;
        case '1':
            bit_image = numimg[1];
            bit_info = numinfo[1];
            break;
        case '2':
            bit_image = numimg[2];
            bit_info = numinfo[2];
            break;
        case '3':
            bit_image = numimg[3];
            bit_info = numinfo[3];
            break;
        case '4':
            bit_image = numimg[4];
            bit_info = numinfo[4];
            break;
        case '5':
            bit_image = numimg[5];
            bit_info = numinfo[5];
            break;
        case '6':
            bit_image = numimg[6];
            bit_info = numinfo[6];
            break;
        case '7':
            bit_image = numimg[7];
            bit_info = numinfo[7];
            break;
        case '8':
            bit_image = numimg[8];
            bit_info = numinfo[8];
            break;
        case '9':
            bit_image = numimg[9];
            bit_info = numinfo[9];
            break;
        default:
            break;
        }
        PutSprite(bit_image, x, y, &bit_info);
        x += 55;
    }
}

//
//  ウィンドウの表示内容を更新する
//
void Display(void) {
    int i, j, k, l;

    glClear(GL_COLOR_BUFFER_BIT);

    // ゲーム中のとき
    if (current_stage == 1) {
        // chainの総数を初期化
        int total_chain = 0;

        // ゲームのクリア判定
        for (i = 0; i < Y; i++) {
            for (j = 0; j < X; j++) {
                // field[i][j]が'9'を超えていればクリア
                if (field[i][j] > '9') {
                    current_stage = 2;
                }
                for (k = 0; k < Y; k++) {
                    for (l = 0; l < X; l++) {
                        field_flag[k][l] = 0;
                    }
                }
                // chainがあるか(=まだ動かせるところがあるか)
                CheckChain(j, i, &total_chain);
            }
        }
        // クリア判定ならクリアボーナス(クリック回数が少ないほど高い)を加える
        if (current_stage == 2) {
            score += 200000 / (move_count + 1);
        }

        // 動かせるところがなく，シャッフルもできなければ強制的にクリア判定にする
        if (total_chain == 0 && shuffle_limit == 0) {
            current_stage = 2;
        }
    }

    // ステージ判定
    // 0: タイトル
    // 1: ゲーム
    // 2: 結果発表
    switch (current_stage) {
    case 0:
        // タイトル画面を描画
        DisplayTitle();
        break;
    case 1:
        // ゲームを描画
        DisplayGame();
        break;
    case 2:
        // 結果表示画面を描画
        DisplayResult();
        break;
    default:
        break;
    }
    glFlush();
}

//
//  num番のPNG画像を座標(x,y)に表示する
//
void PutSprite(int num, int x, int y, pngInfo *info) {
    int w, h; //  テクスチャの幅と高さ

    w = info->Width; //  テクスチャの幅と高さを取得する
    h = info->Height;

    // 画像を貼る設定
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, num);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS); //  幅w, 高さhの四角形

    glTexCoord2i(0, 0);
    glVertex2i(x, y);

    glTexCoord2i(0, 1);
    glVertex2i(x, y + h);

    glTexCoord2i(1, 1);
    glVertex2i(x + w, y + h);

    glTexCoord2i(1, 0);
    glVertex2i(x + w, y);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}