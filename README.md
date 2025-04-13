
# XOER

This is a Command-Line Interface (CLI) tool for encrypting and decrypting files using XOR encryption combined with SHA-256 hashing. It uses a password that is hashed using SHA-256, which is then used to XOR the contents of the file. The tool offers simple operations for file encryption and decryption, ensuring the integrity of the file with a hash and offering user-friendly handling of file names.

## Features

- **File Encryption**: Encrypts a file with a user-provided password. The password is hashed using SHA-256 and XORed with the file's data.
- **File Decryption**: Decrypts a previously encrypted file using the same password. The tool verifies file integrity by checking the hash stored in the file.
- **Integrity Verification**: The integrity of the encrypted or decrypted file is verified using SHA-256 hashing. Any tampering or incorrect password results in a failure to decrypt.
    
-   **Automatic Output File Naming**: If no output file is provided, the tool automatically generates an output filename based on the input file name.
    
    -   For encryption, it appends `.enc` to the filename.
        
    -   For decryption, it removes `.enc` and appends `.dec` (if `.enc` isn't already present).

## Usage


To see the help message please type:


`λ xoer -h` 

### Encrypt a file:

secret.txt


`λ xoer encrypt -i secret.txt -o more-secure-secret.txt` 

### Decrypt a file:

more-secure-secret.txt


`λ xoer decrypt -i more-secure-secret.txt -o secret.txt` 

### Arguments:

-   [REQUIRED] `-i <input_file>`: Specifies the input file to encrypt or decrypt.
    
-   [OPTIONAL] `-o <output_file>`: Specifies the output file. If not provided, the tool will automatically generate an output filename based on the input file.

![Actual demonstration of the tool via gif](https://s6.gifyu.com/images/bMwBo.gif)

## How It Works

1.  **Password Hashing**: The password provided by the user is hashed using the SHA-256 algorithm. This hash is used as the encryption key.
    
2.  **XOR Encryption**: The data from the file is XORed with the hashed password. This process transforms the file's data, making it unreadable without the correct password.
    
3.  **Integrity Check**: A SHA-256 hash of the original data is prepended to the encrypted data. During decryption, the hash is extracted and checked against the computed hash of the decrypted file to verify the integrity and ensure the correct password was used.
    
4.  **File Output**: The encrypted or decrypted file is saved with the specified (or automatically generated) output filename.

![Flowchart explaining how encryption and decryption works](https://i.ibb.co/h14NbFFs/xoer.png)

## BYOB (Build Your Own Binary)

Get the xoer code on your local machine:
`λ git clone https://github.com/kartikjain14/xoer.git && cd xoer`

To compile the tool, you can use the following command:

`λ g++ xoer.cpp -o xoer -lssl -lcrypto`

Or, you could just make the build file using:

`λ make`

## Security Note

This tool does not delete the original file after encryption or decryption and it is an intentional behaviour. You are responsible for deleting the original file if desired. Make sure to keep backups of important files.
Also, this tool isn't following the best standards which casues it to show your password in clear text in your terminal history, You are responsible to deal with it until the next version is here!

### Things to add:
1. Mask passwords.

2. Add Salting.

3. Progress Bars.

4. Add Tests.

## Community:

To **contribute** on this project, please make pull request with concise descriptions.
To **report bug** on this project please create an issue in the repository.
