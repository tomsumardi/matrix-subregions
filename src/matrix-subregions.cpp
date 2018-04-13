//============================================================================
// Name        : codetest1.cpp
// Author      : Tom Sumardi
// Version     : 1.0.0
// Copyright   : GPL, will be posted on github
// Description : Code Test
//============================================================================

#include <iostream>
#include <vector>
#include <list>
#include <assert.h>
#include <sstream>
#include <iterator>
#include <string>
#include <map>
using namespace std;

#define CT_ARG_ERRORNO    100
#define CT_SUCCESS          0
#define CT_ERROR            -1
#define CT_STS              int

//inputs:
// 1. two-dimensional array of values x,y
// 2. Threshold (T)
//output:
// sub-regions, each containing list of (x,y) value pairs
// average of each sub-regions

class cell {
    public:
        cell() : x(0), y(0), value(0), region(0), check(false) {}
        int x;
        int y;
        int value;
        int region;
        bool check;
};

class util {
    public:
        util() {}
        template<typename Out>
        void split(const string &s, char delim, Out result) {
            stringstream ss(s);
            string item;
            while (getline(ss, item, delim)) {
                *(result++) = item;
            }
        }
        vector<string> split(const string &s, char delim) {
            vector<string> elems;
            split(s, delim, back_inserter(elems));
            return elems;
        }
};

class codeTest : util {
    public:
        codeTest() : c_yrSize(0), c_xcSize(0), c_threshold(0){}
        CT_STS processArgs(int argc, char *argv[]);
        CT_STS validateArgs();
        CT_STS findSubRegions();

    private:
        vector<string>  c_rList;
        int             c_yrSize;
        int             c_xcSize;
        int             c_threshold;
};

class region : util {
    public:
        region(int thredhold, int xcol, int yrow, vector<string> alist)
             :  c_xcSize(xcol), c_yrSize(yrow), c_threshold(thredhold), c_matrix(NULL)
        {
            int value = 0;
            int x = 0;
            int y = 0;
            c_matrix = new int*[c_xcSize];
            for(int i = 0; i < c_xcSize; ++i)
                c_matrix[i] = new int[c_yrSize];
            for (auto it = alist.begin(); it != alist.end(); it++)
            {
                vector<string> lyrow = this->split(*it, ',');
                for (auto itr = lyrow.begin(); itr != lyrow.end(); itr++)
                {
                   value = atoi((*itr).c_str());
                   assert(c_xcSize > x && c_yrSize > y);
                   c_matrix[x][y] = value;
                   ++x;
                   if(x >= c_xcSize)
                       x = 0;
                }
                ++y;
                if(y >= c_yrSize)
                    y = 0;
            }
        }
        ~region()
        {
            for(int i = 0; i < c_xcSize; ++i) {
                delete [] c_matrix[i];
            }
            delete [] c_matrix;
        }
    protected:
        int          c_xcSize;
        int          c_yrSize;
        int          c_threshold;
        int**        c_matrix;
};

class subregion : region {
    public:
        subregion(int thredhold, int xcol, int yrow, vector<string> alist) :
            region(thredhold, xcol, yrow,alist), c_totalRegions(0) {}
        ~subregion()
        {
            for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++) {
                delete(*elm);
            }
            for (auto elm = c_lOutputCells.begin(); elm != c_lOutputCells.end(); ++elm) {
                for (list<cell*>::iterator out = (*elm).begin(); out != (*elm).end(); ++out) {
                    delete(*out);
                }
            }
        }
        void    setListOfCells(cell* elm){
            c_lrawCells.push_back(elm);
        }
        bool   recurseSearch(int x, int y, int regn){
            bool b = false;
            for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++)
            {
                if (((*elm)->x == x) && ((*elm)->y == y))
                {
                    //cout << "set" << endl;
                    (*elm)->region = regn;
                    (*elm)->check = true;
                    b = true;
                }
            }

            return b;
        }
        void   recurseFindCellLeft(int x, int y, int regn){
            x = x-1;
            if(x > c_xcSize || x < 0) {
                return;
            }
            else
            {
                //cout << "x:" << x << ", y:" << y << endl;
                if (recurseSearch(x,y,regn) == true)
                    recurseFindCellLeft(x,y,regn);
            }
        }
        void   recurseFindCellRight(int x, int y, int regn){
            x = x+1;
            if(x > c_xcSize || x < 0) {
                return;
            }
            else
            {
                //cout << "x:" << x << ", y:" << y << endl;
                if (recurseSearch(x,y,regn) == true)
                    recurseFindCellRight(x,y,regn);
            }
        }
        void   recurseFindCellDown(int x, int y, int regn){
            y = y-1;
            if(y > c_yrSize || y < 0) {
                return;
            }
            else
            {
                //cout << "x:" << x << ", y:" << y << endl;
                if (recurseSearch(x,y,regn) == true)
                    recurseFindCellDown(x,y,regn);
            }
        }
        void   recurseFindCellUp(int x, int y, int regn){
            y = y+1;
            if(y > c_yrSize || y < 0) {
                return;
            }
            else
            {
                //cout << "x:" << x << ", y:" << y << endl;
                if (recurseSearch(x,y,regn) == true)
                    recurseFindCellUp(x,y,regn);
            }
        }
        void recurseFindCell(int regn)
        {
            // Initializer to get the recurse going..
            for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++) {
                (*elm)->region = regn;
                (*elm)->check = true;
                break;
            }
            //Not very efficient here with two pass but will work marking all the cells...
            for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++)
            {
                for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++)
                {
                    if ((*elm)->check == true)
                    {
                        //cout << "start looking" << ", x:" << (*elm)->x << ", y:" << (*elm)->y << endl;
                        recurseSearch((*elm)->x,(*elm)->y,regn);
                        recurseFindCellRight((*elm)->x,(*elm)->y,regn);
                        recurseFindCellLeft((*elm)->x,(*elm)->y,regn);
                        recurseFindCellUp((*elm)->x,(*elm)->y,regn);
                        recurseFindCellDown((*elm)->x,(*elm)->y,regn);
                    }
                }
            }
        }
        void findRegions()
        {
            list<cell*>*  lCells;
            // if I have to redo this again, more likely binary tree...
            for (int regn = 1; regn < (c_xcSize*c_yrSize)+1; regn++) {
                recurseFindCell(regn);
                lCells = new list<cell*>;
                auto ptr = c_lrawCells.begin();
                auto elm = c_lrawCells.begin();
                while(elm != c_lrawCells.end()) {
                    ptr = elm;
                    if ((*elm)->check == true) {
                        c_totalRegions = (*elm)->region;
                        elm++;
                        (*lCells).splice((*lCells).begin(),(*lCells),ptr);
                   }
                   else
                       elm++;
                }
                if (lCells)
                    c_lOutputCells.push_back(*lCells);
            }

        }
        void dumpOutput()
        {
            /*
            cout << "rcells" << endl;
            for (auto elm = c_lrawCells.begin(); elm != c_lrawCells.end(); elm++) {
                cout << "x:" << (*elm)->x << ", y:" << (*elm)->y << ", value:"
                << (*elm)->value << ", region=" <<  (*elm)->region << endl;
            }
            cout << "rx" << endl;
            for (auto elm = (*r_lCells).begin(); elm != (*r_lCells).end(); elm++) {
                cout << "x:" << (*elm)->x << ", y:" << (*elm)->y << ", value:"
                << (*elm)->value << ", region=" <<  (*elm)->region << endl;
            }*/
            int sum;
            int avg;
            int i;
            int regcnt = 0;
            if(c_totalRegions){
                cout << "regions: " << c_totalRegions << endl;
                for (auto elm = c_lOutputCells.begin(); elm != c_lOutputCells.end(); ++elm) {
                    sum=0;
                    i=0;
                    for (list<cell*>::iterator out = (*elm).begin(); out != (*elm).end(); ++out) {
                        cout << "x:" << (*out)->x << ", y:" << (*out)->y << ", value:"
                                << (*out)->value << ", region=" <<  (*out)->region << endl;
                        i++;
                        sum += (*out)->value;
                    }
                    if (i == 0 && sum != 0)
                        throw runtime_error("divide by zero failure");
                    else {
                        avg = sum/i;
                        cout << "---------com average: " << avg << endl;
                    }
                    regcnt++;
                    if(regcnt == c_totalRegions)
                        break;
                }
            }
            else{
                cout << "regions: 0" << endl;
                cout << "com average: None" << endl;
            }
        }
        int& operator()(int xcol, int yrow) {
            assert(c_xcSize > xcol && c_yrSize > yrow);
            return c_matrix[xcol][yrow];
        }
    private:
        list<cell*>             c_lrawCells;
        vector <list<cell*> >   c_lOutputCells;
        int                     c_totalRegions;
};

CT_STS codeTest::findSubRegions()
{
    cout << "threshold: " << c_threshold << endl;
    cout << "row size: " << c_yrSize << endl;
    cout << "column size: " << c_xcSize << endl;

    subregion mtable = subregion(c_threshold,
            c_xcSize, c_yrSize, c_rList);
    cell* elm = NULL;
    for(int y = 0; y < c_yrSize; y++) {
        for(int x = 0; x < c_xcSize; x++) {
            if(mtable(x,y) > c_threshold)
            {
                elm = new cell();
                elm->x = x;
                elm->y = y;
                elm->region = 0;
                elm->value = mtable(x,y);
                mtable.setListOfCells(elm);
            }
        }
    }
    mtable.findRegions();
    mtable.dumpOutput();

    return CT_SUCCESS;
}

CT_STS codeTest::processArgs(int argc, char *argv[])
{
    CT_STS          sts = CT_ERROR;
    string          st = "";
    string          thld = "";
    string          banner1 = "Usage is -a <x,y array list> -t <threshold>";
    string          banner2 = "example -a \"{{0,80,45,95,170,145}:{115,210,60,5,230,220}:"
            "{5,0,145,250,245,140}:{15,5,175,250,185,160}:{0,5,95,115,165,250}:"
            "{5,0,25,5,145,250}\" -t 200";
    if (argc < 5) {
        cout << banner1 << endl;
        cout << banner2 << endl;
        exit(CT_ARG_ERRORNO);
    }
    else {
        vector<string> allArgs(argv, argv + argc);
        auto it = allArgs.begin();
        while(it != allArgs.end())
        {
            if (*it == "-h"){
                cout << banner1 << endl;
                cout << banner2 << endl;
                exit(CT_SUCCESS);
            }
            else if (*it == "-a"){
                st = *(it+1);
            }
            else if (*it == "-t"){
                thld = *(it+1);
            }
            it++;
        }
        assert(st != "");
        assert(thld != "");

        vector<string> xydict = this->split(st, ':');
        char mstr[] = "{}";
        int csize = 0;
        for (auto it = xydict.begin(); it != xydict.end(); it++) {
            for (unsigned int i = 0; i < strlen(mstr); ++i)
               //remove {} for easy parsing
               (*it).erase (remove((*it).begin(), (*it).end(), mstr[i]), (*it).end());
               c_rList.push_back(*it);
               vector<string> row = this->split(*it, ',');
               csize = row.size();
        }
        c_threshold = atoi(thld.c_str());
        c_yrSize = c_rList.size();
        c_xcSize = csize;
        sts = CT_SUCCESS;
    }

    return sts;
}

CT_STS codeTest::validateArgs()
{
    CT_STS sts = CT_SUCCESS;
    int refrows = 0;
    int nrows = 0;
    bool init = true;
    //TODO: need more Validation for arguments
    for (auto it = c_rList.begin(); it != c_rList.end(); it++) {
        vector<string> row = this->split(*it, ',');
        nrows = 0;
        for (auto clmn = row.begin(); clmn != row.end(); clmn++)
            nrows++;
        if(init == true)
        {
            refrows = nrows;
            init = false;
        }
        if (nrows != refrows)
        {
            sts = CT_ERROR;
            cerr << "error, there is a mismatch on one of the row (x): " << (*it) << endl;
            cerr << "row (x) only has: " << nrows << " collumns"
                    << " out of " << refrows << " collumns" << endl;
            break;
        }
    }
    return sts;
}

CT_STS main(int argc, char *argv[]) {
  codeTest testApp;

  if (CT_SUCCESS != testApp.processArgs(argc,argv))
      throw runtime_error("arguments processing failure");
  if (CT_SUCCESS != testApp.validateArgs())
      throw runtime_error("arguments validation failure");
  if(CT_SUCCESS != testApp.findSubRegions())
      throw runtime_error("find subregions failure");

  return CT_SUCCESS;
}
