//This code will compute the embeddings of entities listed in the Wikidata that are present in the Wikipedia. The code also computes the subspaces using the least squares regression method where it fits the points in the embedding space. Some of the components in the code are borrowed from the GloVe model of Pennington et. al.

//To run the code, do this: ./subspace_glove.out COOCCURRENCE_MATRIX VOCABULARY DIMENSIONS X_MAX ALPHA

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include </home/msjameel/gsl-2.1/gsl/gsl_rng.h>
#include <cblas.h>

struct record
{
  unsigned long int word1; //these are word IDs which are integer values.
  unsigned long int word2;
  float cooccurrence_value;
  struct record *next_record;
};

void read_data ( struct record * , FILE * , unsigned long int );
void print_data ( struct record * );
unsigned long int lines_in_file ( FILE * );
float **random_initialization ( unsigned long int ,  unsigned int ); //pass any matrix of any size
void print_word_vectors ( unsigned long int , unsigned int , float ** );
float **initialize ( unsigned long int ,  unsigned int );
float iterations ( float * , float * , float * , float * , float * , float * , float * , float * , float , unsigned int , float , float );
float dot_product ( float * , float * , int  );

int main ( int argc , char ** argv )
{
//Read the input data from the co-occurrence matrix prepared using the other scripts.
FILE *input_cooccurrence_matrix = NULL;
FILE *vocabulary_file = NULL;

float **word_vectors = NULL;
float **biases = NULL;
float **gradient_word_vectors = NULL;
float **gradient_biases = NULL;
float x_max = 0;
float alpha = 0;

unsigned long int vocabulary_length = 0;
unsigned long int cooccurrence_lines = 0;
unsigned int dimensions = 0;
unsigned long int row_counter = 0;

input_cooccurrence_matrix = fopen ( argv [ 1 ] , "r" );
if ( input_cooccurrence_matrix == NULL )
{
  fprintf ( stderr , "Co-occurrence file open error\n" );
  return ( EXIT_FAILURE );
}

vocabulary_file = fopen ( argv [ 2 ] , "r" );
if ( vocabulary_file == NULL )
{
  fprintf ( stderr , "Vocabulary file read error\n" );
  return ( EXIT_FAILURE );
}

dimensions = atoi ( argv [ 3 ] );
x_max = ( float ) atof ( argv [ 4 ] );
alpha = ( float ) atof ( argv [ 5 ] );

struct record *record_entries = NULL;
record_entries = ( struct record * ) malloc ( sizeof ( struct record ) );
if ( record_entries == NULL )
{
  fprintf ( stderr , "malloc() memory allocation failure\n" );
  return ( EXIT_FAILURE );
}

cooccurrence_lines = lines_in_file ( input_cooccurrence_matrix );
rewind ( input_cooccurrence_matrix );
read_data ( record_entries , input_cooccurrence_matrix , cooccurrence_lines );
print_data ( record_entries );//data has been read all well.
vocabulary_length = lines_in_file ( vocabulary_file );
word_vectors = random_initialization ( vocabulary_length , dimensions );
biases = random_initialization ( vocabulary_length , dimensions );
print_word_vectors ( vocabulary_length , dimensions , word_vectors );
gradient_word_vectors = initialize ( vocabulary_length , dimensions );
gradient_biases = initialize ( vocabulary_length , dimensions );

free ( record_entries );
fclose ( input_cooccurrence_matrix );
fclose ( vocabulary_file );

for ( row_counter = 0 ; row_counter < ( 2 * vocabulary_length ) ; row_counter ++ )
{
  free ( word_vectors [ row_counter ] );
}
free ( word_vectors );

for ( row_counter = 0 ; row_counter < ( 2 * vocabulary_length ) ; row_counter ++ )
{
  free ( biases [ row_counter ] );
}
free ( biases );

for ( row_counter = 0 ; row_counter < ( 2 * vocabulary_length ) ; row_counter ++ )
{
  free ( gradient_word_vectors [ row_counter ] );
}
free ( gradient_word_vectors );

for ( row_counter = 0 ; row_counter < ( 2 * vocabulary_length ) ; row_counter ++ )
{
  free ( gradient_biases [ row_counter ] );
}
free ( gradient_biases );

return ( EXIT_SUCCESS );
}

void read_data ( struct record *record_entries , FILE * input_cooccurrence_matrix , unsigned long int cooccurrence_lines )
{
  unsigned long int line_counter = 0;
while ( line_counter < cooccurrence_lines )
{
  fscanf ( input_cooccurrence_matrix , "%ld " , &record_entries ->word1 );
  fscanf ( input_cooccurrence_matrix , "%ld" , &record_entries -> word2 );
  fscanf ( input_cooccurrence_matrix , "%f" , &record_entries -> cooccurrence_value );
  record_entries -> next_record = ( struct record * ) malloc ( sizeof ( struct record ) );
  record_entries = record_entries -> next_record;
  line_counter ++;
}
record_entries -> next_record = NULL;
}

void print_data ( struct record *record_entries )
{
  while ( record_entries -> next_record != NULL )
  {
    printf ("%ld %ld %f\n" , record_entries -> word1 , record_entries -> word2 , record_entries -> cooccurrence_value );
    record_entries = record_entries -> next_record;
  }
}

unsigned long int lines_in_file ( FILE * vocabulary_file )
{
  char ch;
  unsigned long int counter = 0;
  
  while ( !feof ( vocabulary_file ) )
  {
    ch = fgetc ( vocabulary_file );
    if ( ch == '\n' )
    {
      counter ++;
    }
  }
  return ( counter );
}

float ** random_initialization ( unsigned long int vocabulary_length , unsigned int dimensions )
{
  float **W = NULL;
  unsigned long int outer_counter = 0;
  unsigned int inner_counter = 0;
  const gsl_rng_type * T;
  gsl_rng * r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  
  W = ( float ** ) malloc ( ( 2 * vocabulary_length ) * sizeof ( float * ) );
  if ( W == NULL)
    {
      fprintf ( stderr, "Out of memory\n");
      exit ( EXIT_FAILURE );
     }
 
  for ( outer_counter = 0 ; outer_counter < ( 2 * vocabulary_length ) ; outer_counter ++ )
  {
    W [ outer_counter ] = ( float * ) malloc ( dimensions * sizeof ( float ) );
    if ( W [ outer_counter ] == NULL )
      {
	fprintf(stderr , "Out of memory\n");
	exit ( EXIT_FAILURE );
     }
  }
  
  for ( outer_counter = 0 ; outer_counter < ( 2 * vocabulary_length ) ; outer_counter ++ ) //rows
  {
    for ( inner_counter = 0 ; inner_counter < dimensions ; inner_counter ++ ) //columns
    {
      W [ outer_counter ] [ inner_counter ] = gsl_rng_uniform ( r );
    }
  }
  gsl_rng_free (r);
  return ( W );
}

void print_word_vectors ( unsigned long int vocabulary_length , unsigned int dimensions , float ** word_vectors )
{
  unsigned long int row_counter = 0;
  unsigned int column_counter = 0;
  for ( row_counter = 0 ; row_counter < ( 2 * vocabulary_length ) ; row_counter ++ )
  {
    for ( column_counter = 0 ; column_counter < dimensions ; column_counter ++ )
    {
      printf ( "%f " , word_vectors [ row_counter ] [ column_counter ] );
    }
    printf ( "\n" );
  }
}

float ** initialize ( unsigned long int vocabulary_length , unsigned int dimensions )
{
  float **gradients = NULL;
  unsigned long int outer_counter = 0;
  unsigned int inner_counter = 0;
  
  gradients = ( float ** ) malloc ( ( 2 * vocabulary_length ) * sizeof ( float * ) );
  if ( gradients == NULL)
    {
      fprintf ( stderr, "Out of memory\n");
      exit ( EXIT_FAILURE );
     }
 
  for ( outer_counter = 0 ; outer_counter < ( 2 * vocabulary_length ) ; outer_counter ++ )
  {
    gradients [ outer_counter ] = ( float * ) malloc ( dimensions * sizeof ( float ) );
    if ( gradients [ outer_counter ] == NULL )
      {
	fprintf(stderr , "Out of memory\n");
	exit ( EXIT_FAILURE );
     }
  }
  
  for ( outer_counter = 0 ; outer_counter < ( 2 * vocabulary_length ) ; outer_counter ++ ) //rows
  {
    for ( inner_counter = 0 ; inner_counter < dimensions ; inner_counter ++ ) //columns
    {
      gradients [ outer_counter ] [ inner_counter ] = 1;
    }
  }
  
  return ( gradients );
}

float iterations ( float *main_word_vector , float *context_word_vector , float *bias_main_word , float *bias_context_word , float *gradient_main_word , float *gradient_context_word , float *gradient_bias_main , float *gradient_bias_context , float cooccurrence , unsigned int dimensions , float x_max , float alpha )
{
  float inner_cost = 0;
  float cost = 0;
  float global_cost = 0;
  float weight= 0;
  float return_dot_product = 0;
  float grad_main = 0;
  float gram_context = 0;
  float grad_bias_main = 0;
  float grad_bias_context = 0;
  
  if ( cooccurrence < x_max )
  {
    weight = ( float ) pow ( ( cooccurrence / x_max ) , alpha );
  }
  else
  {
    weight = 1;
  }
  
  return_dot_product = dot_product ( main_word_vector , context_word_vector , dimensions );
  inner_cost = return_dot_product + bias_main_word [ 0 ] + bias_context_word [ 0 ] - ( float ) log ( cooccurrence );
  
  cost = weight * ( inner_cost * inner_cost );
  global_cost = global_cost + ( cost / 2 );
  
  cblas_sscal ( dimensions , (weight * inner_cost ) , context_word_vector , 1 );
  
  
  return ( cost );
}

float dot_product ( float v[] , float u[] , int n)
{
    float result = 0.0;
    int i = 0;
    
    for ( i = 0 ; i < n ; i++ )
    {
      result += v[i]*u[i];
    }
    return ( result );
}
