# ⏱️ ChronoC

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

Feel free to contribute or suggest improvements!
