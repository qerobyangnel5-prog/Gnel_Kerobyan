#include <iostream>
#include <string>

using namespace std;

string compress(string input) {

    string result = "";
    int count = 1;

    for (int i = 1; i <= input.length(); i++) {

        if (input[i] == input[i-1]) {
            count++;
        } else {
            result += to_string(count);
            result += input[i-1];
            count = 1;
        }
    }

    return result;
}

string decompress(string input) {

    string result = "";

    for (int i = 0; i < input.length(); i+=2) {

        int count = input[i] - '0';
        char ch = input[i+1];

        for(int j = 0; j < count; j++)
            result += ch;
    }

    return result;
}

int main() {

    string text;

    cout << "Enter text: ";
    cin >> text;

    string encoded = compress(text);
    cout << "Compressed: " << encoded << endl;

    string decoded = decompress(encoded);
    cout << "Decompressed: " << decoded << endl;

    return 0;
}
