# Text Editor in C

A simple command-line text editor written in C, designed to mimic basic editing functionality.  
This project uses a **doubly linked list** to represent text and a **stack** to support undo operations.

---

## ✨ Features

- **Navigation**
  - `B` → Move cursor **backward** by one character  
  - `F` → Move cursor **forward** by one character  
  - `S` → Jump to **start of the text**  
  - `E` → Jump to **end of the text**  

- **Editing**
  - `I` → Insert a character at the current cursor position  
  - `D` → Delete the character at the current cursor position  

- **Undo**
  - `Z` → Undo the last operation (move, insert, delete, jump)  

- **Exit**
  - `X` → Exit the editor  

---

## 📂 Project Structure

```plaintext
.
├── .gitignore      # Ignore build artifacts and other unneeded files
├── README.md       # Project documentation
├── main.c          # Source code for the text editor
└── text.txt        # Input file loaded at startup
```

⚡ **How It Works**

1. The editor loads text from text.txt and stores it in a linked list of words.
2. A cursor (|) shows the current editing position.
3. Users enter single-character commands to move around and edit text.
4. Undo (Z) reverts the last modification.

▶️ **Compilation & Usage**

1. Compile the program:
```gcc main.c -o editor```
2. Make sure you have a file named text.txt in the same directory with some text. Example:
```
Hello World
This is a simple text editor.
```
3. Run the editor:
```
./editor
```
4. Enter commands interactively (B, F, I, D, S, E, Z, X).

🖼 **Example Run**
```
Hello| World
F
Hello |World
I
Enter a character: !
Hello!| World
Z
Hello| World
X
```

🚀 **Future Improvements**

- Save the edited text back to a file.
- Insert multiple characters instead of one at a time.
- Add redo functionality.
- Improve memory management and error handling.

🛠 **Requirements**

- GCC or any C compiler
- macOS/Linux/WSL (Windows may require small adjustments)
