#include "data.h"
#include "model.h"
#include "prob.h"
#include <string>
#include <vector>
#include <utility>
#include <stdio.h>
#include <iostream>
using namespace std;

#define ModelNum 20
#define PenaltyOOV 5000
#define TopicNum 3

void init();
vector< char* > testModel( const char* fileName );
vector< pair< int, double > > transform( WordsDistrib& );
double calProb( vector< pair< int, double > >&, int );
void output();

Model m[ ModelNum ];
char modelName[ ][ 20 ] = { "atheism",
                            "autos",
                            "baseball",
                            "christian",
                            "crypt",
                            "eletronics",
                            "forsale",
                            "graphics",
                            "guns",
                            "hockey",
                            "ibm.pc.hardware",
                            "mac.hardware",
                            "med",
                            "mideast",
                            "motorcycles",
                            "ms-windows.misc",
                            "politics",
                            "religion",
                            "space",
                            "windows.x" };
vector< string > ansF;
vector< vector< char* > > ansM;

int main() {
  init();
  FILE* fin1 = fopen( "filelist_test/filelist.list", "r" );

  char list[ 100 ];
  while ( fscanf( fin1, "%s", list ) != EOF ) { 
    FILE* fin2 = fopen( list, "r" );

    char path[ 100 ];
    while ( fscanf( fin2, "%s", path ) != EOF ) {
      ansF.push_back( string( path ) );
      ansM.push_back( testModel( path ) );
    }

    fclose( fin2 );
  }

  output();
  fclose( fin1 );
}

void init() {
  char fname[ 100 ];
  for ( int i = 0 ; i < ModelNum ; ++i ) {
    sprintf( fname, "models/model_%d.mod", i );
    m[ i ].load( fname );
  }
}

vector< char* > testModel( const char* fileName ) {
  WordsDistrib tdb = read( fileName );
  vector< pair< int, double > > temp = transform( tdb );
  vector< pair< double, char* > > ta;
  for ( int i = 0 ; i < ModelNum ; ++i ) 
    ta.push_back( make_pair( calProb( temp, i ), modelName[ i ] ) );
  
  sort( ta.begin(), ta.end() );
  vector< char* > ans;
  for ( size_t i = 0 ; i < TopicNum ; ++i )  
    ans.push_back( ta[ i ].second );
  return ans;
}

vector< pair< int, double > > transform( WordsDistrib& data ) {
  vector< pair< int, double > > temp;
  long long num = 0;
  for ( size_t i = 0  ; i < data.size() ; ++i )
    num += data[ i ].Count;
  for ( size_t i = 0 ; i <  data.size() ; ++i ) 
    temp.push_back( make_pair( data[ i ].Word, ( double ) data[ i ].Count / num ) );
  return temp;
}

double calProb( vector< pair< int, double > >& data, int index ) {
  Model& cm = m[ index ];
  double ans = 0;
  size_t im = 0;
  for ( size_t i = 0 ; i < data.size() ; ++i ) {
    while ( ( im + 1 ) < cm.para.size() && 
            data[ i ].Word > cm.para[ im ].Word )
      ++im;
    if ( im >= cm.para.size() || data[ i ].Word < cm.para[ im ].Word )
      ans = ans + data[ i ].second * data[ i ].second * PenaltyOOV;
    else if ( data[ i ].Word == cm.para[ im ].Word ) {
      ans = ans + ( data[ i ].second - cm.para[ im ].second.value() ) *
                  ( data[ i ].second - cm.para[ im ].second.value() );
      ++im;
    }
/*
    else {
      ans = ans + data[ i ].second * data[ i ].second * 5000;
      ++im;
    }
*/
  }
  return ans;
}

void output() {
  FILE* fout = fopen( "analysis.out", "w" );

  for ( size_t i = 0 ; i < ansF.size() ; ++i ) {
    fprintf( fout, "%s ", ansF[ i ].c_str() );
    for ( size_t j = 0 ; j < ansM[ i ].size() ; ++j ) 
      fprintf( fout, "%s ", ansM[ i ][ j ] );
    fprintf( fout, "\n" );
  }

  fclose( fout );
}

