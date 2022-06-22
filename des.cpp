// including header files
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

// Array to hold the 16 version of keys
string round_keys[16];

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

// convert binary to string
string binToStr(string s)
{
    int n = s.length();
    n = n / 8;
    string res = "";
    for (int i = 0; i < n; i++)
    {
        string buff = s.substr(i * 8, 8);
        int decVal = binToDec(buff);
        char x = decVal;
        res += x;
    }
    return res;
}

// convert binary string to hexadecimal string
string bin2hex(string s)
{
    // binary to hexadecimal conversion
    string mp[16];
    mp[0] = "0";
    mp[1] = "1";
    mp[2] = "2";
    mp[3] = "3";
    mp[4] = "4";
    mp[5] = "5";
    mp[6] = "6";
    mp[7] = "7";
    mp[8] = "8";
    mp[9] = "9";
    mp[10] = "A";
    mp[11] = "B";
    mp[12] = "C";
    mp[13] = "D";
    mp[14] = "E";
    mp[15] = "F";

    string hex = "";
    for (int i = 0; i < s.length(); i += 4)
    {
        string ch = "";
        ch = s[i];
        ch += s[i + 1];
        ch += s[i + 2];
        ch += s[i + 3];
        int x = binToDec(ch);
        hex += mp[x];
    }
    return hex;
}

// Circular left shift by 1
string shift_1(string key_chunk)
{
    string shifted = "";
    for (int i = 1; i < 28; i++)
    {
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];
    return shifted;
}

// Circular left shift by 2
string shift_2(string key_chunk)
{
    string shifted = "";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 1; j < 28; j++)
        {
            shifted += key_chunk[j];
        }
        shifted += key_chunk[0];
        key_chunk = shifted;
        shifted = "";
    }
    return key_chunk;
}

// XOR operation between two strings
string Xor(string a, string b)
{
    string result = "";
    int size = b.size();
    for (int i = 0; i < size; i++)
    {
        if (a[i] != b[i])
        {
            result += "1";
        }
        else
        {
            result += "0";
        }
    }
    return result;
}

// Generate the 16 version of keys.
void keys(string key)
{
    // The PC1 permutation table
    int pc1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

    // The PC2 permutation table
    int pc2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32};

    // Compression permutation with PC1 table
    string perm_key = "";
    for (int i = 0; i < 56; i++)
    {
        perm_key += key[pc1[i] - 1];
    }

    // Dividing the key into two halves of 28 bits each
    string left = perm_key.substr(0, 28);
    string right = perm_key.substr(28, 28);

    // generating 16 keys
    for (int i = 0; i < 16; i++)
    {
        // For rounds 1, 2, 9, 16 the LCS is performed by one.
        if (i == 0 || i == 1 || i == 8 || i == 15)
        {
            left = shift_1(left);
            right = shift_1(right);
        }

        // For other rounds, the LCS is done by two or shifted by 2
        else
        {
            left = shift_2(left);
            right = shift_2(right);
        }

        // Combining the left and right now for permutation
        string combined_key = left + right;
        string round_key = "";

        // Using the PC2 table to transpose the key bits
        for (int i = 0; i < 48; i++)
        {
            round_key += combined_key[pc2[i] - 1];
        }

        // keys are ready to use - 48 bits each
        round_keys[i] = round_key;
    }
}

// DES algorithm
string DES(string pt)
{
    // The initial permutation table
    int initial_permutation[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7};

    // The expansion permutation table
    int expansion_table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1};

    // S-boxes or substitution boxes
    // 8 boxes, each with 4 rows and 16 columns and contains value from 0 to 15
    int substition_boxes[8][4][16] =
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
         {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
         {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
         {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
         {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
         {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
         {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
         {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

    // permutation table
    int permutation_tab[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25};

    // inverse permutation table
    int inverse_permutation[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};

    // Using the initial permutation
    string perm = "";
    for (int i = 0; i < 64; i++)
    {
        perm += pt[initial_permutation[i] - 1];
    }

    // Dividing the plain text bits into two equal halves of 32 bits each
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    // The plain text is run through round functions 16 times
    for (int i = 0; i < 16; i++)
    {
        string right_expanded = "";

        // right half of the plain text is expanded
        for (int i = 0; i < 48; i++)
        {
            right_expanded += right[expansion_table[i] - 1];
        }

        // result is xored with a key
        string xored = Xor(round_keys[i], right_expanded);
        string res = "";

        // result is divided into 8 equal parts and passed to the S-boxes
        // each s-box reduces passed 6 bits to 4 bits
        for (int i = 0; i < 8; i++)
        {
            // Finding row and column indices to lookup the substituition box
            string row1 = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
            int row = binToDec(row1);

            // finding column
            string col1 = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);
            int col = binToDec(col1);
            int val = substition_boxes[i][row][col];
            res += decToBin(val);
        }

        // another permutation
        string perm2 = "";
        for (int i = 0; i < 32; i++)
        {
            perm2 += res[permutation_tab[i] - 1];
        }

        // result is xored with the left half
        xored = Xor(perm2, left);

        // the left and the right parts of the plain text are swapped
        left = xored;
        if (i < 15)
        {
            string temp = right;
            right = xored;
            left = temp;
        }
    }
    // The halves of the plain text are concatenated
    string combined_text = left + right;
    string ciphertext = "";

    // The inverse of the initial permuttaion is used
    for (int i = 0; i < 64; i++)
    {
        ciphertext += combined_text[inverse_permutation[i] - 1];
    }

    // return the cipher text generated
    return ciphertext;
}

// main function
int main()
{
    // A 64 bit key - in binary or can be in hexadecimal or text as well but needs to be converted in binary string
    string key = "1010101010111011000010010001100000100111001101101100110011011101";

    // plain text in block of 64 bits - taking user input
    string pt;
    cout << "Enter a string: ";
    getline(cin, pt);

    // dividing the input into blocks of 64 bits
    int n_blocks = ceil(pt.length() / 8.0);

    // array of strings each of 64 bits
    string *inp = new string[n_blocks];

    // assign 64 bits to each of the string
    for (int i = 0; i < n_blocks; i++)
    {
        *(inp + i) = pt.substr(i * 8, 8);
    }

    // pad null character to the string that is short of 64 bits
    int last_size = 8 - ((*(inp + n_blocks - 1)).length());
    // or use PKCS5 Padding
    // char x = last_size;
    char x = 0;
    for (int i = 0; i < last_size; i++)
    {
        (*(inp + n_blocks - 1)) += x;
    }

    // generate the keys (16 versions)
    keys(key);

    // print the plain text
    cout << "Plain text input: " << pt << endl;
    cout << endl
         << "******* Encryption *******" << endl;

    // array of strings to hold cipher text
    string *ct = new string[n_blocks];

    // generate cipher text
    string finalCipher = "";
    for (int i = 0; i < n_blocks; i++)
    {
        *(inp + i) = strToBinary(*(inp + i));
        *(ct + i) = DES(*(inp + i));
        // cout << "Ciphertext in binary: " << *(ct + i) << endl;
        finalCipher += bin2hex(*(ct + i));
        cout << "Cipher for \"" << binToStr(*(inp + i)) << "\": " << bin2hex(*(ct + i)) << endl;
    }

    // print the final cipher text
    cout << "Final Cipher in HEX is: " << finalCipher << endl;

    // decrypting the generated cipher text
    int m = 15;
    int n = 0;

    // swap the keys
    while (m > n)
    {
        string temp = round_keys[m];
        round_keys[m] = round_keys[n];
        round_keys[n] = temp;
        m--;
        n++;
    }

    // decrypt each block of plain text
    cout << endl
         << "******* Decryption *******" << endl;
    string decrText = "";
    int checkFlag = 1;
    for (int i = 0; i < n_blocks; i++)
    {
        pt = *(ct + i);
        string decrypted = DES(pt);
        // cout << "Decrypted text in binary:" << decrypted << endl;
        // cout << "Decrypted text:" << binToStr(decrypted) << endl;
        decrText += binToStr(decrypted);

        // Comapring the initial text with the decrypted text
        if (decrypted != *(inp + i))
            checkFlag = 0;
    }

    // print the decrypted text
    cout << "Decrypted text: " << decrText << endl;

    // check if decryption is successfull
    if (checkFlag)
        cout << "\nDecryption is Successfull." << endl;
    else
        cout << "\nDecryption Failed!" << endl;

    // delete the heap memory used
    delete inp, ct;
}