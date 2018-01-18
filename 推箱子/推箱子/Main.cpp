//Main.cpp
//主程序所在文件
#include "MapT.h"
#include "BoxMazeT.h"

const int R = 9;
const int L = 9;
const int N = 4;

typedef CMapT<R,L,N> CMap;
typedef CBoxMazeT<R,L,N> CMaze;
CMaze Maze;
CMap Map;


void main()
{
	using namespace std;
	int Xs[R*L] =
	{

		0,0,1,1,1,1,1,0,0,
		0,1,1,9,9,9,1,1,1,
		0,1,9,0,0,0,0,9,1,
		0,1,0,1,0,1,0,9,1,
		0,1,0,1,0,0,0,1,1,
		1,1,0,1,0,1,0,1,0,
		1,9,0,0,0,0,0,1,0,
		1,9,9,9,1,9,9,1,0,
		1,1,1,1,1,1,1,1,0

	};
	CPoint ptBoxs[N]	=
	{
		CPoint(3,4),CPoint(5,4),CPoint(7,4),CPoint(6,5)
	};
	CPoint ptAims[N] =
	{
		CPoint(3,4),CPoint(5,4),CPoint(7,4),CPoint(7,6)
	};
	CPoint ptStart = CPoint(5,5);
	Maze.Create(Xs,ptBoxs,ptStart,ptAims);
	if( Maze.Try() )
	{
		Maze.Analyze();
		Maze.Print(cout,2);
	}


}