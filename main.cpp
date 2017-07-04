#include <cstdio>
#include <cstring>

#include "unblockme.h"

using namespace std ;

#define MASTER_BLOCK 	4

#if 0
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0}
#endif

Block block = 
{
{2, 2, 3, 5, 9, 11},
{0, 0, 3, 5, 9, 11},
{4, 4, 3, 7, 0, 11},
{0, 1, 0, 7, 0, 0},
{0, 1, 6, 6, 0, 0},
{0, 0, 0, 0, 0, 0}
};

int BlockforSet::row = ARRAY_ROW ;
int BlockforSet::col = ARRAY_COL ;

int main()
{ 
	Block newBlock ;
	BlockforSet 	blockforSet ;

	set<int> 		setIndex ;
	set<int>::iterator 	setIndexIter ;

	MapNode 		mapNode ;
	MapNodeIter 		mapNodeIter ;

	SetBlock  		setBlock ;
	SetBlockIter 		setBlockIter ;

	int blockNumber ;
	Result ret ;
	Node node ;
	MapKey key ;

	int curDepth = 1 ;
	int curIndex = 1 ;
	int pushIndex = 1 ;

	memset(&node, 0x00, sizeof(Node)) ;
	memcpy(&node.block, block, sizeof(block)) ;

	key = make_pair(curDepth, curIndex) ;
	mapNode.insert(make_pair(key, node)) ;

	for(int ii = 0; ii < 36; ii++)
		setIndex.insert(block[ii / 6][ii % 6]) ;
	setIndex.erase(0) ;
	
	Direct direct ;
	char chd ;
	setIndexIter = setIndex.begin() ;

	key = make_pair(curDepth, curIndex) ;
	mapNodeIter = mapNode.begin() ;

	while(1)
	{
		blockNumber = *setIndexIter ;

		for(int dd = 0; dd < 4; dd++)
		{
			direct = (dd == 0) ? eDirect_Left : direct ;
			direct = (dd == 1) ? eDirect_Up : direct ;
			direct = (dd == 2) ? eDirect_Right : direct ;
			direct = (dd == 3) ? eDirect_Down : direct ;

			ret = UBM_CheckMove(blockNumber, MASTER_BLOCK, direct, &block, &newBlock) ;
			
			if(ret == RET_OK)
			{
				ret = UBM_CheckExist(&setBlock, &newBlock) ;

				if(ret == RET_EXIST)
					continue ;

				memcpy(&node.block, &newBlock, sizeof(Block)) ;
				node.parentIndex = curIndex ;
				node.parentIter = mapNodeIter ;
				node.direct = direct ;
				node.firstChildIndex = 0 ;
				node.lastChildIndex = 0 ;
				node.flagOK = 0 ;
				node.movedBlockNumber = blockNumber ;

				if(mapNodeIter->second.firstChildIndex == 0)
					mapNodeIter->second.firstChildIndex = pushIndex ;

				mapNodeIter->second.lastChildIndex = pushIndex ;

				key = make_pair(curDepth + 1, pushIndex) ;
				mapNode.insert(make_pair(key, node)) ;

				memcpy(&blockforSet.block, &newBlock, sizeof(Block)) ;
				setBlock.insert(blockforSet) ;

				pushIndex++ ;
			}
			else if(ret == RET_MASTER)
			{
				printf("GOOD JOB~~!!\n") ;
				printf("node Count : %d\n", mapNode.size()) ;
				printf("setBlock Size : %d\n\n", setBlock.size()) ;

				int count = 0 ;

				while(1)
				{
					mapNodeIter->second.flagOK = 1 ;

					if(mapNodeIter->first.first == 1)
						break ;

					mapNodeIter = mapNodeIter->second.parentIter ;
				}

				printf("R = Right\n") ;
				printf("L = Left\n") ;
				printf("U = Up\n") ;
				printf("D = Down\n\n") ;

				for(int ii = 0; ii < 6; ii++)
				{
					for(int jj = 0; jj < 6; jj++)
					{
						if(mapNodeIter->second.block[ii][jj])
							printf("%2d ", mapNodeIter->second.block[ii][jj]) ;
						else
							printf(" . ") ;
					}
					printf("\n") ;
				}
				printf("\n") ;
				
				mapNodeIter = mapNode.begin() ;

				for(; mapNodeIter != mapNode.end(); mapNodeIter++)
				{
					if(mapNodeIter->second.flagOK)
					{
						count++ ;

						if(mapNodeIter == mapNode.begin())
							continue ;
						
						direct = mapNodeIter->second.direct ;
						chd = (direct == eDirect_Left) 	? 'L' : chd ;
						chd = (direct == eDirect_Up) 	? 'U' : chd ;
						chd = (direct == eDirect_Right) ? 'R' : chd ;
						chd = (direct == eDirect_Down) 	? 'D' : chd ;

						printf("[%d.%c]", mapNodeIter->second.movedBlockNumber, chd) ;

						if(count % 3 == 1)
							printf("\n\n") ;
						else
							printf(" ->") ;
					}
				}
				printf("\nTotal Count : %d\n", count) ;
				return 0 ;
			}
		}

		///////////////////////////
		setIndexIter++ ;
		if(setIndexIter == setIndex.end())
		{
			curIndex++ ;

			key = make_pair(curDepth, curIndex) ;
			mapNodeIter = mapNode.find(key) ;
			
			if(mapNodeIter == mapNode.end())
			{
				curDepth++ ;
				curIndex = 1 ;
				pushIndex = 1 ;

				key = make_pair(curDepth, curIndex) ;
				mapNodeIter = mapNode.find(key) ;

				if(mapNodeIter == mapNode.end())
				{
					printf("ERROR~~!! byebye~\n") ;

					mapNodeIter = mapNode.begin() ;
					return -1 ;
				}
			}

			memcpy(&block, &mapNodeIter->second.block, sizeof(Block)) ;
			setIndexIter = setIndex.begin() ;
		}
	}
	return 0 ;
}
