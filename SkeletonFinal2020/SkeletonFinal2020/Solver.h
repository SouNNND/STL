#pragma once

#include "State.h"
#include "io.h"

#include <iostream>
#include <set>
#include <queue>
#include <algorithm>
#include <unordered_set>

template <class State_t>
class Heuristics {
private:
    static std::pair<size_t, size_t> getPositions(size_t index, size_t size) {
        return { index / size, index % size };
    }
public:
    static size_t GetManhattanDistance(const State_t& state) {
        auto solvedPuzzle = State_t::GoalState().GetData();
        size_t idx = 0;
        size_t sum = 0;

        for (auto element : state.GetData()) {
            auto solvedIndex = std::distance(solvedPuzzle.begin(), std::find(solvedPuzzle.begin(), solvedPuzzle.end(), element));

            std::pair<size_t, size_t> myPositions = getPositions(idx, State_t::Dimension);
            std::pair<size_t, size_t> solvedPositions = getPositions(solvedIndex, State_t::Dimension);
            
            idx++;
            sum += std::abs(static_cast<int>(myPositions.first - solvedPositions.first));
            sum += std::abs(static_cast<int>(myPositions.second - solvedPositions.second));
        }   

        return sum;
    }

    static size_t GetLinearConflictions(const State_t& state) {
        //itislinearconflict
        //how many conflicts are in rows
        //for (iteram pe linie)
        //how many conflicts are in columns
        // for(iteram pe column)
        // return nrRowConflict + nrColumnConflict
    }

/*public:
    static size_t Difference(size_t first, size_t second)
    {
        return (first > second) ? first - second : second - first;
    }

    static size_t GetManhattanDistance(const State_t& state)
    {
        static auto getManhattanDistanceForTile = [](size_t goal, size_t actual)
        {
            return Difference(goal / State_t::Dimension, actual / State_t::Dimension) +
                Difference(goal % State_t::Dimension, actual % State_t::Dimension);
        };

        size_t distance = 0u;
        size_t index = 0;
        for (auto it = state.GetData().begin(); it != state.GetData().end(); ++it, ++index)
        {
            if (*it != 0)
                distance += getManhattanDistanceForTile(index, *it - 1);
        }

        return distance;
    }*/
};

class Solver
{
public:
    template <class State_t>
    static size_t ComputeHash(const State_t& state)
    {
        static const std::hash<uint8_t> hasher;
        size_t seed = 0u;
        for (auto&& piece : state.GetData())
        {
            seed ^= hasher(piece) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    template <class State_t>
    static Moves SolveBFS(const State_t& initialState)
    {
        std::cout << "Solving..." << std::endl << initialState;

        Validate(initialState);
        if (initialState.IsGoalState()) return {}; // no moves required. Initial state is goal state.

        using Node = std::pair<State_t, Moves>;

		//TODO: define OPEN SET correctly
        auto greaterComparator = [](const Node& a, const Node& b) -> bool
        {
            auto& [state1, moves1] = a;
            auto& [state2, moves2] = b;

            auto cost1 = Heuristics<State_t>::GetManhattanDistance(state1) + moves1.size();
            auto cost2 = Heuristics<State_t>::GetManhattanDistance(state2) + moves2.size();

            return cost1 > cost2;
        };

        /*auto priorityComparator = [](const Node& a, const Node& b) -> bool
        {
            return Heuristics<State_t>::GetManhattanDistance(a.first) > Heuristics<State_t>::GetManhattanDistance(b.first);
        };*/

		std::priority_queue<Node, std::vector<Node>, decltype(greaterComparator)> 
            openSet(greaterComparator);

        openSet.push({ initialState, {} });

		//TODO: define CLOSED SET correctly
        auto EqualityCmp = [](const State_t& first, const State_t& second) -> bool
        {
            return first.GetData() == second.GetData();
        };
		std::unordered_set<State_t, std::function<size_t(const State_t&)>, decltype(EqualityCmp)>
            closedSet(8u, &ComputeHash<State_t>, EqualityCmp);

        while (!openSet.empty())
        {
			// TODO: Do it nicer , who is first? second? structure binding
            auto currentNode = openSet.top();
            auto&& [currentState, currentMoves] = currentNode;
            /*auto&& currentState = currentNode.first;
            auto&& currentMoves = currentNode.second;*/
            openSet.pop();

            // some logging
            static size_t maxDepth = 0;
            if (currentMoves.size() > maxDepth)
            {
                maxDepth = currentMoves.size();
                std::cout << "Max Depth: " << maxDepth << std::endl;
            }
            // end logging

            if (currentState.IsGoalState())
            {
                std::cout << "Visited: " << closedSet.size() << std::endl;                
                return currentMoves; // leaving cycle statement
            }

            closedSet.insert(currentState);

            for (auto&& childMovePair : currentState.GetChildren())
            {
				//TODO: Do it nicer - who is first? second? 
                auto&& [childState, move] = childMovePair;
				if(closedSet.find( childState) == closedSet.end())
                {
                    Moves childMoves = currentMoves;
                    childMoves.push_back(move);
                    openSet.push({ std::move(childState), std::move(childMoves) });
                }
            }
        }

        throw std::runtime_error("Couldn't solve");
    }

private:
	//musthaveTODO implement Validate correctly
    template <class State_t>
    static void Validate(const State_t& state)
    {
        if (!state.IsValid())
            throw std::runtime_error("state not valid");
        if (!state.IsSolvable())
            throw std::runtime_error("state not solvable");

    }
    
};
