#include <iostream>
#include <fstream>
#define clean '8'

using namespace std;

int main(void)
{
    int num_step;
    int row, col, total_energy;
    int now_energy;
    int now_position_row, now_position_col;
    int next_position_row, next_position_col;
    int R_col, R_row;
    char *temp1;
    char **temp;

    fstream path, map;
    path.open("final.path", ios::in);
    map.open("floor.data", ios::in);
    
    map >> row >> col >> total_energy;
    
    temp1 = new char[row * col];
    temp = new char*[row];
    
    for (int i = 0; i < row; i++) {
        temp[i] = temp1 + i * col;
        map >> temp[i];
    }
    map.close();

    path >> num_step;
    path >> now_position_row >> now_position_col;
    if (temp[now_position_row][now_position_col] != 'R') {
        cout << "fail since the R_position is wrong" << endl;
        return -1;
    }
    R_row = now_position_row, R_col = now_position_col; 
    cout << "R_position " << now_position_row << ' ' << now_position_col << endl;

    for (int i = 0, now_energy = total_energy; i < num_step; i++) {
        if (now_position_row == R_row && now_position_col == R_col)
            now_energy = total_energy;
        path >> next_position_row >> next_position_col;
        if ((next_position_col - now_position_col) * (next_position_col - now_position_col) +
            (next_position_row - now_position_row) * (next_position_row - now_position_row) != 1) {
            cout << "fail since jumping from some step" << endl;
            return -1;
        }
        if (temp[next_position_row][next_position_col] == '1') {
            cout << "fail since walk into wall" << endl;
            return -1;       
        }
        now_energy--;
        if (now_energy < 0) {
            cout << "fail since energy is used out" << endl;
            cout << next_position_row << ' ' << next_position_col << endl;
            cout << i << endl;
            cout << now_energy << endl;
            return -1;
        }
        now_position_row = next_position_row;
        now_position_col = next_position_col;
        temp[now_position_row][now_position_col] = clean;
    }
    if (now_position_row == R_row && now_position_col != R_col) {
        cout << "did'nt go home" << endl;
        cout << now_position_row << ' ' << now_position_col << endl;
    }
    for (int i = 0; i < row * col; i++) {
        if (temp[i / col][i % col] == '0') {
            cout << "not all clean" << endl;
            return -1;
        }
    }

    
    cout << "success" << endl;    
    return 0;
}