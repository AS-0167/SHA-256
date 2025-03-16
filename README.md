# SHA-256 Hash CLI Tool

## Overview
The **SHA-256 Hash CLI Tool** is a command-line utility that allows users to compute the SHA-256 hash of a file and store the result in a specified output file. This tool is lightweight, efficient, and ensures that large files exceeding system memory are not processed, preventing crashes. I've implemented this hasing algorithm from scratch in **C language**.

## Features
- Computes the **SHA-256** hash of a given file.
- Prevents loading files larger than available system memory.
- Outputs the hash to a user-specified file.
- Simple installation and usage via the command line.
- **Implemented from scratch without using any hashing library.**

---

## Installation
To install the tool, follow these steps:

### **1. Clone the Repository**
```sh
git clone https://github.com/AS-0167/SHA-256.git
cd SHA-256
```

### **2. Build the Executable**
```sh
make
```

### **3. Install the CLI System-Wide**
```sh
sudo make install
```
This installs the tool as `sha-256-hash` in `/usr/local/bin`, making it accessible from anywhere in the terminal.

---

## Usage
Once installed, you can run the tool using the following syntax:
```sh
sha-256-hash --fpath <file_path> --sha256path <output_path>
```

### **Example**
To compute the SHA-256 hash of `example.txt` and save it to `hash_output.txt`, use:
```sh
sha-256-hash --fpath example.txt --sha256path hash_output.txt
```

### **Error Handling**
- If the file does not exist, the program will display an error message.
- If the file size exceeds system memory, the program will **terminate safely**.
- If incorrect flags are provided, a usage message will be shown.

---

## Uninstallation
To remove the tool from your system, run:
```sh
sudo make uninstall
```
This deletes the installed binary from `/usr/local/bin/sha-256-hash`.

---

## Development
If you wish to modify or contribute to this project, follow these steps:

### **1. Clean Previous Builds**
```sh
make clean
```

### **2. Delete the Executable**
```sh
make delete
```

### **3. Run Without Installing**
```sh
make run
```
This will execute the tool without installing it system-wide.

---

## Author
Developed by **[AS]**.
For any questions or contributions, feel free to reach out or submit a pull request.

