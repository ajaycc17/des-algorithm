#include "../des_header.h"
#include "../conversions.h"

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
string DES(string pt)
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

    // The plain text is run through round functions 16 times
    for (int i = 0; i < 15; i++)
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
        if (i < 15)
        {
            string temp = right;
            right = XORed;
            left = temp;
        }
    }
    // The halves of the plain text are concatenated
    string combined_text = left + right;

    // The 16th round
    for (int i = 15; i < 16; i++)
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
        if (i < 15)
        {
            string temp = right;
            right = XORed;
            left = temp;
        }
    }
    // The halves of the plain text are concatenated
    combined_text = left + right;
    string ciphertext = "";

    // The inverse of the initial permuttaion is used
    for (int i = 0; i < 64; i++)
    {
        ciphertext += combined_text[inverse_permutation[i] - 1];
    }
    // cout<<bin_to_hex(combined_text)<<endl;

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
        *(inp + i) = str_to_bin(*(inp + i));
        *(ct + i) = DES(*(inp + i));
        // cout << "Ciphertext in binary: " << *(ct + i) << endl;
        finalCipher += bin_to_hex(*(ct + i));
        cout << "Cipher for \"" << bin_to_str(*(inp + i)) << "\": " << bin_to_hex(*(ct + i)) << endl;
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
        // cout << "Decrypted text:" << bin_to_str(decrypted) << endl;
        decrText += bin_to_str(decrypted);

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