import random as rng
import numpy as np
import time
import Maze


x, y = 0, 0
PRINTING_STEPS = True

#Taxing memory more
#wildly unpractical for purpose, expensive, just fun
#inspired by AI learning to play games

#distance is 4 ookal is er pad
#in score functie, niet bereikte uitrekenen:
#   self.fields**2 - len(visited)


def distance_to_end(maze):
    searched = []
    to_search = [(0, 0)]
    shortest_distance = (maze.fields - 1) * 2

    while len(to_search) > 0:
        x, y = to_search.pop(0)

        distance = abs(x - maze.fields - 1) + abs(y - maze.fields - 1) #manhattan distance
        if distance < shortest_distance:
            shortest_distance = distance
            if distance == 0:
                return distance

        connected_neighbours = maze.get_connected_neighbours(x, y, True)
        for neighbour in connected_neighbours:
            if(neighbour not in searched and neighbour not in to_search):
                to_search.append(neighbour)

        searched.append((x, y))
    return shortest_distance

WEIGHT_DISTANCE = 3
UNUSED_FIELDS_PENALTY = 1

def score_function(maze):
    sum = 0

    distance = distance_to_end(maze)
    sum -= distance * WEIGHT_DISTANCE

    for x in range(maze.fields):
        for y in range(maze.fields):
            connected_neighbours = maze.get_connected_neighbours(x, y, True)
            number_of_connected_neighbours = len(connected_neighbours)
            if number_of_connected_neighbours == 0 or number_of_connected_neighbours == 4:
                sum -= UNUSED_FIELDS_PENALTY
    return sum

GENERATIONS = 15

def create_maze_evolutionary(size, begin_density):
    instances = []
    maze = Maze.Maze(size)

    for dummy in range(150):
        maze = Maze.Maze(size)
        maze.randomize(begin_density)
        instances.append(maze)

    scored_instances = []

    for dummy in range(GENERATIONS):
        scored_instances = []
        for maze in instances:
            if maze == None:
                raise
            scored_instances.append((maze, score_function(maze)))

        scored_instances.sort(key = lambda pair: pair[1], reverse = True)

        current_maze, score = scored_instances[0]
        current_maze.print(False)
        print(score)

        # 10 * (14 + 1) = 150
        # 1 < 4
        instances = []
        for i in range(10):
            high_scoring_maze = scored_instances[i][0]
            instances.append(high_scoring_maze)
            for j in range(14):
                new_maze = high_scoring_maze.return_mutated(3, 6)
                instances.append(new_maze)

        print(len(instances))

    return scored_instances[0][0]

maze = create_maze_evolutionary(10, 0.7)
maze.print(False)
print(score_function(maze))
print(distance_to_end(maze))
