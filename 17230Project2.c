/*********************Importing Libraries***********************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/*********************Defining ********************************/
#define vert 	"\u2502"                          //vertical line character
#define hori 	"\u2500"                          //Horizontal line character
#define conect  "\u2514"                        // connecter of vertical and horizontal line
#define fill    "\u2591"                        // Filler used to display the graph



/********************* Data type character node************************/
typedef struct c_n{

    char character;				                       /// chracter

    int c_count;				                        /// No of the character

    int c_place;				                       /// Place before sorting.

    struct c_n *c_next;				                /// Address of the next list.

}c_node;					                           /// Naming as  c_node



/************************Data type word node***************************/
typedef struct w_n{

    char *word;                              /// Word

    int w_place;                            /// place before sorting

    int w_count;                            /// no of words

    struct w_n *w_next;                     /// address of the next node

}w_node;                                    /// Naming as w_node



/********************** ALL FUNCTIONS*********************************/
int com_argu(int c,char **s);                         ///com_argu >> command line argument spliter function

int char_check(char c);                             /// Check the character existence in the list

int word_check(int ind,char *str);                 /// To check the word existence in the list

int store(FILE **file_ptr);                       /// store function to store character or words in a linked list

void open_file(char **file_name);                 /// Open file to open all text file and close them after they were stored

void list_sorter();                               /// Function to stort them in decending order

int Draw();                                       ///Function to plot the graph

long max_length();                                /// function to get the max length word


/*************************Global Variables *********************************/

int total=0; 				                     	///total character or words

c_node *c_head;                           /// declaring both heads as global variables
w_node *w_head;

int col=10;					                       /// Default columns

char method='w';				               /// Default method 'c'

int p=1;					                   /// PLACE Indicating it first occurence place in the text

int scale=0;					               /// If scale =1 if scale flag added or 0

char *file_name[30];                  /// Intializing the file_name array to store the file names


/**************************************************MAIN FUNCTION********************************/

int main(int argc,char **argv){

  int error;                                                                    // To deferntiate the error type

    error=com_argu(argc,argv);

  if(error==1){
      printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);    //Errors except emptyfile and file is not exists
      return 0;
    }
  else if(error ==-1){                                                                        //Error when the given file is not exist
    return 0;
  }

if (method=='c'){

c_head=(c_node *)malloc(sizeof(c_node));          		                          /// Creating c_head node for reference.
c_head->c_next=NULL;						                                                /// Initializing c_head pointer as NULL
	}

else{
	w_head=(w_node *)malloc(sizeof(w_node));                                       //Creating w_head node for reference.
	w_head->w_next=NULL;
}


for(int i=0;file_name[i]!=NULL;i++){

if(fopen(file_name[i],"r")==NULL){                                      //Checking that file exist or not
  printf("Cannot open one or more given files\n");
  return 0;
}
fclose(fopen(file_name[i],"r"));
}

open_file(file_name);                                                           //Calling open function to open every text file and store in linked list

if(total==0){                                                                   //If the given files are empty
  printf("No data to process\n");
  return 0;
}

list_sorter();                                                                  // Calling list_sorter to sort the words/characters
/*
for(w_node *current=w_head->w_next;current!=NULL;current=current->w_next){

  printf("%s--%d\n",current->word,current->w_count);

}*/
//printf("%d\n",total);


Draw();                                                                         //Calling Daraw function to plot the graph

      	return 0;   						                                                /// End Of main Function
}



/*************************************Command Line argument spliting***********************************/

int com_argu(int argc1,char **argv1){

  int index=0;                                                          // Index variable for file_name array
  char option;
  int both=0;                                                          // to solve the error when giving both -c -w

	if(argc1==1){						                                              ///If no arguments were given.

printf("No input files were given\n");                                  //Error message
return 1;
}

else{
for(int i=1;i<argc1;i++){                                               ///Loop until the end of the argument

  if(argv1[i][0]=='-'){
                                                                        //Flags (They start with "-")
  // First check for length [-l]
    if(strcmp(argv1[i],"-l")==0){                                       ///first check -l

    if(argv1[i+1]==NULL){                                               // Check whether the next argument is NULL or Not

      printf("Not enough options for [-l]\n");                         /// Error message
      return 1;                                                        /// Return 1 and end the function
    }

    //Check the next argument is 0
    else if(strlen(argv1[i+1])==1 && argv1[i+1][0]=='0'){
      col=0;                                                          ///col as 0
      return -1;

    }
    //check the next arugument is a string or character.
    else{

      col=atoi(argv1[i+1]);

      if(col<0){                                                          //check it is negative or not

        printf("Invalid option(negative) for [-l]\n");
        return 1;
      }

      for(int j=0;j<strlen(argv1[i+1]);j++){


        if(argv1[i+1][j]>'9' || argv1[i+1][j]<'0' ){
          printf("Invalid options for [-l]\n");                       //Error message
          return 1;

        }
      }//Loop end
    }
  }
  ///Length finished

// For character or word [c/w]

    else if(strcmp(argv1[i],"-w")==0){                              //If method=w

      if(option=='c'){
        both=1;
      }
      option='w';
    }

    else if(strcmp(argv1[i],"-c")==0){                            // If method =c
      if(option=='w'){
        both=1;
      }
      option='c';
      method='c';
    }

  else if(strcmp(argv1[i],"--scaled")==0){
      scale=1;

    }

  else{
        printf("Invalid option [%s]\n",argv1[i]);                //If any other options
        return 1;
        }

  } //End of flags

  else if(strcmp(argv1[i-1],"-l")!=0){

    *(file_name+index)= argv1[i];
    index++;

  }
}
*(file_name+index)= NULL;                                             //Last as NULL

if(file_name[0]==NULL){                                                  // If no files were given
  printf("No input files were given\n");
  return 1;
}
}
if(both==1){
        printf("[-c] and [-w] cannot use together\n");            //When both arguments were given
        return 1;
      }

return 0;
} //End of function

/*******************Check function to check wether the character or word already exist******/

/****************************************For characters************************************/

int char_check(char c){

  int return_val=0;

  for(c_node *current=c_head;current!=NULL;current=current->c_next){              ///Checking in the whole list

    if(tolower(c)==current->character){
      current->c_count++;                                                         /// Already exist the c_count will increase
        return_val=1;                                                             /// If already exist return_val =1
        break;
    }
}
  return return_val;                                                              /// returning val
}


/************************check function for For Word ************************************/

int word_check(int ind,char str_array[]){

	int return_val=0;

	for(w_node *current=w_head->w_next;current!=NULL;current=current->w_next){

	  if(strcmp(current->word,str_array)==0){                                              /// comparing the two string arrays

	  current->w_count++;
    total++;
		return_val=1;

		break;
	}

}
	return return_val;
}                                                                              /// end of word_check


/******************************************Storing word or character******************************************/

int store(FILE **file_ptr){

/***************************************** method to storing the character************************************/

    if (method== 'c' ){

      char c;                                                        /// char variable used to   tempraily store data.

      while((c=fgetc(*file_ptr))!=EOF){                              //// LOOP until the End Of FILE
                                                                              /// O`nly store alpha numeric characters
          if(('a'<=tolower(c) && tolower(c)<='z')|| ('0'<=c && c<='9')){

              if(char_check(c)==0){                                 /// If char_check==o then c is not in the linked list

                  c_node *current=(c_node*)malloc(sizeof(c_node));  /// creating a new current node. only if c is not exist in a nodes

		  current->character=tolower(c);		     /// Storing lower case character in the current node.

		  current->c_count=1;

		  current->c_place=p;				      ///Place
		  p++;
		                                          	      ///Linking the nodes process
      current->c_next=c_head->c_next;
      c_head->c_next=current;			                      /// End of linking

                }

	total++;						      /// counting Total number ocharacter

	  }
        }
}                                                                     ///End of charcter storing


/*************************************************Storing words****************************/

else{

    char c;                                                                     /// char variable used to   tempraily store data.
    int ind=0;	                                                               /// Index

    char *str=(char *)malloc(70);
    str[0]='\0';

      while((c=fgetc(*file_ptr))!=EOF){                              	       /// LOOP until the End Of FILE

	if(('a'<=tolower(c) && tolower(c)<='z')|| ('0'<=c && c<='9')){	      /// Only Alpha-numeric

    str[ind]=tolower(c);

    ind++;

	}

	else if((c==' ' || c=='\n' ) && (str[0]!='\0')){		           ///spliting them

	str[ind]='\0';

	if((word_check(ind,str)==0) && str[0]!='\0'){				/// If the wordnotexistin the list.

	w_node *current=(w_node*)malloc(sizeof(w_node));                 	     ///  New node

  	current->word=str;

	str=(char *)malloc(70);

	str[0]='\0';
       current->w_count=1;                                           		/// Initializing count

	current->w_place=p;				                                            ///Place
	p++;

      current->w_next=w_head->w_next;                            		   /// Linking
      w_head->w_next=current;				                                 /// End of linking of words
      total++;
	}
  	ind=0;                                                         		/// Make index as 0 for next turn

		}
}
/*for(w_node *w=w_head->w_next;w!=NULL;w=w->w_next){
  total=total+w->w_count;

}*/

  }
	return total;                                               		/// Returnnig the total number of words or characters.

}                                                            		 	///End of store function


/***********************************************OPEN FILE ***********************************************/
void open_file(char **file_name){

  for(int i=0;file_name[i]!=NULL;i++){

	  FILE* file_ptr;

	file_ptr = fopen(file_name[i],"r");

	store(&file_ptr);

 	 fclose(file_ptr);

  }
}


/******************************************list_sort function**********************************/

void list_sorter(){

/*************For Character******************/

	if(method =='c'){

	for(c_node *current=c_head;current->c_next!=NULL;current=current->c_next){

	c_node *left=current->c_next;

	for(c_node *right=left;right!=NULL;right=right->c_next){

	///if right side > left side
	if((right->c_count > left->c_count) || ((right->c_count==left->c_count) && (right->c_place < left->c_place)))
{

	char temp_c;
	int temp_p;
	int temp_count;								       /// temmpirary variables to save the data

	temp_c=right->character;						/// Swap right datas  to left

	temp_p=right->c_place;

	temp_count=right->c_count;

	right->character=left->character;
	right->c_place=left->c_place;
	right->c_count=left->c_count;

	left->character=temp_c;
	left->c_place=temp_p;
	left->c_count=temp_count;

		}
}
}
}else{

/***********For word*****************/

	for(w_node *current=w_head;current->w_next!=NULL;current=current->w_next){

	w_node *left=current->w_next;

	for(w_node *right=left;right!=NULL;right=right->w_next){

	//printf("%d - %d\n",left->w_count,right->w_count);
	///if right side > left side
	if((right->w_count > left->w_count) || ((right->w_count==left->w_count) && (right->w_place < left->w_place)))

{
										                                          /// temmpirary variables to save the data
	char temp_w[50];
	strcpy(temp_w,right->word);						                    /// Swap right datas  to left

	int temp_p=right->w_place;

	int temp_count=right->w_count;

	strcpy(right->word,left->word);
	right->w_place=left->w_place;
	right->w_count=left->w_count;

	strcpy(left->word,temp_w);
	left->w_place=temp_p;
	left->w_count=temp_count;

			}
								}
										}
	}
}


/*******************************Max string length******************************/
long max_length(){

long max_len=0;
w_node *current=w_head->w_next;
for(int i=0;i<col;i++){
	if(current==NULL){
	break;
}
	max_len=(max_len<strlen(current->word)) ? strlen(current->word) : max_len;
	current=current->w_next;
}

	return max_len;
	}



/********************************Draw Function ********************************/
int Draw(){					//Function to draw the chart!

  printf("\n");

  int return_val=0;

	if(method=='c'){
	int box;
  int sub=0;
	int max_count=c_head->c_next->c_count;
	c_node *current=c_head->c_next;

  double percentage=((double)c_head->c_next->c_count/total)*100;		//Perecentage;

  if(percentage>=10.00 && percentage<100.00){
  sub=1;
  }

else if(percentage==100.00){
sub=2;
}

	for(int i=0; i<col;i++){

percentage=((double)current->c_count/total)*100;		//Perecentage;

/***************************Finding no of boxes*******************************/


	if(scale==0){							                       // if not scaled
	box=(int)((double)((71.0-sub)/total)*current->c_count);

	}

	else{								                          // if scaled
	box=(int)((double)((71.0-sub)/max_count)*current->c_count);
	}
	//printf("%d",box);

/// One Bar
	for(int j=0;j<3;j++){

//One Line
	if(j==1){							                     //Middle Line

		printf(" %c "vert,current->character);

		for(int k=0;k<box;k++){

		printf(fill);

		}
		printf("%.2f%%",percentage);
		}
		else{							                    //up and down lines

		printf("   "vert);

		for(int k=0;k<box;k++){

		printf(fill);

		}

		}
		printf("\n");						           //Printing new line after one line finishes.

		}

		printf("   "vert"\n");						//Printing new line after one bar finishes.

		if(current->c_next==NULL){

		break;

			} else{

		return_val=0;
			}

		current=current->c_next;

}
/********************************** Drawing horizontal line*******************************/

	printf("   "conect);

	for(int i=0; i<76;i++){
	printf(hori);
	}
	printf("\n");
//***************************************************************************************



}
/****************************** for word**************************************/
else{


	int box;
  int sub=0;
	int max_count=w_head->w_next->w_count;
	w_node *current=w_head->w_next;
	long max_len=max_length();

  double percentage=((double)w_head->w_next->w_count/total)*100;		//Perecentage;

  if(percentage>=10.00 && percentage<100.00){
  sub=1;
  }

else if(percentage==100.00){
sub=2;
}

	for(int i=0; i<col;i++){

    percentage=((double)current->w_count/total)*100;		//Perecentage

	/**********Finding no of boxes***************/

	if(scale==0){							// if not scaled
	box=(int)((double)((72.0-max_len-sub)/total)*current->w_count);

	}

	else{								// if scaled
	box=(int)((double)((72.0-max_len-sub)/max_count)*current->w_count);
	}
	//printf("%d",box);



	/// One Bar
	for(int j=0;j<3;j++){

/*****************************One Line****************************************/

/*****************************Middle Line**************************************/
	if(j==1){

		printf(" %s",current->word);
		for(long i=max_len-strlen(current->word);i>=0;i--){
		printf(" ");

		}
		printf(vert);

		for(int k=0;k<box;k++){

		printf(fill);

		}
		printf("%.2f%%",percentage);

		}
/****************************up and down lines************************************/
		else{

		printf("  ");

		for(long i=max_len;i>0;i--){
		printf(" ");

		}
		printf(vert);

		for(int k=0;k<box;k++){

		printf(fill);

		}

		}
		printf("\n");						//Printing new line after one line finishes.
		}
/******************BAR FINISHED******************************/

							//Printing new line after one bar finishes.
		for(long i=max_len+1;i>=0;i--){
		printf(" ");

		}
		printf(vert"\n");

		if(current->w_next==NULL){

		break;

			} else{

		return_val=0;
			}
		current=current->w_next;
}

/********************************** Drawing horizontal line*******************************/

	for(long i=max_len+1;i>=0;i--){
		printf(" ");

		}
		printf(conect);


	for(int i=0; i<77-max_len;i++){
	printf(hori);
	}
	printf("\n");

}
return return_val;
}

//end of Draw 1

