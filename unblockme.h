#ifndef _UNBLOCKME_H_
#define _UNBLOCKME_H_

#include <map>
#include <set>

using namespace std ;


enum Result
{
	RET_OK,
	RET_FAIL,
	RET_EXIST,
	RET_NOTHING,
	RET_MASTER
};

enum Direct
{
	eDirect_None,
	eDirect_Left = 1,
	eDirect_Up,
	eDirect_Right,
	eDirect_Down
};

#define ARRAY_ROW 	6
#define ARRAY_COL 	6

// 	<depth, index>
typedef pair<int, int> 	MapKey ;
typedef unsigned char (Block)[ARRAY_ROW][ARRAY_COL] ;

struct BlockforSet
{
	static int row ;
	static int col ;

	Block block ;
};

struct BlockCompare
{
	bool operator() (const BlockforSet& lhs, const BlockforSet& rhs) const
	{
		for(int ii = 0; ii < lhs.row; ii++)
		{
			for(int jj = 0; jj < lhs.col; jj++)
			{
				if( lhs.block[ii][jj] < rhs.block[ii][jj])
					return 1 ;
				else if( lhs.block[ii][jj] > rhs.block[ii][jj])
					return 0 ;
			}
		}
		return 0 ;
	}
};


struct Node
{
	Block block ;
	int movedBlockNumber ;
	Direct direct ;

	map<MapKey, Node>::iterator 	parentIter ;

	int parentIndex ;
	int firstChildIndex ;
	int lastChildIndex ;
	int flagOK ;

};

typedef map<MapKey, Node> 		MapNode ;
typedef map<MapKey, Node>::iterator 	MapNodeIter ;

typedef	set<BlockforSet, BlockCompare> 			SetBlock ;
typedef	set<BlockforSet, BlockCompare>::iterator 	SetBlockIter ;

Result UBM_CheckMove(int blockNumber, int masterNumber, Direct direct, const Block* pBlock, Block* pNewBlock) ;
Result UBM_CheckExist(SetBlock* setBlock, Block* pBlock) ;

#endif
