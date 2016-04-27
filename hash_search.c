//This program will read the bigram, trigram and quadgram file generated from the Wikipedia and search for the entities from it.
//replace space with - before running this code in both files.

#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<search.h>

int main ( int argc , char **argv )
{
  FILE *wikipedia_ngram = NULL;
  FILE entities = NULL;
  
  char *line = NULL;
  char *file_name = NULL;
  char *word = NULL;
  
  size_t len = 0;
  ssize_t read;
  //Defining the hash variables
  ENTRY e, *ep;
  unsigned long int lines_ngram = 0; //number of lines in the n-gram file
  unsigned long iterator = 0;
  
  wikipedia_ngram = fopen ( argv [ 1 ] , "r" );
  if ( wikipedia_ngram == NULL )
  {
    fprintf ( stderr , "Wikipedia n-gram file open error\n" );
    return ( EXIT_FAILURE );
  }
  
  entities = fopen ( argv [ 2 ] , "r" );
  if ( entities == NULL )
  {
    fprintf ( stderr , "Entities file open error\n" );
    return ( EXIT_FAILURE );
  }
  
  file_name = ( char * ) malloc ( 5000 * sizeof ( char ) );
  if ( file_name == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation failure in file_name\n" );
    return ( EXIT_FAILURE );
  }
  
  word = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( word == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation failure in word\n" );
    return ( EXIT_FAILURE );
  }
    
  while ( ( read = getline ( &line , &len , wikipedia_ngram ) ) != -1 )
  {
    lines_ngram ++;
  }
  rewind ( wikipedia_ngram );
  //got the number of lines above
  //create the hash table now.
  
  hcreate ( lines_ngram );
  
  for ( iterator = 0; iterator < lines_ngram; iterator++ )
  {
    fscanf ( wikipedia_ngram , "%s %s\n" , file_name , word );
    e.key = word;
    e.data = (char *) file_name;
    ep = hsearch ( e , ENTER );
    /* there should be no failures */
    if (ep == NULL)
    {
      fprintf(stderr, "Entry failed\n");
      exit ( EXIT_FAILURE );
    }
    memset ( file_name , 0 , 5000 );
    memset ( word , 0 , 1000 );
  }
  
  //find the entities in the hash table.
  while ( !feof ( entities ) )
  {
    fscanf ( entities , "%s\n" , word );
    e.key = word;
    ep = hsearch(e, FIND);
    printf ("%s\n" , ( char ) ( ep->data ) );
  }
  
  if ( line )
  {
    free ( line );
  }
  fclose ( wikipedia_ngram );
  fclose ( entities );
  free ( file_name );
  free ( word );
  hdestroy();
  return ( EXIT_SUCCESS );
}
