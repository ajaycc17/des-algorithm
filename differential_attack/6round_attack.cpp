#include "./diff_dist.h"

string round15 = "";
string f1 = "";
string f2 = "";

void generate_random_inputs()
{
    long long int rand_no, j = 0;
    char a[64];
    FILE *fr;
    fr = fopen("random.txt", "w+");
    // generate 250 random 64-bit numbers
    while (j < 250)
    {
        // each string is of length 64-bits
        for (int i = 0; i < 64; i++)
        {
            rand_no = rand() % 2;
            rand_no += 48; // conversion to ascii char
            a[i] = rand_no;
        }
        // print to random.txt file
        for (int i = 0; i < 64; i++)
            fprintf(fr, "%c", a[i]);
        fprintf(fr, "\n");
        j++;
    }
}

// generate pairs for the 250 random inputs
void gen_pairs_random()
{
    FILE *fi, *fo;
    fi = fopen("random.txt", "r+");
    fo = fopen("input_random.txt", "w+");
    long long int i = 0;
    long long int k;
    char temp1[64];
    long long int temp2[64];
    long long int t1, t2;
    char s1[17], s2[17];
    int s[64] = {0};

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
    string alpha = "0100000000001000000000000000000000000100000000000000000000000000";

    // The inverse of the initial permuttaion is used
    for (int i = 0; i < 64; i++)
    {
        s[i] = alpha[inverse_permutation[i] - 1] - 48;
    }
    for (int i = 0; i < 64; i++)
    {
        cout << s[i] << ", ";
    }
    cout << endl;

    while (i < 250)
    {
        fscanf(fi, "%s", temp1);
        for (int j = 0; j < 64; j++)
        {
            t1 = temp1[j] - 48;
            t2 = s[j];
            temp2[j] = (t1 ^ t2);
        }
        i++;
        fprintf(fo, "%s\n", temp1);
        for (int k = 0; k < 64; k++)
            fprintf(fo, "%d", temp2[k]);
        fprintf(fo, "\n");
    }
}
// Array to hold the 16 version of keys
string round_keys[16];

// Generate the 16 version of keys.
void keys(string key)
{
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
string DES(string pt, int flag)
{
    // Using the initial permutation
    string perm = "";
    for (int i = 0; i < 64; i++)
    {
        perm += pt[initial_permutation[i] - 1];
    }

    // Dividing the plain text bits into two equal halves of 32 bits each
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    // The plain text is run through round functions 5 times
    for (int i = 0; i < 5; i++)
    {
        string right_expanded = "";

        // right half of the plain text is expanded
        for (int i = 0; i < 48; i++)
        {
            right_expanded += right[expansion_table[i] - 1];
        }

        // result is XORed with a key
        string XORed = XOR(round_keys[i], right_expanded);
        string res = "";

        // result is divided into 8 equal parts and passed to the S-boxes
        // each s-box reduces passed 6 bits to 4 bits
        for (int i = 0; i < 8; i++)
        {
            // Finding row and column indices to lookup the substituition box
            string row1 = XORed.substr(i * 6, 1) + XORed.substr(i * 6 + 5, 1);
            int row = bin_to_dec(row1);

            // finding column
            string col1 = XORed.substr(i * 6 + 1, 1) + XORed.substr(i * 6 + 2, 1) + XORed.substr(i * 6 + 3, 1) + XORed.substr(i * 6 + 4, 1);
            int col = bin_to_dec(col1);
            int val = substition_boxes[i][row][col];
            res += dec_to_bin(val);
        }

        // another permutation
        string perm2 = "";
        for (int i = 0; i < 32; i++)
        {
            perm2 += res[permutation_tab[i] - 1];
        }

        // result is XORed with the left half
        XORed = XOR(perm2, left);

        // the left and the right parts of the plain text are swapped
        left = XORed;
        if (i < 5)
        {
            string temp = right;
            right = XORed;
            left = temp;
            if (flag == 0)
                f1 = right;
            else if (flag == 1)
                f2 = right;
        }
        cout << "Comb text after round: " << i + 1 << " : " << left << " " << right << endl;
        cout << f1 << "\t" << f2 << endl;
    }
    // The halves of the plain text are concatenated
    string combined_text = left + right;

    // The 6th round
    for (int i = 5; i < 6; i++)
    {
        string right_expanded = "";

        // right half of the plain text is expanded
        for (int i = 0; i < 48; i++)
        {
            right_expanded += right[expansion_table[i] - 1];
        }

        // result is XORed with a key
        string XORed = XOR(round_keys[i], right_expanded);
        string res = "";

        // result is divided into 8 equal parts and passed to the S-boxes
        // each s-box reduces passed 6 bits to 4 bits
        for (int i = 0; i < 8; i++)
        {
            // Finding row and column indices to lookup the substituition box
            string row1 = XORed.substr(i * 6, 1) + XORed.substr(i * 6 + 5, 1);
            int row = bin_to_dec(row1);

            // finding column
            string col1 = XORed.substr(i * 6 + 1, 1) + XORed.substr(i * 6 + 2, 1) + XORed.substr(i * 6 + 3, 1) + XORed.substr(i * 6 + 4, 1);
            int col = bin_to_dec(col1);
            int val = substition_boxes[i][row][col];
            res += dec_to_bin(val);
        }

        // another permutation
        string perm2 = "";
        for (int i = 0; i < 32; i++)
        {
            perm2 += res[permutation_tab[i] - 1];
        }

        // result is XORed with the left half
        XORed = XOR(perm2, left);

        // the left and the right parts of the plain text are not swapped for the last round
        left = XORed;
        if (i < 5)
        {
            string temp = right;
            right = XORed;
            left = temp;
            if (flag == 0)
                f1 = right;
            else if (flag == 1)
                f2 = right;
        }
        cout << "Comb text after round: " << i + 1 << " : " << left << " " << right << endl;
        cout << f1 << "\t" << f2 << endl;
    }
    // The halves of the plain text are concatenated
    combined_text = left + right;
    string ciphertext = "";
    cout << "cipher before IP-1: " << combined_text << endl;

    // The inverse of the initial permutaion is used
    for (int i = 0; i < 64; i++)
    {
        ciphertext += combined_text[inverse_permutation[i] - 1];
    }

    // return the cipher text generated
    return ciphertext;
}

// f-box
string f_box(string right)
{
    // expansion
    string right_expanded = "";
    for (int i = 0; i < 48; i++)
    {
        right_expanded += right[expansion_table[i] - 1];
    }

    // s-box
    vector<vector<vector<int>>> frequency_table = generate_diff_dist_tables(); // [8][64][16]
    string res = "";
    for (int i = 0; i < 8; i++)
    {
        string inp = right_expanded.substr(i * 6, 6);
        int maxP = 0, beta = 0;
        for (int j = 0; j < 16; j++)
        {
            if (maxP < frequency_table[i][bin_to_dec(inp)][j])
            {
                maxP = frequency_table[i][bin_to_dec(inp)][j];
                beta = j;
            }
            else if (maxP == frequency_table[i][bin_to_dec(inp)][j])
            {
            }
        }
        res += dec_to_bin(beta);
    }

    // permutation
    string perm2 = "";
    for (int i = 0; i < 32; i++)
    {
        perm2 += res[permutation_tab[i] - 1];
    }
    return perm2;
}

// function to generate characteristics
void characteristics(string alpha)
{

    // Dividing the plain text bits into two equal halves of 32 bits each
    string left = alpha.substr(0, 32);
    string right = alpha.substr(32, 32);
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        cout << i << ": " << bin_to_hex(left) << "\t" << bin_to_hex(right) << endl;

        // pass the right string to f-box
        string res = f_box(right);

        // result is XORed with the left half
        string XORed = "";
        XORed = XOR(res, left);

        // the left and the right parts of the plain text are swapped
        left = XORed;
        if (i < 5)
        {
            string temp = right;
            right = XORed;
            left = temp;
        }
    }
    // The halves of the plain text are concatenated
    string combined_text = left + right;

    // The last(6th) round
    for (; i < 6; i++)
    {
        cout << i << ": " << bin_to_hex(left) << "\t" << bin_to_hex(right) << endl;
        // pass the right string to f-box
        string res = f_box(right);

        // result is XORed with the left half
        string XORed = "";
        XORed = XOR(res, left);

        // the left and the right parts of the plain text are swapped
        left = XORed;
    }
    // The halves of the plain text are concatenated
    combined_text = left + right;
    cout << i << ": " << bin_to_hex(left) << "\t" << bin_to_hex(right) << endl;
}

// main function
int main()
{
    // A 64 bit key - in binary or can be in hexadecimal or text as well but needs to be converted in binary string
    string key = "1110111100110011011101101101111000110100010101111111000100010011";

    // plain text in block of 64 bits - if less than 64 pad 0 in binary at end
    string alpha = "0100000000001000000000000000000000000100000000000000000000000000";
    string pt1 = "1101010100100010110100110111001011100001101111000011001000101010";
    string pt2 = "1101010100100010010100110110001011100001101111000111001000101010";

    characteristics(alpha);

    // generate the keys (16 round keys) - key scheduling algorithm
    keys(key);

    cout << "\n******* Encryption *******" << endl;

    // generate cipher text
    string ct = DES(pt1, 0);
    string ct2 = DES(pt2, 1);
    string newres = bin_to_hex(XOR("1110111101111001111011110011110000111010010001001000000011101010", "1111000111110110000101110010010010001001001001110000010010111100"));
    cout << "after XOR: " << newres << endl;
    cout << "after XOR: " << XOR("1110111101111001111011110011110000111010010001001000000011101010", "1111000111110110000101110010010010001001001001110000010010111100") << endl;
    // cout << "Ciphertext in binary: " << ct << endl;
    // cout << "Cipher for \"" << pt1 << "\": " << bin_to_hex(ct) << endl;
    // cout << "Cipher for \"" << pt1 << " in binary\": " << ct << endl;
    // cout << "Cipher for \"" << pt2 << " in binary\": " << ct2 << endl;

    // decrypting the generated cipher text
    int m = 5;
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
    cout << "\n******* Decryption *******" << endl;
    int checkFlag = 1;
    string decrypted = DES(ct, 0);
    if (decrypted != pt2)
        checkFlag = 0;

    // print the decrypted text
    cout << "Decrypted text: " << decrypted << endl;

    // check if decryption is successfull
    if (checkFlag)
        cout << "\nDecryption is Successfull." << endl;
    else
        cout << "\nDecryption Failed!" << endl;
}