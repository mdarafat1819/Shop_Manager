#include "interface.h"
#include <bits/stdc++.h>
#include <dirent.h>

int w1;
using namespace std;

char userName[50] = "", pinCode[10], shopName[50] = "";

FILE* getOrderIdWithPath(char *date, int serial, char* orderId);
void createAccount();
void login();
void admin();
char readLine(char*, FILE*);
char* filePath(char*);
void warning(char*);
void frame();
void cashMemo(char*);
void store_room();
void printMemo(vector<Item> productList, char* fileName);
vector<Item> importItem(vector<Item>);
void sellSummary();
void userProfile();

char* filePath(char *fileName)
{
    char path[100];
    strcpy(path, userName);
    strcat(path, "\\");
    strcat(path, fileName);
    return(path);
}

FILE* getOrderIdWithPath(char *date, int serial, char* orderId)
{
    char temp[40];
    strcpy(temp, orderId);
    strcpy(orderId, userName);
    strcat(orderId, "\\");
    strcat(orderId, date);
    if(serial)itoa(serial, temp, 10);/*if serial = 0 no need generate new order Id bcz serial = 0 means order id already generated*/
    strcat(orderId, temp);/*temp variable hold the char type string of serial variable*/
    strcat(orderId, ".txt");

    FILE *file = fopen(orderId, "r");
    fclose(file);
    return file;
}

void warning(char *text)
{
    int td = textwidth(text) / 2;
    int th = textheight("A")/2;
    setbkcolor(LIGHTGRAY);
    setcolor(RED);
    outtextxy(400 - td, 473 - th, text);
    delay(1000);
    setcolor(LIGHTGRAY);
    outtextxy(400 - td, 473 - th, text);
    setcolor(WHITE);
}
void createAccount()
{
    strcpy(userName, "");
    strcpy(pinCode, "");
    strcpy(shopName, "");
    cleardevice();
    frame();

    setcolor(BLACK);
    setlinestyle(1, 0, 1);
    line(16, 110, 784,110);
    setlinestyle(0, 0, 1);

    setcolor(CYAN);
    setbkcolor(LIGHTGRAY);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 6);
    outtextxy(400-textwidth("Shop Manager")/2, 68-textheight("A")/2, "Shop Manager");

    setcolor(GREEN);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(220, 120, "User Name:");
    outtextxy(220, 200, "Pin Code:");
    outtextxy(220, 280, "Shop Name:");

    Input user(220, 150, 580, 190);
    Input pin(220, 230, 580, 270);
    Input shop(220, 310, 580, 350);
    setcolor(WHITE);
    Button submit(220, 360, 580, 400, 3, "Submit");
    Button lgn(220, 410, 580, 450, 5, "Log In");

    while(true)
    {
        submit.hover(GREEN);
        lgn.hover(BLUE);
        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {

            if(user.cursor()) user.getText(userName);
            else if(pin.cursor()) pin.getPin(pinCode);
            else if(shop.cursor()) shop.getText(shopName);
            else if(lgn.cursor())
            {
                login();
                return;
            }
            else if(submit.cursor() && strlen(pinCode) >= 4 && strlen(userName) >= 6)
            {
                DIR *folder;
                folder = opendir(userName);
                if(folder == NULL)
                {
                    mkdir(userName);
                    char file_path[40];
                    strcpy(file_path, filePath("admin.txt"));
                    FILE *file = fopen(file_path, "w");
                    fprintf(file, "%s%s", pinCode, "\n");
                    fprintf(file, "%s%s", shopName, "\n");
                    strcpy(pinCode, "");
                    warning("Note: Successfully Created");
                    fclose(file);
                }
                else warning("Note: Invalid or Already Exist");
                closedir(folder);
            }
        }
        if(kbhit()) getch();
    }
}

void frame()
{
    cleardevice();
    //Frame Start
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, 0, 15, 505);//For Left Side
    bar(785, 0, 800, 505);//For Right Side
    bar(15, 0, 800, 15);//For Top Side
    bar(0, 505, 800, 520);//For Bottom Side
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(16, 16, 784, 504);//For Middle rectangle
    //Frame End
}
void login()
{
    strcpy(userName, "");
    strcpy(pinCode, "");
    frame();
    setcolor(CYAN);
    setbkcolor(LIGHTGRAY);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 6);
    outtextxy(400-textwidth("Shop Manager")/2, 68-textheight("A")/2, "Shop Manager");

    setcolor(BLACK);
    setlinestyle(1, 0, 1);
    line(16, 110, 784,110);
    setlinestyle(0, 0, 1);

    setcolor(GREEN);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(220, 120, "User Name:");
    outtextxy(220, 200, "Pin Code:");
    outtextxy(400 - textwidth("Or") / 2, 350, "Or");

    Input user(220, 151,580,190);
    Input pin(220, 231, 580, 270);

    settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
    Button log_in(220, 301, 580, 340, CYAN, "Log In");
    Button create_account(220, 381, 580, 420, CYAN, "Create Account");

    while(true)
    {
        log_in.hover(RED);
        create_account.hover(RED);

        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            if(create_account.cursor())
            {
                createAccount();
                return;
            }
            else if(user.cursor()) user.getText(userName);
            else if(pin.cursor()) pin.getPin(pinCode);
            else if(log_in.cursor() && strlen(pinCode) >= 4 && strlen(userName) >= 4)
            {
                DIR *folder;
                folder = opendir(userName);
                if(folder == NULL) warning("Note: User name not found");
                else if(folder != NULL)
                {
                    char temp_pin[20], file_path[40];
                    strcpy(file_path, filePath("admin.txt"));
                    FILE *file = fopen(file_path, "r");
                    readLine(temp_pin, file);
                    readLine(shopName, file);
                    fclose(file);

                    if(strcmp(temp_pin, pinCode) == 0)
                    {
                        admin();
                        return;
                    }
                    else warning("Note: Pin code not match");
                }
            }
            else if(log_in.cursor()) warning("Note: User name or Pin Empty");
        }
        if(kbhit()) getch();
    }
}

void admin()
{
    cleardevice();
    frame();

    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    outtextxy(400 - textwidth(shopName) / 2, 58 - textheight("A") / 2, shopName);
    setlinestyle(1, 0, 1);
    line(16, 84.5 + textheight("A")/ 4, 784,84.5 + textheight("A")/ 4);
    setlinestyle(0, 0, 1);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    Button new_order(16, 111, 400, 229, CYAN, "New Order");
    Button store(401, 111, 783, 229, MAGENTA, "Store Room");
    Button find_memo(16, 230, 400, 347, GREEN, "Find Memo");
    Button sell_summary(401, 230, 783, 347, BLUE, "Sell Summary");
    Button profile(16, 348, 400, 463, MAGENTA, "Profile");
    Button logout(401, 348, 783, 463, RED, "Log Out");
    //Button End
    ///Footer Section
    setfillstyle(SOLID_FILL, CYAN);
    bar(16, 465, 784, 504);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
    setbkcolor(CYAN);
    setcolor(BLACK);
    outtextxy(400 - textwidth("Developed By Yeasin Arafat")/2, 485 - textheight("A") / 2, "Developed By Yeasin Arafat");
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    while(true)
    {
        find_memo.hover(RED);
        new_order.hover(BLUE);
        store.hover(GREEN);
        sell_summary.hover(CYAN);
        profile.hover(BLUE);
        logout.hover(GREEN);


        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            if(new_order.cursor()) cashMemo("");
            else if(store.cursor()) store_room();
            else if(find_memo.cursor())
            {
                cleardevice();
                frame();
                settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
                setbkcolor(LIGHTGRAY);
                outtextxy(101, 155 - textheight("A")/2, "Enter order ID to find Memo:");
                setcolor(BLACK);
                Input order_id(101, 211, 600, 300);
                Button exit(101, 311, 340, 400, MAGENTA, "Exit");
                Button enter(361, 311, 600, 400, CYAN, "Enter");
                char id[40] = "";
                while(true)
                {
                    exit.hover(RED);
                    enter.hover(BLUE);
                    if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
                    {
                        if(exit.cursor())
                        {
                            admin();
                            return;
                        }
                        else if(order_id.cursor()) order_id.getText(id);
                        else if(enter.cursor() && strlen(id) > 3)
                        {
                            cashMemo(id);
                            break;
                        }
                    }
                    if(kbhit()) getch();
                }

            }
            else if(sell_summary.cursor())
            {
                sellSummary();
            }
            else if(profile.cursor())
            {
                userProfile();
            }
            else if(logout.cursor())
            {
                login();
                return;
            }
        }
        if(kbhit()) getch();
    }

}

void item_heading(int top, int bottom)
{
    int th = textheight("A") / 2;
    int y = (top + bottom) / 2 - th;
    settextstyle(6, HORIZ_DIR, 1);
    setfillstyle(SOLID_FILL, CYAN);
    setcolor(BLACK);
    bar(15, top, 785, bottom);
    rectangle(15, top, 317, bottom);//For Product Name
    rectangle(317, top, 417, bottom);//For Quantity
    rectangle(417, top, 550, bottom);//ForRate
    rectangle(550, top, 650, bottom);//For Discount
    rectangle(650, top, 784, bottom);//For Amount

    setbkcolor(CYAN);
    setcolor(WHITE);
    outtextxy(166-textwidth("Products Name")/2,y,"Product Name");
    outtextxy(365.5-textwidth("Quantity")/2, y, "Quantity");
    outtextxy(483.5-textwidth("Rate")/2, y, "Rate");
    outtextxy(600-textwidth("Discount")/2, y, "Discount");
    outtextxy(717-textwidth("Amount")/2, y, "Amount");
}

void writeFile(vector<Item>pList, FILE *file)
{
    for(int i = 0; i < pList.size(); i++)
    {
        fprintf(file, "%s%s", pList[i].p_name, "\n");
        fprintf(file, "%s%s", pList[i].qnt, "\n");
        fprintf(file, "%s%s", pList[i].rt, "\n");
        fprintf(file, "%s%s", pList[i].dis, "\n");
    }
    return;
}

vector <Item> readFile(FILE *file)
{
    vector<Item> v;
    Item temp;
    while(readLine(temp.p_name, file) != EOF)
    {
        readLine(temp.qnt, file);
        readLine(temp.rt, file);
        readLine(temp.dis, file);
        v.push_back(temp);
    }
    return v;
}

char readLine(char *data, FILE *file)
{
    char ch;
    int idx = 0;
    while(1)
    {
        ch = fgetc(file);
        if(ch == '\n') break;
        if(ch == EOF)
        {
            data[idx] = '\0';
            return ch;
        }
        data[idx++] = ch;
    }
    data[idx] = '\0';
    return '\n';
}

vector <Item> amountCalculation(vector<Item>pList)
{
    for(int i = 0; i < pList.size(); i++)
    {
        char *a, temp[10];
        double b, c;
        b = strtod(pList[i].rt, &a);
        c = strtod(pList[i].dis, &a);
        double am = b * (1 - c/100.0);
        double qnt = strtod(pList[i].qnt, &a);
        am *= qnt;
        sprintf(temp,"%.1lf", am);
        strcpy(pList[i].amnt, temp);//Amount
    }
    return pList;
}

void printItems(int top, vector<Item>v)
{
    for(int i = 0; i < v.size(); i++)
        v[i].show(top + (i* 30), top + (i + 1) * 30);
}

void cashMemo(char *memoId)
{

    /*
    Note: memoId, orderId, Id are same things.
    */

    frame();

    //Head Start
    settextstyle(4, HORIZ_DIR, 1);
    new Field(15, 15, 140, 44, MAGENTA, "Order ID");
    new Field(450, 15, 545, 44, MAGENTA, "Date   ");
    new Field(15, 44, 140, 71, MAGENTA, "Name    ");
    new Field(450, 44, 545, 71, MAGENTA, "Phone");
    new Field(15, 71, 140, 100, MAGENTA, "Address");

    int th = textheight("A") / 2;
    setcolor(WHITE);
    setbkcolor(MAGENTA);

    char orderId[50]="", name[33] = "", phone[12] = "",adrss[74] = "", currentTime[30];
    char printableId[50];/*PrintableId refers to OrderId that print orderId without Path and file extension*/
    Input inptOrderId(140, 15, 450, 44);
    Input inptDate(540, 15, 784, 44);
    Input inptName(140, 44, 450, 71);
    Input inptPhone(540, 44, 784, 71);
    Input inptAddress(140, 71, 784, 100);
    //Head End

    item_heading(100, 130);
    //Button Start
    Button btnNewItem(15, 460, 150,505, GREEN, "New Item");
    Button btnImport(151, 460, 250, 505, CYAN, "Import");
    Button btnTotal(251, 460, 384, 505, MAGENTA, "Total");
    Button btnPrint(385, 460, 518, 505, BLUE, "Print");
    Button btnSave(519, 460, 585, 505, CYAN, "Save");
    Button btnExit(586, 460, 651, 505, MAGENTA, "Exit");
    Button btnRemove(652, 460, 784, 505, RED, "Remove");
    //Button End

    vector<Item> productList;
    FILE *file;

    if(!strlen(memoId))//Since memoId is empty then need to created new memoId;
    {
        //cout<<"adfsd"<<endl;
        char temp[3] = "", currentDate[7] = "";
        time_t tm;
        struct tm*  ptm;
        tm = time(NULL);
        ptm = localtime(&tm);
        strftime(currentTime, 30, "%d-%b-%Y %H:%M:%S", ptm);
        SYSTEMTIME t;
        GetLocalTime(&t);//Fill out the struct so that it can be used
        itoa(t.wDay, temp, 10);
        strcat(currentDate, temp);
        itoa(t.wMonth, temp, 10);
        strcat(currentDate, temp);
        itoa(t.wYear - 2000, temp, 10);
        strcat(currentDate, temp);

        int serial = 1;
        while(getOrderIdWithPath(currentDate, serial, orderId) != NULL) serial++;

    }
    else if(strlen(memoId))
    {
        if(getOrderIdWithPath("", 0, memoId) == NULL)
        {
            setcolor(RED);
            setbkcolor(LIGHTGRAY);
            outtextxy(350 - textwidth("Memo Not Found") / 2, 287.5 - textheight("A")/2, "Memo Not Found");
            while(true)
            {
                btnExit.hover(GREEN);
                if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
                {
                    if(btnExit.cursor()) admin();
                }
            }
            return;
        }
        else
        {
            strcpy(orderId, memoId);
            FILE *file = fopen(orderId, "r");
            readLine(currentTime, file);
            readLine(name, file);
            readLine(phone, file);
            readLine(adrss, file);
            productList = readFile(file);
            fclose(file);
        }
    }

    int j = 0;
    //cout<<orderId<<endl;
    for(int i = strlen(userName) + 1; i < strlen(orderId); i++)
        printableId[j++] = orderId[i];
    printableId[j - 4] = '\0';///.txt is removed

    setbkcolor(WHITE);
    setcolor(BLACK);

    outtextxy(145, 28 - th, printableId);///printableId means orderId
    outtextxy(545, 28 - th, currentTime);
    outtextxy(142, 57.5 - th, name);
    outtextxy(545, 57.5 - th, phone);
    outtextxy(142, 85.5 - th, adrss);

    int idx = productList.size();
    Item total;
    for(int i = 0; i < idx; i++)
        productList[i].show(130 + (i* 30), 130 + (i + 1) * 30);
    //printItems(130, productList);

    while(true)
    {
        btnNewItem.hover(BLUE);
        btnTotal.hover(BLACK);
        btnPrint.hover(GREEN);
        btnSave.hover(GREEN);
        btnRemove.hover(BLUE);
        btnExit.hover(RED);
        btnImport.hover(BLUE);

        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            // cout<<"Yes"<<endl;
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(GetForegroundWindow(), &p);
            int find_idx = p.y - 130;
            find_idx /= 30;
            int top = 130 + find_idx * 30;
            int bottom = 130 + (find_idx + 1) * 30;
            if(find_idx >= 0 && find_idx < idx)productList[find_idx].action(top, bottom);
            else if(btnNewItem.cursor() && idx < 10)
            {
                productList.push_back(Item());
                productList[idx].show(130 + (idx * 30), 130 + (idx + 1) * 30);
                idx++;
            }
            else if(btnRemove.cursor() && idx)
            {
                for(int i = 0; i < idx; i++)
                {
                    if(productList[i].mark.bgColor == GREEN)
                        productList.erase(productList.begin() + i), idx--, i--;
                }
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 131,784, 459);
                for(int i = 0; i < idx; i++)
                    productList[i].show(130 + (i * 30), 130 + (i + 1) * 30);
            }
            else if(btnTotal.cursor())
            {
                productList = amountCalculation(productList);
                for(int i = 0; i < idx; i++)
                    productList[i].show(130 + (i * 30), 130 + (i + 1) * 30);
                double t = 0;
                strcpy(total.p_name, "Total: ");
                for(int i = 0; i < idx; i++)
                {
                    char *a, temp[10];
                    double b, c;
                    b = strtod(productList[i].amnt, &a);
                    t += b;
                    sprintf(temp,"%.1lf", t);
                    strcpy(total.amnt, temp);//Total Amount
                }
                total.show(130 + (idx * 30), 130 + (idx + 1) * 30);
                int y = ((130 + (idx * 30)) + (130 + (idx + 1) * 30)) / 2;
                setcolor(BLACK);
                outtextxy(17, y - textheight("A") / 2, "Total:");
                setcolor(WHITE);
            }
            else if(btnSave.cursor())
            {
                FILE *file = fopen(orderId, "w");
                fprintf(file, "%s%s", currentTime, "\n");
                fprintf(file, "%s%s", name, "\n");
                fprintf(file, "%s%s", phone, "\n");
                fprintf(file, "%s%s", adrss, "\n");
                writeFile(productList, file);
                fclose(file);
            }
            else if(btnExit.cursor()) admin();
            //else if(order_id.cursor()) order_id.inputText(orderId, BLACK, 20);
            //else if(date.cursor()) currentTimee.inputText(currentTime, BLACK, 20);
            else if(inptName.cursor()) inptName.getText(name);
            else if(inptPhone.cursor()) inptPhone.getText(phone);
            else if(inptAddress.cursor()) inptAddress.getText(adrss);
            else if(btnImport.cursor())
            {
                // cashMemo(fileName, importItem(productList));
                // closegraph(w1);
                int w2 = initwindow(800, 520);
                setcurrentwindow(w2);
                productList = importItem(productList);
                setcurrentwindow(w1);
                closegraph(w2);
                idx = productList.size();
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 131,784, 459);
                for(int i = 0; i < idx; i++)
                    productList[i].show(130 + (i * 30), 130 + (i + 1) * 30);
                // printItems(130, productList);
            }
            else if(btnPrint.cursor())
            {
                FILE *file = fopen(orderId, "w");
                fprintf(file, "%s%s", currentTime, "\n");
                fprintf(file, "%s%s", name, "\n");
                fprintf(file, "%s%s", phone, "\n");
                fprintf(file, "%s%s", adrss, "\n");
                writeFile(productList, file);
                fclose(file);
                productList.push_back(Item());
                productList[idx] = total;
                printMemo(productList, orderId);
                productList.erase(productList.begin() + idx + 1);
            }
        }
        if(kbhit()) getch();
    }
    return;
}
bool cmp(Item a, Item b)
{

    if(strcmp(a.p_name, b.p_name) > 0) return false;
    if(strcmp(a.p_name, b.p_name) == 0) return NULL;
    return true;
}

void counter(int left, int right, int cnt, char* msg)
{
    setcolor(BLACK);
    setbkcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    bar(left, 465, right, 505);///For Marked Item display
    rectangle(left, 465, right, 505);
    setcolor(WHITE);
    char tem[10] = "";
    itoa(cnt, tem, 10);
    strcat(tem, msg);
    outtextxy((left + right) / 2 - textwidth(tem) / 2, 485 - textheight("0") / 2, tem);
}

vector<Item> importItem(vector<Item> productList)
{
    frame();

    settextstyle(6, HORIZ_DIR, 1);
    item_heading(15, 45);
    //Button Start
    Button prev(15, 465, 150, 505, GREEN, "Previous");
    Button next(151, 465, 260, 505, CYAN, "Next");
    Button exprt(350, 465, 653, 505, 5, "Export marked Item");
    Button exit(654, 465, 784, 505, RED, "Exit");
    //Button End

    int currentPage = 0, idx = 0, temp_idx, markedItemCount = productList.size();
    counter(261, 349, markedItemCount, " Item");///Show the number of marked Item
    // bar(261, 460, 350, 505);

    char storRoomPath[40];
    strcpy(storRoomPath, filePath("store.txt"));
    FILE *file = fopen(storRoomPath, "r");
    vector<Item> v = readFile(file);///Stored all product list from store room..
    fclose(file);

    idx = v.size();
    for(int i = 0; i < idx && i < 14; i++)
    {
        v[i].show(45 + (30 * i), 45 + (30 * (i + 1)));
    }
    while(true)
    {
        prev.hover(BLUE);
        next.hover(BLACK);
        exprt.hover(BLACK);
        exit.hover(GREEN);

        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(GetForegroundWindow(), &p);
            int find_idx = p.y - 45;
            find_idx /= 30;
            int top = 45 + find_idx * 30;
            int bottom = 45 + (find_idx + 1) * 30;
            if(find_idx >= 0 && find_idx < idx && find_idx < 14)
            {
                temp_idx = find_idx + 14 * currentPage;
                if(v[temp_idx].mark.cursor() && markedItemCount < 10)
                {
                    if(v[temp_idx].mark.bgColor == GREEN) markedItemCount--;
                    else markedItemCount++;
                    v[temp_idx].action(top, bottom);
                    counter(261, 349, markedItemCount, " Item");///Show the number of marked Item
                }
            }
            if(prev.cursor())
            {
                if(currentPage) currentPage--;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                for(int i = 0; (currentPage * 14 + i)< v.size() && i < 14; i++)
                    v[currentPage * 14 + i].show(45 + (i * 30), 45 + (i + 1) * 30);

            }
            else if(next.cursor())
            {
                if((currentPage + 1) * 14 < v.size())currentPage++;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                for(int i = 0;  (currentPage * 14 + i)< v.size() && i < 14; i++)
                    v[currentPage * 14 + i].show(45 + (i * 30), 45 + (i + 1) * 30);
            }
            else if(exit.cursor()) return productList;
            else if(exprt.cursor())
            {
                for(int i = 0; i < v.size(); i++)
                {
                    if(v[i].mark.bgColor == GREEN)
                    {
                        v[i].mark.bgColor = WHITE;
                        productList.push_back(v[i]);
                    }
                }
                return productList;
            }
        }
        if(kbhit()) getch();
    }
}

void store_room()
{
    frame();
    setcolor(BLACK);
    item_heading(15, 45);

    settextstyle(6, HORIZ_DIR, 1);
    Button new_item(15, 465, 150, 505, GREEN, "New Item");
    Button prev(151, 465, 230, 505, CYAN, "Prev.");
    Button next(291, 465, 391, 505, 5, "Next");
    Button update(392, 465, 522, 505, BLUE, "Update");
    Button exit(523, 465, 653, 505, 3, "Exit");
    Button remove_item(654, 465, 784, 505, RED, "Remove");
    //Button End

    int idx = 0, prev_idx = 0, new_idx, currentPage = 0;
    bool newButtonClicked = false;

    char storRoomPath[40];
    strcpy(storRoomPath, filePath("store.txt"));
    // cout<<storRoomPath<<endl;
    FILE *file = fopen(storRoomPath, "r");

    vector<Item> v = readFile(file);
    fclose(file);

    idx = v.size();
    for(int i = 0; i < idx && i < 14; i++)
    {
        v[i].show(45 + (30 * i), 45 + (30 * (i + 1)));
    }
    counter(231, 289, currentPage, "");///Show the number of marked Item
    while(true)
    {
        new_item.hover(BLUE);
        prev.hover(BLACK);
        next.hover(BLACK);
        update.hover(GREEN);
        exit.hover(GREEN);
        remove_item.hover(BLUE);

        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(GetForegroundWindow(), &p);
            int find_idx = p.y - 45;
            find_idx /= 30;
            int top = 45 + find_idx * 30;
            int bottom = 45 + (find_idx + 1) * 30;
            if(find_idx >= 0 && find_idx < idx && find_idx < 14)v[find_idx + prev_idx + 14 * currentPage].action(top, bottom);

            if(new_item.cursor())
            {
                currentPage = 0;
                setcolor(WHITE);
                if(!newButtonClicked)
                {
                    setfillstyle(SOLID_FILL, LIGHTGRAY);
                    bar(16, 46, 784, 465);
                    newButtonClicked = true;
                    prev_idx = idx;
                }
                new_idx = idx - prev_idx;
                if(new_idx < 14)
                {
                    v.push_back(Item());
                    v[idx].show(45 + 30 * new_idx, 45 + (30 * (new_idx + 1)));
                    idx++;
                }
            }
            else if(update.cursor())
            {
                sort(v.begin(), v.end(), cmp);
                prev_idx = 0;
                currentPage = 0;
                newButtonClicked = false;

                FILE *file;
                file = fopen(storRoomPath,"w");
                writeFile(v, file);
                fclose(file);

                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                v = amountCalculation(v);
                for(int i = 0; i < idx && i < 14; i++)
                    v[i].show(45 + (30 * i), 45 + (30 * (i + 1)));
            }
            else if(remove_item.cursor() && idx)
            {
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                for(int i = prev_idx; i < idx; i++)
                {
                    if(v[i].mark.bgColor == GREEN)
                    {
                        v.erase(v.begin() + i);
                        i--;
                        idx--;
                    }
                }
                new_idx = 0;
                for(int i = prev_idx; i < idx && new_idx < 13; i++)
                {
                    new_idx = i - prev_idx;
                    v[i].show(45 + (new_idx * 30), 45 + (new_idx + 1) * 30);
                }
            }
            else if(next.cursor() && !newButtonClicked)
            {
                if((currentPage + 1) * 14 < v.size())currentPage++;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                for(int i = 0;  (currentPage * 14 + i)< v.size() && i < 14; i++)
                    v[currentPage * 14 + i].show(45 + (i * 30), 45 + (i + 1) * 30);
            }
            else if(prev.cursor() && !newButtonClicked)
            {
                if(currentPage) currentPage--;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 46, 784, 465);
                for(int i = 0; (currentPage * 14 + i)< v.size() && i < 14; i++)
                    v[currentPage * 14 + i].show(45 + (i * 30), 45 + (i + 1) * 30);
            }
            else if(exit.cursor())
            {
                admin();
                return;
            }
            counter(231, 289, currentPage, "");
        }
        if(kbhit()) getch();
    }
}

void sellSummary()
{
    frame();

    settextstyle(4, HORIZ_DIR, 1);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, MAGENTA);
    bar(15, 15, 785, 45);
    rectangle(15, 15, 450, 45);//For title
    setcolor(WHITE);
    setbkcolor(MAGENTA);
    outtextxy(17, 30 - textheight("A")/2, "Type date to details(DDMMYY): ");
    setcolor(BLACK);
    Input date(450, 15, 650, 45);
    char dat[20] = "";

    item_heading(45, 75);
    //Button Start
    Button prev(15, 465, 260, 505, GREEN, "Previous");
    //Button next(151, 465, 260, 505, CYAN, "Next");
    Button next(350, 465, 620, 505, 5, "Next");
    Button exit(621, 465, 784, 505, RED, "Exit");
    Button srch(650, 15, 784, 45, MAGENTA, "Search");
    int currentPage = 0, idx = 0;
    counter(261, 349, currentPage, "");
    vector<Item> productList;
    while(true)
    {
        srch.hover(BLUE);
        prev.hover(CYAN);
        next.hover(CYAN);
        exit.hover(BLUE);
        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            if(date.cursor()) date.getText(dat);
            else if(srch.cursor() && strlen(dat) > 4)
            {
                int serial = 1;

                char orderId[40], temp[40];
                while(getOrderIdWithPath(dat, serial, orderId))
                {
                    FILE *f = fopen(orderId, "r");
                    if(f == NULL) break;
                    if(f != NULL)
                    {
                        //cout<<"Test"<<endl;
                        readLine(temp, f);
                        readLine(temp, f);
                        readLine(temp, f);
                        readLine(temp, f);
                        vector<Item> v = readFile(f);
                        for(int i = 0; i < v.size(); i++)
                            productList.push_back(v[i]);
                    }
                    fclose(f);
                    // strcpy(temp, "");
                    serial++;
                }
                idx = productList.size();
                //cout<<idx<<endl;
                productList = amountCalculation(productList);
                for(int i = 0; i < idx; i++)
                {
                    for(int j = i + 1; j < idx; j++)
                    {
                        //cout<<"This is first loop"<<endl;
                        if(strcmp(productList[i].p_name, productList[j].p_name) == 0)
                        {
                            //cout<<"Hello World"<<endl;
                            char *a, temp[10];
                            double b, c;
                            b = strtod(productList[i].qnt, &a);
                            b += strtod(productList[j].qnt, &a);

                            c = strtod(productList[i].amnt, &a);
                            c += strtod(productList[j].amnt, &a);

                            sprintf(temp,"%.1lf", b);
                            strcpy(productList[i].qnt, temp);
                            sprintf(temp, "%.1lf", c);
                            strcpy(productList[i].amnt, temp);
                            productList.erase(productList.begin() + j);
                            j--;
                            idx--;
                        }
                    }
                }
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 76, 784, 465);
                for(int i = 0; i < idx && i < 13; i++)
                    productList[i].show(75 + (i * 30), 75 + ((i + 1) * 30));
                setbkcolor(LIGHTGRAY);
                if(serial == 1) outtextxy(400 - textwidth("Not Found")/2, 270 - textheight("A")/2, "Not Found");
            }
            if(prev.cursor())
            {
                if(currentPage) currentPage--;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 76, 784, 465);
                for(int i = 0; (currentPage * 13 + i)< idx && i < 13; i++)
                    productList[currentPage * 13 + i].show(75 + (i * 30), 75 + (i + 1) * 30);

            }
            else if(next.cursor())
            {
                if((currentPage + 1) * 13 < idx)currentPage++;
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(16, 76, 784, 465);
                for(int i = 0;  (currentPage * 13 + i)< idx && i < 13; i++)
                    productList[currentPage * 13 + i].show(75 + (i * 30), 75 + (i + 1) * 30);
            }
            else if(exit.cursor())
            {
                admin();
                return;
            }

            counter(261, 349, currentPage, "");
        }
        if(kbhit()) getch();
    }
}
void userProfile()
{
    cleardevice();
    frame();

    setcolor(CYAN);
    setbkcolor(LIGHTGRAY);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 6);
    outtextxy(350-textwidth("Shop Manager")/2, 68-textheight("A")/2, "Shop Manager");

    setcolor(GREEN);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(170, 120, "Current Pin:");
    outtextxy(170, 200, "New Pin:");
    outtextxy(170, 280, "Shop Name:");

    char cPin[20] = "", oldPin[20] = "", fPath[50] = "", newPin[20] = "";

    strcpy(fPath, filePath("admin.txt"));

    FILE *f = fopen(fPath, "r");
    readLine(oldPin, f);
    readLine(shopName, f);
    fclose(f);

    Input currPin(170, 150, 530, 190);
    Input nPin(170, 230, 530, 270);
    Input shop(170, 310, 530, 350);
    setcolor(WHITE);
    Button submit(170, 360, 530, 400, 3, "Submit");
    Button ext(170, 410, 530, 450, 5, "Exit");
    shop.getText(shopName);
    while(true)
    {
        submit.hover(GREEN);
        ext.hover(RED);
        if(GetAsyncKeyState(VK_LBUTTON) & (0x8000 != 0))
        {
            if(currPin.cursor()) currPin.getPin(cPin);
            else if(nPin.cursor()) nPin.getPin(newPin);
            else if(shop.cursor()) shop.getText(shopName);
            else if(submit.cursor())
            {
                if(strlen(shopName) && strlen(cPin) && strlen(newPin) > 3)
                {
                    if(strcmp(cPin, oldPin) != 0) warning("Note: Current Pin is not Correct");
                    else if(strcmp(cPin, oldPin) == 0)
                    {
                        f = fopen(fPath, "w");
                        fprintf(f, "%s%s", newPin, "\n");
                        fprintf(f, "%s%s", shopName, "\n");
                        fclose(f);
                        admin();
                        return;
                    }
                }
                else warning("Note: Something Wrong");
            }
            else if(ext.cursor())
            {
                admin();
                return;
            }
        }
        if(kbhit()) getch();
    }
}
