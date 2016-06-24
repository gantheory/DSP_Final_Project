#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <iostream>
#include <utility>

using namespace std;

typedef vector< pair<int,int> > WordsDistrib;
#define Word first
#define Count second

#define BS 13131
#define mod 1000000007

inline int mapping( const char*  _word ){
  int len = strlen( _word );
  long long _h;
  for( int _ = 0 ; _ < len ; _ ++ )
    _h = ( _h * BS + ( _word[ _ ] + 128 ) ) % mod;
  return (int)_h;
}

inline WordDistrib normalize( WordsDistrib _dist ){
  WordsDistrib _newdist;
  sort( _dist.begin() , _dist.end() );
  for( size_t l = 0 , r = 0 ; l < _dist.size() ; l = r ){
    while( r < _dist.size() && _dist[ l ].Word == _dist[ r ].Word ) r ++;
    _newdist.push_back( { _dist[ l ].Word , r - l } );
  }
  return _newdist;
}

char input[ 256 ] , tmp[ 256 ];

inline void standardize(){
  int l = 0 , r = strlen( input ) - 1;
  while( l <= r ){
    if( !isalpha( input[ l ] ) && !isdigit( input[ l ] ) ) l ++;
    else if( !isalpha( input[ r ] ) && !isdigit( input[ r ] ) ) r --;
    else break;
  }
  for( int i = 0 , j = l ; j <= r ; j ++ , i ++ )
    tmp[ i ] = input[ j ];
  tmp[ r - l + 1 ] = '\0';
  for( int i = 0 ; i <= r - l + 1 ; i ++ )
    input[ i ] = tmp[ i ];
}

inline WordsDistrib read( const char* path ){
  WordDistrib _dist;
  
  FILE *fin = fopen( path );
  
  while( fscanf( fin , "%s" , input ) == 1 ){
    standardize();
    if( strlen( input ) == 0 ) continue;
    int _hsh = mapping( input );
    _dist.push_back( { _hsh , 1 } );
  }

  fclose( fin );

  return normalize( _dist );
}

#endif