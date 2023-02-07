# Wordle-Solver
This project uses statistical evaluations to calculate the best words for the word game "Wordle" given the hints so far.

This program is the same as "Wordle", but the opposite, instead of the computer having a secret word and you trying to guess it, the computer is the one who is guessing, using statistics to evaluate the best guess.

# How to use the Wordle Solver:

1. Open a terminal window on your computer.
2. Navigate to the directory where the wordle.cpp file is stored.
3. Compile the program by running the following command: `g++ wordle.cpp -o wordle`
4. Run the program by typing the following command: `./wordle`
5. The program will give you the estimated best starting word.
6. The program will prompt you to enter hints for the hidden word. Enter five letters in capital, the letters should be the first letter of the color for each letter in the guessed word. For example "GBYBB".
7. The program will use the hints to calculate the next best guess word.
