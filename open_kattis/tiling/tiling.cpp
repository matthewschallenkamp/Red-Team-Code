#include <iostream>
#include <array>

using namespace std;

array<array<uint8_t, 20>, 20> board;
array<array<bool, 10>, 10> dominoes;
const array<array<int8_t, 2>, 4> diffs{{{{-1, 0}}, {{0, -1}}, {{0, 1}}, {{1, 0}}}};
const string outChars = " u] [n";

int32_t m, n;
bool printed;

bool open(const int8_t& i, const int8_t& j)
{
    return board[i][j] <= 9;
}

bool valid(const int8_t& i, const int8_t& j)
{
    return i >= 0 && i < m && j >= 0 && j < n;
}

void printBoard()
{
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
            cout << (board[i][j] == 'X' ? 'X' : outChars[board[i][j]-10]);
        cout << endl;
    }
}

uint64_t backtrack()
{
    uint64_t out = 0;
    uint8_t min = 5;
    int8_t minI, minJ, adjI, adjJ;

    //Search entire board for square
    //with mininum number of placeable dominoes
    for(int8_t i=0; i<m; i++)
        for(int8_t j=0; j<n; j++)
        {
            if(open(i, j))
            {
                uint8_t count = 0;

                for(auto& p : diffs)
                {
                    adjI = i+p[0]; adjJ = j+p[1];
                    if(valid(adjI, adjJ) && open(adjI, adjJ) &&
                    dominoes[board[i][j]][board[adjI][adjJ]])
                        count++;
                }

                if(count <= min)
                {
                    min = count;
                    minI = i;
                    minJ = j;
                }
            }
        }

    //If none found, the board is covered
    if(min == 5)
    {
        if(!printed)
            printBoard();
        printed = true;
        return 1;
    }

    //If space found with min 0, the board's not filled
    if(min == 0) return 0;

    //Fill the min space all ways
    //and continue backtrack
    int8_t bak1, bak2;
    for(auto& p : diffs)
    {
        adjI = minI+p[0]; adjJ = minJ+p[1];
        if(valid(adjI, adjJ) && open(adjI, adjJ) &&
        dominoes[board[minI][minJ]][board[adjI][adjJ]])
        {
            dominoes[board[minI][minJ]][board[adjI][adjJ]] =
            dominoes[board[adjI][adjJ]][board[minI][minJ]] = false;

            bak1 = board[minI][minJ]; bak2 = board[adjI][adjJ];

            //Magic equations to index into outChars
            board[minI][minJ] = 10+2*(1+p[0])+1+p[1];
            board[adjI][adjJ] = 10+2*(1-p[0])+1-p[1];
					
            out += backtrack();

            board[minI][minJ] = bak1; board[adjI][adjJ] = bak2;

            dominoes[board[minI][minJ]][board[adjI][adjJ]] =
            dominoes[board[adjI][adjJ]][board[minI][minJ]] = true;
        }
    }
    return out;   
}

int main()
{
    int k;
    cin >> m >> n >> k;
    do
    {
        //Mark all dominoes unusable
        for(auto& l : dominoes)
            for(auto& b : l) b = false;

        int l, r;
        //Read dominoes and mark them usable
        //both ways
        for(int i=0; i<k; i++)
        {
            cin >> l >> r;
            dominoes[l][r] = dominoes[r][l] = true;
        }

        int8_t s;
        //Read the board
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++) 
            {
                cin >> s;
                if(s != 'X') board[i][j] = s-'0';
                else board[i][j] = 'X';
            }

        printed = false;
        uint64_t out = backtrack();
        if(!out) cout << "impossible" << endl;
        else cout << out-1 << endl;
        cout << endl;

        cin >> m >> n >> k;
    }while(m != 0);

    return 0;
}