//Main.cpp
//主程序所在文件
#include "MapT.h"
#include "BoxMazeT.h"

const int R = 5;
const int L = 12;
const int N = 9;

typedef CMapT<R,L,N> CMap;
typedef CBoxMazeT<R,L,N> CMaze;
CMaze Maze;
CMap Map;


void main()
{
	using namespace std;
	int Xs[R*L] =
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,9,0,0,0,0,0,0,0,0,9,1,
		1,9,0,0,0,0,0,0,0,0,9,1,
		1,9,0,0,0,0,0,0,0,0,9,1,
		1,1,1,1,1,1,1,1,1,1,1,1


	};
	CPoint ptBoxs[N]	=
	{
		CPoint(3,3),CPoint(4,3),CPoint(5,3),
		CPoint(6,3),CPoint(7,3),CPoint(8,3),
		CPoint(10,2),CPoint(10,3),CPoint(11,3)
	};
	CPoint ptAims[N] =
	{
		CPoint(6,2),CPoint(7,2),CPoint(8,2),
		CPoint(6,3),CPoint(7,3),CPoint(8,3),
		CPoint(6,4),CPoint(7,4),CPoint(8,4)
	};
	CPoint ptStart = CPoint(11,3);
	Maze.Create(Xs,ptBoxs,ptStart,ptAims);
//	cout<<Maze.Try()<<endl;
	cout<<"1:向左推,2:向上推,3:向下推,4:向右推"<<endl;
	if( Maze.Try() )
	{
		Maze.Analyze();
		Maze.Print(cout,2);
	}
	

}