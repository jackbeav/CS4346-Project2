

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
    return -1; //check if this causes issue
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
    /*for (auto i = vect.begin(); i != vect.end();i++){
        cout << *i <<" ";
    }
    cout<<endl;
    */
    shuffle(vect.begin(), vect.end(), default_random_engine(seed));
   /* for (auto i = vect.begin(); i != vect.end();i++){
        cout << *i <<" ";
    }
    cout<<endl; */
    
    return vect;
}

bool winning_move(int board[ROWS][COLS], const int piece){
    //printBoard(board);
    
    //Check horizontal locations for win
    for (int c = 0; c < COLS-3; c++){
        for (int r = 0; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r][c+1] == piece) && (board[r][c+2] == piece) && (board[r][c+3] == piece)){
                //cout<< "winning move is horizontal" <<endl;
                return true;
            }
        }
    }
//Check vertical locations for win
    for (int c = 0; c < COLS; c++){
        for (int r = 0; r < ROWS-3; r++){
            if ((board[r][c] == piece) && (board[r+1][c] == piece) && (board[r+2][c] == piece) && (board[r+3][c] == piece)){
                //cout<< "winning move is vertical" <<endl;
                return true;
            }
        }
    }
//Check positively sloped diaganols
    for (int c = 0; c < COLS-3;c++){
        for (int r = 0; r< ROWS-3;r++){
            if ((board[r][c] == piece) && (board[r+1][c+1] == piece) && (board[r+2][c+2] == piece) && (board[r+3][c+3] == piece)){
                //cout<< "winning move is positive diagonal" <<endl;
                return true;
            }
        }
    }
//Check negatively sloped diaganols
    for (int c = 0; c < COLS-3; c++){
        for (int r = 3; r < ROWS; r++){
            if ((board[r][c] == piece) && (board[r-1][c+1] == piece) && (board[r-2][c+2] == piece) && (board[r-3][c+3] == piece)){
                //cout<< "winning move is negative diagonal" <<endl;
                return true;
            }
        }
    }
    //cout<< "Not a winning move" <<endl;
    return false;
}

bool is_terminal_node(int board[ROWS][COLS]){
    //printBoard(board);
    //cout<< "Inside is_terminal_node function" <<endl;
    if (winning_move(board, MAX_PLAYER_PIECE)){
        //cout<< "Max player win in terminal node" <<endl;
        return true;
    }
    if (winning_move(board, MIN_PLAYER_PIECE)){
        //cout<< "Min player win in terminal node" <<endl;
        return true;
    }
    if (is_draw(board)){
        //cout<< "Is a draw in terminal node" <<endl;
        return true;
    }
    //cout<< "Not a terminal node from is_terminal_node function" <<endl;
    return false;
}

bool deepEnough(int board[ROWS][COLS],int depth,const int player){
    //printBoard(board);
    
    if (player == MAX_PLAYER_NUM){
        //cout<<"Player: "<<player<<" depth: "<<depth<< " in deepEnough function"  <<endl;
        if (is_terminal_node(board)){
            //cout<< "It's a terminal node" <<endl;
            return true;
        }
        if (depth >= MAX_DEPTH){
            //cout<< "depth is equal to MAX_DEPTH" <<endl;
            return true;
        }
        else{
            //cout<< "Not deep Enough" <<endl;
            return false;
        }
    }
    else{
        //cout<<"Player: "<<player<<" depth: "<<depth<< " in deepEnough function"<<endl;
        if (is_terminal_node(board)){
            //cout<< "It's a terminal node" <<endl;
            return true;
        }
        if (depth >= MIN_DEPTH){
            //cout<< "depth is equal to MIN_DEPTH" <<endl;
            return true;
        }
        else{
            //cout<< "Not deep Enough" <<endl;
            return false;
        }
    }
}

int evalFunctionAli(int board[ROWS][COLS]){
    int total_score = 0;
    
    if (winning_move(board, MAX_PLAYER_PIECE)){
        total_score = 1000;
        //cout<<"In eval function MAX player is winning so return 1000"<<endl;
        return total_score;
    }
    if (winning_move(board, MIN_PLAYER_PIECE)){
        total_score = -1000;
        //cout<<"In eval function MIN player is winning so return -1000"<<endl;
        return total_score;
    }
    
    if (is_draw(board)){
        total_score = 0;
        //cout<<"In eval function it's a draw so return 0"<<endl;
        return total_score;
    }
    
    int max_player_array[4] = {0,0,0,0};
    int min_player_array[4] = {0,0,0,0};
    
    //for MAX
    //horizontal
    for (int c = 0; c < 5 ; c++){
        for (int r = 0; r < ROWS; r++){
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r][c+1] == MAX_PLAYER_PIECE))
                max_player_array[0] += 2;
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r][c+1] == MAX_PLAYER_PIECE) && (board[r][c+2] == MAX_PLAYER_PIECE))
                max_player_array[0] += 3;
        }
    }
    //vertical
    for (int c = 0; c < COLS ; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r+1][c] == MAX_PLAYER_PIECE))
                max_player_array[1] += 2;
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r+1][c] == MAX_PLAYER_PIECE) && (board[r+2][c] == MAX_PLAYER_PIECE))
                max_player_array[1] += 3;
        }
    }
    //positive diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r+1][c+1] == MAX_PLAYER_PIECE))
                max_player_array[2] += 2;
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r+1][c+1] == MAX_PLAYER_PIECE) && (board[r+2][c+2] == MAX_PLAYER_PIECE))
                max_player_array[2] += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[4][c] == MAX_PLAYER_PIECE) && (board[5][c+1] == MAX_PLAYER_PIECE))
            max_player_array[2] += 2;
    }
    for (int r = 0; r < 5; r++){
        if ((board[r][5] == MAX_PLAYER_PIECE) && (board[r+1][6] == MAX_PLAYER_PIECE))
            max_player_array[2] += 2;
    }
    
    //negative diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 2; r < ROWS; r++){
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r-1][c+1] == MAX_PLAYER_PIECE))
                max_player_array[3] += 2;
            if ((board[r][c] == MAX_PLAYER_PIECE) && (board[r-1][c+1] == MAX_PLAYER_PIECE) && (board[r-2][c+2] == MAX_PLAYER_PIECE))
                max_player_array[3] += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[1][c] == MAX_PLAYER_PIECE) && (board[0][c+1] == MAX_PLAYER_PIECE))
            max_player_array[3] += 2;
    }
    for (int r = 1; r < 6; r++){
        if ((board[r][5] == MAX_PLAYER_PIECE) && (board[r-1][6] == MAX_PLAYER_PIECE))
            max_player_array[3] += 2;
    }
    //for MIN
    //horizontal
    for (int c = 0; c < 5 ; c++){
        for (int r = 0; r < ROWS; r++){
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r][c+1] == MIN_PLAYER_PIECE))
                min_player_array[0] += 2;
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r][c+1] == MIN_PLAYER_PIECE) && (board[r][c+2] == MIN_PLAYER_PIECE))
                min_player_array[0] += 3;
        }
    }
    //vertical
    for (int c = 0; c < COLS ; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r+1][c] == MIN_PLAYER_PIECE))
                min_player_array[1] += 2;
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r+1][c] == MIN_PLAYER_PIECE) && (board[r+2][c] == MIN_PLAYER_PIECE))
                min_player_array[1] += 3;
        }
    }
    //positive diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 0; r < 4; r++){
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r+1][c+1] == MIN_PLAYER_PIECE))
                min_player_array[2] += 2;
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r+1][c+1] == MIN_PLAYER_PIECE) && (board[r+2][c+2] == MIN_PLAYER_PIECE))
                min_player_array[2] += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[4][c] == MIN_PLAYER_PIECE) && (board[5][c+1] == MIN_PLAYER_PIECE))
            min_player_array[2] += 2;
    }
    for (int r = 0; r < 5; r++){
        if ((board[r][5] == MIN_PLAYER_PIECE) && (board[r+1][6] == MIN_PLAYER_PIECE))
            min_player_array[2] += 2;
    }
    
    //negative diagonal
    for (int c = 0; c < 5; c++){
        for (int r = 2; r < ROWS; r++){
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r-1][c+1] == MIN_PLAYER_PIECE))
                min_player_array[3] += 2;
            if ((board[r][c] == MIN_PLAYER_PIECE) && (board[r-1][c+1] == MIN_PLAYER_PIECE) && (board[r-2][c+2] == MIN_PLAYER_PIECE))
                min_player_array[3] += 3;
        }
    }
    for (int c = 0; c < 6; c++){
        if ((board[1][c] == MIN_PLAYER_PIECE) && (board[0][c+1] == MIN_PLAYER_PIECE))
            min_player_array[3] += 2;
    }
    for (int r = 1; r < 6; r++){
        if ((board[r][5] == MIN_PLAYER_PIECE) && (board[r-1][6] == MIN_PLAYER_PIECE))
            min_player_array[3] += 2;
    }
    
    total_score = (max_player_array[0] + max_player_array[1] + max_player_array[2] + max_player_array[3]) - (min_player_array[0] + min_player_array[1] + min_player_array[2] + min_player_array[3]);
    
    //cout<<"In eval function current score returning is: "<<total_score<<endl;
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
            if(line[j] == pToken){
                sum += 1;
            }
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
            int test = board[j][i];
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
        int ls = scoreLine(line, player);
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
        int ls = scoreLine(line, player);
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
        int ls = scoreLine(line, player);
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

    switch(player) {
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
    } else {
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
            return evalFunctionAli(board);
        else if (MAX_EVAL_FUNCTION == 2)
            return evalFunctionJack(board,player);
        else
            return evalFunctionMasin(board, player);
    }
    else{
        if (MIN_EVAL_FUNCTION == 1)
            return evalFunctionAli(board);
        else if (MIN_EVAL_FUNCTION == 2)
            return evalFunctionJack(board,player);
        else
            return evalFunctionMasin(board, player);
    }
}

Position minimaxAB(int board[ROWS][COLS],int depth,const int player,int use_thresh,int pass_thresh){
    //cout<<"Entered Function -> player: "<<player<<" depth: "<<depth<<" use thres: "<<use_thresh<<" pass thres: "<<pass_thresh<<endl;
    
    Position funcPos;
    
    if (deepEnough(board,depth,player)){
        funcPos.val = evalFunction(board,player);
        if (player == MAX_PLAYER_NUM)
            funcPos.val = funcPos.val;
        else
            funcPos.val = -funcPos.val;
        //cout<<"Exiting Deep Node with funcPos value: "<< funcPos.val<<endl;
        //cout<<"Exiting Function -> player: "<<player<<" depth: "<<depth<<" funcPos col returning: "<<funcPos.c<<" funcPos value: "<<funcPos.val<<endl;
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
            //printBoard(board);
            //cout<<"current row & col: "<<validRow<<"  "<< validColumn<<endl;
            
            newValue = -(minimaxAB(boardCopy,depth+1, !player, -pass_thresh, -use_thresh).val);
            localPos.val = newValue;
            
            if (newValue > pass_thresh){
                pass_thresh = newValue;
                localPos.c = validColumn;
                localPos.val = newValue;
                //funcPos.val = localPos.val; // change this if needed testing
            }
            //else
            //    localPos.val = newValue;
            
            board[validRow][validColumn] = EMPTY_POSITION;
            
            //cout<<"newValue: "<<newValue<<" pass thres: "<<pass_thresh<<endl;
            if (pass_thresh >= use_thresh){
                //cout<<"Tree is pruned"<<endl;
                break; //pruned
            }
            
        }
        funcPos.c = localPos.c;
        funcPos.val = localPos.val;
        if (player == MAX_PLAYER_NUM)
            funcPos.val = funcPos.val;
        else
            funcPos.val = -funcPos.val;
        //cout<<"Exiting Level with funcPos col: "<<funcPos.c<< " funcPos value: "<< funcPos.val<<endl;
        //cout<<"Exiting Function -> player: "<<player<<" depth: "<<depth<<" funcPos col returning: "<<funcPos.c<<" funcPos value: "<<funcPos.val<<endl;
        return funcPos;
    }
    
}

int main(int argc, const char * argv[]) {
    if (argc != 5) {
        cerr << "ERROR: INCORRECT PROGRAM EXECUTION" << endl << "Correct program execution: mimimaxAB.exe MAX_DEPTH MAX_EVAL_FUNCTION MIN_DEPTH MIN_EVAL_FUNCTION" << endl;
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
    int turn = rand() % 2;
    
    if (turn == MAX_PLAYER_NUM)
        cout<<"MAX player starts the game"<<endl;
    else
        cout<<"MIN player starts the game"<<endl;
    
    while (!game_over){
        if (turn == MAX_PLAYER_NUM){
            //cout<<"MAX player turn" <<endl;
            Position pos = minimaxAB(board, 0, MAX_PLAYER_NUM, ALPHA, BETA);
            //cout<<"In main function, col returned: "<<pos.c<<" value : "<<pos.val<<endl;
            if (is_valid_location(board, pos.c)){
                int row = get_next_open_row(board, pos.c);
                drop_piece(board, row, pos.c, MAX_PLAYER_PIECE);
                //cout<<"current row & col: "<<row<<"  "<< pos.c<<endl;

                
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
                printBoard(board);
            }
        }
        else{
            //cout<<"MIN player turn" <<endl;
            Position pos = minimaxAB(board, 0, MIN_PLAYER_NUM, ALPHA, BETA);
            //cout<<"In main function, col returned: "<<pos.c<<" value : "<<pos.val<<endl;
            if (is_valid_location(board, pos.c)){
                int row = get_next_open_row(board, pos.c);
                drop_piece(board, row, pos.c, MIN_PLAYER_PIECE);
                //cout<<"current row & col: "<<row<<"  "<< pos.c<<endl;

                
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
                //printBoard(board);
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
