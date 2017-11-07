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

#include <random>
#include <memory>

class RandInt
{
public:
    RandInt( int lower, int upper ) : seed_(0), lower_(lower), upper_(upper) {}

    int GetRand()
    {
        std::uniform_int_distribution<int> dist {lower_,upper_};
        engine_.seed(seed_);
        return dist(engine_);
    }

    void SetRange( int lower, int upper )
    {
        lower_ = lower;
        upper_ = upper;
    }

    void Seed( int val ) {
        seed_ = val;
    }

    int operator()() {
        return GetRand();
    }

private:
    int seed_;
    int lower_;
    int upper_;
    std::default_random_engine engine_;
};

template <typename T>
class Chain
{
public:

    //virtual ~Chain( );
    std::vector<T> GetWordList() const {
        return word_list_;
    }

    int GetWordCount() const {
        return word_list_.size();
    }

    std::vector<T> GetPossibleWords( const T& word0, const T& word1 ) const;
    std::vector<T> GetNearWords(const T& word0) const;

protected:
    ///Do not allow user to create this class.
    Chain() : word_list_() {};
    void AppendToWordList( const std::vector<T>& data );
    void AppendToWordList( const T& data );
    std::vector<T> word_list_;
    std::vector<T> GeneratePhrase( const T& word0, const T& word1, const unsigned& maxLen, const int& retries, const int& repeatings, const int& seed );
};

class StringChain : public Chain<std::string>
{
public:
    StringChain();
    virtual ~StringChain();
    static std::vector<std::string> SplitString( const std::string& text, const char splitchar=' ' );
    void AppendTextToWordList( const std::string& data, const char splitchar=' ' );
    std::string GenerateText( const std::string& word0, const std::string& word1="", const std::string& separator=" ", const unsigned& maxLen=255, const int& retries=10, const int& repeatings=1, const int& seed=1991 );
private:
    std::string word_string_;
};

class IntChain : public Chain<int>
{
public:
    IntChain();
    virtual ~IntChain();
    void AppendIntToWordList(const std::vector<int>& data);
    void AppendIntToWordList(const int& data);
    std::vector<int> GenerateIntPhrase( const int& word0, const int& word1, const unsigned& maxLen=255, const int& retries=10, const int& repeatings=1, const int& seed=1991 );
    std::vector<int> GenerateIntPhraseSingle( const int& word0, const unsigned& maxLen=255, const int& retries=10, const int& repeatings=1, const int& seed=1991 );
};





