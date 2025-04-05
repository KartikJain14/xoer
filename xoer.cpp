// NOTE: This CLI Tool does not delete the original file after encryption/decryption

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <openssl/sha.h>  // SHA-256 hashing
#include <stdexcept> // Exception handling
#include <sstream>   // String Stream processing (File read/write operations)
#include <sys/stat.h> // For checking file existence and permissions

using namespace std;

// Goated Stackoverflow: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c

// Check to see if the file exists
bool file_exists(const string &filename) {
    struct stat buffer;  // idk what this is but it is required in stat()
    return (stat(filename.c_str(), &buffer) == 0);  // Checks if the file exists by querying the metadata
}

// Function to compute SHA-256 hash which is going to be used for checking the data integrity and verification and also dor hashing the password
vector<unsigned char> compute_sha256(const vector<char> &data) {
    // Since SHA256 function below does not return a value,we need a bugger to store the hash
    vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
    // reinterpret_cast<const unsigned char *>(data.data()) converts from data to unsigned char pointer
    SHA256(reinterpret_cast<const unsigned char *>(data.data()), data.size(), hash.data());
    return hash;
}

// Backbone of the entire project
// This function XORs the input data with the hashed password (additional layer of security)
void xor_crypt(vector<char> &data, const vector<unsigned char> &key) {
    // size_t is used because int would be too small for large files and data.size is also in size_t type
    for (size_t i = 0; i < data.size(); i++) {
        // Use this legendary operation to get the encrypted data
        data[i] ^= key[i % key.size()];  // i & key.size() is required when key is smaller than data to reuse the bytes
    }
}

// Convert file data to vector<char> (used in the program ahead)
vector<char> read_file(const string &filename) {
    if (!file_exists(filename)) {
        cerr << "Error: File not found: " << filename;
        exit(1);
    }
    ifstream file(filename, ios::binary);  // Open file in binary mode to support all file types and prevent formattings
    if (!file) {
        cerr << "Error: Could not open " << filename;  // In case of file open error (admin permissions etc)
        exit(1);
    }
    return vector<char>(istreambuf_iterator<char>(file), istreambuf_iterator<char>());  // Convert file contents into vector
}

// Function to write a vector to a file
// Saves binary data to the file
void write_file(const string &filename, const vector<char> &data) {
    ofstream file(filename, ios::binary);  // Open file in binary mode
    if (!file) {
        cerr << "Error: Could not write to " << filename << " (Check directory permissions)";  // Handle file write error
        exit(1);
    }
    file.write(data.data(), data.size()); // Actually write the content in the file
}

// I'll be explaining this in the README file in actual english

// Encrypts the file with our password and also has features to check the integrity of the file
void encrypt(const string &input_file, const string &output_file, const vector<unsigned char> &key) {
    vector<char> input_data = read_file(input_file);
    vector<unsigned char> hash = compute_sha256(input_data);
    xor_crypt(input_data, key);
    input_data.insert(input_data.begin(), hash.begin(), hash.end());  // Prepend the hash to the encrypted data
    write_file(output_file, input_data);
    cout << "Encryption successful: " << output_file << endl;  // ggwp
}

// Decrypts the file provied you use the same password as the one you used to encrypt the file with and aaditionally verify integrity of the file
void decrypt(const string &input_file, const string &output_file, const vector<unsigned char> &key) {
    vector<char> input_data = read_file(input_file);
    if (input_data.size() < SHA256_DIGEST_LENGTH) { // Check if the file is too small to even contain the hash which means it is invalid
        cerr << "Error: Invalid encrypted file.";
        exit(1);  // Exit the program with error status
    }
    vector<unsigned char> stored_hash(input_data.begin(), input_data.begin() + SHA256_DIGEST_LENGTH);  // Extract stored hash
    input_data.erase(input_data.begin(), input_data.begin() + SHA256_DIGEST_LENGTH);  // Remove hash from data
    xor_crypt(input_data, key);
    vector<unsigned char> computed_hash = compute_sha256(input_data);
    if (stored_hash != computed_hash) {
        cerr << "Error: Decryption failed. Incorrect password or file tampered." << endl;
        exit(1);
    }
    write_file(output_file, input_data);  // ggwp
    cout << "Decryption successful: " << output_file << endl;
}

// Display help message and terminate the program
void help() {
    cerr << "Usage: \n"
         << "  encrypt -i <input_file> -o [output_file]\n"
         << "  decrypt -i <input_file> -o [output_file]\n";
    exit(1);
}

// https://www.youtube.com/watch?v=fyRMUnhvjqM definitely helped me

// Main function: Handles command-line arguments and runs encryption/decryption
int main(int argc, char *argv[]) {
    string input_file, output_file, password;

    if (argc < 3 || argc > 7) {
        help();
    }

    // Check what is the input and output file passed by the cli user
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if(arg == "-h" || arg == "--help") {
            help();
        } else if ((arg == "-i" || arg == "--input") && argc > i+1) {
            input_file = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && argc > i+1) {
            output_file = argv[++i];
        }
    }

    // Check if input file is provided
    if (input_file.empty()) {
        cerr << "Error: Missing compulsory argument -i (input file)\n";
        return 1;
    }

    if(argv[1] != string("encrypt") && argv[1] != string("decrypt")) {
        cerr << "Error: Invalid operation. Use 'encrypt' or 'decrypt'.\n";  // Why would someone try anything else?
        cerr << "To check the usage, try ./xoer"; // Yet to name ths project
        return 1;
    }

    // Shift to taking password from the user instead of command line arguments to prevent password leaks in the terminal history
    // I could implement the secure password input that would hide the password but that would require a lots of library installations and copy pasting
    // The password is still visible to users since output is stored
    cout << "Enter password: [The password will be visible in terminal command history]\n";
    getline(cin, password);

    if (password.empty()) {
        cerr << "Error: Password cannot be empty.\n";
        return 1;
    }

    // Sweet feature I added that I am proud of

    // Default output file if not provided
    // I am not explaining this in the README file so here you go:
    // If the user does not provide an output file, we will create one based on the input file name and the operation.
    // For encryption, we append ".enc" to the input file name.
    // For decryption, we remove ".enc" from the input file name
    // and append ".dec" if the ".enc" is not already present.
    // This way, the user does not have to specify an output file name every time.
    // This is a good feature because it retains the original file name and makes it easier to find the crypted files.

    if (output_file.empty()) {
        if (argv[1] == string("decrypt")) {
            // For decryption: Remove ".enc" from input filename
            if(input_file.size() < 4 || input_file.substr(input_file.size() - 4) != ".enc") {
                output_file = input_file + ".dec";  // Append ".dec" if not present
            }
            output_file = input_file.substr(0, input_file.size() - 4);
        } else {
            // For encryption: Add ".enc" to input filename
            output_file = input_file + ".enc";
        }
    }
    // You're an OG if you are reading this

    // Convert password to SHA-256 hash for an additional layer of security not sure how though
    const vector<unsigned char> key = compute_sha256(vector<char>(password.begin(), password.end()));  // Hash the password
    // Perform encryption or decryption
    if (argv[1] == string("decrypt")) {
        decrypt(input_file, output_file, key);  // Decrypt
    } else {
        encrypt(input_file, output_file, key);  // Encrypt
    }
    
    return 0;
}