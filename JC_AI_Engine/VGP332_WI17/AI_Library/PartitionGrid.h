#pragma once
#include <X\Inc\XEngine.h>
#include <list>
namespace AI
{
	class Agent;
	class PartitionGrid
	{
	public:
		struct Cell
		{
			std::list<Agent*> agents;
		};

		void Initialize(uint32_t columns, uint32_t rows)
		{
			XASSERT(mCells.empty(), "[AI::PartitionGrid] Already initialized");
			mCells.resize(columns * rows);
			mColumns = columns;
			mRows = rows;
		}
		void ClearCells()
		{
			for (auto& cell : mCells)
			{
				cell.agents.clear();
			}
		}

		Cell& GetCell(uint32_t column, uint32_t row)
		{
			XASSERT(column < mColumns && row < mRows, "[AI::PartitionGrid] Invalid parameter");
			return mCells[column + (row * mColumns)];
		}

		uint32_t GetRows() { return mRows; }
		uint32_t GetColumns() { return mColumns; }
	private:
		std::vector<Cell> mCells;
		uint32_t mColumns;
		uint32_t mRows;
	};
}