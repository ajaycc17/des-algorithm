// convert a number from decimal to binary
string dec_to_bin(int decimal)
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

// convert a number from binary to decimal
int bin_to_dec(string binary)
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

// convert string to binary
string str_to_bin(string s)
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
string bin_to_str(string s)
{
    int n = s.length();
    n = n / 8;
    string res = "";
    for (int i = 0; i < n; i++)
    {
        string buff = s.substr(i * 8, 8);
        int decVal = bin_to_dec(buff);
        char x = decVal;
        res += x;
    }
    return res;
}

// convert binary to hexadecimal
string bin_to_hex(string s)
{
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
    int bin_len = 0;
    // make string of 8 bits atleast
    if (s.length() % 8 == 0)
        bin_len = 0;
    else
        bin_len = 8 - (s.length() % 8);
    if (bin_len > 0)
        for (int j = 0; j < bin_len; j++)
            s = "0" + s;
    // convert to hex
    for (int i = 0; i < s.length(); i += 4)
    {
        string ch = "";
        ch = s[i];
        ch += s[i + 1];
        ch += s[i + 2];
        ch += s[i + 3];
        int x = bin_to_dec(ch);
        hex += mp[x];
    }
    return hex;
}

// convert hexadecimal to binary
string hex_to_bin(string s)
{
    unordered_map<char, string> mp;
    mp['0'] = "0000";
    mp['1'] = "0001";
    mp['2'] = "0010";
    mp['3'] = "0011";
    mp['4'] = "0100";
    mp['5'] = "0101";
    mp['6'] = "0110";
    mp['7'] = "0111";
    mp['8'] = "1000";
    mp['9'] = "1001";
    mp['A'] = "1010";
    mp['B'] = "1011";
    mp['C'] = "1100";
    mp['D'] = "1101";
    mp['E'] = "1110";
    mp['F'] = "1111";

    string bin = "";
    for (int i = 0; i < s.size(); i++)
    {
        bin += mp[s[i]];
    }
    return bin;
}