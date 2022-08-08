#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

// convert string to binary in string form
string strToBinary(string s)
{
    int n = s.length();
    string res = "";
    for (int i = 0; i < n; i++)
    {
        // convert each char to ASCII value
        int val = int(s[i]);

        // Convert ASCII value to binary
        string bin = "";
        while (val > 0)
        {
            (val % 2) ? bin.push_back('1') : bin.push_back('0');
            val /= 2;
        }
        int len = 8 - bin.length();
        for (int i = 0; i < len; i++)
        {
            bin.push_back('0');
        }
        reverse(bin.begin(), bin.end());
        // store in res variable
        res += bin;
    }
    return res;
}

// Convert a number in binary (string) to decimal
int binToDec(string binary)
{
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    for (int i = size - 1; i >= 0; i--)
    {
        if (binary[i] == '1')
        {
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}

// Convert a number from decimal to binary
string decToBin(int decimal)
{
    string binary;
    while (decimal != 0)
    {
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal = decimal / 2;
    }
    while (binary.length() < 4)
    {
        binary = "0" + binary;
    }
    return binary;
}