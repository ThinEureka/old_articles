#include<fstream>
#include<iostream>
#include<string>
#include<math.h>

using namespace std; const int width =20; const int height = 9; const int I1 =5; const int I2 =12; const int I3 =13; const int I4 =134; const int I5 =135; const int I6 =520; const int N =521; const int T= I4 -I1 +N +2 +I6 -I5; int Bars [4] [21] 
 ={{ -503, 1503, -402, 702, 1402, 1702, -302, 802, 1302, 1802, -202, 902, 1202, 1902, -102, 1003, 2002}, {-304, -202, 602, -102, 702, -2, -102, -202, -303, -502, -602, -2, 702, -102, 602,  -202, 502, -303}, {-12, -12, -502, -502, -502, -502, 
-502, -502, -502, -502}, {-404, -302, 702, -202, 802, -102, 902, -2, -1, -1, -1, -2, -102, -202, -302, -402, 902, -502, 802, -603 }}; string S1 [N] ={ "3", "M.cpp", "S.cpp", "T.cpp", "C.cpp", "#include", "<fstream>", "#include", "<iostream>", 
"#include", "<string>", "#include", "<math.h>", "using namespace", "std;", "const", "int", "width", "=20;", "const", "int", "height", "= 9;", "const", "int", "I1", "=5;", "const", "int", "I2", "=12;", "const", "int", "I3", "=13;", "const", 
"int", "I4", "=134;", "const", "int", "I5", "=135;", "const", "int", "I6", "=520;", "const", "int", "N", "=521;", "const", "int", "T=", "I4", "-I1", "+N", "+2", "+I6", "-I5;", "int", "Bars", "[4]", "[21]", " ={{", "-503,", "1503,", "-402,", 
"702,", "1402,", "1702,", "-302,", "802,", "1302,", "1802,", "-202,", "902,", "1202,", "1902,", "-102,", "1003,", "2002},", "{-304,", "-202,", "602,", "-102,", "702,", "-2,", "-102,", "-202,", "-303,", "-502,", "-602,", "-2,", "702,", "-102,", 
"602,", " -202,", "502,", "-303},", "{-12,", "-12,", "-502,", "-502,", "-502,", "-502,", "-502,", "-502,", "-502,", "-502},", "{-404,", "-302,", "702,", "-202,", "802,", "-102,", "902,", "-2,", "-1,", "-1,", "-1,", "-2,", "-102,", "-202,", 
"-302,", "-402,", "902,", "-502,", "802,", "-603", "}};", "string", "S1", "[N]", "={", "};", "string", "S2", "[T];", "class", "MSTC", "{int", "flag;", "int", "row;", "int", "pos;", "int", "sID;", "int", "bID;", "int", "hID;", "int", "*pB;", 
"ofstream", "*pF;", "public:", "MSTC", "(){", "CS", "();", "CF", "();", "pos", "=0;", "row", "=-1;", "sID", "=I3-", "I1;", "bID", "=-1;", "hID", "=0;", "pB =", "Bars", "[flag];", "Draw", "();", "};", "void", "CF", "(){", "int", "i=", "flag", 
"+1;", "pF", "=", "new", "ofstream", "( S1", "[i]", ".c_str", "());", "};", "void", "CS", "(){", "flag", "=S1", "[0]", ".at(0)", "-'0';for", "(int", "i=0;", "i<I4", "-I1+", "1;", "i++", "){", "S2", "[i]", "=S1", "[I1+i];}", "string ", 
"s0=S1[0];", "s0.at(0)", "=char(", "'0'+", "(flag+1)", "%4);", "S2[I4-I1", "+1]=", "(char)", "34+s0", "+(char)34;", "S2[I4-I1", "+1]+=',';", "for", "(int", "i=0;", "i<N", "-1;", "i++", "){", "S2[", "I4-", "I1+", "2+i]", "=", "(char)34", "+S1", 
"[1+i]+", "(char)34;", "if", "(i!=", "N-2", "){", "S2", "[I4", "-I1", "+2", "+i]", "+=", "',';", "}}", "for", "(int", "i=0;", "i<I6", "-I5", "+1;", "i++", "){", "S2", "[I4", "-I1", "+N", "+1", "+i]", "=S1", "[I5", "+i];", "}};", "int", "NBI", 
"(){", "if(", "pB[", "bID+", "1]>", " 0)", "return", "bID", "+1;", "else", "{if(", "row", "<", "height", "-1){", "return", "hID", ";}", "else{", "return", "bID", "+1", ";}}};", "string", "&s", "(){", "return", "S2", "[sID]", ";};", "int", 
"&bar", "(){", "return", "pB[", "bID", "];};", "void", "WS", "(){", "*pF", "<<", "s()+", " (char)32; ", "pos", "+=", "s()", ".length()", "+1;", "sID", "++;};", "void", "NL", "(){", "*pF", "<<", "endl;", "pos", "=0;", "};", "int", "BH", "(){", 
"int", "tmp=", "bar", "();", "return", "abs", "(tmp)", "/100", "*width", ";};", "int", "BL", "(){", "int", "tmp", "=bar", "();", "return", "(abs(", "tmp)", "%100)", "*width", ";};", "int", "BT", "(){", "return", "BH", "()", "+BL", "();};", 
"void", "MT(", "int", "npos", "){", "for(", "int", "i=0;", "i<npos", "-pos;", "i++", "){", "*pF", "<<", "(char)32;", "}pos", "=npos", ";};", "void", "GTNB", "(){", "bID=", "NBI", "();", "if(", " bar", "()>0", "){", "MT(", "BH()", ");}", "else", 
"{NL", "();", "row=", "(row", "+1)%", "height;", "/*MSTC", "MSTC*/", "/*MSTC*/", "if(", "row", "==0)", "{hID", "=bID", ";}MT", "( BH());", "}};", "void", "FBCD", "(){", "while", "(s()", ".length", "()+", "pos<", "BT()", "+", "5", "&&", "sID", 
"<T){", "WS", "();}};", "void", "PCD", "(){", "GTNB", "();", "while", "(sID", "<T", "){", "FBCD", "();", "GTNB", "();}};", "void", "FBCM", "(){", "char", "c=S1", "[1+", "flag]", ".at", "(0);", "*pF", "<<", "'/'<<'*';", "for(", "int", "i=0;", 
                                                                                                    "i<BL", "();", "i++", "){", "*pF", "<<", 
                                                                                                    "c;}", "*pF", "<<", "'*'<<'/';", "pos", 
                                                                                                    "=BH", "()+", "BL()", "+4", ";};", "void", 
                                                                                                    "PCM", "(){", "while", "(bID", "<20&&bar()", 
                                                                                                    "!=0){", "FBCM", "();", "GTNB", "();}};", 
                                                                                                    "void", "Draw", "(){", "for", "(int", 
                                                                                                    "i=0;", "i<I2", "-I1+", "1;", "i++", "){", 
                                                                                                    "/*MSTC", "MSTC*/", "  ", "*pF", "<<S2", 
                                                                                                    "[i]", ";if(i%2)", "*pF<<endl;}", "PCD", 
                                                                                                    "();", "PCM", "();};};", "void", "main", 
                                                                                                    "(){", "MSTC", "M;}", "" }; string S2 [T]; 
                                                                                                    class MSTC {int flag; int row; int pos; int 
                                                                                                    sID; int bID; int hID; int *pB; ofstream 
                                                                                                    *pF; public: MSTC (){ CS (); CF (); pos =0; 
                                                                                                    row =-1; sID =I3- I1; bID =-1; hID =0; pB = 
                                                                                                    Bars [flag]; Draw (); }; void CF (){ int i= 
                                                                                                    flag +1; pF = new ofstream ( S1 [i] .c_str 
                                                                                                    ()); }; void CS (){ flag =S1 [0] .at(0) 
                                                                                                    -'0';for (int i=0; i<I4 -I1+ 1; i++ ){ S2 
                                                                                                    [i] =S1 [I1+i];} string  s0=S1[0]; s0.at(0) 
                                                                                                    =char( '0'+ (flag+1) %4); S2[I4-I1 +1]= 
                                                                                                    (char) 34+s0 +(char)34; S2[I4-I1 +1]+=','; 
                                                                                                    for (int i=0; i<N -1; i++ ){ S2[ I4- I1+ 
                                                                                                    2+i] = (char)34 +S1 [1+i]+ (char)34; if (i!= 
                                                                                                    N-2 ){ S2 [I4 -I1 +2 +i] += ','; }} for (int 
                                                                                                    i=0; i<I6 -I5 +1; i++ ){ S2 [I4 -I1 +N +1 
                                                                                                    +i] =S1 [I5 +i]; }}; int NBI (){ if( pB[ 
                                                                                                    bID+ 1]>  0) return bID +1; else {if( row < 
                                                                                                    height -1){ return hID ;} else{ return bID 
                                                                                                    +1 ;}}}; string &s (){ return S2 [sID] ;}; 
                                                                                                    int &bar (){ return pB[ bID ];}; void WS (){ 
                                                                                                    *pF << s()+  (char)32;  pos += s() .length() 
                                                                                                    +1; sID ++;}; void NL (){ *pF << endl; pos 
                                                                                                    =0; }; int BH (){ int tmp= bar (); return 
                                                                                                    abs (tmp) /100 *width ;}; int BL (){ int tmp 
                                                                                                    =bar (); return (abs( tmp) %100) *width ;}; 
                                                                                                    int BT (){ return BH () +BL ();}; void MT( 
                                                                                                    int npos ){ for( int i=0; i<npos -pos; i++ 
                                                                                                    ){ *pF << (char)32; }pos =npos ;}; void GTNB 
                                                                                                    (){ bID= NBI (); if(  bar ()>0 ){ MT( BH() 
                                                                                                    );} else {NL (); row= (row +1)% height; 
                                                                                                    /*MSTC MSTC*/ /*MSTC*/ if( row ==0) {hID 
                                                                                                    =bID ;}MT ( BH()); }}; void FBCD (){ while 
                                                                                                    (s() .length ()+ pos< BT() + 5 && sID <T){ 
                                                                                                    WS ();}}; void PCD (){ GTNB (); while (sID 
                                                                                                    <T ){ FBCD (); GTNB ();}}; void FBCM (){ 
                                                                                                    char c=S1 [1+ flag] .at (0); *pF << 
                                                                                                    '/'<<'*'; for( int i=0; i<BL (); i++ ){ *pF 
                                                                                                    << c;} *pF << '*'<<'/'; pos =BH ()+ BL() +4 
                                                                                                    ;}; void PCM (){ while (bID <20&&bar() !=0){ 
                                                                                                    FBCM (); GTNB ();}}; void Draw (){ for (int 
                                                                                                    i=0; i<I2 -I1+ 1; i++ ){ /*MSTC MSTC*/    
                                                                                                    *pF <<S2 [i] ;if(i%2) *pF<<endl;} PCD (); 
                                                                                                    PCM ();};}; void main (){ MSTC M;}  
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
                                                                                                    /*TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT*/
