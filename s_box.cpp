#include <bits/stdc++.h>
#include <fstream>
#include <algorithm>
#include "conversions.h"
using namespace std;

// s-box lookup table
int s_box[4][16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

int SBox(string bit_6)
{
    string row = bit_6.substr(0, 1) + bit_6.substr(5, 6);
    string col = bit_6.substr(1, 4);
    int row_int = binToDec(row);
    int col_int = binToDec(col);
    int bit_4 = s_box[row_int][col_int];
    // string res = decToBin(bit_4);
    return bit_4;
}
// main function
int main()
{

    // generate all 6-bit X
    string x1[64];
    int bin_len = 0;
    for (int i = 0; i < 64; i++)
    {
        x1[i] = decToBin(i);
        bin_len = 6 - x1[i].length();
        if (bin_len > 0)
        {
            for (int j = 0; j < bin_len; j++)
            {
                x1[i] = "0" + x1[i];
            }
        }
    }

    // generate all 6-bit alpha-values
    int alpha[64];
    bin_len = 0;
    for (int i = 0; i < 64; i++)
    {
        alpha[i] = i;
        // for binary

        // alpha[i] = decToBin(i);
        // bin_len = 6 - alpha[i].length();
        // if (bin_len > 0)
        // {
        //     for (int j = 0; j < bin_len; j++)
        //     {
        //         alpha[i] = "0" + alpha[i];
        //     }
        // }
    }

    // for each alpha generate another x2 value
    string x2[64];
    int frequency_table[64][16] = {0};
    int temp = 0;
    int temp_beta = 0;

    // generate the frequency table
    for (int i = 0; i < 64; i++)
    {
        bin_len = 0;
        for (int j = 0; j < 64; j++)
        {
            temp = (binToDec(x1[j])) ^ alpha[i];
            x2[j] = decToBin(temp);
            bin_len = 6 - x2[j].length();
            if (bin_len > 0)
            {
                for (int k = 0; k < bin_len; k++)
                {
                    x2[j] = "0" + x2[j];
                }
            }
            temp_beta = SBox(x1[j]) ^ SBox(x2[j]);
            frequency_table[alpha[i]][temp_beta] += 1;
        }
    }

    // print the frequency table
    ofstream fw("out.txt", std::ofstream::out);
    int maxP[3] = {INT_MIN};
    if (fw.is_open())
    {
        fw << hex << " "
           << "\t";
        for (int i = 0; i < 16; i++)
        {
            fw << hex << i << "\t";
        }
        fw << "\n";

        // print the frequencies
        for (int i = 0; i < 64; i++)
        {
            fw << hex << i << "\t";
            for (int j = 0; j < 16; j++)
            {
                fw << dec << frequency_table[i][j] << "\t";
                if (frequency_table[i][j] > maxP[0] && frequency_table[i][j] < 64)
                {
                    maxP[0] = frequency_table[i][j];
                    maxP[1] = i;
                    maxP[2] = j;
                }
            }
            fw << "\n";
        }
        fw.close();
    }
    else
        cout << "Problem with opening file";
    cout << "Max frequency: " << maxP[0] << endl
         << "Max probabilty: " << (double)maxP[0] / 64 << endl
         << "Alpha(In HEX):" << hex << maxP[1] << endl
         << "Beta(In HEX): " << hex << maxP[2] << endl;
    fw.close();
    return 0;
}