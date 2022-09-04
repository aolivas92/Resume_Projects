# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    with open(sys.argv[1]) as file:
        # Read every team from the file
        reader = csv.DictReader(file)
        # add every team into the list as it's own dict value
        for row in reader:
            teams.append({'team': row['team'], 'rating': int(row['rating'])})

    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    # Add all the teams to the counts dict and give them 0 wins to start with
    for i in range(len(teams)):
        # get the name of each team
        counts[teams[i]['team']] = 0

    # call the simulate tournament function and add the wins to each team that is returned
    for i in range(N):
        winner = simulate_tournament(teams)
        counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    # Keep simulating each round until there is only one team left
    while len(teams) > 1:
        teams = simulate_round(teams)

    # Return the last team left
    return teams[0]['team']


if __name__ == "__main__":
    main()
