#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "unblockme.h"

Result UBM_CheckMove(int blockNumber, int masterNumber, Direct direct, const Block* pBlock, Block* pNewBlock)
{
	int firstRow = -1, firstCol, lastRow= -1, lastCol ;
	int firstMasterRow, firstMasterCol, lastMasterRow, lastMasterCol ;
	int flagFirst = 0 ;
	int flagMasterFirst = 0 ;

	int flagVertical ;

	for(int ii = 0; ii < 36; ii++)
	{
		if((*pBlock)[ii / 6][ii % 6] == blockNumber)
		{
			if(flagFirst == 0)
			{
				flagFirst = 1 ;
				firstRow = ii / 6 ;
				firstCol = ii % 6 ;
			}
			else
			{
				lastRow = ii / 6 ;
				lastCol = ii % 6 ;
			}
		}

		if((*pBlock)[ii / 6][ii % 6] == masterNumber)
		{
			if(flagMasterFirst == 0)
			{
				flagMasterFirst = 1 ;
				firstMasterRow = ii / 6 ;
				firstMasterCol = ii % 6;
			}
			else
			{
				lastMasterRow = ii / 6 ;
				lastMasterCol = ii % 6 ;
			}
		}
	}

	if(firstRow == -1 || lastRow == -1)
	{
		printf("firstRow : %d\n", firstRow) ;
		printf("lastRow : %d\n", lastRow) ;
		exit(1) ;
	}

	flagVertical = (firstRow == lastRow) ? 0 : 1 ;

	int ret = 0 ;

	ret  = flagVertical && ((direct == eDirect_Left) || (direct == eDirect_Right)) ;
	ret |= flagVertical && (direct == eDirect_Up) && (firstRow == 0) ;
	ret |= flagVertical && (direct == eDirect_Down) && (lastRow == 5) ;

	ret |= !flagVertical && ((direct == eDirect_Up) || (direct == eDirect_Down)) ;
	ret |= !flagVertical && (direct == eDirect_Left) && (firstCol == 0) ;
	ret |= !flagVertical && (direct == eDirect_Right) && (lastCol == 5) ;

	if(ret)
		return RET_FAIL ;

	ret |= (direct == eDirect_Left) && ((*pBlock)[firstRow][firstCol-1] != 0) ;
	ret |= (direct == eDirect_Right) && ((*pBlock)[firstRow][lastCol+1] != 0) ;
	ret |= (direct == eDirect_Up) && ((*pBlock)[firstRow-1][firstCol] != 0) ;
	ret |= (direct == eDirect_Down) && ((*pBlock)[lastRow+1][firstCol] != 0) ;

	if(ret)
		return RET_FAIL ;

	memcpy(pNewBlock, pBlock, sizeof(Block)) ;

	switch(direct)
	{
	case eDirect_Left :
		(*pNewBlock)[firstRow][firstCol-1] = (*pNewBlock)[firstRow][firstCol] ;
		(*pNewBlock)[firstRow][lastCol] = 0 ;
		break ;
	case eDirect_Up :
		(*pNewBlock)[firstRow-1][firstCol] = (*pNewBlock)[firstRow][firstCol] ;
		(*pNewBlock)[lastRow][firstCol] = 0 ;
		break ;
	case eDirect_Right :
		(*pNewBlock)[firstRow][lastCol+1] = (*pNewBlock)[firstRow][lastCol] ;
		(*pNewBlock)[firstRow][firstCol] = 0 ;
		break ;
	case eDirect_Down :
		(*pNewBlock)[lastRow+1][firstCol] = (*pNewBlock)[lastRow][firstCol] ;
		(*pNewBlock)[firstRow][firstCol] = 0 ;
		break ;
	default :
		break ;
	}

	if((direct == eDirect_Left) || (direct == eDirect_Right))
		return RET_OK ;

	ret = 0 ;
	for(int ii = lastMasterCol + 1; ii < 6; ii++)
		ret += (*pNewBlock)[firstMasterRow][ii] ;

	if(ret == 0)
		return RET_MASTER ;

	return RET_OK ;
}

Result UBM_CheckExist(SetBlock* setBlock, Block* pBlock)
{
	SetBlockIter 	setBlockIter ;
	BlockforSet 	blockforSet ;

	memcpy(&blockforSet.block, pBlock, sizeof(Block)) ;
	setBlockIter = setBlock->find(blockforSet) ;

	if(setBlockIter != setBlock->end())
		return RET_EXIST ;

	return RET_NOTHING ;
}

