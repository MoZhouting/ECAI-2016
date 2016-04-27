//search for the semantic types and print the names of the semantic types using their IDs.
//TO RUN: ./search_entities.out database.txt instance_of.txt > results.txt

#include<stdlib.h>
#include<stdio.h>
#include<strings.h>
#include<string.h>

int main ( int argc , char ** argv )
{
  FILE *database = NULL;
  FILE *instance_of_file = NULL;
  
  char *entity_id = NULL;
  char *name = NULL;
  char *name_database = NULL;
  char *subtype_id = NULL;
  char *subtype_id_database = NULL;
  
  int flag = 0;
  
  entity_id = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( entity_id == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation error in entity_id\n" );
    return ( EXIT_FAILURE );
  }
  
  name = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( name == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation error in name\n" );
    return ( EXIT_FAILURE );
  }
  
  name_database = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( name_database == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation error in name\n" );
    return ( EXIT_FAILURE );
  }
  
  subtype_id = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( subtype_id == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation error in name\n" );
    return ( EXIT_FAILURE );
  }
  
  subtype_id_database = ( char * ) malloc ( 1000 * sizeof ( char ) );
  if ( subtype_id_database == NULL )
  {
    fprintf ( stderr , "malloc() memory allocation error in name\n" );
    return ( EXIT_FAILURE );
  }
  
  database = fopen ( argv [ 1 ] , "r" );
  if ( database == NULL )
  {
    fprintf ( stderr , "Database file open error\n" );
    return ( EXIT_FAILURE );
  }
  
  instance_of_file = fopen ( argv [ 2 ] , "r" );
  if ( instance_of_file == NULL )
  {
    fprintf ( stderr , "Instance-of file read error\n" );
    return ( EXIT_FAILURE );
  }
  
  while ( !feof ( instance_of_file ) )
  {
    fscanf ( instance_of_file , "%s %s %s\n" , entity_id , name , subtype_id );
    while ( !feof ( database ) )
    {
      fscanf ( database , "%s %s\n" , subtype_id_database , name_database );
      if ( strcasecmp ( subtype_id , subtype_id_database ) == 0 )
      {
	printf ( "%s\n" , name_database );
	flag = 1;
      }
      if ( feof ( database ) && flag == 0 )
      {
	printf ( "\n" );
	rewind ( database );
	break;
      }
      if ( flag == 1 )
      {
	flag = 0;
	memset ( entity_id , 0 , 1000 );
	memset ( subtype_id , 0 , 1000 );
	memset ( subtype_id_database , 0 , 1000 );
	memset ( name , 0 , 1000 );
	memset ( name_database , 0 , 1000 );
	rewind ( database );
	break;
      }
    }
  }
  
  fclose ( database );
  fclose ( instance_of_file );
  
  free ( entity_id );
  free ( name );
  free ( name_database );
  free ( subtype_id_database );
  free ( subtype_id );
  
  return ( EXIT_FAILURE );
}
