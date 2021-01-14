#include "main.h"
#include "move.h"

void InitGame(void) {
    int i, j;

    // スコア，移動回数，シャッフルの残回数を初期化
    score = 0;
    move_count = 0;
    shuffle_limit = 3;
    // ブロックをランダムに初期化
    for (i = 0; i < Y; i++) {
        for (j = 0; j < X; j++) {
            field[i][j] = '0' + (random() % 7 + 1);
        }
    }
}

//
// ブロックをシャッフルする関数
//
void Shuffle(void) {
    int i, j;
    int xrand, yrand;
    char tmp;
    for (i = 0; i < Y; i++) {
        for (j = 0; j < X; j++) {
            // x, yをランダムに取得
            xrand = random() % (X - j) + j;
            yrand = random() % (Y - i) + i;
            // field[yrand][xrand]とfield[i][j]の値を交換する
            tmp = field[i][j];
            field[i][j] = field[yrand][xrand];
            field[yrand][xrand] = tmp;
        }
    }
}

//
// 繋がっているブロックを探索する関数
//
void ReplaceChain(int x, int y, int *chain) {
    field_flag[y][x] = 1; // 調査済みフラグを立てて調査の重複を防ぐ
    // 上方向に同じブロックがあるとき
    if (y > 0 && field_flag[y - 1][x] == 0 && field[y - 1][x] == field[y][x]) {
        field_cpy[y - 1][x] = 'C'; // 上方向のブロックを削除する
        (*chain)++;
        // 上方向のブロックから再帰的に調査する
        ReplaceChain(x, y - 1, chain);
    }
    // 下方向に同じブロックがあるとき
    if (y < Y - 1 && field_flag[y + 1][x] == 0 && field[y + 1][x] == field[y][x]) {
        field_cpy[y + 1][x] = 'C'; // 下方向のブロックを削除する
        (*chain)++;
        // 下方向のブロックから再帰的に調査する
        ReplaceChain(x, y + 1, chain);
    }
    // 左方向に同じブロックがあるとき
    if (x > 0 && field_flag[y][x - 1] == 0 && field[y][x - 1] == field[y][x]) {
        field_cpy[y][x - 1] = 'C'; // 左方向のブロックを削除する
        (*chain)++;
        // 左方向のブロックから再帰的に調査する
        ReplaceChain(x - 1, y, chain);
    }
    // 右方向に同じブロックがあるとき
    if (x < X - 1 && field_flag[y][x + 1] == 0 && field[y][x + 1] == field[y][x]) {
        field_cpy[y][x + 1] = 'C'; // 右方向のブロックを削除する
        (*chain)++;
        // 右方向のブロックから再帰的に調査する
        ReplaceChain(x + 1, y, chain);
    }
}

//
// ブロックを操作する関数
//
void Click(int x, int y) {
    int i, j, tmp;
    // 出現するブロックを抽選するための配列
    char choice[15] = {
        '1',
        '1',
        '1',
        '2',
        '2',
        '2',
        '3',
        '3',
        '4',
        '4',
        '5',
        '5',
        '6',
        '7',
        '8'};
    int chain = 0;

    // x, y座標を現在の座標に代入
    current_x = x;
    current_y = y;

    // field_cpyにfieldの内容をコピー
    for (i = 0; i < Y; i++) {
        for (j = 0; j < X; j++) {
            field_cpy[i][j] = field[i][j];
        }
    }

    // field_flagの初期化
    for (i = 0; i < Y; i++) {
        for (j = 0; j < X; j++) {
            field_flag[i][j] = 0;
        }
    }

    // 押したブロックから繋がっているブロックを削除
    ReplaceChain(x / LENGTH - 1, y / LENGTH - 1, &chain);

    // 繋がっているブロックがあった場合
    if (chain >= 1) {
        move_count++; // 操作回数を1回増やす
        //スコアにブロックを加算
        score += chain * 10 * (field[y / LENGTH - 1][x / LENGTH - 1] - '0');
        // クリックしたブロックに1を加算
        field[y / LENGTH - 1][x / LENGTH - 1] = field[y / LENGTH - 1][x / LENGTH - 1] + 1;
        // 削除したブロックがクリックしたブロックの直下にあった場合，current_yを移動
        while (field_cpy[current_y / LENGTH][current_x / LENGTH - 1] == 'C') {
            current_y += LENGTH;
        }
        // 削除したブロックの移動と新しいブロックの追加
        for (i = 0; i < Y; i++) {
            for (j = 0; j < X; j++) {
                // 削除したブロックだった場合
                if (field_cpy[i][j] == 'C') {
                    tmp = i;
                    // 削除したブロックの上にあるブロックを1つずつ下にもってくる
                    while (tmp > 0) {
                        field[tmp][j] = field[tmp - 1][j];
                        tmp--;
                    }
                    // 出現するブロックを抽選する箱からランダムに取り出して一番上のブロックとする
                    int r = random() % 15;
                    field[0][j] = choice[r];
                }
            }
        }
    }
}

//
// マウスの動作を取得する関数
//
void Mouse(int b, int s, int x, int y) {
    // 左クリックを押下したとき
    if (b == GLUT_LEFT_BUTTON) {
        if (s == GLUT_DOWN) {

            // ステージによって処理を分ける
            // 0: タイトル
            // 1: ゲーム
            // 2: 結果表示
            switch (current_stage) {
            case 0:
                // Playボタンが押されたかを判定
                if (x > 150 && x < 290 && y > 330 && y < 380) {
                    current_stage = 1; //ゲームステージに変更
                    InitGame();        // ゲームの初期化
                }
                break;
            case 1:
                // ブロックが押されたかを判定
                if (x > LENGTH && x < (X + 1) * LENGTH && y > LENGTH && y < (Y + 1) * LENGTH) {
                    Click(x, y); //ブロックに対して操作
                }
                // Shuffleボタンが押されたかを判定
                if (x > 310 && x < 410 && y > 240 && y < 310) {
                    // シャッフルの回数が残っていればシャッフルする
                    if (shuffle_limit > 0) {
                        Shuffle();
                        shuffle_limit--;
                    }
                }
                // Titleボタンが押されたか判定
                if (x > 313 && x < 405 && y > 335 && y < 405) {
                    current_stage = 0; // タイトルステージに変更
                }
                break;
            case 2:
                // Restertボタンが押されたか判定
                if (x > 90 && x < 340 && y > 225 && y < 280) {
                    current_stage = 1; //ゲームステージに変更
                    InitGame();        //ゲームの初期化
                }
                // Titleボタンが押されたか判定
                if (x > 90 && x < 340 && y > 310 && y < 365) {
                    current_stage = 0; // タイトルステージに変更
                }
                break;
            }
            // 画面の再描画
            glutPostRedisplay();
        }
    }
}

//
// ウィンドウがアクティブなときにマウスのx,y座標を取得する関数
//
void PassiveMotion(int x, int y) {
    // ブロックの範囲内かを確認する
    if (x > LENGTH && x < (X + 1) * LENGTH && y > LENGTH && y < (Y + 1) * LENGTH) {
        // マウスの座標を現在の座標にする
        current_x = x;
        current_y = y;
    } else {
        // 現在の座標をブロックの外にする
        current_x = 0;
        current_y = 0;
    }
    // Shuffleボタンの上にマウスがあるとき
    if (x > 310 && x < 410 && y > 240 && y < 310) {
        shufflecursor_flag = 1; // 枠をつける
        current_x = 360;
        current_y = 270;
    } else {
        shufflecursor_flag = 0; // 枠を外す
    }
    // Titleボタンの上にマウスがあるとき
    if (x > 313 && x < 405 && y > 335 && y < 405) {
        titlecursor_flag = 1; // 枠をつける
        current_x = 360;
        current_y = 380;
    } else {
        titlecursor_flag = 0; // 枠を外す
    }
    glutPostRedisplay(); // 画面の再描画
}

//
// 一般的なキーの取得をする関数
//
void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 13:                               //Enterキーが押されたとき
        if (current_stage == 1) {          // ゲームステージの場合
            if (shufflecursor_flag == 1) { // Shuffleボタンに枠があるとき
                // シャッフルの回数が残っていればシャッフルする
                if (shuffle_limit > 0) {
                    Shuffle();
                    shuffle_limit--;
                }
            } else if (titlecursor_flag == 1) { // Titleボタンに枠があるとき
                current_stage = 0;              // タイトルステージに変更する
            } else {
                Click(current_x, current_y); // マウスが押されたときと同じ動作をする
            }
        }
        break;
    case 32: //スペースキーが押されたときの動作
        if (current_stage == 0) {
            current_stage = 1; //ゲームステージに変更
            InitGame();        //ゲームの初期化
        }
        break;
    case '1':                     // '1'キーが押されたときの動作
        if (current_stage == 2) { //結果表示ステージのとき
            current_stage = 1;    // ゲームステージに変更
            InitGame();           //ゲームの初期化
        }
        break;
    case '2':                     // '2'キーが押されたときの動作
        if (current_stage == 2) { //結果表示ステージのとき
            current_stage = 0;    //タイトルステージに変更
        }
        break;
    default:
        break;
    }
    glFlush();
}

//
// 特殊なキーの取得をする関数
//
void SpecialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:                // 上方向キー
        if (titlecursor_flag == 1) { // Titleボタンに枠があるときはShuffleボタンに枠をつける
            shufflecursor_flag = 1;
            titlecursor_flag = 0;
            current_y -= LENGTH * 2;
        } else if (current_y - (LENGTH * 2) >= 0) { // 上方向に移動できれば移動
            current_y -= LENGTH;
        } else {
        }
        break;
    case GLUT_KEY_DOWN:                // 下方向キー
        if (shufflecursor_flag == 1) { // Shuffleボタンに枠があるときはTitleボタンに枠をつける
            shufflecursor_flag = 0;
            titlecursor_flag = 1;
            current_y += LENGTH * 2;
        } else if (current_y <= LENGTH * Y) { // 下方向に移動できれば移動
            current_y += LENGTH;

        } else {
        }
        break;
    case GLUT_KEY_LEFT: // 左方向キー
        //いずれかのフラグが立っている場合はフラグを0にして，ブロックの領域に移動
        if (shufflecursor_flag == 1) {
            shufflecursor_flag = 0;
            current_x = LENGTH * 5;
            current_y = LENGTH * 5;
        } else if (titlecursor_flag == 1) {
            titlecursor_flag = 0;
            current_x = LENGTH * 5;
            current_y = LENGTH * 7;
        } else if (current_x - (LENGTH * 2) >= 0) { // 左方向に移動できれば移動
            current_x -= LENGTH;
        } else {
        }
        break;
    case GLUT_KEY_RIGHT: // 右方向キー
        // 右方向に移動できれば移動
        if (current_x < LENGTH * X) {
            current_x += LENGTH;
        } else if (titlecursor_flag == 0) { // 現在地がブロックの一番右側なら，Shuffleボタンに移動
            shufflecursor_flag = 1;
            current_x = 360;
            current_y = 270;
        } else {
        }
        break;
    default:
        break;
    }
}