#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
using namespace std;

const int ROWS = 6;
const int COLS = 7;
const int MAX_PLAYER_NUM = 0;
const int MIN_PLAYER_NUM = 1;
const int EMPTY_POSITION = 0;
const int MAX_PLAYER_PIECE = 1;
const int MIN_PLAYER_PIECE = 2;
int MAX_DEPTH;
int MIN_DEPTH;
const int ALPHA = 1000;
const int BETA = -1000;
int MAX_EVAL_FUNCTION;
int MIN_EVAL_FUNCTION;
int MAX_NODES_GENERATED = 0;
int MIN_NODES_GENERATED = 0;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

struct Position{
    int c;
    int val;
};

void create_board(int board[ROWS][COLS]){
    for (int i=0; i<ROWS; i++){
        for (int j=0; j<COLS; j++){
              board[i][j] = EMPTY_POSITION;
        }
    }
}

void printBoard(int board[ROWS][COLS]){
    cout<<"["<<endl;
    for (int i=ROWS -1; i>=0; i--){
        cout<<"[";
        for (int j=0; j<COLS-1; j++){
            cout<<board[i][j]<<", ";
        }
        cout<<board[i][COLS-1]<<"]"<<endl;
    }
    cout<<"]"<<endl;
}

void drop_piece(int board[ROWS][COLS], int row, int col, const int piece){
    board[row][col] = piece;
}

bool is_draw(int board[ROWS][COLS]){
    for (int r =0;r<ROWS;r++){
        for (int c =0;c<COLS;c++){
            if (board[r][c] == EMPTY_POSITION)
                return false;
        }
    }
    return true;
}

bool is_valid_location(int board[ROWS][COLS], int col){
    return (board[ROWS-1][col] == 0);
}

int get_next_open_row(int board[ROWS][COLS], int col){
    for(int r = 0; r < ROWS; r++){
        if (board[r][col] == 0)
            return r;
    }
    return -1;
}

vector<int> get_valid_locations(int board[ROWS][COLS]){
    vector<int> vect = vector<int>(7,-1);
    int index = 0;
    
    for (int c = 0;c<COLS;c++){
        if (is_valid_location(board, c)){
            vect[index]= c;
            index++;
        }
    }
    shuffle(vect.begin(), vect.end(), default_random_engine(seed));
    
    return vect;
}

bool winning_move(int board[ROWS][COLS], const int piece){
    
    //Check horizontal locations for win
    for (int c = 0; c < COLS-3; c++){
        for (int r = 0; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r][c+1] == piece) && (board[r][c+2] == piece) && (board[r][c+3] == piece)){
                return true;
            }
        }
    }
//Check vertical locations for win
    for (int c = 0; c < COLS; c++){
        for (int r = 0; r < ROWS-3; r++){
            if ((board[r][c] == piece) && (board[r+1][c] == piece) && (board[r+2][c] == piece) && (board[r+3][c] == piece)){
                return true;
            }
        }
    }
//Check positively sloped diaganols
    for (int c = 0; c < COLS-3;c++){
        for (int r = 0; r< ROWS-3;r++){
            if ((board[r][c] == piece) && (board[r+1][c+1] == piece) && (board[r+2][c+2] == piece) && (board[r+3][c+3] == piece)){
                return true;
            }
        }
    }
//Check negatively sloped diaganols
    for (int c = 0; c < COLS-3; c++){
        for (int r = 3; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r-1][c+1] == piece) && (board[r-2][c+2] == piece) && (board[r-3][c+3] == piece)){
                return true;
            }
        }
    }
    return false;
}

bool is_terminal_node(int board[ROWS][COLS]){
    if (winning_move(board, MAX_PLAYER_PIECE)){
        return true;
    }
    if (winning_move(board, MIN_PLAYER_PIECE)){
        return true;
    }
    if (is_draw(board)){
        return true;
    }
    return false;
}

bool deepEnough(int board[ROWS][COLS],int depth,const int player){
    
    if (player == MAX_PLAYER_NUM){
        if (is_terminal_node(board)){
            return true;
        }
        if (depth >= MAX_DEPTH){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if (is_terminal_node(board)){
            return true;
        }
        if (depth >= MIN_DEPTH){
            return true;
        }
        else{
            return false;
        }
    }
}

int evalFunctionAli(int board[ROWS][COLS], const int player){
    int total_score = 0;
    int piece = player + 1;
    
    if (winning_move(board, piece)){
        total_score = 1000;
        return total_score;
    }
    
    for (int c = 0; c < 5 ; c++){
        for (int r = 0; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r][c+1] == piece))
                total_score += 2;
            if ((board[r][c] == piece) && (board[r][c+1] == piece) && (board[r][c+2] == piece))
                total_score += 3;
        }
    }
    //vertical
    for (int c = 0; c < COLS ; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == piece) && (board[r+1][c] == piece))
                total_score += 2;
            if ((board[r][c] == piece) && (board[r+1][c] == piece) && (board[r+2][c] == piece))
                total_score += 3;
        }
    }
    //positive diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == piece) && (board[r+1][c+1] == piece))
                total_score += 2;
            if ((board[r][c] == piece) && (board[r+1][c+1] == piece) && (board[r+2][c+2] == piece))
                total_score += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[4][c] == piece) && (board[5][c+1] == piece))
            total_score += 2;
    }
    for (int r = 0; r < 5; r++){
        if ((board[r][5] == piece) && (board[r+1][6] == piece))
            total_score += 2;
    }
    
    //negative diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 2; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r-1][c+1] == piece))
                total_score += 2;
            if ((board[r][c] == piece) && (board[r-1][c+1] == piece) && (board[r-2][c+2] == piece))
                total_score += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[1][c] == piece) && (board[0][c+1] == piece))
            total_score += 2;
    }
    for (int r = 1; r < 6; r++){
        if ((board[r][5] == piece) && (board[r-1][6] == piece))
            total_score += 2;
    }
    
    return total_score;
}

int scoreLine(vector<int> line, const int player){
    int test[8];
    copy(line.begin(), line.end(), test);
    //shorthand
    int empty = 0;
    int pToken = player + 1;
    if(line.size() < 4){
        return 0;
    }
    //score line
    int score = 0;
    for(int i = 0; i < line.size() - 3; i++){
        int sum = 0;
        for(int j = i; j < i + 4; j++){
            if(line[j] != pToken && line[j] != empty){
                sum = 0;
                break;
            }
            sum += line[j];
        }
        if(sum > score){
            score = sum;
        }
        if(sum == 4){
            return 1000;
        }
    }
    return score;
}

int evalFunctionJack(int board[ROWS][COLS],const int player){
    int score = 0;
    for(int i = 0; i < COLS; i++){
        //find playable position in colummn
        int index = 0;
        while(board[index+1][i] == EMPTY_POSITION && (index + 1) != ROWS){
            index++;
        }

        vector<int> line;
        //check vertical
        for(int j = index - 3; j <= index + 3 && j < ROWS; j++){
            if(j < 0){ j = 0; }
            //int test = board[j][i];
            line.push_back(board[j][i]);
        }
        int ls = scoreLine(line, player);
        if(ls == 1000){
            return 1000;
        } else {
            score += ls;
        }

        //check horizontal
        line.clear();
        for(int j = i - 3; j <= i + 3 && j < COLS; j++){
            if(j < 0){ j = 0; }
            line.push_back(board[index][j]);
        }
        ls = scoreLine(line, player);
        if(ls == 1000){
            return 1000;
        } else {
            score += ls;
        }

        //check 1st diagonal
        line.clear();
        int j = index - 3;
        int k = i - 3;
        while(j <= index + 3 && j < ROWS && k <= i + 3 && k < COLS){
            if(j >= 0 && k >= 0){
                line.push_back(board[j][k]);
            }
            j++;
            k++;
        }
        ls = scoreLine(line, player);
        if(ls == 1000){
            return 1000;
        } else {
            score += ls;
        }

        //check 2nd diagonal
        line.clear();
        j = index - 3;
        k = i + 3;
        while(j <= index + 3 && j < ROWS && k >= i - 3 && k >= 0){
            if(j >= 0 && k < COLS){
                line.push_back(board[j][k]);
            }
            j++;
            k--;
        }
        ls = scoreLine(line, player);
        if(ls == 1000){
            return 1000;
        } else {
            score += ls;
        }
    }
    return score;
}

int evalFunctionMasin(int board[ROWS][COLS], const int player){
    int score = 0;
    int piece = player + 1;
    int oppositePiece;

    switch(player){
        case MAX_PLAYER_NUM:
            oppositePiece = MIN_PLAYER_PIECE;
            break;
        case MIN_PLAYER_NUM:
            oppositePiece = MAX_PLAYER_PIECE;
            break;
    }
    
    // check if board creates a winning move
    if (winning_move(board, piece)) {
        score = 1000;
    }
    else{
        // count concurrent pieces with potential to improve position (not blocked)
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                // check if current index is occupied by player
                if (board[r][c] == piece) {
                    // check above
                    if (r - 3 >= 0) {   // ensure check stays in bounds
                    // check if path is blocked
                        if (board[r - 1][c] != oppositePiece && board[r - 2][c] != oppositePiece && board[r - 3][c] != oppositePiece) {
                            if (board[r - 1][c] == piece && board[r - 2][c] == piece && board[r - 3][c] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r - 1][c] == piece && board[r - 2][c] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                    
                    // check up left
                    if (r - 3 >= 0 && c - 3 >= 0) { // ensure check stays in bounds
                    // check if path is blocked
                        if (board[r - 1][c - 1] != oppositePiece && board[r - 2][c - 2] != oppositePiece && board[r - 3][c - 3] != oppositePiece) {
                            if (board[r - 1][c - 1] == piece && board[r - 2][c - 2] == piece && board[r - 3][c - 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r - 1][c - 1] == piece && board[r - 2][c - 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else
                            {
                                score++;
                            }
                        }
                    }
                    
                    // check up right
                    if (r - 3 >= 0 && c + 3 <= COLS) {  // ensure check stays in bounds
                        // check if path is blocked
                        if (board[r - 1][c + 1] != oppositePiece && board[r - 2][c + 2] != oppositePiece && board[r - 3][c + 3] != oppositePiece) {
                            if (board[r - 1][c + 1] == piece && board[r - 2][c + 2] == piece && board[r - 3][c + 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r - 1][c + 1] == piece && board[r - 2][c + 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                    
                    // check left
                    if (c - 3 >= 0) {   // ensure check stays in bounds
                        // check if path is blocked
                        if (board[r][c - 1] != oppositePiece && board[r][c - 2] != oppositePiece && board[r][c - 3] != oppositePiece) {
                            if (board[r][c - 1] == piece && board[r][c - 2] == piece && board[r][c - 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r][c - 1] == piece && board[r][c - 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                    
                    // check right
                    if (c + 3 <= COLS) {    // ensure check stays in bounds
                        // check if path is blocked
                        if (board[r][c + 1] != oppositePiece && board[r][c + 2] != oppositePiece && board[r][c + 3] != oppositePiece) {
                            if (board[r][c + 1] == piece && board[r][c + 2] == piece && board[r][c + 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r][c + 1] == piece && board[r][c + 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                    
                    // check down left
                    if (r + 3 <= ROWS && c - 3 >= 0) {  // ensure check stays in bounds
                        // check if path is blocked
                        if (board[r + 1][c - 1] != oppositePiece && board[r + 2][c - 2] != oppositePiece && board[r + 3][c - 3] != oppositePiece) {
                            if (board[r + 1][c - 1] == piece && board[r + 2][c - 2] == piece && board[r + 3][c - 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r + 1][c - 1] == piece && board[r + 2][c - 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                    
                    // check down right
                    if (r + 3 <= ROWS && c + 3 <= COLS) {   // ensure check stays in bounds
                        // check if path is blocked
                        if (board[r + 1][c + 1] != oppositePiece && board[r + 2][c + 2] != oppositePiece && board[r + 3][c + 3] != oppositePiece) {
                            if (board[r + 1][c + 1] == piece && board[r + 2][c + 2] == piece && board[r + 3][c + 3] == EMPTY_POSITION) {
                                score += 5;
                            }
                            else if (board[r + 1][c + 1] == piece && board[r + 2][c + 2] == EMPTY_POSITION) {
                                score += 2;
                            }
                            else {
                                score++;
                            }
                        }
                    }
                }
            }
        }
    }
    return score;
}


int evalFunction(int board[ROWS][COLS], const int player){
    
    if (player == MAX_PLAYER_NUM){
        if (MAX_EVAL_FUNCTION == 1)
            return evalFunctionAli(board,player);
        else if (MAX_EVAL_FUNCTION == 2)
            return evalFunctionJack(board,player);
        else
            return evalFunctionMasin(board,player);
    }
    else{
        if (MIN_EVAL_FUNCTION == 1)
            return evalFunctionAli(board,player);
        else if (MIN_EVAL_FUNCTION == 2)
            return evalFunctionJack(board,player);
        else
            return evalFunctionMasin(board,player);
    }
}

Position minimaxAB(int board[ROWS][COLS],int depth,const int player,int use_thresh,int pass_thresh){
    
    Position funcPos;
    
    if (deepEnough(board,depth,player)){
        funcPos.val = evalFunction(board,player);
        if (player == MAX_PLAYER_NUM)
            funcPos.val = funcPos.val;
        else
            funcPos.val = -funcPos.val;

        return funcPos;
    }
    else{
        int newValue;
        Position localPos;
        vector<int> successors = get_valid_locations(board);
        
        for (int i = 0;(i < successors.size());i++){
            if (successors[i] == -1)
                continue;
            
            int validColumn = successors[i];
            int validRow = get_next_open_row(board, validColumn);
            if (player == MAX_PLAYER_NUM){
                drop_piece(board, validRow, validColumn, MAX_PLAYER_PIECE);
                MAX_NODES_GENERATED++;
            }
            else{
                drop_piece(board, validRow, validColumn, MIN_PLAYER_PIECE);
                MIN_NODES_GENERATED++;
            }
            
            int boardCopy[ROWS][COLS];
            std::copy(&board[0][0], &board[0][0]+ROWS*COLS, &boardCopy[0][0]);
            
            newValue = -(minimaxAB(boardCopy,depth+1, !player, -pass_thresh, -use_thresh).val);
            localPos.val = newValue;
            
            if (newValue >= pass_thresh){
                pass_thresh = newValue;
                localPos.c = validColumn;
                localPos.val = newValue;
            }
            
            board[validRow][validColumn] = EMPTY_POSITION;
            
            if (pass_thresh >= use_thresh){
                break; //pruned
            }
            
        }
        funcPos.c = localPos.c;
        funcPos.val = localPos.val;

        return funcPos;
    }
    
}

int main(int argc, const char * argv[]) {
    if (argc != 5) {
        cerr << "ERROR: INCORRECT PROGRAM EXECUTION" << endl << "Correct program arguments: executable MAX_DEPTH MAX_EVAL_FUNCTION MIN_DEPTH MIN_EVAL_FUNCTION" << endl;
        return -1;
    }
    
    MAX_DEPTH = atoi(argv[1]);
    MAX_EVAL_FUNCTION = atoi(argv[2]);
    MIN_DEPTH = atoi(argv[3]);
    MIN_EVAL_FUNCTION = atoi(argv[4]);
    
    auto start = chrono::high_resolution_clock::now(); //start clock here
    int board[ROWS][COLS] = {};
    create_board(board);
    bool game_over = false;
    int turn = seed % 2;
    
    if (turn == MAX_PLAYER_NUM)
        cout<<"MAX player starts the game"<<endl;
    else
        cout<<"MIN player starts the game"<<endl;
    
    while (!game_over){
        if (turn == MAX_PLAYER_NUM){
            Position pos = minimaxAB(board, 0, MAX_PLAYER_NUM, ALPHA, BETA);
            if (is_valid_location(board, pos.c)){
                int row = get_next_open_row(board, pos.c);
                drop_piece(board, row, pos.c, MAX_PLAYER_PIECE);
                
                if ((winning_move(board, MAX_PLAYER_PIECE))){
                    game_over = true;
                    cout<< "MAX player wins" <<endl;
                }
                else if ((winning_move(board, MIN_PLAYER_PIECE))){
                    game_over = true;
                    cout<< "MIN player wins" <<endl;
                }
                else if (is_draw(board)){
                    game_over = true;
                    cout << "Game ends in a Draw" <<endl;
                }
                
                turn++;
                turn = turn % 2;
            }
        }
        else{
            Position pos = minimaxAB(board, 0, MIN_PLAYER_NUM, ALPHA, BETA);
            if (is_valid_location(board, pos.c)){
                int row = get_next_open_row(board, pos.c);
                drop_piece(board, row, pos.c, MIN_PLAYER_PIECE);
                
                if ((winning_move(board, MIN_PLAYER_PIECE))){
                    game_over = true;
                    cout<< "MIN player wins" <<endl;
                }
                else if ((winning_move(board, MAX_PLAYER_PIECE))){
                    game_over = true;
                    cout<< "MAX player wins" <<endl;
                }
                else if (is_draw(board)){
                    game_over = true;
                    cout << "Game ends in a Draw" <<endl;
                }
                
                turn++;
                turn = turn % 2;
            }
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    long long time = duration.count();
    printBoard(board);
    cout<<"Max Depth used: "<<MAX_DEPTH<<endl;
    cout<<"Evaluation Function used by Max: "<<MAX_EVAL_FUNCTION<<endl;
    cout<<"Min Depth used: "<<MIN_DEPTH<<endl;
    cout<<"Evaluation Function used by Min: "<<MIN_EVAL_FUNCTION<<endl;
    
    cout << "Program Execution Time: " << time << " milliseconds" << endl;
    cout<<"Total Max Nodes Generated: "<<MAX_NODES_GENERATED<<endl;
    cout<<"Total Min Nodes Generated: "<<MIN_NODES_GENERATED<<endl;
    
    return 0;
}

