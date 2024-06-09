#include<fstream>
#include <iostream>
#include<Windows.h>
using namespace std;
enum color { White, Black };
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
void SetClr(int clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
struct Position
{
    char x;
    int y;
};
void init(char**& B, char**& isFlip, string PNames[2], int& Dim, int& Turn, char**& PlayerCaptured, ifstream& Rdr)
{
    Dim = 9;
    B = new char* [Dim];
    isFlip = new char* [Dim];
    for (int ri = 0; ri < Dim; ri++)
    {
        B[ri] = new char[Dim];
        isFlip[ri] = new char[Dim];
        for (int ci = 0; ci < Dim; ci++)
        {
            Rdr >> B[ri][ci];
            isFlip[ri][ci] = 'n';
        }
    }
    PlayerCaptured = new char* [2];
    for (int i = 0; i < 2; i++)
    {
        string a;
        cout << "Enter Player " << i + 1 << " Name" << endl;
        cin >> a;
        PNames[i] = a;
        PlayerCaptured[i] = new char[20];
    }
    Turn = White;
}
bool iswhite(char sym)
{
    if (sym >= 'A' && sym <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isblack(char sym)
{
    if (sym >= 'a' && sym <= 'z')
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool ismypiece(int Turn, char sym)
{
    if (Turn == Black)
    {
        return(isblack(sym));
    }
    else if (Turn == White)
    {
        return(iswhite(sym));
    }
}
void selectposition1(char** B, Position& P, int Dim, string message, int Turn)
{
    char a;
    int i = 0;
    do
    {
        if (Turn == White)
        {
            cout << message << " (Select Grey)" << endl;
        }
        else
        {
            cout << message << " (Select Blue)" << endl;
        }
        do
        {
            if (i == 0)
            {
                cout << "Enter a Letter(A-I):" << endl;
                cin >> a;
                i++;
            }
            else
            {
                cout << "Incorrect Input...Enter a Letter(A-I):" << endl;
                cin >> a;
            }
        } while (a - char('A') > Dim - 1 || a < 65 || a>73);
        P.x = a - 'A';
        int b = 0;
        i = 0;
        do
        {
            if (i == 0)
            {
                cout << "Enter a number(1:9)";
                cin >> b;
                i++;
            }
            else
            {
                cout << "Incorrect Input...Enter a number(1:9)" << endl;
                cin >> b;
            }
        } while (b<1 || b>Dim);
        P.y = b - 1;
    } while (!ismypiece(Turn, B[P.y][P.x]));
}
void selectposition2(char** B, Position& P, int Dim, string message, int Turn)
{
    char a;
    int i = 0;
    do
    {
        cout << message << endl;
        do
        {
            if (i == 0)
            {
                cout << "Enter a Letter(A-I):" << endl;
                cin >> a;
                i++;
            }
            else
            {
                cout << "Incorrect Input...Enter a Letter(A-I):" << endl;
                cin >> a;
            }
        } while (a - char('A') > Dim - 1 || a < 65 || a>73);
        P.x = a - 'A';
        int b = 0;
        i = 0;
        do
        {
            if (i == 0)
            {
                cout << "Enter a number(1:9)";
                cin >> b;
                i++;
            }
            else
            {
                cout << "Incorrect Input...Enter a number(1:9)" << endl;
                cin >> b;
            }
        } while (b<1 || b>Dim);
        P.y = b - 1;
    } while (ismypiece(Turn, B[P.y][P.x]));
}
void Turnchange(int& Turn)
{
    if (Turn == Black)
    {
        Turn = White;
    }
    else
    {
        Turn++;
    }
}
void Turnmessage(string PNames[], int Turn)
{
    cout << PNames[Turn] << "'s Turn:" << endl;
}
void FakeMove(char**& A, Position SP, Position DP)
{
    A[DP.y][DP.x] = A[SP.y][SP.x];
    A[SP.y][SP.x] = '-';
}
void Move(char**& B, Position SP, Position DP, char**& PlayersCaptured, int turn, int& PCapturesize1, int& PCapturesize2, int Dim)
{
    if (B[DP.y][DP.x] != '-')
    {
        if (turn == White)
        {
            PlayersCaptured[turn][PCapturesize1] = B[DP.y][DP.x];
            B[DP.y][DP.x] = B[SP.y][SP.x];
            B[SP.y][SP.x] = '-';
            PCapturesize1++;
        }
        else if (turn == Black)
        {
            PlayersCaptured[turn][PCapturesize2] = B[DP.y][DP.x];
            B[DP.y][DP.x] = B[SP.y][SP.x];
            B[SP.y][SP.x] = '-';
            PCapturesize2++;
        }
    }
    else
    {
        B[DP.y][DP.x] = B[SP.y][SP.x];
        B[SP.y][SP.x] = '-';
    }
    ofstream out("BSCS22148-loadPrevious.txt");
    for (int ri = 0; ri < Dim; ri++)
    {
        for (int ci = 0; ci < Dim; ci++)
        {
            out << B[ri][ci] << " ";
        }
        out << endl;
    }
}
void PrintPlayersCaptured(char** PlayersCaptured, int PCapturesize1, int PCapturesize2, int Turn)
{
    cout << "PlayersCaptured{";
    if (Turn == White)
    {
        for (int ci = 0; ci < PCapturesize1; ci++)
        {
            cout << PlayersCaptured[Turn][ci] << " ";
        }
    }
    else
    {
        for (int ci = 0; ci < PCapturesize2; ci++)
        {
            cout << PlayersCaptured[Turn][ci] << " ";
        }
    }
    cout << "}" << endl;
}
bool ishorizontal(Position SP, Position DP)
{
    return(SP.y == DP.y);
}
bool isvertical(Position SP, Position DP)
{
    return(SP.x == DP.x);
}
bool isDiagonalL2R(Position SP, Position DP)
{
    if (DP.y > SP.y && DP.x > SP.x || DP.y < SP.y && DP.x < SP.x)
    {
        int r = abs(DP.y - SP.y);
        int c = abs(DP.x - SP.x);
        return(r == c);
    }
    else
    {
        return false;
    }
}
bool isDiagonal(Position SP, Position DP)
{
    int y = abs(SP.y - DP.y);
    int x = abs(SP.x - DP.x);
    return(y == x);
}
bool isDiagonalR2L(Position SP, Position DP)
{
    if (DP.y > SP.y && DP.x < SP.x || DP.y < SP.y && DP.x > SP.x)
    {
        int r = abs(DP.y - SP.y);
        int c = abs(DP.x - SP.x);
        return(r == c);
    }
    else
    {
        return false;
    }
}
bool ishorizontalpathclear(char** B, Position SP, Position DP)
{
    int i = SP.y;
    if (SP.x < DP.x)
    {
        for (int j = SP.x + 1; j < DP.x; j++)
        {
            if (B[i][j] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        for (int j = SP.x - 1; j > DP.x; j--)
        {
            if (B[i][j] != '-')
            {
                return false;
            }
        }
        return true;
    }
}
bool isverticalpathclear(char** B, Position SP, Position DP)
{
    int j = SP.x;
    if (SP.y < DP.y)
    {
        for (int i = SP.y + 1; i < DP.y; i++)
        {
            if (B[i][j] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        for (int i = SP.y - 1; i > DP.y; i--)
        {
            if (B[i][j] != '-')
            {
                return false;
            }
        }
        return true;
    }
}
bool isdiagonalpathclearl2r(char** B, Position SP, Position DP)
{
    if (SP.y < DP.y&& SP.x < DP.x)
    {
        for (int i = SP.y + 1, b = SP.x + 1; i < DP.y; i++, b++)
        {
            if (B[i][b] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else if (SP.y > DP.y && SP.x > DP.x)
    {
        for (int i = SP.y - 1, b = SP.x - 1; i > DP.y; i--, b--)
        {
            if (B[i][b] != '-')
            {
                return false;
            }
        }
        return true;
    }
}
bool isdiagonalpathclearR2l(char** B, Position SP, Position DP)
{
    if (SP.y > DP.y && SP.x < DP.x)
    {
        for (int i = SP.x + 1, b = SP.y - 1; b > DP.y; i++, b--)
        {
            if (B[b][i] != '-')
            {
                return false;
            }
        }
        return true;
    }
    else if (SP.y<DP.y && SP.x > DP.x)
    {
        for (int i = SP.y + 1, b = SP.x - 1; i < DP.y; i++, b--)
        {
            if (B[i][b] != '-')
            {
                return false;
            }
        }
        return true;
    }
}
bool Rooklegality(char** B, Position SP, Position DP)
{
    return(ishorizontal(SP, DP) && ishorizontalpathclear(B, SP, DP) || isvertical(SP, DP) && isverticalpathclear(B, SP, DP));
}
bool Bishoplegality(char** B, Position SP, Position DP)
{
    return(isDiagonalL2R(SP, DP) && isdiagonalpathclearl2r(B, SP, DP) || isDiagonalR2L(SP, DP) && isdiagonalpathclearR2l(B, SP, DP));
}
bool lancelegality(char** B, Position SP, Position DP, int Turn)
{
    if (Turn == White)
    {
        return(isvertical(SP, DP) && isverticalpathclear(B, SP, DP) && DP.y < SP.y);
    }
    else
    {
        return(isvertical(SP, DP) && isverticalpathclear(B, SP, DP) && DP.y > SP.y);
    }
}
bool kinglegality(char** B, Position SP, Position DP)
{
    int y = abs(SP.y - DP.y);
    int x = abs(SP.x - DP.x);
    return(y == 1 && x == 1 || y == 1 && x == 0 || y == 0 && x == 1);
}
bool knightlegality(char** B, Position SP, Position DP, int Turn)
{
    if (Turn == White)
    {
        int y = SP.y - DP.y;
        int x = abs(DP.x - SP.x);
        return(y == 2 && x == 1);
    }
    else
    {
        int y = DP.y - SP.y;
        int x = abs(DP.x - SP.x);
        return(y == 2 && x == 1);
    }
}
bool SilverGeneralslegality(char** B, Position SP, Position DP, int Turn)
{
    if (Turn == White)
    {
        int y = abs(DP.y - SP.y);
        int f = SP.y - DP.y;
        int x = abs(DP.x - SP.x);
        return(y == 1 && x == 1 || f == 1 && x == 0);
    }
    else
    {
        int y = abs(DP.y - SP.y);
        int f = DP.y - SP.y;
        int x = abs(DP.x - SP.x);
        return(y == 1 && x == 1 || f == 1 && x == 0);
    }
}
bool GoldGenerals(char** B, Position SP, Position DP, int Turn)
{
    if (Turn == White)
    {
        int y = (SP.y - DP.y);
        int x = abs(SP.x - DP.x);
        return(y == 1 && x == 1 || y == 0 && x == 1 || y == -1 && x == 0 || y == 1 && x == 0);
    }
    else
    {
        int y = (DP.y - SP.y);
        int x = abs(SP.x - DP.x);
        return(y == 1 && x == 1 || y == 0 && x == 1 || y == -1 && x == 0 || y == 1 && x == 0);
    }
}
bool PromotedRook(char** B, Position SP, Position DP)
{
    return(Rooklegality(B, SP, DP) || kinglegality(B, SP, DP));
}
bool PromotedBishop(char** B, Position SP, Position DP)
{
    return(Bishoplegality(B, SP, DP) || kinglegality(B, SP, DP));
}
bool Pawnlegality(Position SP, Position DP, int Turn)
{
    if (Turn == White)
    {
        int y = SP.y - DP.y;
        int x = abs(DP.x - SP.x);
        return(y == 1 && x == 0);
    }
    else
    {
        int y = DP.y - SP.y;
        int x = abs(DP.x - SP.x);
        return(y == 1 && x == 0);
    }
}
bool islegalmove(char** B, char** isFlip, Position SP, Position DP, int Turn)
{
    if (isFlip[SP.y][SP.x] == 'y')
    {
        char i = B[SP.y][SP.x];
        switch (i)
        {
        case('p'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('P'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('N'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('n'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('S'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('s'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('L'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('l'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('R'):
        {
            return(PromotedRook(B, SP, DP));
        }
        case('r'):
        {
            return(PromotedRook(B, SP, DP));
        }
        case('B'):
        {
            return(PromotedBishop(B, SP, DP));
        }
        case('b'):
        {
            return(PromotedBishop(B, SP, DP));
        }
        }
    }
    else
    {
        char i = B[SP.y][SP.x];
        switch (i)
        {
        case('P'):
        {
            return(Pawnlegality(SP, DP, Turn));
        }
        case('p'):
        {
            return(Pawnlegality(SP, DP, Turn));
        }
        case('R'):
        {
            return (Rooklegality(B, SP, DP));
            break;
        }
        case('r'):
        {
            return (Rooklegality(B, SP, DP));
            break;
        }
        case('K'):
        {
            return (kinglegality(B, SP, DP));
            break;
        }
        case('k'):
        {
            return (kinglegality(B, SP, DP));
            break;
        }
        case('N'):
        {
            return (knightlegality(B, SP, DP, Turn));
            break;
        }
        case('n'):
        {
            return (knightlegality(B, SP, DP, Turn));
            break;
        }
        case('B'):
        {
            return (Bishoplegality(B, SP, DP));
            break;
        }
        case('b'):
        {
            return (Bishoplegality(B, SP, DP));
            break;
        }
        case('L'):
        {
            return (lancelegality(B, SP, DP, Turn));
        }
        case('l'):
        {
            return(lancelegality(B, SP, DP, Turn));
        }
        case('s'):
        {
            return(SilverGeneralslegality(B, SP, DP, Turn));
        }
        case('S'):
        {
            return(SilverGeneralslegality(B, SP, DP, Turn));
        }
        case('g'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        case('G'):
        {
            return(GoldGenerals(B, SP, DP, Turn));
        }
        }
    }
}
bool isflip(char** isFlip, char** B, Position SP, int Turn)
{
    if (Turn == White)
    {
        if (iswhite(B[SP.y][SP.x]) && SP.y < 3)
        {
            return true;
        }
        return false;
    }
    else if (Turn == Black)
    {
        if (isblack(B[SP.y][SP.x]) && SP.y > 5)
        {
            return true;
        }
        return false;
    }
}
void flip(char** isFlip, char** B, Position SP, int Turn)
{
    if (B[SP.y][SP.x] != 'G' && B[SP.y][SP.x] != 'g' && B[SP.y][SP.x] != 'K' && B[SP.y][SP.x] != 'k')
    {
        char i;
        if (isflip(isFlip, B, SP, Turn) && isFlip[SP.y][SP.x] == 'n')
        {
            cout << "Do you want to flip(Enter y(yes) OR n(No)" << endl;
            cin >> i;
            isFlip[SP.y][SP.x] = i;
        }
    }
}
void l(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim, c + i);
        SetClr(Clr);
        cout << sym;
    }
}
void N(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int ri = 0; ri < Dim; ri++)
    {
        gotoRowCol(r + ri, c + ri);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
}
void G(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim / 2 + 1; i++)
    {
        gotoRowCol(r + Dim / 2 + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim / 2; i++)
    {
        gotoRowCol(r + Dim / 2, c + Dim / 2 + i);
        SetClr(Clr);
        cout << sym;
    }
}
void K(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim * 2; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = Dim - 1, k = 0; k < Dim / 1.5; k++, i--)
    {
        gotoRowCol(r + i, c + k);
        SetClr(Clr);
        cout << sym;
    }
    for (int ri = 0; ri < Dim; ri++)
    {
        gotoRowCol(r + Dim + ri - 1, c + ri);
        SetClr(Clr);
        cout << sym;
    }
}
void S(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim - 1, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim * 2 - 1, c + i);
        SetClr(Clr);
        cout << sym;
    }
}
void B(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim * 2; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim * 1.25; i++)
    {
        gotoRowCol(r + Dim, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim * 1.25; i++)
    {
        gotoRowCol(r + Dim * 2, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim + i, c + 1.25 * Dim);
        SetClr(Clr);
        cout << sym;
    }
}
void P(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim * 2; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim - 1, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
}
void R(int Dim, int r, int c, char sym, int Clr)
{
    for (int i = 0; i < Dim * 2; i++)
    {
        gotoRowCol(r + i, c);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + Dim - 1, c + i);
        SetClr(Clr);
        cout << sym;
    }
    for (int i = 0; i < Dim; i++)
    {
        gotoRowCol(r + i, c + Dim - 1);
        SetClr(Clr);
        cout << sym;
    }
    for (int ri = 0; ri <= Dim; ri++)
    {
        gotoRowCol(r + Dim - 1 + ri, c + ri);
        SetClr(Clr);
        cout << sym;
    }
}
void DrawBox(int sr, int sc, int brows, int bcols, int col, char** B)
{
    for (int i = 0; i < brows; i++)
    {
        for (int j = 0; j < bcols; j++)
        {
            SetClr(col);
            gotoRowCol(sr + i, sc + j);
            cout << char(-37);
        }
        cout << endl;
    }
}
void Player(char** isFlip, char L, int r, int c, int i, int j, char b)
{
    int Clr = 0;
    if (isFlip[i][j] == 'y')
    {
        Clr = 4;
    }
    else if (L < 90)
    {
        Clr = 8;
    }
    else
    {
        Clr = 11;
    }
    switch (L)
    {
    case('p'):
    {
        P(3, r, c, b, Clr);
        break;
    }
    case('P'):
    {
        P(3, r, c, b, Clr);
        break;
    }
    case('N'):
    {
        N(5, r, c, b, Clr);
        break;
    }
    case('n'):
    {
        N(5, r, c, b, Clr);
        break;
    }
    case('L'):
    {
        l(5, r, c, b, Clr);
        break;
    }
    case('l'):
    {
        l(5, r, c, b, Clr);
        break;
    }
    case('B'):
    {
        B(3, r, c, b, Clr);
        break;
    }
    case('b'):
    {
        B(3, r, c, b, Clr);
        break;
    }
    case('R'):
    {
        R(3, r, c, b, Clr);
        break;
    }
    case('r'):
    {
        R(3, r, c, b, Clr);
        break;
    }
    case('K'):
    {
        K(4, r, c, b, Clr);
        break;
    }
    case('k'):
    {
        K(4, r, c, b, Clr);
        break;
    }
    case('G'):
    {
        G(5, r, c, b, Clr);
        break;
    }
    case('g'):
    {
        G(5, r, c, b, Clr);
        break;
    }
    case('S'):
    {
        S(3.5, r, c, b, Clr);
        break;
    }
    case('s'):
    {
        S(3.5, r, c, b, Clr);
        break;
    }
    }
}
void clearfunction(int Dim, int Brows, int Bcols, char** B, char** isFlip, char b)
{
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            if ((i + j) % 2 == 0)
            {
                //DrawBox(i * Brows, j * Bcols, Brows, Bcols, 15, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, b);
                }
            }
            else
            {
                //DrawBox(i * Brows, j * Bcols, Brows, Bcols, 0, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, b);
                }
            }
        }
    }
    SetClr(14);
}
void function(int Dim, int Brows, int Bcols, char** B, char** isFlip)
{
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            if ((i + j) % 2 == 0)
            {
                //DrawBox(i * Brows, j * Bcols, Brows, Bcols, 15, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, -37);
                }
            }
            else
            {
                //DrawBox(i * Brows, j * Bcols, Brows, Bcols, 0, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, -37);
                }
            }
        }
    }
    SetClr(14);
}
void DrawBoard(int Dim, int Brows, int Bcols, char** B, char** isFlip)
{
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            if ((i + j) % 2 == 0)
            {
                DrawBox(i * Brows, j * Bcols, Brows, Bcols, 15, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, -37);
                }
            }
            else
            {
                DrawBox(i * Brows, j * Bcols, Brows, Bcols, 0, B);
                if (B[i][j] != '-')
                {
                    Player(isFlip, B[i][j], i * Brows + 2, j * Bcols + 2, i, j, -37);
                }
            }
        }
    }
    SetClr(14);
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
}
void unhighlight(char** isFlip, char** B, int Dim, Position SP, int Turn, int Brows, int Bcols)
{
    Position DP;
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            DP.y = i;
            DP.x = j;
            if (islegalmove(B, isFlip, SP, DP, Turn) && !ismypiece(Turn, B[DP.y][DP.x]))
            {
                gotoRowCol(i * Brows, j * Bcols);
                SetClr(4);
                cout << " ";
                for (int c = 0; c < 10; c++)
                {
                    gotoRowCol(i * Brows, j * Bcols + c);
                    SetClr(4);
                    cout << " ";
                }
                for (int d = 0; d < 10; d++)
                {
                    gotoRowCol(i * Brows + d, j * Bcols);
                    SetClr(4);
                    cout << " ";
                }
                for (int e = 0; e < 10; e++)
                {
                    gotoRowCol(i * Brows + e, j * Bcols + 9);
                    SetClr(4);
                    cout << " ";
                }
                for (int a = 0; a < 10; a++)
                {
                    gotoRowCol(i * Brows + 9, j * Bcols + a);
                    SetClr(4);
                    cout << " ";
                }
            }
        }
    }
    system("cls");
    DrawBoard(9, 10, 10, B, isFlip);
}
void highlight(char** isFlip, char** B, int Dim, Position SP, int Turn, int Brows, int Bcols)
{
    Position DP;
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            DP.y = i;
            DP.x = j;
            if (islegalmove(B, isFlip, SP, DP, Turn) && !ismypiece(Turn, B[DP.y][DP.x]))
            {
                gotoRowCol(i * Brows, j * Bcols);
                SetClr(4);
                cout << char(-37);
                for (int c = 0; c < 10; c++)
                {
                    gotoRowCol(i * Brows, j * Bcols + c);
                    SetClr(4);
                    cout << char(-37);
                }
                for (int d = 0; d < 10; d++)
                {
                    gotoRowCol(i * Brows + d, j * Bcols);
                    SetClr(4);
                    cout << char(-37);
                }
                for (int e = 0; e < 10; e++)
                {
                    gotoRowCol(i * Brows + e, j * Bcols + 9);
                    SetClr(4);
                    cout << char(-37);
                }
                for (int a = 0; a < 10; a++)
                {
                    gotoRowCol(i * Brows + 9, j * Bcols + a);
                    SetClr(4);
                    cout << char(-37);
                }
            }
        }
    }
    for (int i = 0; i < Dim * 2; i++)
    {
        cout << endl;
    }
    SetClr(14);
}
void UpdateisFlip(char**& isFlip, char** B, Position SP, Position DP)
{
    if (isFlip[SP.y][SP.x] == 'y')
    {
        isFlip[DP.y][DP.x] = 'y';
        isFlip[SP.y][SP.x] = 'n';
    }
}
bool Winningcondition(char** B, int Dim)
{
    int count1 = 0, count2 = 0;
    for (int i = 0; i < Dim; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            if (B[i][j] == 'k')
            {
                count1++;
            }
            else if (B[i][j] == 'K')
            {
                count2++;
            }
        }
    }
    if (count1 == 0 || count2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
Position findking(char** B, int Dim, int Turn)
{
    Position DP{};
    for (int ri = 0; ri < Dim; ri++)
    {
        DP.y = ri;
        for (int ci = 0; ci < Dim; ci++)
        {
            DP.x = ci;
            if (ismypiece(Turn, B[ri][ci]) && B[ri][ci] == 'k' || B[ri][ci] == 'K')
            {
                return DP;
            }
        }
    }
}
bool check(char** B, char** isflip, int Dim, int Turn)
{
    Turnchange(Turn);
    Position DP = findking(B, Dim, Turn);
    Turnchange(Turn);
    Position SP{};
    for (int ri = 0; ri < Dim; ri++)
    {
        SP.y = ri;
        for (int ci = 0; ci < Dim; ci++)
        {
            SP.x = ci;
            if (ismypiece(Turn, B[ri][ci]) && islegalmove(B, isflip, SP, DP, Turn))
            {
                return true;
            }
        }
    }
    return false;
}
bool selfcheck(char** B, char** isflip, int Turn, int Dim)
{
    Turnchange(Turn);
    return(check(B, isflip, Dim, Turn));
}
void CopyingOriginaltoFakeBoard(char** B, char**& A, int Dim)
{
    A = new char* [Dim];
    for (int ri = 0; ri < Dim; ri++)
    {
        A[ri] = new char[Dim];
        for (int ci = 0; ci < Dim; ci++)
        {
            A[ri][ci] = B[ri][ci];
        }
    }
}
void Undo(int Dim, char c)
{
    if (c == 'n')
    {
        ofstream out("BSCS22148-Undo.txt");
        ifstream Rdr("BSCS22148-loadPrevious.txt");
        for (int ri = 0; ri < Dim; ri++)
        {
            for (int ci = 0; ci < Dim; ci++)
            {
                char i;
                Rdr >> i;
                out << i << " ";
            }
            out << endl;
        }
    }
    else if (c == 'y')
    {
        ofstream out("BSCS22148-Undo.txt");
        ifstream Rdr("BSCS22148-loadPrevious.txt");
        for (int ri = 0; ri < Dim; ri++)
        {
            for (int ci = 0; ci < Dim; ci++)
            {
                char i;
                Rdr >> i;
                out << i << " ";
            }
            out << endl;
        }
    }
}
void CopyingUndofiletoB(char**& B, int Dim)
{
    ifstream Rdr("BSCS22148-Undo.txt");
    for (int ri = 0; ri < Dim; ri++)
    {
        for (int ci = 0; ci < Dim; ci++)
        {
            Rdr >> B[ri][ci];
        }
    }
}
int main()
{
    char** A;
    char** B;
    char U;
    char** isFlip;
    char** PlayersCaptured = nullptr;
    string PNames[2] = {};
    int Dim = 0, Turn = 0, PCapturesize1 = 0, PCapturesize2 = 0;
    Position SP;
    Position DP;
    cout << "  ####WELCOME TO SHOGI####" << endl;
    cout << "Do you want to continue from where you left(y)Yes or (n)No?" << endl;
    char c;
    cin >> c;
    if (c == 'y')
    {
        ifstream Rdr("BSCS22148-loadPrevious.txt");
        init(B, isFlip, PNames, Dim, Turn, PlayersCaptured, Rdr);
    }
    else
    {
        ifstream Rdr("BSCS22148-NewGame.txt");
        init(B, isFlip, PNames, Dim, Turn, PlayersCaptured, Rdr);
    }
    system("cls");
    DrawBoard(9, 10, 10, B, isFlip);
    while (true)
    {
        do
        {
            do
            {
                Turnmessage(PNames, Turn);
                do
                {
                    PrintPlayersCaptured(PlayersCaptured, PCapturesize1, PCapturesize2, Turn);
                    selectposition1(B, SP, Dim, "source selection", Turn);
                    flip(isFlip, B, SP, Turn);
                    highlight(isFlip, B, Dim, SP, Turn, 10, 10);
                    selectposition2(B, DP, Dim, "Destination selection", Turn);
                    unhighlight(isFlip, B, Dim, SP, Turn, 10, 10);
                    if (!islegalmove(B, isFlip, SP, DP, Turn))
                    {
                        cout << "Not Legal Move!!!...(Enter Again)" << endl;
                        Turnmessage(PNames, Turn);
                    }
                } while (!islegalmove(B, isFlip, SP, DP, Turn));
                CopyingOriginaltoFakeBoard(B, A, Dim);
                FakeMove(A, SP, DP);
                if (selfcheck(A, isFlip, Turn, Dim))
                {
                    cout << "SELF CHECK" << endl;
                }
            } while (selfcheck(A, isFlip, Turn, Dim));
            Undo(Dim, c);
            Move(B, SP, DP, PlayersCaptured, Turn, PCapturesize1, PCapturesize2, Dim);
            system("cls");
            DrawBoard(9, 10, 10, B, isFlip);
            cout << "Do you want to UNDO?(y)Yes OR (n)No?" << endl;
            cin >> U;
            if (U == 'y')
            {
                CopyingUndofiletoB(B, Dim);
                system("cls");
                DrawBoard(9, 10, 10, B, isFlip);
                ofstream out("BSCS22148-loadPrevious.txt");
                for (int ri = 0; ri < Dim; ri++)
                {
                    for (int ci = 0; ci < Dim; ci++)
                    {
                        out << B[ri][ci] << " ";
                    }
                    out << endl;
                }
            }
        } while (U == 'y');
        UpdateisFlip(isFlip, B, SP, DP);
        system("cls");
        DrawBoard(9, 10, 10, B, isFlip);
        if (check(B, isFlip, Dim, Turn))
        {
            cout << "CHECK!!!!" << endl;
        }
        if (Winningcondition(B, Dim))
        {
            cout << PNames[Turn] << " WON THE GAME!!!!";
            exit(1);
        }
        Turnchange(Turn);
    }
}