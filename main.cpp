#include <iostream>  // std::cout
#include <algorithm> // std::shuffle
#include <array>     // std::array
#include <random>    // std::default_random_engine
#include <map>       // std::map
#include <chrono>    // std::chrono::system_clock

#define us unsigned short

const us PLAYERS = 8;
const us REVOLUTION = 5;
const unsigned GAMES = 10000000;

/**
 * Create a sorted pack of cards with enough cards for all players, ignoring kind.
 *
 * @param players Number of players in the game.
 * @return Array of unsigned shorts with value [0,12].
 */
std::array<us, 13 * PLAYERS> makePack()
{
    std::array<us, 13 * PLAYERS> pack;
    for (int i = 0; i < 13; i++)
        for (int y = 0; y < PLAYERS; y++)
            pack[y + PLAYERS * i] = i;
    return pack;
}

/**
 * Distiutes shuffled cards amongst players, similar to in real life.
 *
 * @param pack Array of unsigned shorts with value [0,12]
 * @return Array of arrays unsigned shorts with value [0,12] of length 13.
 */
std::array<std::array<us, 13>, PLAYERS> distributeCards(const std::array<us, 13 * PLAYERS> &pack)
{
    std::array<std::array<us, 13>, PLAYERS> hands;
    for (int i = 0; i < PLAYERS; i++)
        for (int y = 0; y < 13; y++)
            hands[i][y] = pack[PLAYERS * y + i];
    return hands;
}

/**
 * Counts how many revolution occurs in a specific game.
 *
 * @param counts Array of maps of player card count.
 * @return Unsigned short, contains number of revolutions.
 */
bool checkRevolution(std::array<std::map<us, us>, PLAYERS> &counts)
{
    for (std::map<us, us> &count : counts)
        for (us i = 0; i < 13; i++)
            if (count[i] >= REVOLUTION)
                return true;
    return false;
}

/**
 * Simulates a singular game.
 *
 * @param seed Seed for the random array shuffler.
 * @return Unsigned short, contains number of revolutions.
 */
unsigned simulateGames(unsigned &seed)
{
    unsigned revolutions;
    // Generate sorted pack
    std::array<us, 13 *PLAYERS> pack = makePack();

    for (unsigned game = 0; game < GAMES; game++)
    {
        // Shuffle pack
        std::shuffle(pack.begin(), pack.end(), std::default_random_engine(seed));

        // Distribute cards
        std::array<std::array<us, 13>, PLAYERS> hands = distributeCards(pack);

        // Count occurences of each card in each hand individually
        std::array<std::map<us, us>, PLAYERS> counts;
        for (us i = 0; i < PLAYERS; i++)
            for (us &card : hands[i])
                counts[i][card]++;
        revolutions += checkRevolution(counts);
        if (!(game % 10000))
            std::cout << "\nGame: " << game << " temporary chance: " << (long double)revolutions / (long double)game;
    }
    // Return revolution counts
    return revolutions;
}

int main()
{
    // simulate GAMES games and counts revolutions as 0, 1, 2 or more revolutions
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    unsigned revolutions = simulateGames(seed);

    // show results
    std::cout << "\nNumber of players: " << PLAYERS;
    std::cout << "\nNumber of identical cards for a revolution: " << REVOLUTION;
    std::cout << "\n\nResults:\n\n";
    std::cout << "Revolutions: " << revolutions << " (" << (long double)revolutions / (long double)GAMES << ")\n";
    std::cout << "Total games: " << GAMES;
    return 0;
}