//
//  main.cpp
//  LorentzGas
//
//  Created by Hana&Tina on 5/19/14.
//  Copyright (c) 2014 Georgia Institute of Technology. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "gsl/gsl_multimin.h"
#include "gsl/gsl_math.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::vector;

template<typename T>
std::ostream& operator << (std::ostream& out, const vector<T>& vec)
{
    for(auto& it : vec)
        out << it << " ";
    return out;
}

typedef list<vector<int> > iveclist;
typedef list<vector<double> > dveclist;
void genNecklace(int n, int k, iveclist& out)
{
    
    int i = 0;
    vector<int> tmp(n);
    for(; i < n; ++i)
        tmp[i] = 0;
    out.push_back(tmp);
    i = n - 1;
    do
    {
        tmp[i] = tmp[i] + 1;
        int j = 0;
        while(j < n - i)
        {
            tmp[j + i + 1] = tmp[j];
            ++j;
        }
        if (n % (i + 1) == 0)
            out.push_back(tmp);
        i = n - 1;
        while( tmp[i] == k - 1)
            i = i - 1;
    } while(i >= 0);
}

class LorentzGasElCells
{
private:
    static const int mNdim = 2;
    
    // gsl params and variables
    static const int mMaxIters = 200;
    static constexpr double mAbsTol = 1e-8;
    const gsl_multimin_fminimizer_type * mType;
    gsl_multimin_fminimizer * mSystem;
    gsl_vector * mStep;
    gsl_vector * mXvec;
    gsl_multimin_function mGslFunc;

    int nSyms;
    iveclist mSymbols; // initial list of symbols
    vector<int>* mPtrCurSymbols; // to be used internally with pathLength
    iveclist mLabels; // final list of symbols
    dveclist mThetas; // final list of thetas


    // geometric params
    const double mRadius = 1;
    const double mWidth = 0.3;
    vector<double> mCenterListX;
    vector<double> mCenterListY;
    // internal functions 
    bool pruneRule(const vector<int>& curSymbols);
    bool testLink();
    double pathLength(const gsl_vector * x);
public:
    void init(int n);
    void mainLoop();
    LorentzGasElCells();
    ~LorentzGasElCells() {};
    // for calling the gsl routine
    static double minSrchFunc(const gsl_vector * x, void * params)
    {
       return static_cast<LorentzGasElCells*>(params)->pathLength(x);
    };
};

LorentzGasElCells::LorentzGasElCells(): mType(gsl_multimin_fminimizer_nmsimplex2), mStep(nullptr), mXvec(nullptr), nSyms(0)
{
    mCenterListX.resize(12);
    mCenterListY.resize(12);
    const double distEven = 2 * mRadius + mWidth;
    const double distOdd = distEven * M_SQRT3;
    // the x array
    const double arrayX[12] = {distEven, distOdd * M_SQRT3 / 2, distEven / 2, 0, -distEven / 2, -distOdd * M_SQRT3 / 2, -distEven, -distOdd * M_SQRT3 / 2, -distEven / 2, 0, distEven / 2, distOdd * M_SQRT3 / 2};
    mCenterListX.assign(arrayX, arrayX + 12);
    // the y array
    const double arrayY[12] = {0, distOdd / 2, distEven * M_SQRT3 / 2, distOdd, distEven * M_SQRT3 / 2, distOdd / 2, 0, -distOdd / 2, -distEven * M_SQRT3 / 2, -distOdd, -distEven * M_SQRT3 / 2, -distOdd / 2};
    mCenterListY.assign(arrayY, arrayY + 12);
    // cout << mCenterListX << endl;
    // cout << mCenterListY << endl;
}

bool LorentzGasElCells::pruneRule(const vector<int>& curSymbols)
{
    // do sth to the mSymbols;
    return false;
}

void LorentzGasElCells::init(int n)
{
    nSyms = n;
    // generate length n topolotical cycle out of 12 symbols
    genNecklace(n, 12, mSymbols);
    mSymbols.remove_if([this](const vector<int>& curSymbols){return pruneRule(curSymbols);});
    /*
    for (auto it : mSymbols)
    {
        cout << it << endl;
    }
    */
    // the path depend on Ndim * n variables
    mXvec = gsl_vector_alloc(n);
    mStep = gsl_vector_alloc(n);
    gsl_vector_set_all (mStep, 0.1);
    mGslFunc.n = n;
    mGslFunc.f = minSrchFunc;
    mGslFunc.params = static_cast<void*>(this);
    mSystem = gsl_multimin_fminimizer_alloc(mType, n);
}

// this is going to be our path function
// depends on internal symbol list
double LorentzGasElCells::pathLength(const gsl_vector * thetas)
{
    double sLength = 0;
    int idx = 0;
    vector<double> thvec(nSyms + 1, gsl_vector_get(thetas, 0));
    thvec.assign(thetas->data, thetas->data + nSyms);
    for (; idx < nSyms; ++idx)
    {
        // do sth, according to symbols
        int tmpidx = (*mPtrCurSymbols)[idx]; 
        // with tmpidx we can find the seperation of the two disks for the next bounce
        double diskX = mCenterListX[tmpidx];
        double diskY = mCenterListY[tmpidx];
        double deltaX = mRadius * (cos(thvec[idx + 1]) - cos(thvec[idx]));
        double deltaY = mRadius * (sin(thvec[idx + 1]) - sin(thvec[idx]));
        double tmpLength = sqrt((deltaX + diskX) * (deltaX + diskX) + (deltaY + diskY) * (deltaY + diskY));
        sLength = sLength + tmpLength;

    }
    return sLength;
}

void LorentzGasElCells::mainLoop()
{
    for (auto& pSymbol : mSymbols)
    {
        cout << "current symbols: " << pSymbol << endl;
        // set the current symbol to evaluate
        mPtrCurSymbols = &pSymbol;
        gsl_multimin_fminimizer_set(mSystem, &mGslFunc, mXvec, mStep);
        // search a solution for each of the symbol sequences in the list
        int iters = 0;
        int status;
        do
        {
            ++iters;
            status = gsl_multimin_fminimizer_iterate(mSystem);
            if (status)
                break;
            double size = gsl_multimin_fminimizer_size (mSystem);
            status = gsl_multimin_test_size (size, mAbsTol);
        } while (status == GSL_CONTINUE && iters < mMaxIters);
        if (status == GSL_SUCCESS || status == GSL_CONTINUE)
        {

            const int nVars =  mGslFunc.n;
            vector<double> tmpvec(nVars);
            int i = 0;
            for ( ; i < nVars; ++i)
            {
                tmpvec[i] = gsl_vector_get(mSystem->x, i);
            }
            mLabels.push_back(pSymbol);
            mThetas.push_back(tmpvec);
            cout << tmpvec << endl;
        }
        // check if it is a physically valid solution by testing intersection
        // store the solution and symbol sequence as well
    }
}


int main(int argc, const char * argv[])
{
    LorentzGasElCells billiardSystem;
    billiardSystem.init(2);
    billiardSystem.mainLoop();
    cout << "done\n";
    return 0;
}
