// BSD 2-Clause License
//
// Copyright (c) 2017, Alex Piola
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <fstream>
#include <algorithm>
#include "include/Markovpp.h"

using namespace std;

int main()
{
    string alphabet = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
    string separator = " ";

    ///We read a sample file...
    ifstream ifs("example.txt");
    if(ifs)
    {
        alphabet = "";
        string line;
        while(getline(ifs,line))
            alphabet += line+separator;
        ///Remove trailing space
        alphabet.erase(alphabet.size()-1);
        ifs.close();
    }

    vector<int> numbers = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    RandInt rnd = RandInt(1,100);
    rnd.Seed(123978);

    StringChain schain;
    schain.AppendTextToWordList(alphabet);

    int maxLength = 8;
    int retries = 10;
    int repeatings = 10;
    int seed = 12398;

    string v = schain.GenerateText("the", "", separator, maxLength, retries, repeatings, seed);
    printf("[Single word example] %s\n",v.c_str());

    v = schain.GenerateText("the", "fire", separator, maxLength, retries, repeatings, seed);
    printf("[Two words example] %s\n",v.c_str());

    v = schain.GenerateText("", "", separator, maxLength, retries*100, repeatings*100, seed*10);
    printf("[No word example/completely random] %s\n",v.c_str());

    IntChain ichain;
    ichain.AppendIntToWordList(numbers);
    ichain.AppendIntToWordList(11);

    auto printVal = []( const int& p ) { printf("%i ",p); };
    vector<int> ic = ichain.GenerateIntPhrase(1,2, maxLength, retries, repeatings, seed);
    printf("[Good numeric sequence] ");
    for_each(ic.begin(), ic.end(), printVal);

    ic = ichain.GenerateIntPhrase(4,2, maxLength, retries, repeatings, seed);
    printf("\n[Bad numeric sequence] ");
    for_each(ic.begin(), ic.end(), printVal);

    return 0;
}


