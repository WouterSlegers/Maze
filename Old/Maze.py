"""
Simple game
"""

import random as rng
import time

class Maze:
    def __init__(self):
        self.fields = 10 #Changing this requires new initialisation of self.connections

        #Connections is fields by fields big but last row and column one value is useless
        self.connections = [
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]],
                            [[False, False], [False, False], [False, False], [False, False], [False, False],
                            [False, False], [False, False], [False, False], [False, False], [False, False]]
                            ]

        self.size = 1 + (self.fields - 1)*2
        self.board = []

        outer_row = ['=']
        middle_row = ['=']
        for dummy in range(self.size):
            outer_row.append('=')
            middle_row.append('.')
        outer_row.append('=')
        middle_row.append('=')

        row = ['=', ' ']
        for x in range(self.fields - 1):
            row.append(".")
            row.append(" ")
        row.append("=")

        self.board.append(outer_row)
        self.board.append(list(row))
        for x in range(self.fields - 1):
            self.board.append(list(middle_row))
            self.board.append(list(row))
        self.board.append(outer_row)

    def print(self):
        form = "{} " * len(self.board)
        for row in self.board:
            print(form.format(*row))
        print("")

    def print_connections(self):
        form = "{} " * self.fields
        for row in self.connections:
            print(form.format(*row))
            print()
        print("")

    def insert(self, input, xco, yco):
        if self.board[yco][xco] != '=':
            self.board[yco][xco] = input

    def update_board(self):
        for index_x, row in enumerate(self.board):
            for index_y, col in enumerate(row):
                if index_x != 0 and index_x < len(self.board) - 1 and index_y != 0 and index_y < len(self.board) - 1:
                    if index_y % 2 == 0 and index_x % 2 == 0:
                        self.board[index_x][index_y] = '+'
                    elif index_y % 2 == 0:
                        x = (index_x - 1)//2 #xy for opening above
                        y = (index_y - 1)//2
                        if self.connections[x][y][0] == False:
                            if index_y % 2 == 0:
                                self.board[index_x][index_y] = '|'
                            else:
                                self.board[index_x][index_y] = '-'
                        else:
                            self.board[index_x][index_y] = ' '
                    elif index_x % 2 == 0:
                        x = (index_x - 1)//2 #xy for opening to the left
                        y = (index_y - 1)//2
                        if self.connections[x][y][1] == False:
                            if index_y % 2 == 0:
                                self.board[index_x][index_y] = '|'
                            else:
                                self.board[index_x][index_y] = '-'
                        else:
                            self.board[index_x][index_y] = ' '
#Files: Maze.py, MazeGeneration.py(, Main.py)

#Idea below is cool, but recursively:
#Find dead ends, open random one as long as it doesn't open to already searched one. If surrounded by searched ones take other dead end
#Repeat search from new opening and add dead ends and searched to existing lists.
#Different from depth-first-search since it doesn't explore one branch before going to the next, rather takes random from entire list of dead ends.

#Randomly generate dense maze, look for path
#No path to end? Add Connections to some of the dead ends
#
#Requires:
#Algorithm to find path:
# Start with 'to_search = [(0, 0)]'
#Iterate:
#Take first node in to_search
#Add nodes that are connected to the current node to 'to_search' unless they are already in list 'searched'
#(note that you have to check left and above as well: if connections[y-1][x][1] == True OR if connections[y][x-1][0] == True)
#Pop current node if it isn't the end, if it did not have any connections save it as a dead end
#

maze = Maze()
x, y = 0, 0
counter = 0
for x in range(maze.fields):
    for y in range(maze.fields):
        counter += 1
        for i in range(2):
            maze.connections[y][x][i] = (rng.random() > 0.5)
        if counter % 10 == 0:
            maze.update_board()
            maze.print()
            time.sleep(0.3)

#When using terminal:
# input("Press enter to exit ")
