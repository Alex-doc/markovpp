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
#include <algorithm>
#include <sstream>
#include "../include/Markovpp.h"

///Get a list of the possible words that can be concatenated
template <typename T>
std::vector< T > Chain< T >::GetPossibleWords(const T& word0, const T& word1) const
{
    std::vector<T> result;
    int wordCount = word_list_.size();
    for( typename std::vector<T>::const_iterator i = word_list_.begin(); i != word_list_.end(); ++i)
    {
        T word = *i;
        int index = i - word_list_.begin();
        if( word == word0 && index < (wordCount - 1) )
        {
            //result.push_back(word_list_[index+1]);
            if(word_list_[index+1] == word1 && index < wordCount - 2)
                result.push_back(word_list_[index+2]);
        }
    }
    return result;
}

template <typename T>
std::vector< T > Chain< T >::GetNearWords(const T& word0) const
{
    std::vector<T> result;
    int wordCount = word_list_.size();
    for( typename std::vector<T>::const_iterator i = word_list_.begin(); i != word_list_.end(); ++i)
    {
        T word = *i;
        int index = i - word_list_.begin();
        if( word == word0 && index < (wordCount - 1) )
            result.push_back(word_list_[index+1]);
    }
    ///this must not be orderer, we allow multiple entries of the same word, so it helps with statistics
    std::random_shuffle( result.begin(), result.end() );
    return result;
}

template <typename T>
void Chain<T>::AppendToWordList(const std::vector<T>& data)
{
    for( typename std::vector<T>::const_iterator i = data.begin(); i != data.end(); ++i )
        word_list_.push_back( *i );
}

template <typename T>
void Chain<T>::AppendToWordList(const T& data)
{
    word_list_.push_back( data );
}

///Generates the phrase/list
///@param word0 the first "suggestion"
///@param word1 the second "suggestion"
///@param maxLen the max lengh of the list
///@param retries the number of retries if unsuccesfull
///@param repeatings the number of repeatings/randomizations
///@param seed the random seed to use
template<typename T>
std::vector<T> Chain<T>::GeneratePhrase( const T& word0, const T& word1, const unsigned& maxLen, const int& retries, const int& repeatings, const int& seed )
{
    std::vector<T> result;
    int triesLeft = retries;

    std::unique_ptr<RandInt> rnd( new RandInt( 0, word_list_.size( ) - 1 ) );
    rnd->Seed( seed );

    std::vector<T> state { word0, word1 };

    result = { state[0], state[1] };

    ///Force max length to be at least 2
    unsigned mlen = maxLen < 2 ? 2 : maxLen;

    bool done = false;
    while( !done )
    {
        std::vector<T> options = GetPossibleWords( state[0], state[1] );
        if( options.size( ) > 0 && result.size( ) < mlen )
        {
            rnd->SetRange( 0, options.size( ) - 1 );
            T next = options[rnd->GetRand( )];
            int repeats = repeatings;
            while( repeats > 0 )
            {
                next = options[rnd->GetRand( )];
                if( next != state[0] )
                    repeats = 0;
                repeats -= 1;
                state[0] = state[1];
                state[1] = next;
                result.push_back( next );
            }
        }
        else
        {
            if( triesLeft <= 0 || result.size( ) > mlen )
                done = true;
            else
            {
                if( result.size( ) < mlen )
                {
                    rnd->SetRange( 0, word_list_.size() - 1 );
                    int r0 = rnd->GetRand( );
                    int r1 = rnd->GetRand( );
                    ///We try to get the near word,
                    ///if unsuccesfull, use a random one
                    T state1 = word_list_[r1];
                    std::vector<T> wl = GetNearWords(word_list_[r0]);
                    if(wl.size() > 0)
                    {
                        rnd->SetRange( 0, wl.size() - 1 );
                        state1 = wl[rnd->GetRand()];
                    }

                    state = { word_list_[r0], state1 };
                }
                triesLeft -= 1;
            }
        }
    }
    return result;
}


StringChain::StringChain() :
    Chain<std::string>(),
    word_string_("")
{
    word_list_.clear();
}

StringChain::~StringChain()
{
}


std::vector<std::string> StringChain::SplitString(const std::string& text, const char splitchar )
{
    std::vector<std::string> result;
    std::string subword;
    for(std::string::const_iterator c = text.begin(); c != text.end(); ++c)
    {
        if(*c != splitchar)
            subword.push_back(*c);
        else
        {
            result.push_back(subword);
            subword = "";
        }
    }
    ///we push back the last word as there's no splitchar to assign.
    result.push_back(subword);
    return result;
}

///Generates a list of words with the given std::string.
void StringChain::AppendTextToWordList(const std::string& data, const char splitchar)
{
    word_string_ = word_string_+data;
    word_list_.clear();
    AppendToWordList(SplitString(word_string_, splitchar));
}

std::string StringChain::GenerateText( const std::string& word0, const std::string& word1, const std::string& separator, const unsigned& maxLen, const int& retries, const int& repeatings, const int& seed )
{
    std::unique_ptr<RandInt> rnd( new RandInt( 0, word_list_.size( ) - 1 ) );
    rnd->Seed( seed );
    int r0 = rnd->GetRand( );
    int r1 = rnd->GetRand( );

    std::string word0_ = word_list_[r0];
    std::string word1_ = word_list_[r1];

    if( word0 != "" )
        word0_ = word0;
    if( word1 != "" )
        word1_ = word1;
    else
    {
        std::vector<std::string> words = GetNearWords(word0_);
        if(words.size() > 0)
        {
            rnd->SetRange(0,words.size()-1);
            word1_ = words[rnd->GetRand()];
        }
    }

    std::vector<std::string> result = GeneratePhrase( word0_, word1_, maxLen, retries, repeatings, seed );

    std::stringstream resultString("");

    for( std::vector<std::string>::iterator i = result.begin(); i != result.end(); ++i)
    {
        resultString << *i;
        if(i != (result.end() - 1))
            resultString << separator;
    }
    return resultString.str();
}

///

IntChain::IntChain() :
    Chain<int>()
{
    word_list_.clear();
}

IntChain::~IntChain()
{
}

void IntChain::AppendIntToWordList(const std::vector<int>& data)
{
    AppendToWordList(data);
}

void IntChain::AppendIntToWordList(const int& data)
{
    AppendToWordList(data);
}

std::vector<int> IntChain::GenerateIntPhraseSingle(const int& word0, const unsigned& maxLen, const int& retries, const int& repeatings, const int& seed)
{
    std::vector<int> result;

    std::vector<int> words = GetNearWords(word0);
    std::unique_ptr<RandInt> rnd( new RandInt( 0, words.size( ) - 1 ) );
    rnd->Seed(seed);
    int word1 = word0+1;

    if(words.size() > 0)
        word1 = words[rnd->GetRand()];

    result = GenerateIntPhrase(word0, word1, maxLen, retries, repeatings, seed );
    return result;
}

std::vector<int> IntChain::GenerateIntPhrase(const int& word0, const int& word1, const unsigned& maxLen, const int& retries, const int& repeatings, const int& seed)
{
    return GeneratePhrase(word0, word1, maxLen, retries, repeatings, seed);
}
