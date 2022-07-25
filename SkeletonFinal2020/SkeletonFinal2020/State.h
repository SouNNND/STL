#pragma once

#include "types.h"
#include <array>
#include <optional>
#include <map>
#include <numeric>
#include <functional>
#include <algorithm>
#include <iostream>

template<size_t N = 3>
class State
{
public: // Types
    using Position2D = std::pair<size_t, size_t>;
    using ElementType = uint8_t;
	using Data = std::array<ElementType, N * N>; // TODO

private: // members
    Data m_data;

public:
    static const size_t Dimension = N;    

    State() = delete;
    State(Data data) : m_data{ std::move(data) } {};

    const Data& GetData() const
    {
        return m_data;
    }

    static const State GoalState()
    {
        // TODO: Refactor with STL        
        Data goalData;
        size_t idx = 0;
        /*for (auto element : goalData)
        {
            elem = ++idx;
        }*/
        std::iota(goalData.begin(), goalData.end(), 1);
        /*for (auto idx = 0u; idx < goalData.size(); ++idx)
        {
            goalData[idx] = idx+1;
        }*/

        goalData.back() = 0;        
        return State(goalData);
    }

    bool IsGoalState() const
    {
        return m_data == GoalState().m_data;
    }

    bool IsValid() const
    {
        /*Data sortedData = m_data;
        //std::copy(m_data.begin(), m_data.end(), sortedData.begin()); -> my solution

        std::sort(m_data.begin(), m_data.end());

        Data validSortedData;
        std::iota(validSortedData.begin(), validSortedData.end(), 0);

        return sortedData == validSortedData;*/

        return std::is_permutation(m_data.begin(), m_data.end(), GoalState().GoalData().begin());
    }

    size_t countInversions(typename Data::iterator begin, typename Data::iterator end) const
    {
        size_t acc{ 0u };
        for (auto it = begin; it != end; ++it)
        {
            auto&& current = *it;
            if (current != 0)
                acc += std::count_if(it, end, [current](auto next) { return next != 0 && next < current; });
        }

        return acc;
    };

    bool IsSolvable() const
    {		
        const auto inversionsCount = countInversions(m_data.begin(), m_data.end());
        const auto isInversionCountEven = inversionsCount % 2 == 0;
        const bool isNOdd = N % 2 == 1;
        const bool isBlankRowEven = GetBlankPosition2D().first % 2 == 0;

        return (isNOdd) ? isInversionCountEven :
            (isBlankRowEven) ? !isInversionCountEven :
            isInversionCountEven;
    }

    std::vector<std::pair<State, MoveDirection>> GetChildren() const
    {
        /*auto child1 = MoveLeft();
        auto child2 = MoveRight();
        auto child3 = MoveUp();
        auto child4 = MoveDown();

        std::vector<std::pair<State, MoveDirection>> children;
        if (child1) children.emplace_back(*child1, MoveDirection::LEFT);
        if (child2) children.emplace_back(*child2, MoveDirection::RIGHT);
        if (child3) children.emplace_back(*child3, MoveDirection::UP);
        if (child4) children.emplace_back(*child4, MoveDirection::DOWN);*/

        //TODO: See Move method and refactor the Move method
        std::vector<std::pair<State, MoveDirection>> children;
        Moves allmoves = {MoveDirection::DOWN, MoveDirection::LEFT, MoveDirection::RIGHT, MoveDirection::UP };
        for (auto direction : allmoves)
        {
            std::optional<State> child = Move(direction);
            if (child.has_value())
            {
                children.emplace_back(*child, direction);
            }
        }
        return children;
    }

private: // methods

    size_t GetBlankPosition() const
    {
        auto found = std::find(m_data.begin(), m_data.end(), 0);
        if (found != m_data.end())
            return std::distance(m_data.begin(), found);
        throw std::runtime_error("Unexpected");
    }

    Position2D GetBlankPosition2D() const
    {
        auto&& absolute = GetBlankPosition();
        return { absolute / N, absolute % N }; // structure binding instead of std::make_pair<>
    }

    // TODO: Perform the move if possible and return the state. Returns std::nullopt otherwise.
    std::optional<State> Move(MoveDirection direction) const
    {
        switch (direction)
        {
        case MoveDirection::LEFT:   return MoveLeft();
        case MoveDirection::UP:     return MoveUp();
        case MoveDirection::RIGHT:  return MoveRight();
        case MoveDirection::DOWN:   return MoveDown();
        default:                    throw std::runtime_error("Not implemented.");
        }

        /*static const std::map<MoveDirection, std::function<std::optional<State>(const State&)>> moveByDirection {
            {MoveDirection::LEFT, std::mem_fn(&State::MoveLeft)},
            {MoveDirection::RIGHT, std::mem_fn(&State::MoveRight)},
            {MoveDirection::UP, std::mem_fn(&State::MoveUp)},
            {MoveDirection::DOWN, std::mem_fn(&State::MoveDown)},
        };

        if (std::find(moveByDirection.begin(), moveByDirection.end(), direction) != moveByDirection.end()) {
            return moveByDirection.at(direction)(*this);
        }
        else {
            return std::nullopt;
        }*/

    }

    static State SwapTiles(const State& state, size_t firstPos, size_t secondPos)
    {
        auto childData = state.GetData();
        std::swap(childData[firstPos], childData[secondPos]);
        return { std::move(childData) };
    }

    std::optional<State> MoveRight() const
    {
        if (GetBlankPosition2D().second == 0) return std::nullopt;

        auto blankPosition = GetBlankPosition();
        return SwapTiles(*this, blankPosition, blankPosition - 1);
    }

    std::optional<State> MoveLeft() const
    {
        if (GetBlankPosition2D().second == N - 1) return std::nullopt;

        auto blankPosition = GetBlankPosition();
        return SwapTiles(*this, blankPosition, blankPosition + 1);
    }

    std::optional<State> MoveDown() const
    {
        if (GetBlankPosition2D().first == 0) return std::nullopt;

        auto blankPosition = GetBlankPosition();
        return SwapTiles(*this, blankPosition, blankPosition - N);
    }

    std::optional<State> MoveUp() const
    {
        if (GetBlankPosition2D().first == N - 1) return std::nullopt;

        auto blankPosition = GetBlankPosition();
        return SwapTiles(*this, blankPosition, blankPosition + N);
    }

};

std::ostream& operator<< (std::ostream& os, MoveDirection dir)
{
    static const std::map<MoveDirection, const char*> namesMap
    {
        { MoveDirection::LEFT, "l" },
        { MoveDirection::RIGHT, "r" },
        { MoveDirection::UP, "u" },
        { MoveDirection::DOWN, "d" },
    };

    os << namesMap.at(dir);
    return os;
}

template <size_t N>
std::ostream& operator<< (std::ostream& os, const State<N>& state)
{
    os << std::endl;
    for (size_t index = 0; index < state.GetData().size(); ++index)
    {
        os << static_cast<unsigned int>(state.GetData()[index]) << ' ';

        if (index % State<>::Dimension == State<>::Dimension - 1) os << std::endl;
    }
    os << std::endl;

    return os;
}

using State3X3 = State<3>;
using State4X4 = State<4>;
