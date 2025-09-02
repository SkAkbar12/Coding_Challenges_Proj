# Coding Challenges Project

A collection of command-line tools and algorithms implemented in C++.

---

### **Installation**
```bash
# clone the repo (example)
git clone https://github.com/SkAkbar12/Codong_Challenges_Proj.git
cd Codong_Challenges_Proj


```


## **1. Word Count (wc)**

A classic implementation of the Unix `wc` command.

**Algorithm:** Simple state-based counter.




### **Usage**
```bash
# Count lines, words, and bytes
wc file.txt

# Count lines only
wc -l file.txt

# Count words only
wc -w file.txt

# Count characters
wc -m file.txt

# Count bytes
wc -c file.txt

# Count multiple files
wc file1.txt file2.txt

```

2. JSON Parser

A lightweight tool to parse and process JSON data.

Algorithm: Recursive descent parser.

```bash
# Compile
g++ jsonparser.cpp -o jsonparser

# Run
./jsonparser input.json

```

3. Huffman Compression Tool

A file compression and decompression tool using Huffman coding.

Algorithm: Huffman Coding (priority queue & prefix codes).

```bash
# Compile
g++ huffman_compressor.cpp -o huffman

# Compress a file
./huffman compress input.txt

# Decompress a file
./huffman decompress archive.huff

```


