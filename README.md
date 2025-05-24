# ⏱️ ChronoC
<img width="1292" alt="Screenshot 2025-05-24 alle 16 17 44" src="https://github.com/user-attachments/assets/1246725a-b13d-458a-84b1-b67ffd1064f7" />

**ChronoC** is a terminal-based stopwatch application written in C, utilizing the `ncurses` library to provide an interactive text-based user interface. The project is structured modularly, separating concerns such as timekeeping, display, menu navigation, and utility functions for maintainability and clarity.

---

## 📁 Project Structure

- `src/`: Contains the main source files.
  - `main.c`: Entry point of the application.
  - `chronometer_engine.c`: Core logic for stopwatch operations.
  - `clock.c`: Time management functionalities.
  - `display.c`: Handles the user interface using `ncurses`.
  - `menu.c`: Implements the menu system.
  - `utils.c`: Miscellaneous helper functions.

- `include/`: Header files corresponding to the source files.

- `Makefile`: Facilitates compilation of the project.

- `icon.png`: Icon representing the application.

## 🛠️ Requirements

- **Operating System**: Unix-like system (Linux, macOS)
- **Compiler**: GCC
- **Libraries**:
  - `ncurses`: For managing the text-based user interface.

## ⚙️ Compilation

Ensure that `ncurses` is installed on your system. Then, compile the application by running:

```bash
make
```

This will produce an executable named `chrono`.

## 🚀 Execution

After compilation, start the application with:

```bash
./ChronoC
```

## 🧹 Clean Up

To remove compiled objects and the executable, run:

```bash
make clean
```

## 📄 License

IDK what licenses are.
