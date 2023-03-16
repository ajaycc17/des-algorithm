#include "../des_header.h"
#include "../conversions.h"
using namespace std;

// produce output of S-box
int SBox(string bit_6, int h)
{
    string row = bit_6.substr(0, 1) + bit_6.substr(5, 6);
    string col = bit_6.substr(1, 4);
    int row_int = bin_to_dec(row);
    int col_int = bin_to_dec(col);
    int bit_4 = substition_boxes[h][row_int][col_int];
    return bit_4;
}

// convert any binary number to 6-bits length
void make_6bit_len(string x[], int i)
{
    int bin_len = 6 - x[i].length();
    if (bin_len > 0)
    {
        for (int k = 0; k < bin_len; k++)
        {
            x[i] = "0" + x[i];
        }
    }
}

// generate difference distribution tables
vector<vector<vector<int>>> generate_diff_dist_tables()
{
    // generate all possible 6-bit binary strings i.e. 2^6 = 64
    string x1[64];
    for (int i = 0; i < 64; i++)
    {
        x1[i] = dec_to_bin(i);
        make_6bit_len(x1, i);
    }

    // generate all possible 6-bit alpha-values
    int alpha[64];
    for (int i = 0; i < 64; i++)
    {
        alpha[i] = i;
    }

    // for each alpha generate another x2 value
    string x2[64];
    int temp = 0;
    // int frequency_table[8][64][16]; // similar to the below statement
    vector<vector<vector<int>>> frequency_table(8, vector<vector<int>>(64, vector<int>(16, 0)));
    int temp_beta = 0;

    // update the difference distribution table for each S-boxes
    for (int h = 0; h < 8; h++)
    {
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                temp = (bin_to_dec(x1[j])) ^ alpha[i];
                x2[j] = dec_to_bin(temp);
                make_6bit_len(x2, j);

                // check beta and increment table
                temp_beta = SBox(x1[j], h) ^ SBox(x2[j], h);
                frequency_table[h][alpha[i]][temp_beta] += 1;
            }
        }
    }

    // print the difference distribution table in 8 txt files for record
    for (int l = 0; l < 8; l++)
    {
        char ch = l + 48;
        string name = "Diff_dist_S";
        name.push_back(ch);
        name = "./diff_dist_tables/" + name + ".txt";
        ofstream fw(name, std::ofstream::out);
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
                    fw << dec << frequency_table[l][i][j] << "\t";
                    if (frequency_table[l][i][j] > maxP[0] && frequency_table[l][i][j] < 64)
                    {
                        maxP[0] = frequency_table[l][i][j];
                        maxP[1] = i;
                        maxP[2] = j;
                    }
                }
                fw << "\n";
            }
            fw.close();
        }
    }
    return frequency_table;
}