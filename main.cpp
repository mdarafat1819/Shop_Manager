#include "function.h"
#include <time.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdlib.h>

void printMemo(vector<Item> productList, char* fileName)
{
    char printableId[40];
    int j = 0;
    for(int i = strlen(userName) + 1; i < strlen(fileName); i++)
        printableId[j++] = fileName[i];
    printableId[j - 4] = '\0';///.txt is removed

    char memo[40];
    strcpy(memo, fileName);
    memo[strlen(memo) - 4] = 0;
    strcat(memo, ".html");
    char *html = ""
                 "<!DOCTYPE html>\n"
                 "<html>\n<head>\n<style>\ntable"
                 "{\nfont-family:arial, sans-serif;\nborder-collapse:collapse;\nwidth: 100%;\n}\n"
                 "td, th{\nborder: 1px solid #dddddd;\ntext-align:left;\npadding: 8px;\n}\n"
                 "tr:nth-child(odd){\nbackground-color:#dddddd;\n}\n</style>\n</head>\n<body>\n"
                 "";
    FILE *m = fopen(memo, "w");
    FILE *f = fopen(fileName, "r");
    fprintf(m, "%s", html);

    fprintf(m, "%s", "<h2 align=\"center\">");

    fprintf(m, "%s", shopName);
    fprintf(m, "%s", "</h2>\n");
    fprintf(m, "%s", "<table>\n<tr background=""#ddddd0\">\n<th bgcolor=\"#339966\">Order ID:</th>\n");
    fprintf(m, "%s", "<th>");
    char temp[100];
    fprintf(m, "%s", printableId);
    fprintf(m,"%s", "</th>\n");
    fprintf(m, "%s", "<th bgcolor=\"#339966\">Date:</th>\n<th colspan=\"3\">");
    readLine(temp, f);///Date
    fprintf(m, "%s", temp);
    fprintf(m, "%s", "</th>\n</tr>\n<tr>\n");
    fprintf(m, "%s", "<th bgcolor=\"#009999\">Name:</th>\n<th>");///Name
    readLine(temp, f);///Name
    fprintf(m, "%s", temp);
    fprintf(m, "%s", "</th>\n");
    readLine(temp, f);///Phone
    fprintf(m, "%s", "<th bgcolor=\"#009999\">Phone:</th>\n<th colspan=\"3\">");///Phone
    fprintf(m, "%s", temp);
    fprintf(m, "%s", "</th>\n</tr>\n<tr>\n");
    fprintf(m, "%s", "<th bgcolor=\"#339966\">Address:</th>\n<th colspan=\"4\">");///Address
    readLine(temp, f);///Address
    fprintf(m, "%s", temp);
    fprintf(m, "%s", "</th>\n</tr>\n");
    fprintf(m, "%s", " <tr bgcolor=\"#009999\">\n<td>Product Name</td>\n<td>Quantity</td>\n<td>Rate</td>\n<td>Discount</td>\n<td>Amount</td>\n</tr>\n");


    for(int i = 0; i < productList.size(); i++)
    {
        fprintf(m, "%s", "<tr>\n<td>");
        fprintf(m, "%s%s", productList[i].p_name, "</td>\n<td>");
        fprintf(m, "%s%s", productList[i].qnt, "</td>\n<td>");
        fprintf(m, "%s%s", productList[i].rt, "</td>\n<td>");
        fprintf(m, "%s%s", productList[i].dis, "</td>\n<td>");
        fprintf(m, "%s%s", productList[i].amnt, " &#2547</td>\n</tr>");
    }

    fclose(f);
    fclose(m);

    system(memo);
    _unlink(memo);
}

HWND GetConsoleHandle()
{
#define BUFFSIZE 768
    HWND hwnd;
    char pszWindowTitle[BUFFSIZE];

    GetConsoleTitle(pszWindowTitle, BUFFSIZE);
    int i = strlen(pszWindowTitle);
    _itoa(GetTickCount(), &pszWindowTitle[i], 16);
    _itoa(GetCurrentProcessId(), &pszWindowTitle[strlen(pszWindowTitle)], 16);
    SetConsoleTitle(pszWindowTitle);
    Sleep(50);
    hwnd = FindWindow(NULL, pszWindowTitle);
    pszWindowTitle[i] = 0;
    SetConsoleTitle(pszWindowTitle);
    return(hwnd);
}

void createExampleShop()
{
    DIR *folder;
    folder = opendir("example");
    if(folder == NULL)
    {
        mkdir("example");
        system("move admin.txt example");
        system("move store.txt example");
        return;
    }
    return;
}

int main()
{

    HWND hWnd = GetConsoleHandle();
    //ShowWindow( hWnd, SW_HIDE );
    SetMenu(hWnd, NULL);
    w1 = initwindow(800, 520, "Shop Manager", 100, 100);

    createExampleShop();
    login();

    return 0;
}
