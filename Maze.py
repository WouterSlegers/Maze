import random as rng
import numpy as np

class Maze:
    def __init__(self, fields):
        self.fields = fields
        self.connections = np.full((self.fields, self.fields, 2), False)

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

    def print(self, field_status):
        alt = 0
        temp_row = []
        self.update_walls()
        form = "{} " * len(self.board)
        for row in self.board:
            if not field_status: #whether we print the fields
                alt += 1
                if alt % 2 == 0:
                    temp_row = list(row)
                    for x in range(self.fields):
                        temp_row[1 + 2*x] = ' '
                    print(form.format(*temp_row))
                else:
                    print(form.format(*row))

            else:
                print(form.format(*row))
        print("")

    def insert(self, input, xco, yco):
        if self.board[yco][xco] != '=':
            self.board[yco][xco] = input

    def field_on_board(self, x, y):
        return (x >= 0 and x <= self.fields - 1 and
                    y >= 0 and y <= self.fields - 1)

    def field_to_board_coords(self, x, y):
        return (1 + 2*x, 1 + 2*y)

    def set_field(self, x, y, char):
        x, y = self.field_to_board_coords(x, y)
        self.board[y][x] = char

    def clear_fields(self):
        temp_x, temp_y = 0, 0
        for x in range(self.fields):
            for y in range(self.fields):
                temp_x, temp_y = self.field_to_board_coords(x, y)
                self.board[temp_y][temp_x] = ' '

    def remove_walls(self):
        for x in range(self.fields):
            for y in range(self.fields):
                for i in range(2):
                    self.connections[x][y][i] = True

    def randomize(self, density):
        for x in range(self.fields):
            for y in range(self.fields):
                for i in range(2):
                    self.connections[x][y][i] = (rng.random() > density)

    def return_mutated(self, min, max):
        number_of_mutations = rng.randrange(min, max + 1)
        new_maze = Maze(self.fields)
        new_maze.connections = np.copy(self.connections)

        for dummy in range(number_of_mutations):
            x = rng.randrange(0, self.fields)
            y = rng.randrange(0, self.fields)
            dir = rng.randrange(0, 2)
            if new_maze.connections[x][y][dir]:
                new_maze.connections[x][y][dir] = False
            else:
                new_maze.connections[x][y][dir] = True
        return new_maze

    def get_connected_neighbours(self, x, y, connected):
        """
        Returns a list of coordinates of the connected neighbours of field (x,y) on the board if connected = True. Returns unconnected neighbours otherwise.
        """
        result = []
        if (self.field_on_board(x + 1, y) and self.connections[x][y][0] == connected):
            result.append((x + 1, y))
        if (self.field_on_board(x - 1, y) and self.connections[x - 1][y][0] == connected):
            result.append((x - 1, y))
        if (self.field_on_board(x, y + 1) and self.connections[x][y][1] == connected):
            result.append((x, y + 1))
        if (self.field_on_board(x, y - 1) and self.connections[x][y - 1][1] == connected):
            result.append((x, y - 1))
        return result

    def list_neighbours(self, x, y, connected):
        """
        Returns a list of coordinates of the connected neighbours of field (x,y) on the board or None in its stead otherwise.
        At the end we also add the amount of not None neighbours.
        """

        result = []
        possible_paths = 0
        if (self.field_on_board(x + 1, y) and self.connections[x][y][0] == connected):
            result.append((x + 1, y))
            possible_paths += 1
        else:
            result.append(None)
        if (self.field_on_board(x, y + 1) and self.connections[x][y][1] == connected):
            result.append((x, y + 1))
            possible_paths += 1
        else:
            result.append(None)
        if (self.field_on_board(x - 1, y) and self.connections[x - 1][y][0] == connected):
            result.append((x - 1, y))
            possible_paths += 1
        else:
            result.append(None)
        if (self.field_on_board(x, y - 1) and self.connections[x][y - 1][1] == connected):
            result.append((x, y - 1))
            possible_paths += 1
        else:
            result.append(None)
        return result, possible_paths

    def connect_neighbours(self, x, y, new_x, new_y):
        if x > new_x:
            self.connections[new_x][y][0] = True
        elif x < new_x:
            self.connections[x][y][0] = True
        elif y > new_y:
            self.connections[x][new_y][1] = True
        elif y < new_y:
            self.connections[x][y][1] = True

    def update_walls(self):
        """
        Updates all the walls in board according to connections
        """
        for index_x, row in enumerate(self.board):
            for index_y, col in enumerate(row):
                if index_x != 0 and index_x < len(self.board) - 1 and index_y != 0 and index_y < len(self.board) - 1:
                    if index_y % 2 == 0 and index_x % 2 == 0:
                        self.board[index_y][index_x] = '+'
                    elif index_y % 2 == 0:
                        x = (index_x - 1)//2 #xy for field above
                        y = (index_y - 1)//2
                        if self.connections[x][y][1] == False:
                            if index_y % 2 == 0:
                                self.board[index_y][index_x] = '-'
                            else:
                                self.board[index_y][index_x] = '|'
                        else:
                            self.board[index_y][index_x] = ' '
                    elif index_x % 2 == 0:
                        x = (index_x - 1)//2 #xy for field to the left
                        y = (index_y - 1)//2
                        if self.connections[x][y][0] == False:
                            if index_y % 2 == 0:
                                self.board[index_y][index_x] = '-'
                            else:
                                self.board[index_y][index_x] = '|'
                        else:
                            self.board[index_y][index_x] = ' '
