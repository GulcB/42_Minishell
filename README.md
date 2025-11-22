# 🐚 42 Minishell

> **A faithful recreation of the Bash shell, built from scratch in C.**

This project is a core part of the **42 Istanbul** curriculum. It involves writing a custom shell to deeply understand process creation, synchronization, file descriptors, and the Unix API.

---

### 🛠️ Features

It's not just a toy shell; it handles complex operations:

* **Prompt:** Displays a custom prompt awaiting user input.
* **Command History:** Maintains a history of commands (using `readline`).
* **System Executables:** Searches `$PATH` to launch binaries (e.g., `ls`, `grep`, `vim`).
* **Redirections:**
    * `>` (Output redirection)
    * `>>` (Append mode)
    * `<` (Input redirection)
    * `<<` (Here-document)
* **Pipes (`|`):** Connects the output of one command to the input of the next.
* **Environment Variables:** Handles expansion (`$VAR`) and management (`export`, `unset`).
* **Signal Handling:** Mimics Bash behavior for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.
* **Built-in Commands:**
    * `echo` (with `-n`)
    * `cd` (with relative/absolute paths)
    * `pwd`
    * `export`
    * `unset`
    * `env`
    * `exit`

---

### 🧠 Engineering Challenges & Learnings

Building this project required mastering low-level system engineering concepts:

* **Process Management:** Using `fork()` to create child processes and `execve()` to run commands without crashing the main shell.
* **Memory Management:** Strict manual memory management to ensure **zero memory leaks**, validating with Valgrind.
* **File Descriptors:** Manually manipulating file descriptors (using `dup2`, `pipe`) to handle complex pipelines and redirections.

---

### 🚀 How to Run

```bash
# 1. Clone the repository
git clone [https://github.com/GulcB/42_Minishell.git](https://github.com/GulcB/42_Minishell.git)

# 2. Enter the directory
cd 42_Minishell

# 3. Compile the program (using Makefile)
make

# 4. Start the shell
./minishell
