import random as rng
import numpy as np
import time
import Maze

visited = []

PRINTING_STEPS = True

SIZE_MAZE = 15


def create_maze_walker(size):
    maze = Maze.Maze(size)
    steps = self_avoiding_walk(maze, (0, 0))
    print(f"We're done after {steps} random walks!")
    return maze

def self_avoiding_walk(maze, starting_point):
    current_node = starting_point

    if PRINTING_STEPS:
        for node in visited:
            maze.set_field(*node, 'v')

    while current_node:
        visited.append(current_node)
        maze.set_field(*current_node, 'n')

        disconnected_neighbours = maze.get_connected_neighbours(*current_node, False)

        next_node = None
        if len(disconnected_neighbours) > 0:
            rng.shuffle(disconnected_neighbours)
            for neighbour in disconnected_neighbours:
                if neighbour not in visited:
                    next_node = neighbour

        if next_node:
            maze.connect_neighbours(*current_node, *next_node)
            current_node = next_node
        else:
            maze.set_field(*current_node, 'e')
            current_node = None

    if PRINTING_STEPS:
        maze.print(True)

    if len(visited) >= maze.fields**2:
        return 0

    rng.shuffle(visited)
    for node in visited:
        disconnected_neighbours = maze.get_connected_neighbours(*node, False)
        for neighbour in disconnected_neighbours:
            if neighbour not in visited:
                maze.connect_neighbours(*node, *neighbour)
                return 1 + self_avoiding_walk(maze, neighbour)

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    maze = create_maze_walker(SIZE_MAZE)
    print("The resulting maze:\n")
    maze.print(False)
    input("Press any button to quit.")
