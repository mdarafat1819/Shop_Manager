#include <graphics.h>
#include <bits/stdc++.h>

using namespace std;

class Field
{
public:
    int left, top, right, bottom, bgColor;
    char *title;
    Field()
    {
        bgColor = WHITE;
    }
    Field(int left, int top, int right, int bottom, int bgColor, char* title)
    {
        visible(left, top, right, bottom, bgColor, title);
    }
    void visible(int left, int top, int right, int bottom, int bgColor, char* title)
    {
        this -> left = left;
        this -> top = top;
        this -> right = right;
        this -> bottom = bottom;
        this -> bgColor = bgColor;
        this -> title = title;
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, bgColor);
        bar(left, top, right, bottom);
        rectangle(left, top, right, bottom);
        setcolor(WHITE);
        setbkcolor(bgColor);
        outtextxy((left+right)/2-textwidth(title)/2, (bottom+top)/2-textheight("A")/2, title);
    }
    bool cursor()
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(GetForegroundWindow(), &p);
        if(p.x >= left && p.x<=right && p.y>=top && p.y<=bottom) return true;
        return false;
    }
};

class Input : public Field
{
private:
    int x, y;
    bool isPinInput;
public:
    Input()
    {
    }
    Input(int left, int top, int right, int bottom)
    {
        visible(left, top, right, bottom);
    }
    void visible(int left, int top, int right, int bottom)
    {
        Field::visible(left, top, right, bottom, WHITE, "");
        x = left + 3;
        y = (top+bottom)/2 - textheight("A")/2;
        isPinInput = false;
    }
    void sleep()
    {
        for (int c = 1; c <= 12760; c++)
        {
            if(kbhit() || !cursor()) return;
            for(int d = 1; d <= 2276; d++)
                if(kbhit()) return;
        }
    }
    void showCursor(char* data, int color)
    {
        double tw;
        if(isPinInput) tw = strlen(data) * textwidth("x");
        else tw = textwidth(data);
        setcolor(color);
        outtextxy(x + tw, y, "|");
    }
    void print(char* data, int* idx, int color)
    {
        setcolor(color);
        if(isPinInput && (*idx))
        {
            if(data[(*idx) - 1] > 47 && data[(*idx) - 1] < 58 && (*idx) < 9)
            {
                char temp[*idx];
                for(int i = 0; i < *idx; i++) temp[i] = 'x';
                temp[*idx] = 0;
                outtextxy(x, y, temp);
            }
            else data[--(*idx)] = 0;
            return;
        }
        outtextxy(x, y, data);
    }
    char getText(char *data)
    {
        setbkcolor(WHITE);
        int idx = strlen(data);
        print(data, &idx, BLACK);
        while(true)
        {
            if(kbhit())
            {
                char ch = getch();
                if(idx && ch == 8)
                {
                    print(data, &idx, WHITE);
                    showCursor(data, WHITE);
                    idx--;
                }
                else if(ch >= 32  && ch <= 126)
                {
                    data[idx++] = ch;
                    data[idx] = 0;
                    if(left + textwidth(data) + textwidth("|") >= right - 3)
                        idx--;//If true that means Input Area is overflow
                }
                else if(ch == 9 || ch == 13)//9 = tab, 13 = enter key
                {
                    showCursor(data, WHITE);
                    return 9;
                }
                data[idx] = 0;
                print(data, &idx, BLACK);
            }
            else if(!cursor())
            {
                showCursor(data, WHITE);
                return 0;
            }
            showCursor(data, BLACK);
            sleep();
            showCursor(data, WHITE);
            sleep();
        }
    }
    void getPin(char* data)
    {
        isPinInput = true;
        getText(data);
        isPinInput = false;
    }
};

class Button : public Field
{
private:
    bool flag;
public:
    Button() {}
    Button(int left, int top, int right, int bottom, int bgColor, char* title)
        :Field(left, top, right, bottom, bgColor, title)
    {
        flag = false;
    }
    void hover(int color)
    {
        if(cursor() && !flag)
            new Field(left, top, right, bottom, color, title), flag = true;
        else if(!cursor() && flag)
            new Field(left, top, right, bottom, bgColor, title), flag = false;
    }

};

class Item
{
public:
    char p_name[50], rt[25], qnt[20], dis[20], amnt[20];
    int x, y;

public:
    Input productName, rate, quantity, discount, amount;
    Button mark;

    Item()
    {
        setbkcolor(WHITE);
        strcpy(p_name, "");
        strcpy(rt, "");
        strcpy(qnt, "");
        strcpy(dis, "");
        strcpy(amnt, "");
    }
    void markButton(int color, int top, int bottom)
    {
        mark.visible(757, top, 784, bottom, LIGHTGRAY, "");
        mark.visible(762, top + 5, 779, bottom - 5, WHITE, "");
        if(color == GREEN) mark.visible(764, top + 7, 777, bottom - 7, GREEN, "");
        else mark.visible(764, top + 7, 777  , bottom-7, WHITE, "");
    }
    void show(int top, int bottom)
    {
        y = (top + bottom) / 2 - textheight("A") / 2;
        setcolor(BLACK);
        setbkcolor(WHITE);

        productName.visible(15, top, 317, bottom);
        quantity.visible(317, top, 417, bottom);
        rate.visible(417, top, 550, bottom);
        discount.visible(550, top, 650, bottom);
        amount.visible(650, top, 757, bottom);

        setcolor(BLACK);
        outtextxy(18, y, p_name);
        outtextxy(319, y, qnt);
        outtextxy(419, y, rt);
        outtextxy(552, y, dis);
        outtextxy(652, y, amnt);

        if(mark.bgColor == WHITE) markButton(WHITE, top, bottom);
        else markButton(GREEN, top, bottom);
    }
    void action(int top, int bottom)
    {
        if(mark.cursor() && mark.bgColor == GREEN) markButton(WHITE, top, bottom);
        else if(mark.cursor() && mark.bgColor == WHITE) markButton(GREEN, top, bottom);
        else if(productName.cursor()) productName.getText(p_name);
        else if(quantity.cursor())quantity.getText(qnt);
        else if(rate.cursor())rate.getText(rt);
        else if(discount.cursor())discount.getText(dis);
        else if(amount.cursor())amount.getText(amnt);
    }
};
