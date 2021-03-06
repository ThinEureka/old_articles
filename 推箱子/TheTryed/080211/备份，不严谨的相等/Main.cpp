//Main.cpp
//主程序所在文件
#include "MapT.h"
#include "BoxMazeT.h"

const int R = 7;
const int L = 8;
const int N = 6;

typedef CMapT<R,L,N> CMap;
typedef CBoxMazeT<R,L,N> CMaze;
CMaze Maze;
CMap Map;


void main()
{
	using namespace std;
	int Xs[R*L] =
	{
		1,1,1,1,1,1,1,1,
		1,0,0,1,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,1,
		1,0,0,0,0,0,0,1,
		1,0,0,1,0,0,0,1,
		1,1,1,1,1,1,1,1

	};
	CPoint ptBoxs[N]	=
	{
		CPoint(3,3),CPoint(6,3),CPoint(3,4),CPoint(5,4),CPoint(3,5),CPoint(6,5)
	};
	CPoint ptAims[N] =
	{
		CPoint(4,3),CPoint(5,3),CPoint(4,4),CPoint(5,4),CPoint(4,5),CPoint(5,5)
	};
	CPoint ptStart = CPoint(2,4);
	Maze.Create(Xs,ptBoxs,ptStart,ptAims);
//	cout<<Maze.Try()<<endl;
	cout<<"1:向左推,2:向上推,3:向下推,4:向右推"<<endl;
	if( Maze.Try() )
	{
		Maze.Analyze();
		Maze.Print(cout,2);
	}
	

}