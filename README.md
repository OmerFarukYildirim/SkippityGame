Game Board: The game board is a square matrix of user-defined size, up to a maximum of 20x20 to fit on the screen. The cell colors are irrelevant, but clear cell boundaries are printed for better visualization.

Random Initial Placement: At the start of the game, pieces are placed randomly on the board according to the rules.

Piece Representation: Pieces are represented by letters (A, B, C, ...).

Player Moves: Players specify the coordinates of the piece they want to move and the target cell. Captured pieces are automatically removed from the board, and scores are updated.

Undo/Redo Functionality: Each player has one chance to undo or redo a move before the opponent makes their move.

Real-Time Board Updates: The game board is updated and displayed after every move.

Save and Load Game: Players can end the game at any time, save the current state, and resume later.

Game Modes: The game supports two modes - two-player mode and single-player mode against the computer.

Score Calculation: Scores are calculated according to the game rules and displayed throughout the game.

AI Opponent: In single-player mode, the AI selects moves that either prevent the player from capturing pieces, maximize its captures, or obtain necessary pieces to win. The efficiency of these algorithms affects the AI's performance.

Function-Based Code: All code is modular, utilizing functions to promote clarity and reusability. Global variables are avoided, and information is passed through parameters.

Dynamic Memory Allocation: The game board and other necessary structures are allocated dynamically.

Game Save Format: The game state is saved in either text format, depending on the design.
