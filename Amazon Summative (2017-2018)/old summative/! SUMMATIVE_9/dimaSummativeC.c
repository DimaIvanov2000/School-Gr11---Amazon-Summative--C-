/***************************************************************

    Electronics Shopping Software
    Programmed in C
    By: Dima Ivanov

    Summative assignment for ICS3U (Ms.Payne)

    Started:  Dec 18, 2017
    Due Date: Jan 24, 2018

    Submitted Items:
    - C File
    - Corresponding Text Files
    - Google Doc with code
    - Additional PDF

    A program of a mock shopping software for electronics

    **************************************
    Admin code is "ALPHA" (case-sensitive)
    **************************************


    Things that this program does:
    - Provides a way for users to create accounts with unique usernames
    - grants additional capabilities to users which acquire administrator status
        - editing / deleting others' items
        - changing others' passwords / admin statuses
    - allows users to create products which are put on the market place
    - grants owners of said items the ability to edit or delete them at will
    - allows other users to permanently buy items in an "add to cart" style

***************************************************************/

/*
    SOME NOTES BEFORE BEGINNING TO READ THE CODE:

    menus often follow this structure:

    do
    {
        clear screen with system("cls");
        printf menu stuff including options (maybe with a loop)
        get char input with input=getch();
        switch(input)
        {
            case 72:
                cursor moves up
                break;
            case 80:
                cursor moves down
                break;
            case 13 or whatever:
                do stuff based on selection, often leading to more menus of a similar nature
                break;
        }
    }
    while(input != character(s) indicating exit);

    these variables are declared for menus:
    - int i,j  -  random counters, not just for menus
    - char input  -  stores user's last input
    - int selection  -  remembers which option is selected

    These show up in almost very function and are thus not commented on
*/

/** PASSWORD DECRYPTION **//**

       0 1 2 3 4 5 6 7
    0  a i q y G O W 4
    8  b j r z H P X 5
    16 c k s A I Q Y 6
    24 d l t B J R Z 7
    32 e m u D K S 0 8
    40 f n v E L T 1 9
    48 g o w E M U 2
    56 h p x F N V 3

    add rows and columns and convert number to hex

    T = 40 + 5 = 45 = 2D

    this is what is found in the file
**/

/* Header Includes */

#include<stdio.h>
#include<windows.h>
#include<math.h>

/* Structure Declarations */

//Structure for a product, most should be self-explanatory:
typedef struct
{
    char type[31];
    char name[51];
    char brand[31];
    int priceCents;
    char desc[501]; //description

    //user may choose to add specs to an item, max 20:
    int numSpecs;
    char specNames[20][31];
    char specs[20][51];

    int stock;
    char ownerName[31];
    char dateCreated[101];
    char dateEdited[101];
    int numInFile; //what order the product is found in in allProducts.txt, this variable is not saved but is set in loading the product
}
product;

//Structure for a user:
typedef struct
{
    char uName[31]; //username
    int hasAdmin; //0 or 1
    int numOwnedProducts; //loaded from user's productInfo.txt file
}
user;

/* Global Variables */

product loadedProducts[5][5]; //an array of all currently loaded products, organized as: [page][number]
product tempProduct; // a temporary product structure which is used by all functions manipulating products
product tempProduct2; // another temp product used only by the editing function
int numLoadedProducts=0; // how many products are currently loaded (used in loops)
int numProducts=0;  //total number of products existing, used to avoid errors (and more loops)
user curUser; //info of the current user
user tempUser; //a temp user mainly used when admin changes others' profiles

/* Function Prototypes */

/** NOT FUNCTIONING IN PROGRAM **/
int initialCheck(void);
/********************************/

//Names should mostly explain what these functions do, see declarations for clarification

int displayTitle(void);
int loginScreen(void);
int createAccount(void);

void loadNextProduct(FILE*);
void skipNextProduct(FILE*);
int loadProductPage(int);
int loadUserProductPage(int);

int getNumProducts(void);
int setNumProducts(int);
int getNumProductsUser(char*);
int setNumProductsUser(char*,int);
void mainAccountView(void);
    void buyerView(void);
    void sellerView(void);
        void createProduct(void);
        void editProducts(void);
        void editOneProduct(void);
        void removeProduct(int);
    void acquireAdmin(void);
    void adminView(void);
void getStr(FILE*,char*,int);
void getInt(FILE*,int*);

int fcreate(char*);

//accessory functions, used for show
void setColor(int);
void setColor2(int);
void cprintf(int,char*);
void cprinti(int,int);
void cprintc(int,char);

/* MAIN */
int main(void)
{
    //very simple, loop while the user continues further in menus, stop when user exits menus

    while(displayTitle()==1)
    {
        if(loginScreen()==1)
        {
            mainAccountView(); // this function takes over from here (admittedly not great organization-wise but it works)
        }
    }

    setColor(15); // revert color before exiting
    return 0;
}
/* Function Declarations */


/*
    Function:
    Does:
    Local Variables used:
    -
    -
    -
    Receives:
    Returns:
*/

/*******************
    THE FOLLOWING FUNCTION HAS BEEN OMITTED FROM THE FINAL VERSION
    The function "initialCheck" serves as a view of what could have been if time was better invested into this project.
    The original intent of this project was to maximize user friendliness but user friendliness ultimately had to be
    somewhat lessened in order to meet the deadline. In short, this function looked for all the necessary files for
    the program to operate normally, warned the user if one was missing and had the capability to create a workable
    environment in an empty directory.
*******************/

/*
    Function: initialCheck
    Does:

    Runs a quick check to see if all the basic files needed for the program to operate exist, has options for managing the situation if need be.

    Local Variables used:

    - int i : miscellaneous counter, used for cycling through string arrays to print stuff out
    - char input : stores user's input after acquiring it
    - int selection : remembers which option user has currently highlighted for reference if enter is pressed
    - char options[][] : an array of strings that acts as a list of options to be printed out for the user
    - int filesAreLocated[] : corresponds with tryFiles[][], records which files were found (1 or 0)
    - int tryFiles[][] : list of names of the files to search for
    - FILE*fp : file pointer

    Receives:

    * nothing

    Returns:

    int 0 or 1 depending on success
*/

int initialCheck(void)
{
    int i;
    char input;
    int selection=0;
    char options[3][61]={"Do Not Recreate","Try Recreating Missing Files","Create Empty File Setup from Scratch [CLEAR DIRECTORY]"};
    int filesAreLocated[2]={0};
    char tryFiles[2][51]={"products//generalProductInfo.txt","users//generalUserInfo.txt"};
    FILE*fp;
    for(i=0;i<2;i++)
    {
        fp=fopen(tryFiles[i],"r");
        if(fp==NULL)
        {
            filesAreLocated[i]=0;
        }
        else
        {
            filesAreLocated[i]=1;
        }
        fflush(fp);
        fclose(fp);
    }
    if(filesAreLocated[0]==0||filesAreLocated[1]==0)
    {
        do
        {
            system("cls");
            cprintf(12,"\n\tSome key files are missing or cannot be accessed:\n\n");
            setColor(14);
            for(i=0;i<2;i++)
            {
                if(filesAreLocated[i]==0)
                {
                    printf("\t - %s\n",tryFiles[i]);
                }
            }
            cprintf(11,"\n\tInaccessability of files may be a result of missing or inaccessable parent directories\n");
            printf("\tWould you like to attempt to recreate these files?\n");
            printf("\t - Consider emptying directory and starting from scratch\n\t - Consider backing up current files\n\t - Consider attempting manual fix\n\n");
            for(i=0;i<3;i++)
            {
                if(selection==i)
                {
                    cprintf(10,"\t > ");
                }
                else
                {
                    cprintf(15,"\t   ");
                }
                printf("%s\n",options[i]);
            }
            input=getch();
            switch(input)
            {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<2)
                {
                    selection++;
                }
                break;
            case 13:
                if(selection==1)
                {
                    for(i=0;i<2;i++)
                    {
                        if(filesAreLocated[i]==0)
                        {
                            if(fcreate(tryFiles[i])==0)
                            {
                                setColor(12);
                                printf("\n\t * %s was not created successfully",tryFiles[i]);
                            }
                            else
                            {
                                setColor(10);
                                printf("\n\t * %s created successfully",tryFiles[i]);
                            }
                        }
                    }
                }
                else if(selection==2)
                {
                    mkdir("products",0700);
                    mkdir("users",0700);
                    if(fcreate("products//generalProductInfo.txt")==0)
                    {
                        printf("\t***Unknown Issue Encountered\n\tFailed to create generalProductInfo\n\n\tEXITING...\n\n");
                        return 0;
                    }
                    if(fcreate("users//generalUserInfo.txt")==0)
                    {
                        cprintf(12,"\t***Unknown Issue Encountered\n\tFailed to create generalUserInfo");
                        cprintf(14,"\n\n\tEXITING...\n\n");
                        return 0;
                    }
                    cprintf(10,"\n\n\tAll files created successfully, please restart...");
                }
                break;
            }
        }
        while(input!=27&&input!=8&&input!=13);
        cprintf(14,"\n\n\tEXITING...\n\n");
        getch();
        return 0;
    }
    return 1;
}

/*******************
    END OF UNUSED CODE
*******************/


/*
    Function: displayTitle
    Does:

    Shows the title and awaits a user input, returning 0 if user exits (esc) and 1 otherwise

    Local Variables used:

    - int title[][] : list of (color, character) combinations to be printed out in a loop to form the title (for color, see setColor function)
    - int i,j : misc counters used to print out the title

    Receives:

    * nothing

    Returns:

    int 0 or 1 depending on user exit status
*/

int displayTitle(void)
{
    int title[7][62]={
        {15,219,15,219,252,220,252,220,252,220,252,220,252,220,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219},
        {15,219,204,219,204,219,15,219,252,220,252,223,204,219,204,219,15,219,0,219,15,220,0,219,15,220,0,219,15,219,15,220,15,220,0,219,15,219,15,220,15,220,0,219,15,219,0,219,15,220,0,219,15,219,0,219,15,220,0,219,15,219},
        {15,219,204,219,204,219,15,219,252,220,252,223,204,219,204,219,15,219,0,219,15,219,0,219,15,219,0,219,15,219,0,219,15,220,0,219,15,219,15,223,15,220,15,219,15,219,0,219,15,219,0,219,15,219,0,219,15,219,0,219,15,219},
        {15,219,252,223,204,219,252,220,252,220,204,219,204,219,252,223,15,219,15,220,15,219,15,220,15,219,15,220,15,219,15,220,15,220,15,220,15,219,15,220,15,220,15,220,15,219,15,220,15,220,15,220,15,219,15,220,15,219,15,220,15,219},
        {15,219,15,219,15,219,15,219,15,219,15,219,15,219,254,223,254,220,254,220,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,254,223,254,223,14,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219},
        {15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,254,223,254,223,14,219,14,219,254,220,254,220,254,220,254,220,14,219,254,223,15,219,254,223,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219,15,219},
        {15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223,15,223}
    };
    int i,j;

    system("cls");

    //Printing Title with loops and some additional printf's
    //******
    cprintf(15,"\n\tWelcome to...\n\n\n"); //welcome message
    for(i=0;i<7;i++)
    {
        printf("\t");
        for(j=0;j<31;j++)
        {
            cprintc(title[i][2*j],title[i][2*j+1]);
        }
        printf("\n");
    }

    cprintf(10,"\n\n\tPress any key to continue..."); //welcome message
    //******

    //Receiving user input and returning different values depending on whether they exited or not
    //******
    if(getch()==27)
    {
        system("cls");
        cprintf(15,"\n\tYou are now leaving...\n\n\n");
        for(i=0;i<7;i++)
        {
            printf("\t");
            for(j=0;j<31;j++)
            {
                cprintc(title[i][2*j],title[i][2*j+1]); //reprint title when exiting to accommodate new exit message
            }
            printf("\n");
        }

        cprintf(15,"\n\n\tThank you for visiting!"); //new exit message
        getch();
        return 0;
    }
    else
    {
        return 1;
    }
    //******

}

/*
    Function: loginScreen
    Does:

    Presents user with an interactive login screen with options to enter their credentials or create an account (not compensate forgotten passwords, that option was scrapped but funny enough to keep)

    Local Variables used:

    - int i,j : misc counters used for various simple loops
    - char input : stores user's input after acquiring it
    - int selection : remembers which option user has currently highlighted for reference if enter is pressed
    - char curUsername[] : stores the username which the user has entered so far
    - char curPassword[] : stores the password which the user has entered so far
    - int placeInStringUN : for taking in a string (username) from user, keeps track of what char array element to fill next upon input
    - int placeInStringPW : for taking in a string (password) from user, keeps track of what char array element to fill next upon input
    - char topMessages[][] : a list of messages which may be displayed at the top of the screen when it is reprinted (e.g. "welcome" or "try again")
    - int topMessageColors[] : list of colors corresponding to the top messages
    - int topMessage : number to keep track of what message to display (changed when the user enters invalid credentials)
    - FILE*fp : file pointer
    - char userInfoDirectoryName[] : a long string which is manipulated with sprintf so it can then be used as the name of a file being opened (e.g. adding ".txt" to a username)
    - char c : temporary variable holds last char retrieved from file
    - int passwordMatches : keeps track of whether or not the given password matched the stored one after the checking loop has finished

    Receives:

    * nothing

    Returns:

    int 0 or 1 depending on the success of the user in inputting their credentials (0 = they exited)
*/

int loginScreen(void)
{
    int i,j;
    char input;
    int selection=0;
    char curUsername[31]={0};
    char curPassword[31]={0};
    int placeInStringUN=0;
    int placeInStringPW=0;
    char topMessages[2][51]={"Please Input your User Credentials:","Credentials Invalid, Please Try Again:"};
    int topMessageColors[2]={15,12};
    int topMessage=0;
    FILE*fp;
    int userInfoDirectoryName[81]={0};
    char curPasswordChar;
    char c;
    int passwordMatches;

    do
    {
        system("cls");
        printf("\n\n\t");
        cprintf(topMessageColors[topMessage],topMessages[topMessage]);
        printf("\n\n");

        //long list of if statements followed by printf's (if statements set color and print an arrow or blank space based on whther an option is selected)
        //******
        if(selection==0)
        {
            cprintf(11,"\t > ");
            printf("Username: %s\n",curUsername);
        }
        else
        {
            cprintf(15,"\t   ");
            printf("Username: %s\n",curUsername);
        }
        if(selection==1)
        {
            cprintf(11,"\t > ");
            printf("Password: %s\n\n",curPassword);
        }
        else
        {
            cprintf(15,"\t   ");
            printf("Password: %s\n\n",curPassword);
        }
        if(selection==2)
        {
            cprintf(10,"\t > Done\n\n");
        }
        else
        {
            cprintf(15,"\t   Done\n\n");
        }
        if(selection==3)
        {
            cprintf(14,"\t > Forgot Password\n");
        }
        else
        {
            cprintf(15,"\t   Forgot Password\n");
        }
        if(selection==4)
        {
            cprintf(10,"\t > Create New Account");
        }
        else
        {
            cprintf(15,"\t   Create New Account");
        }
        printf("\n");
        //******

        input=getch(); //taking user input and saving it in variable

        switch(input) //deciding what to do based on input
        {
            case 72: //up arrow key, decrease selection if it is not minimal (0)
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down arrow key, increase selection if it is not at maximum (4, 5 options)
                if(selection<4)
                {
                    selection++;
                }
                break;
            case 13: // user pressed enter
                switch(selection) //decide what to do next based on what was selected
                {
                    //1st selection, user wants to input/change their username

                    //The following code follows this basic structure:
                    //clear the screen
                    //print a prompting message as well as the currently entered string and any other options which are not currently selected for extra effect
                    //get the user's input
                    //exit if it is enter or esc (user is done typing, check of validity is not performed yet)
                    //delete character if it is backspace (8)
                    //add character if it is valid (uppercase letters, lowercase letters, numbers, or a space)

                    //******
                    case 0:
                        do
                        {
                            system("cls"); //clear
                            cprintf(15,"\n\n\tPlease Input Your Account Username:\n\n\t   "); //print...
                            printf("Username: %s",curUsername); //print...
                            if(placeInStringUN<30) //print an underscore if you have not reached the character limit
                            {
                                printf("_");
                            }
                            cprintf(8,"\n\t   ");
                            printf("Password: %s\n\n\t   Done\n\n\t   Forgot Password\n\t   Create New Account\n",curPassword); //print...
                            input=getch();
                            if(input==8&&placeInStringUN>0)//checking for backspace and validity of using backspace
                            {
                                placeInStringUN--; //go back a space
                                curUsername[placeInStringUN]=0; //remove latest character
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInStringUN<30) //checks for valid characters and character limit
                            {
                                curUsername[placeInStringUN]=input; //put a character
                                placeInStringUN++; //next space
                            }
                        }
                        while(input!=13&&input!=27);
                        input=0; //reset input so the whole menu loop does not exit
                        break;
                    //******

                    //2st selection, user wants to input/change their password, very similar to previous case
                    //******
                    case 1:

                        //VERY similar, just some variables changed around

                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease Input You Account Password:\n\n");
                            cprintf(8,"\t   ");
                            printf("Username: %s\n",curUsername);
                            cprintf(15,"\t   ");
                            printf("Password: %s",curPassword);
                            if(placeInStringPW<30)
                            {
                                printf("_");
                            }
                            cprintf(8,"\n\n\t   ");
                            printf("Done\n\n\t   Forgot Password\n\t   Create New Account\n",curPassword);
                            input=getch();
                            if(input==8&&placeInStringPW>0)
                            {
                                placeInStringPW--;
                                curPassword[placeInStringPW]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))&&placeInStringPW<30)
                            {
                                curPassword[placeInStringPW]=input;
                                placeInStringPW++;
                            }
                        }
                        while(input!=13&&input!=27);
                        input=0;
                        break;
                    //******

                    // user wants to submit their credentials to be evaluated
                    //******
                    case 2:

                        //making sure that neither the username nor password is nothing
                        // (first character is not the end of the string or 0 or '/0')
                        //******
                        if(curUsername[0]==0||curPassword[0]==0)
                        {
                            topMessage=1; // invalid, change top message appropriately
                            break; // return to inputing credentials menu
                        }
                        //******

                        //making sure that the username is correct (the user exists)
                        //******
                        sprintf(userInfoDirectoryName,"users//%s//userInfo.txt",curUsername); //the username is converted to the full directory name
                        fp=fopen(userInfoDirectoryName,"r"); //file is opened
                        if(fp==NULL) // checking if file exists
                        {
                            fflush(fp); //close the non existant file (precaution)
                            fclose(fp);
                            topMessage=1; // file does not exist, change top message appropriately
                            break; // return to inputing credentials menu
                        }
                        //******

                        //checking to see if password matches that in file
                        //******
                        i=0; // counter reset since a for loop is not used
                        passwordMatches=1; // flag variable reset, assumes the password matches
                        c=fgetc(fp); // get the first character for reference
                        while(c!=10) // loop while the character is not enter (the string has ended)
                        {
                            //conversion of characters in file to usable characters
                            // [SEE DECODING METHOD AT TOP OF CODE]
                            //******

                            //convert the gotten character into the first 1/2 byte of a value (to be the usable character)
                            if(c>=65)//character is a letter A-F
                            {
                                curPasswordChar=(c-55)*16;
                            }
                            else //character is a number 0-9
                            {
                                curPasswordChar=(c-48)*16;
                            }
                            //get next character
                            c=fgetc(fp);
                            //convert the gotten character into the second 1/2 byte of the value
                            if(c>=65)
                            {
                                curPasswordChar+=c-55;
                            }
                            else
                            {
                                curPasswordChar+=c-48;
                            }
                            //switch bits 1-3 of value with bits 4-6 of value
                            // [SEE DECODING METHOD]
                            curPasswordChar=((curPasswordChar%8)*8)+(curPasswordChar/8);
                            // divide also used as floor due to properties of an int variable which are convenient in this case
                            //******
                            curPasswordChar=curPasswordChar-((curPasswordChar/26)*58)+((curPasswordChar/52)*15)+97;
                            if(curPasswordChar!=curPassword[i]) //check if the character matches that of the entered password
                            {
                                passwordMatches=0; // if it does not match, set the flag variable to false
                                break; //no need to check the rest of the string
                            }
                            i++; //increment counter
                            c=fgetc(fp); //get the next character
                        }
                        //******

                        if(c==10) //if it is enter
                        {
                            curUser.hasAdmin=fgetc(fp)-48;
                            //if the loop found an enter (got to the end of the string), get the admin status because
                            //its convenient the only way the password can be invalid now is if the entered password is too long
                            //and in that case the admin status will just be reloaded next attempt anyways

                            //tl;dr : might as well load the admin now, no harm in it
                        }
                        fflush(fp); // close the file
                        fclose(fp);
                        if(passwordMatches==0||curPassword[i]!=0) //if the password does not match or is too long (respectively)
                        {
                            topMessage=1; // change topmessage to say its invalid
                            break; // return to previous menu
                        }
                        else // password is fine
                        {
                            strcpy(curUser.uName,curUsername); //set the username of the current user to the entered username
                            getNumProductsUser(curUser.uName); //load the number of products that the user has
                            return 1; //return success
                        }
                        break;
                    //******

                    // user forgot their password (too bad), literally just print a message that says "too bad" because this was
                    // a planned feature that turned out to be too complex for the final product
                    //******
                    case 3:
                        system("cls");
                        cprintf(15,"\n\n\tWe're terribly sorry but this service\n\tis unavailable, the programmer is lazy...");
                        getch();
                        break;
                    //******

                    // user wants to create a new account, the createAccount function is called and if the user
                    // successfully creates an account, it counts as a login and the function exits (also returning 1 for success)
                    //******
                    case 4:
                        if(createAccount()==1)
                        {
                            return 1;
                        }
                        break;
                    //******
                }
                break;
        }
    }
    while(input!=27);
    return 0; //the user exited from the menu, return 0 telling main to return to previous menu
}

/*
    Function: createAccount
    Does:

    Allows the user to create an account with a unique username and a 10-char password through a menu
    creates the account's directory and files which store the admin status, the number of products owned, and an ENCRYPTED PASSWORD

    Local Variables used:

    - int i,j
    - char input
    - int selection
    - char curUsername[] : remembers the username which the user has so far entered
    - char curPasswords[2][] : like curUsername but with both passwords
    - int placeInStringUN : remembers what element of curUsername to occupy next
    - int placesInStringPW[2] : like placeInStringUN but with passwords
    - char topMessages[][] : a list of the messages which may show up at the top (e.g. welcome or password is too short)
    - int topMessageColors[] : colors of said messages
    - int topMessage : remembers what message to display
    - FILE*fp : file pointer
    - char tempUN[] : temporary string variable
    - int userNameIsSame=0 : flag variable rememebring if usernames matched or not after a check loop
    - char c : stores last character retrieved from file
    - char newUserDirectoryName[] : stores the full name of the users' new directory for fopen

    Receives:

    * nothing

    Returns:

    int 0 or 1 depending on user exit status
*/

int createAccount(void)
{
    int i,j;
    char input;
    int selection=0;
    char curUsername[31]={0};
    char curPasswords[2][31]={0};
    int placeInStringUN=0;
    int placesInStringPW[2]={0};
    char topMessages[5][81]={"Create A New Account by Entering The Following Credentials:","One or More Credentials Were Left Empty, Please Try Again:","Passwords Did Not Match Eachother, Please Try Again:","You Password Should Contain At Least 10 Characters, Please Try Again:","This Username has Been Taken, Please Select a Different Username:"};
    int topMessageColors[5]={15,12,12,12,14};
    int topMessage=0;
    FILE*fp;
    char tempUN[31]={0};
    int userNameIsSame=0;
    char c;
    char newUserDirectoryName[81]={0};

    do
    {
        //standard menu stuff
        system("cls");
        printf("\n\n\t");
        //displaying the appropriate top message
        cprintf(topMessageColors[topMessage],topMessages[topMessage]);
        printf("\n\n");
        //displaying options
        if(selection==0)
        {
            cprintf(11,"\t > ");
            printf("Username: %s\n",curUsername);
        }
        else
        {
            cprintf(15,"\t   ");
            printf("Username: %s\n",curUsername);
        }
        if(selection==1)
        {
            cprintf(11,"\t > ");
            printf("Password: %s\n",curPasswords[0]);
        }
        else
        {
            cprintf(15,"\t   ");
            printf("Password: %s\n",curPasswords[0]);
        }
        if(selection==2)
        {
            cprintf(11,"\t > ");
            printf("Confirm Password: %s\n\n",curPasswords[1]);
        }
        else
        {
            cprintf(15,"\t   ");
            printf("Confirm Password: %s\n\n",curPasswords[1]);
        }
        if(selection==3)
        {
            cprintf(10,"\t > Done\n");
        }
        else
        {
            cprintf(15,"\t   Done\n");
        }
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<3)
                {
                    selection++;
                }
                break;
            case 13:
                switch(selection)
                {
                    case 0:
                        do
                        {
                            //taking in a username, we've seen this before
                            system("cls");
                            cprintf(15,"\n\n\tPlease Input Your Account Username:\n\n\t   ");
                            printf("Username: %s",curUsername);
                            if(placeInStringUN<30)
                            {
                                printf("_");
                            }
                            cprintf(8,"\n\t   ");
                            printf("Password: %s\n\t   Confirm Password: %s\n\n\t   Done",curPasswords[0],curPasswords[1]);
                            input=getch();
                            if(input==8&&placeInStringUN>0)
                            {
                                placeInStringUN--;
                                curUsername[placeInStringUN]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInStringUN<30)
                            {
                                curUsername[placeInStringUN]=input;
                                placeInStringUN++;
                            }
                        }
                        while(input!=13&&input!=27);
                        input=0;
                        break;
                    case 1:
                        do
                        {
                            //taking in password 1
                            system("cls");
                            cprintf(15,"\n\n\tPlease Input Your Account Password:");
                            cprintf(8,"\n\n\t   ");
                            printf("Username: %s\n",curUsername);
                            cprintf(15,"\t   ");
                            printf("Password: %s",curPasswords[0]);
                            if(placesInStringPW[0]<30)
                            {
                                printf("_");
                            }
                            cprintf(8,"\n\t   ");
                            printf("Confirm Password: %s\n\n\t   Done",curPasswords[1]);
                            input=getch();
                            if(input==8&&placesInStringPW[0]>0)
                            {
                                placesInStringPW[0]--;
                                curPasswords[0][placesInStringPW[0]]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))&&placesInStringPW[0]<30)
                            {
                                curPasswords[0][placesInStringPW[0]]=input;
                                placesInStringPW[0]++;
                            }
                        }
                        while(input!=13&&input!=27);
                        input=0;
                        break;
                    case 2:
                        do
                        {
                            //taking in password 2
                            system("cls");
                            cprintf(15,"\n\n\tPlease Input Your Account Password:");
                            cprintf(8,"\n\n\t   ");
                            printf("Username: %s\n\t   Password: %s",curUsername,curPasswords[0]);
                            cprintf(15,"\n\t   ");
                            printf("Confirm Password: %s",curPasswords[1]);
                            if(placesInStringPW[1]<30)
                            {
                                printf("_");
                            }
                            cprintf(8,"\n\n\t   Done");
                            input=getch();
                            if(input==8&&placesInStringPW[1]>0)
                            {
                                placesInStringPW[1]--;
                                curPasswords[1][placesInStringPW[1]]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))&&placesInStringPW[1]<30)
                            {
                                curPasswords[1][placesInStringPW[1]]=input;
                                placesInStringPW[1]++;
                            }
                        }
                        while(input!=13&&input!=27);
                        input=0;
                        break;
                    case 3:
                        //user has pressed 'done', time to check validiy
                        if(curUsername[0]==0||curPasswords[0][0]==0||curPasswords[1][0]==0) // checking if all criteria not empty
                        {
                            topMessage=1; //appopriate top message
                            break;
                        }
                        if(strcmp(curPasswords[0],curPasswords[1])!=0) //checking that passwords match
                        {
                            topMessage=2; //appopriate top message
                            break;
                        }
                        if(curPasswords[0][9]==0||curPasswords[1][9]==0) //checking that passwords are min. 10 characters
                        {
                            topMessage=3; //appopriate top message
                            break;
                        }
                        if(strcmp(curUsername,"generalUserInfo")==0) //PRECAUTION, doesn't do anything really but a user should not be named generalUserInfo anyways
                        {
                            topMessage=4; //appopriate top message
                            break;
                        }
                        fp=fopen("users//generalUserInfo.txt","r"); // open generalUserInfo for reading

                        //loop checking that this username does not already exist
                        //******
                        c=0; //reset at start
                        userNameIsSame=0; //reset flag variable
                        while(c!=-1) //while not eof
                        {
                                getStr(fp,tempUN,30); //see getStr function declaration, its basically a homebrew fscanf
                                if(strcmp(tempUN,curUsername)==0)//comaring usernames
                                {
                                    userNameIsSame=1; //username matches another name, stop
                                    break;
                                }
                                c=fgetc(fp); //get a character, can either be another enter or -1 (eof)
                        }
                        //******
                        fflush(fp); //close file
                        fclose(fp);
                        if(userNameIsSame==1)
                        {
                            topMessage=4; //username is the same, set appropriate message
                            break;
                        }

                        //all factors have been checked, now to create an account

                        fp=fopen("users//generalUserInfo.txt","a"); //open generalUserInfo for appending
                        i=0;//reset

                        //put all the characters of the username into the file with 2 enters and then close it
                        //******
                        while(curUsername[i]!=0)
                        {
                            fputc(curUsername[i],fp);
                            i++;
                        }
                        fputc(10,fp);
                        fputc(10,fp);
                        fflush(fp);
                        fclose(fp);
                        //******

                        sprintf(newUserDirectoryName,"users//%s",curUsername);//add users// to the username
                        mkdir(newUserDirectoryName,0700); //create a directory with the name of the user
                        strcat(newUserDirectoryName,"//UserInfo.txt"); // add //userInfo.txt to the name
                        fp=fopen(newUserDirectoryName,"w"); //create a file under that name
                        i=0;//reset

                        // a loop like the simpler one seen previously to put a string in a file but every time a character is placed it is
                        //firstly translated into the encoded number and put intot he file as two 1/2 bytes of a number in hex
                        //[SEE PASSWORD DECODING]
                        //******
                        while(curPasswords[0][i]!=0)
                        {
                            //complicated algorythm that basically orders letters and numbers in the desired order occupying #0-62
                            //as opposed to the ascii order where they're all over the place
                            j=curPasswords[0][i]-((curPasswords[0][i]/65)*43)-((curPasswords[0][i]/97)*58)+4;
                            j=8*(j%8)+(j/8); //switching bits 1-3 & 4-6

                            //putting in characters
                            if(j/16>=10)//A-F
                            {
                                fputc((j/16)+55,fp);
                            }
                            else//0-9
                            {
                                fputc((j/16)+48,fp);
                            }
                            if(j%16>=10)//A-F
                            {
                                fputc((j%16)+55,fp);
                            }
                            else//0-9
                            {
                                fputc((j%16)+48,fp);
                            }

                            i++;
                        }
                        fputc(10,fp);
                        fputc(48,fp);
                        fflush(fp);
                        fclose(fp);
                        //******

                        //create and empty file called productInfo.txt in the user's directory
                        //******
                        sprintf(newUserDirectoryName,"users//%s//productInfo.txt",curUsername);
                        fp=fopen(newUserDirectoryName,"w");
                        fflush(fp);
                        fclose(fp);
                        //******

                        //basically load the new user into curUser, self explanatory
                        //******
                        curUser.hasAdmin=0;
                        curUser.numOwnedProducts=0;
                        strcpy(curUser.uName,curUsername);
                        //******

                        //display a message about the success
                        //******
                        system("cls");
                        cprintf(10,"\n\n\tAccount successfully created under the name of ");
                        cprintf(11,curUser.uName);
                        cprintf(15,"\n\tNow entering account interface...");
                        getch();
                        //******

                        return 1; //return success
                        break;
                }
        }
    }
    while(input!=27);

    return 0; //return exit
}

/*
    Function: loadNextProduct
    Does:

    loads the next product into tempProduct given an active file pointer

    Local Variables used:

    - int i : a counter

    Receives:

    a file pointer

    Returns:

    * nothing
*/

void loadNextProduct(FILE*fp)
{
    int i;

    //load all the attributes declared in the product structure EXCEPT NUMINFILE, which is loaded outside of the function in other functions for certain conveniences
    // getInt and getStr used to load in ints and char[]s on one line
    getStr(fp,tempProduct.name,50);
    getStr(fp,tempProduct.type,30);
    getStr(fp,tempProduct.brand,30);
    getInt(fp,&tempProduct.priceCents);
    getStr(fp,tempProduct.desc,500);
    getInt(fp,&tempProduct.numSpecs);

    for(i=0;i<tempProduct.numSpecs;i++)//counter used for loop loading all specs, number of specs is variable and is loaded prior
    {
        getStr(fp,tempProduct.specNames[i],30);
        getStr(fp,tempProduct.specs[i],50);
    }

    getInt(fp,&tempProduct.stock);
    getStr(fp,tempProduct.ownerName,30);
    getStr(fp,tempProduct.dateCreated,100);
    getStr(fp,tempProduct.dateEdited,100);
}

/*
    Function: skipNextProduct
    Does:

    skips the next product given an active file pointer

    Local Variables used:

    - int i : a counter
    - int tempNumSpecs : used to skip the right number of specs

    Receives:

    a file pointer

    Returns:

    * nothing
*/

void skipNextProduct(FILE*fp)
{
    int i;
    int tempNumSpecs=0;

    for(i=0;i<5;i++)
    {
        while(fgetc(fp)!=10)
        {

        }
    }
    tempNumSpecs+=(fgetc(fp)-48)*10;
    tempNumSpecs+=fgetc(fp)-48;
    fgetc(fp);
    for(i=0;i<tempNumSpecs;i++)
    {
        while(fgetc(fp)!=10)
        {

        }
    }
    for(i=0;i<4;i++)
    {
        while(fgetc(fp)!=10)
        {

        }
    }
}

/*
    Function: loadProductPage
    Does:

    Loads a page of product for the user to see without picking and choosing any specific products

    Local Variables used:

    int i : counter
    FILE*fp : file pointer to allProducts
    char c : last char from file
    int numLoops : how many products to load based on the total number of products and the requested page number

    Receives:

    * an integer representing the page number to load

    Returns:

    * nothing
*/

int loadProductPage(int pageNum)
{
    int i;
    FILE*fp=fopen("products//allProducts.txt","r");
    char c;
    int numLoops;

    getNumProducts();

    if(numProducts-((pageNum-1)*25)>25)
    {
        numLoops=25;
    }
    else
    {
        numLoops=numProducts-((pageNum-1)*25);
    }

    for(i=0;i<(pageNum-1)*25;i++)
    {
        c=fgetc(fp);
        skipNextProduct(fp);
    }

    for(i=0;i<numLoops;i++)
    {
        c=fgetc(fp);
        if(c==-1)
        {
            break;
        }
        tempProduct.numInFile=i+1;
        loadNextProduct(fp);
        loadedProducts[i/5][i%5]=tempProduct;
    }
    numLoadedProducts=i;

    fflush(fp);
    fclose(fp);
    return 1;
}

/*
    Function: loadUserProductPage
    Does:

    Like loadProductPage but this function only counts the items which belong to the current user

    Local Variables used:

    int i,j : counters
    FILE*fp : file pointer to allProducts
    char c : last char from file
    int numLoops : how many products to load based on the total number of products and the requested page number

    Receives:

    * an integer representing the page number to load

    Returns:

    * nothing
*/

int loadUserProductPage(int pageNum)
{
    int i,j;
    FILE*fp=fopen("products//allProducts.txt","r");
    char c;
    int numLoops;

    if(curUser.numOwnedProducts-((pageNum-1)*25)>25)
    {
        numLoops=25;
    }
    else
    {
        numLoops=curUser.numOwnedProducts-((pageNum-1)*25);
    }
    i=0;
    j=1;
    while(i<numLoops)
    {
        c=fgetc(fp);
        if(c==-1)
        {
            break;
        }
        loadNextProduct(fp);
        if(strcmp(tempProduct.ownerName,curUser.uName)==0)
        {
            tempProduct.numInFile=j;
            loadedProducts[i/5][i%5]=tempProduct;
            i++;
        }
        j++;
    }
    numLoadedProducts=i;

    fflush(fp);
    fclose(fp);
}

/*
    Function: getNumProducts
    Does:

    sets numProducts to whatever number is storedin numProducts.txt for various uses

    Local Variables used:

    FILE*fp : file pointer to numProducts
    char c : last char from file
    int curNum : stores what numProducts is so far

    Receives:

    * nothing

    Returns:

    0 or 1 depending on if the file loaded successfully
*/

int getNumProducts(void)
{
    FILE*fp=fopen("products//numProducts.txt","r");
    char c;
    int curNum=0;

    if(fp==NULL)
    {
        return 0;
    }
    c=fgetc(fp);
    while(c!=-1)
    {
        curNum=(curNum*10)+c-48;
        c=fgetc(fp);
    }
    numProducts=curNum;
    fflush(fp);
    fclose(fp);
    return 1;
}

/*
    Function: setNumProducts
    Does:

    uses numProducts to rewrite the numProducts.txt file to contain that number

    Local Variables used:

    int i : counter
    FILE*fp : file pointer to numProducts

    Receives:

    * nothing

    Returns:

    0 or 1 depending on if the file loaded successfully
*/

int setNumProducts(int num)
{
    int i;
    FILE*fp=fopen("products//numProducts.txt","w");

    if(fp==NULL)
    {
        return 0;
    }

    if(num==0)
    {
        fputc(48,fp);
    }
    else
    {
        for(i=(int)log10((double)num);i>=0;i--)
        {
            fputc(((num/(int)(pow(10,(double)i)))%10)+48,fp);
        }
    }

    fflush(fp);
    fclose(fp);
    return 1;
}

/*
    Function: getNumProductsUser
    Does:

    recieves the name of a user and loads how many products they own from their file into curUser.numOwnedProducts

    Local Variables used:

    char fullName[101] : for fopen
    char c : last char from file
    int curNum : what the number of oned products is so far
    FILE*fp : file pointer

    Receives:

    a username as a char*

    Returns:

    * nothing
*/

int getNumProductsUser(char*uName)
{
    char fullName[101];
    char c;
    int curNum=0;
    sprintf(fullName,"users//%s//productInfo.txt",uName);
    FILE*fp=fopen(fullName,"r");

    if(fp==NULL)
    {
        return 0;
    }

    c=fgetc(fp);
    while(c!=-1)
    {
        curNum=(curNum*10)+c-48;
        c=fgetc(fp);
    }
    curUser.numOwnedProducts=curNum;
    fflush(fp);
    fclose(fp);

    return 1;
}

//Function: setNumProductsUser
//    Does:
//
//    like getNumProductsUser only the other way around
//
//    Local Variables used:
//
//    char fullName[101] : for fopen
//    int i: counter
//    FILE*fp : file pointer
//
//    Receives:
//
//    a username as a char*
//    an integer of what to set the number of owned products to
//
//    Returns:
//
//    * nothing

int setNumProductsUser(char*uName,int num)
{
    char fullName[101];
    int i;
    sprintf(fullName,"users//%s//productInfo.txt",uName);
    FILE*fp=fopen(fullName,"w");

    if(fp==NULL)
    {
        return 0;
    }

    if(num==0)
    {
        fputc(48,fp);
    }
    else
    {
        for(i=(int)log10((double)num);i>=0;i--)
        {
            fputc(((num/(int)(pow(10,(double)i)))%10)+48,fp);
        }
    }

    fflush(fp);
    fclose(fp);
    return 1;
}

/*
    Function: mainAccountView
    Does:

    simple menu that ties together the 3 views: buyer, seller, and admin

    Local Variables used:

    int i,j;
    char input;
    int selection=0;

    Receives:

    * nothing

    Returns:

    * nothing
*/

void mainAccountView(void)
{
    int i,j;
    char input;
    int selection=0;

    do
    {
        system("cls");
        cprintf(15,"\n\n\tWelcome, ");
        cprintf(11,curUser.uName);
        cprintf(15,",\n\tPlease select an option to continue:\n\n\t");
        if(selection==0)
        {
            cprintf(10," > Shopping View\n\t");
        }
        else
        {
            cprintf(15,"   Shopping View\n\t");
        }
        if(selection==1)
        {
            cprintf(10," > Seller View\n\t");
        }
        else
        {
            cprintf(15,"   Seller View\n\t");
        }
        if(selection==2)
        {
            if(curUser.hasAdmin==0)
            {
                cprintf(14," > Acquire Admin\n\t");
            }
            else
            {
                cprintf(10," > Admin View\n\t");
            }
        }
        else
        {
            if(curUser.hasAdmin==0)
            {
                cprintf(15,"   Acquire Admin\n\t");
            }
            else
            {
                cprintf(15,"   Admin View\n\t");
            }
        }
        input=getch();
        switch(input)
        {
        case 72:
            if(selection>0)
            {
                selection--;
            }
            break;
        case 80:
            if(selection<2)
            {
                selection++;
            }
            break;
        case 13:
            switch(selection)
            {
            case 0:
                buyerView();
                break;
            case 1:
                sellerView();
                break;
            case 2:
                if(curUser.hasAdmin==0)
                {
                    acquireAdmin();
                }
                else
                {
                    adminView();
                }
                break;
            }
            break;
        }
    }
    while(input!=27);
}

/*
    Function: buyerView
    Does:

    presents the user with a buyer view that prints out all the products available, allows user to purchase them by adding to cart, and allows for
    some admin powers to take effect upon an admin's choosing

    Local Variables used:

    int i,j,k,l;
    char input;
    int selection=0;

    int numToLoop : number of items to display on a given page
    int curPageNum=0 : current page number
    FILE*fp : allProducts file pointer
    FILE*temp : for removing things from files

    product cart[10] : a list of the items the user has in their cart
    int cartIsActive=0 : whether or not the user has anything in their cart
    int cartEmptySlot=0 : what space to fill next in the cart (and also acts as a measure of how full it is, max 10 items)
    int totalPrice=0 : keeps track of the total balance of the user's cart
    int tempStock : temporary variable remembering how much of an item the user wishes to buy
    int cartStocks[10]={0} : a list of how much of each item in the user's cart they want to purchase
    int itemAlreadyPresent : used in a loop to see if an item the user wants to add, they already have

    Receives:

    * nothing

    Returns:

    * nothing
*/

void buyerView(void)
{
    int i,j,k,l;
    char input;
    int selection=0;
    int numToLoop;
    int curPageNum=0;
    FILE*fp;
    FILE*temp;

    product cart[10];
    int cartIsActive=0;
    int cartEmptySlot=0;
    int totalPrice=0;
    int tempStock;
    int cartStocks[10]={0};
    int itemAlreadyPresent;

    loadProductPage(1);
    getNumProducts();
    do
    {

        system("cls");
        cprintf(15,"\n\n\tBrowse products on the market\n\tSelect a product to view in detail or complete a transaction:\n\n");
        numToLoop=numLoadedProducts-(curPageNum*5);
        if(numToLoop>5)
        {
            numToLoop=5;
        }
        for(i=0;i<numToLoop;i++)
        {
            if(selection==i)
            {
                setColor(10);
            }
            else
            {
                setColor(15);
            }
            printf("\t%c",218);
            for(j=0;j<80;j++)
            {
                printf("%c",196);
            }
            printf("%c\n",191);

            printf("\t%c               %-50s               %c\n",179,loadedProducts[curPageNum][i].name,179);
            printf("\t%c  Brand: %-30s   Type: %-30s  %c\n",179,loadedProducts[curPageNum][i].brand,loadedProducts[curPageNum][i].type,179);
            printf("\t%c      $%8.2f                                        Available Stock: %2i      %c\n",179,((float)loadedProducts[curPageNum][i].priceCents)/100,loadedProducts[curPageNum][i].stock,179);
            printf("\t%c      Last Edited: %-50s           %c\n",179,loadedProducts[curPageNum][i].dateEdited,179);
            printf("\t%c",192);
            for(j=0;j<80;j++)
            {
                printf("%c",196);
            }
            printf("%c\n",217);
        }
        if(cartIsActive==1)
        {
            cprintf(14,"\n\tF - Finish Transaction\n\tC - Clear Cart");
            printf("\n\n\tCurrent Cost: $%.2f",((float)totalPrice)/100);
        }
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<numToLoop-1)
                {
                    selection++;
                }
                break;
            case 75:
                if(curPageNum>0)
                {
                    curPageNum--;
                }
                break;
            case 77:
                if(curPageNum<(numLoadedProducts-1)/5)
                {
                    curPageNum++;
                }
                break;
            case 'c':case'C':
                cartIsActive=0;
                cartEmptySlot=0;
                totalPrice=0;
                for(i=0;i<10;i++)
                {
                    cartStocks[i]=0;
                }
                break;
            case 'f':case'F':
                selection=0;
                do
                {
                    system("cls");
                    cprintf(15,"\n\n\tAre you sure you would like to complete this transaction?\n");
                    printf("\tTotal Cost: $%.2f\n\n",((float)totalPrice)/100);
                    if(selection==0)
                    {
                        cprintf(11,"\t > No\n\t");
                        cprintf(15,"   Yes\n");
                    }
                    else
                    {
                        cprintf(15,"\t   No\n\t");
                        cprintf(10," > Yes\n");
                    }
                    input=getch();
                    switch(input)
                    {
                        case 72:
                            selection=0;
                            break;
                        case 80:
                            selection=1;
                            break;
                        case 13:
                            if(selection==1)
                            {
                                for(l=0;l<cartEmptySlot;l++)
                                {
                                    if(cart[l].stock-cartStocks[l]==0)
                                    {
                                        removeProduct(cart[l].numInFile);
                                    }
                                    else
                                    {
                                        fp=fopen("products//allProducts.txt","r");
                                        temp=fopen("products//temp.txt","w");

                                        tempProduct=cart[l];
                                        getNumProducts();
                                        for(k=0;k<numProducts;k++)
                                        {
                                            fgetc(fp);
                                            loadNextProduct(fp);
                                            fputc(10,temp);
                                            i=0;
                                            while(tempProduct.name[i]!=0)
                                            {
                                                fputc(tempProduct.name[i],temp);
                                                i++;
                                            }
                                            fputc(10,temp);
                                            i=0;
                                            while(tempProduct.type[i]!=0)
                                            {
                                                fputc(tempProduct.type[i],temp);
                                                i++;
                                            }
                                            fputc(10,temp);
                                            i=0;
                                            while(tempProduct.brand[i]!=0)
                                            {
                                                fputc(tempProduct.brand[i],temp);
                                                i++;
                                            }
                                            fputc(10,temp);
                                            i=0;
                                            for(i=6;i>=0;i--)
                                            {
                                                fputc(((tempProduct.priceCents/((int)pow(10,(double)i)))%10)+48,temp);
                                            }
                                            fputc(10,temp);
                                            i=0;
                                            while(tempProduct.desc[i]!=0)
                                            {
                                                fputc(tempProduct.desc[i],temp);
                                                i++;
                                            }
                                            fputc(10,temp);
                                            fputc((tempProduct.numSpecs/10)+48,temp);
                                            fputc((tempProduct.numSpecs%10)+48,temp);
                                            fputc(10,temp);
                                            for(i=0;i<tempProduct.numSpecs;i++)
                                            {
                                                j=0;
                                                while(tempProduct.specNames[i][j]!=0)
                                                {
                                                    fputc(tempProduct.specNames[i][j],temp);
                                                    j++;
                                                }
                                                fputc(10,temp);
                                                j=0;
                                                while(tempProduct.specs[i][j]!=0)
                                                {
                                                    fputc(tempProduct.specs[i][j],temp);
                                                    j++;
                                                }
                                                fputc(10,temp);
                                            }
                                            if(tempProduct.numInFile-1==k)
                                            {
                                                fputc(((tempProduct.stock-cartStocks[l])/10)+48,temp);
                                                fputc(((tempProduct.stock-cartStocks[l])%10)+48,temp);
                                            }
                                            else
                                            {
                                                fputc((tempProduct.stock/10)+48,temp);
                                                fputc((tempProduct.stock%10)+48,temp);
                                            }
                                            fputc(10,temp);
                                            i=0;
                                            while(tempProduct.ownerName[i]!=0)
                                            {
                                                fputc(tempProduct.ownerName[i],temp);
                                                i++;
                                            }
                                            fputc(10,temp);

                                            j=0;
                                            while(tempProduct.dateCreated[j]!=0)
                                            {
                                                fputc(tempProduct.dateCreated[j],temp);
                                                j++;
                                            }
                                            fputc(10,temp);
                                            while(tempProduct.dateEdited[j]!=0)
                                            {
                                                fputc(tempProduct.dateEdited[j],temp);
                                                j++;
                                            }
                                            fputc(10,temp);
                                        }
                                        fflush(temp);
                                        fclose(temp);
                                        fflush(fp);
                                        fclose(fp);
                                        remove("products//allProducts.txt");
                                        rename("products//temp.txt","products//allProducts.txt");
                                    }
                                }

                                cartIsActive=0;
                                cartEmptySlot=0;
                                totalPrice=0;
                                for(i=0;i<10;i++)
                                {
                                    cartStocks[i]=0;
                                }
                                loadProductPage(1);
                                curPageNum=0;
                                selection=0;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=13);
                selection=0;
                input=0;
                break;
            case 13:
                tempProduct=loadedProducts[curPageNum][selection];
                selection=0;
                do
                {
                    system("cls");
                    cprintf(15,"This is the product you have selected:\n\n");
                    printf("%i",tempProduct.numInFile);
                    printf("\tName: %s\n\tType: %s\n\tBrand: %s\n\tPrice: $%.2f\n\tDescription:\n\t%s\n\tSpecs [%i]:\n\t",tempProduct.name,tempProduct.type,tempProduct.brand,((float)tempProduct.priceCents)/100,tempProduct.desc,tempProduct.numSpecs);
                    printf("Available Stock: %i\n\tOwner: %s\n\tCreated: %s , Edited: %s\n\n",tempProduct.stock,tempProduct.ownerName,tempProduct.dateCreated,tempProduct.dateEdited);
                    if(strcmp(curUser.uName,tempProduct.ownerName)==0)
                    {
                        cprintf(14,"\t[YOU OWN THIS PRODUCT]");
                    }
                    else
                    {
                        cprintf(15,"\tWhat would you like to do with this product?\n\n\t");
                        if(curUser.hasAdmin==0)
                        {
                            if(selection==0)
                            {
                                cprintf(10," > Add to Cart\n\n\t");
                                cprintf(15,"   Exit\n\n");
                            }
                            else
                            {
                                cprintf(15,"   Add to Cart\n\n\t");
                                cprintf(10," > Exit\n\n");
                            }
                        }
                        else
                        {
                            if(selection==0)
                            {
                                cprintf(10," > Add to Cart\n\t");
                            }
                            else
                            {
                                cprintf(15,"   Add to Cart\n\t");
                            }
                            if(selection==1)
                            {
                                cprintf(10," > Remove [Admin]\n\t");
                            }
                            else
                            {
                                cprintf(14,"   Remove [Admin]\n\t");
                            }
                            if(selection==2)
                            {
                                cprintf(10," > Edit [Admin]\n\t");
                            }
                            else
                            {
                                cprintf(14,"   Edit [Admin]\n\t");
                            }
                            if(selection==3)
                            {
                                cprintf(10," > Exit\n\n");
                            }
                            else
                            {
                                cprintf(15,"   Exit\n\n");
                            }
                        }
                    }
                    input=getch();
                    if(strcmp(curUser.uName,tempProduct.ownerName)!=0)
                    {
                        if(curUser.hasAdmin==0)
                        {
                            switch(input)
                            {
                                case 72:
                                    selection=0;
                                    break;
                                case 80:
                                    selection=1;
                                    break;
                                case 13:
                                    if(selection==0)
                                    {
                                        itemAlreadyPresent=0;
                                        for(i=0;i<cartEmptySlot;i++)
                                        {
                                            if(tempProduct.numInFile==cart[i].numInFile)
                                            {
                                                itemAlreadyPresent=1;
                                            }
                                        }
                                       if(itemAlreadyPresent==0)
                                       {
                                            if(cartEmptySlot<10)
                                            {
                                                selection=0;
                                                tempStock=1;
                                                do
                                                {
                                                    system("cls");
                                                    cprintf(15,"How much of this Item would you like to add?\n\n\t");
                                                    printf("%i [Arrow Keys to Adjust]",tempStock);
                                                    input=getch();
                                                    switch(input)
                                                    {
                                                        case 80:case 75:
                                                            if(tempStock>1)
                                                            {
                                                                tempStock--;
                                                            }
                                                            break;
                                                        case 72:case 77:
                                                            if(tempStock<tempProduct.stock)
                                                            {
                                                                tempStock++;
                                                            }
                                                            break;
                                                        case 13:
                                                            cart[cartEmptySlot]=tempProduct;
                                                            cartStocks[cartEmptySlot]=tempStock;
                                                            cartIsActive=1;
                                                            cartEmptySlot++;
                                                            totalPrice+=tempProduct.priceCents*tempStock;
                                                            break;
                                                    }
                                                }
                                                while(input!=27&&input!=13);
                                                selection=0;
                                                input=0;
                                            }
                                            else
                                            {
                                                cprintf(14,"\n\n\tYour Cart is Full");
                                                getch();
                                            }
                                       }
                                       else
                                       {
                                           cprintf(14,"\n\n\tYou already added this item to cart");
                                            getch();
                                       }
                                    }
                                    break;
                            }
                        }
                        else
                        {
                            switch(input)
                            {
                                case 72:
                                    if(selection>0)
                                    {
                                        selection--;
                                    }
                                    break;
                                case 80:
                                    if(selection<3)
                                    {
                                        selection++;
                                    }
                                    break;
                                case 13:
                                    switch(selection)
                                    {
                                    case 0:
                                        itemAlreadyPresent=0;
                                        for(i=0;i<cartEmptySlot;i++)
                                        {
                                            if(tempProduct.numInFile==cart[i].numInFile)
                                            {
                                                itemAlreadyPresent=1;
                                            }
                                        }
                                       if(itemAlreadyPresent==0)
                                       {
                                            if(cartEmptySlot<10)
                                            {
                                                selection=0;
                                                tempStock=1;
                                                do
                                                {
                                                    system("cls");
                                                    cprintf(15,"How much of this Item would you like to add?\n\n\t");
                                                    printf("%i [Arrow Keys to Adjust]",tempStock);
                                                    input=getch();
                                                    switch(input)
                                                    {
                                                        case 80:case 75:
                                                            if(tempStock>1)
                                                            {
                                                                tempStock--;
                                                            }
                                                            break;
                                                        case 72:case 77:
                                                            if(tempStock<tempProduct.stock)
                                                            {
                                                                tempStock++;
                                                            }
                                                            break;
                                                        case 13:
                                                            cart[cartEmptySlot]=tempProduct;
                                                            cartStocks[cartEmptySlot]=tempStock;
                                                            cartIsActive=1;
                                                            cartEmptySlot++;
                                                            totalPrice+=tempProduct.priceCents*tempStock;
                                                            break;
                                                    }
                                                }
                                                while(input!=27&&input!=13);
                                                selection=0;
                                                input=0;
                                            }
                                            else
                                            {
                                                cprintf(14,"\n\n\tYour Cart is Full");
                                                getch();
                                            }
                                       }
                                       else
                                       {
                                           cprintf(14,"\n\n\tYou already added this item to cart");
                                            getch();
                                       }
                                        break;
                                    case 1:
                                        selection=0;
                                        do
                                        {
                                            system("cls");
                                            cprintf(11,"\n\n\tAre you sure you would like to delete this product?\n\n");
                                            if(selection==0)
                                            {
                                                cprintf(10,"\t > No");
                                                cprintf(15,"\n\t   Yes\n\n");
                                            }
                                            else
                                            {
                                                cprintf(15,"\t   No");
                                                cprintf(12,"\n\t > Yes\n\n");
                                            }
                                            input=getch();
                                            switch(input)
                                            {
                                                case 72:
                                                    selection=0;
                                                    break;
                                                case 80:
                                                    selection=1;
                                                    break;
                                                case 13:
                                                    if(selection==1)
                                                    {
                                                        removeProduct(tempProduct.numInFile);
                                                    }
                                                    break;
                                            }
                                        }
                                        while(input!=27&&input!=8&&input!=13);
                                        loadProductPage(1);
                                        curPageNum=0;
                                        selection=0;
                                        input=0;
                                        break;
                                    case 2:
                                        printf("%i",tempProduct.numInFile);
                                        getch();
                                        editOneProduct(1);
                                        curPageNum=0;
                                        selection=0;
                                        loadProductPage(1);
                                        break;
                                    }
                                    break;
                            }
                        }
                    }
                }
                while(input!=27&&input!=13&&input!=0);
                selection=0;
                input=0;
                break;
        }
    }
    while(input!=27);
}

/*
    Function: sellerView
    Does:

    a brief function that provides the user with a seller view that ties together all the other functions necessary for the seller view (create, edit, etc.)

    Local Variables used:

    int i,j,k;
    char input;
    int selection=0;

    char options[][]: a simple list of options for the user to be printed out in a loop

    Receives:

    * nothing

    Returns:

    * nothing
*/

void sellerView(void)
{
    int i,j;
    char input;
    int selection=0;
    char options[3][31]={"Create New Product","Revise Created Products","Remove Created Products"};

    do
    {
        system("cls");
        cprintf(15,"\n\n\tSelect an option to continue:\n\n");
        for(i=0;i<2;i++)
        {
            if(selection==i)
            {
                cprintf(10,"\t > ");
            }
            else
            {
                cprintf(15,"\t   ");
            }
            printf("%s\n",options[i]);
        }
        input=getch();
        switch(input)
        {
            case 72:
                selection=0;
                break;
            case 80:
                selection=1;
                break;
            case 13:
                switch(selection)
                {
                    case 0:
                        createProduct();
                        break;
                    case 1:
                        editProducts();
                        break;
                }
        }
    }
    while(input!=27);
}

/*
    Function: createProduct
    Does:

    allows the user to create a product with menus in which they input the criteria of the product

    Local Variables used:

    int i,j,k;
    char input;
    int selection=0;

    for top messages used as before:
    char topMessages[2][81]
    int topMessageColors[2]
    int topMessage

    FILE*fp : file pointer

    A lot of variables with self explanatory names representing strings the user has entered and the places at which they are in those strings,
    Numbers are handled differently and do not need an accommodating "place" variable:

    char curName[51]={0};
    int placeInCurName=0;

    char curType[31]={0};
    int placeInCurType=0;

    char curBrand[31]={0};
    int placeInCurBrand=0;

    int curPriceCents={0};

    char curDesc[501]={0};
    int placeInCurDesc=0;

    int curNumSpecs=0;

    char curSpecNames[20][31]={0};
    int placesInStringSpecNames[20]={0};

    char curSpecs[20][51]={0};
    int placesInStringSpecs[20]={0};

    int curStock=1;


    For telling time, not sure entirely how they work:
    time_t current_time;
    char* c_time_string;

    Receives:

    * nothing

    Returns:

    * nothing
*/

void createProduct(void)
{

    int i,j;
    char input;
    int selection=0;
    char topMessages[2][81]={"\n\n\tPlease provide the information below to create a product:\n\n\t","\n\n\tOne or more necessary details have not been filled in\n\n\t"};
    int topMessageColors[2]={15,12};
    int topMessage=0;
    FILE*fp;

    char curName[51]={0};
    int placeInCurName=0;

    char curType[31]={0};
    int placeInCurType=0;

    char curBrand[31]={0};
    int placeInCurBrand=0;

    int curPriceCents={0};

    char curDesc[501]={0};
    int placeInCurDesc=0;

    int curNumSpecs=0;

    char curSpecNames[20][31]={0};
    int placesInStringSpecNames[20]={0};

    char curSpecs[20][51]={0};
    int placesInStringSpecs[20]={0};

    int curStock=1;

    time_t current_time;
    char* c_time_string;

    do
    {
        system("cls");
        cprintf(topMessageColors[topMessage],topMessages[topMessage]);
        if(selection==0)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Name: %s\n\t",curName);
        if(selection==1)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Type: %s\n\t",curType);
        if(selection==2)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Brand: %s\n\t",curBrand);
        if(selection==3)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Price: $%.2f\n\n\t",((float)curPriceCents)/100);
        if(selection==4)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Description:\n\t%s\n\n\t",curDesc);
        if(selection==5)
        {
            cprintf(14," > ");
        }
        else
        {
            cprintf(14,"   ");
        }
        printf("Edit Product Specs\n\n\t");
        if(selection==6)
        {
            cprintf(10," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Item Stock: %i\n\n\t",curStock);
        if(selection==7)
        {
            cprintf(10," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Done\n\n");
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<7)
                {
                    selection++;
                }
                break;
            case 13:
                switch(selection)
                {
                    case 0:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Name:\n\n\t   Product Name: ");
                            printf("%s",curName);
                            if(placeInCurName<50)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",curType,curBrand,((float)curPriceCents)/100,curDesc,curStock);
                            input=getch();
                            if(input==8&&placeInCurName>0)
                            {
                                placeInCurName--;
                                curName[placeInCurName]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurName<50)
                            {
                                curName[placeInCurName]=input;
                                placeInCurName++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 1:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Type:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s",curName);

                            setColor(15);
                            printf("\n\t   Product Type: %s",curType);
                            if(placeInCurType<30)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",curBrand,((float)curPriceCents)/100,curDesc,curStock);
                            input=getch();
                            if(input==8&&placeInCurType>0)
                            {
                                placeInCurType--;
                                curType[placeInCurType]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurType<30)
                            {
                                curType[placeInCurType]=input;
                                placeInCurType++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 2:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Brand:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s",curName,curType);
                            setColor(15);
                            printf("\n\t   Product Brand: %s",curBrand);
                            if(placeInCurBrand<30)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",((float)curPriceCents)/100,curDesc,curStock);
                            input=getch();
                            if(input==8&&placeInCurBrand>0)
                            {
                                placeInCurBrand--;
                                curBrand[placeInCurBrand]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurBrand<30)
                            {
                                curBrand[placeInCurBrand]=input;
                                placeInCurBrand++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 3:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Price:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s",curName,curType,curBrand);
                            setColor(15);
                            printf("\n\t   Product Price: $%.2f",((float)curPriceCents)/100);
                            setColor(8);
                            printf("\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",curDesc,curStock);
                            input=getch();
                            if(input==8)
                            {
                                curPriceCents/=10;
                            }
                            else if((input>=48&&input<=57)&&curPriceCents*10<10000000)
                            {
                                curPriceCents=(curPriceCents*10)+input-48;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 4:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Description:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f",curName,curType,curBrand,((float)curPriceCents)/100);
                            setColor(15);
                            printf("\n\n\t   Product Description:\n\t   %s",curDesc);
                            if(placeInCurDesc<500)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",curStock);
                            input=getch();
                            if(input==8&&placeInCurDesc>0)
                            {
                                placeInCurDesc--;
                                curDesc[placeInCurDesc]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurDesc<500)
                            {
                                curDesc[placeInCurDesc]=input;
                                placeInCurDesc++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 5:
                        selection=0;
                        do
                        {
                            system("cls");
                            if(selection==0)
                            {
                                cprintf(14," > ");
                            }
                            else
                            {
                                cprintf(14,"   ");
                            }
                            printf("Create new spec [%i/20]\n",curNumSpecs);
                            if(selection==1)
                            {
                                cprintf(10," > ");
                            }
                            else
                            {
                                cprintf(15,"   ");
                            }
                            printf("Done\n\n");
                            for(i=1;i<=curNumSpecs;i++)
                            {
                                if(selection-1==i)
                                {
                                    cprintf(15," > ");
                                }
                                else
                                {
                                    cprintf(15,"   ");
                                }
                                printf("%s : %s\n",curSpecNames[i-1],curSpecs[i-1]);
                            }
                            input=getch();
                            switch(input)
                            {
                                case 72:
                                    if(selection>0)
                                    {
                                        selection--;
                                    }
                                    break;
                                case 80:
                                    if(selection<=curNumSpecs)
                                    {
                                        selection++;
                                    }
                                    break;
                                case 13:
                                    if(selection==0&&curNumSpecs<20)
                                    {
                                        do
                                        {
                                            system("cls");
                                            cprintf(15,"\n\n\tPlease Enter the requested specification elements:\n\n\tSpec Name: ");
                                            printf("%s",curSpecNames[curNumSpecs]);
                                            if(placesInStringSpecNames[curNumSpecs]<30)
                                            {
                                                printf("_");
                                            }
                                            cprintf(8,"\n\tSpec: ");
                                            printf("%s\n\n",curSpecs[curNumSpecs]);
                                            input=getch();
                                            if(input==8&&placesInStringSpecNames[curNumSpecs]>0)
                                            {
                                                placesInStringSpecNames[curNumSpecs]--;
                                                curSpecNames[curNumSpecs][placesInStringSpecNames[curNumSpecs]]=0;
                                            }
                                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placesInStringSpecNames[curNumSpecs]<30)
                                            {
                                                curSpecNames[curNumSpecs][placesInStringSpecNames[curNumSpecs]]=input;
                                                placesInStringSpecNames[curNumSpecs]++;
                                            }
                                            //acceptChar(input,&placesInStringSpecNames[curNumSpecs],30,&curSpecNames[curNumSpecs]);
                                        }
                                        while((input!=13&&input!=27)||curSpecNames[curNumSpecs][0]==0);
                                        if(input==27)
                                        {
                                            for(i=0;i<30;i++)
                                            {
                                                curSpecNames[curNumSpecs][i]=0;
                                            }
                                            placesInStringSpecNames[curNumSpecs]=0;
                                            break;
                                        }
                                        do
                                        {
                                            system("cls");
                                            cprintf(15,"\n\n\tPlease Enter the requested specification elements:");
                                            cprintf(8,"\n\n\tSpec Name: ");
                                            printf("%s",curSpecNames[curNumSpecs]);
                                            if(placesInStringSpecs[curNumSpecs]<30)
                                            {
                                                printf("_");
                                            }
                                            cprintf(15,"\n\tSpec: ");
                                            printf("%s\n\n",curSpecs[curNumSpecs]);
                                            input=getch();
                                            if(input==8&&placesInStringSpecs[curNumSpecs]>0)
                                            {
                                                placesInStringSpecs[curNumSpecs]--;
                                                curSpecs[curNumSpecs][placesInStringSpecs[curNumSpecs]]=0;
                                            }
                                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placesInStringSpecs[curNumSpecs]<30)
                                            {
                                                curSpecs[curNumSpecs][placesInStringSpecs[curNumSpecs]]=input;
                                                placesInStringSpecs[curNumSpecs]++;
                                            }
                                        }
                                        while((input!=13&&input!=27)||curSpecs[curNumSpecs][0]==0);
                                        if(input==27)
                                        {
                                            for(i=0;i<30;i++)
                                            {
                                                curSpecNames[curNumSpecs][i]=0;
                                                curSpecs[curNumSpecs][i]=0;
                                            }
                                            placesInStringSpecs[curNumSpecs]=0;
                                            break;
                                        }
                                        curNumSpecs++;
                                        selection=0;
                                        input=0;
                                    }
                                    else if(selection==1)
                                    {

                                    }
                                    else
                                    {

                                    }
                                    break;
                            }
                        }
                        while(input!=27&&!(input==13&&selection==1));
                        selection=0;
                        input=0;
                        break;
                    case 6:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tAdjust the number of your product which are for sale:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs",curName,curType,curBrand,((float)curPriceCents)/100,curDesc);
                            setColor(15);
                            printf("\n\n\t   Item Stock: %i [Arrow Keys to Adjust]",curStock);
                            setColor(8);
                            printf("\n\n\t   Done\n\n");
                            input=getch();
                            switch(input)
                            {
                                case 72:case 77:
                                    if(curStock<10)
                                    {
                                        curStock++;
                                    }
                                    break;
                                case 80:case 75:
                                    if(curStock>1)
                                    {
                                        curStock--;
                                    }
                                    break;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 7:
                        selection=0;
                        if(curName[0]!=0&&curType[0]!=0&&curBrand[0]!=0&&curPriceCents!=0&&curDesc[0]!=0)
                        {
                            do
                            {
                                system("cls");
                                cprintf(11,"\n\n\tAre you sure you would like to create this product?\n\n");
                                if(selection==0)
                                {
                                    cprintf(12,"\t > No");
                                    cprintf(15,"\n\t   Yes\n\n");
                                }
                                else
                                {
                                    cprintf(15,"\t   No");
                                    cprintf(10,"\n\t > Yes\n\n");
                                }
                                input=getch();
                                switch(input)
                                {
                                    case 72:
                                        selection=0;
                                        break;
                                    case 80:
                                        selection=1;
                                        break;
                                    case 13:
                                        if(selection==1)
                                        {
                                            fp=fopen("products//allProducts.txt","a");
                                            fseek(fp,0,SEEK_SET);
                                            fputc(10,fp);
                                            getNumProducts();
                                            i=0;
                                            while(curName[i]!=0)
                                            {
                                                fputc(curName[i],fp);
                                                i++;
                                            }
                                            fputc(10,fp);
                                            i=0;
                                            while(curType[i]!=0)
                                            {
                                                fputc(curType[i],fp);
                                                i++;
                                            }
                                            fputc(10,fp);
                                            i=0;
                                            while(curBrand[i]!=0)
                                            {
                                                fputc(curBrand[i],fp);
                                                i++;
                                            }
                                            fputc(10,fp);
                                            i=0;
                                            for(i=6;i>=0;i--)
                                            {
                                                fputc(((curPriceCents/((int)pow(10,(double)i)))%10)+48,fp);
                                            }
                                            fputc(10,fp);
                                            i=0;
                                            while(curDesc[i]!=0)
                                            {
                                                fputc(curDesc[i],fp);
                                                i++;
                                            }
                                            fputc(10,fp);
                                            fputc((curNumSpecs/10)+48,fp);
                                            fputc((curNumSpecs%10)+48,fp);
                                            fputc(10,fp);
                                            for(i=0;i<curNumSpecs;i++)
                                            {
                                                j=0;
                                                while(curSpecNames[i][j]!=0)
                                                {
                                                    fputc(curSpecNames[i][j],fp);
                                                    j++;
                                                }
                                                fputc(10,fp);
                                                j=0;
                                                while(curSpecs[i][j]!=0)
                                                {
                                                    fputc(curSpecs[i][j],fp);
                                                    j++;
                                                }
                                                fputc(10,fp);
                                            }
                                            fputc((curStock/10)+48,fp);
                                            fputc((curStock%10)+48,fp);
                                            fputc(10,fp);
                                            i=0;
                                            while(curUser.uName[i]!=0)
                                            {
                                                fputc(curUser.uName[i],fp);
                                                i++;
                                            }
                                            fputc(10,fp);

                                            current_time = time(NULL);
                                            c_time_string = ctime(&current_time);

                                            for(i=0;i<2;i++)
                                            {
                                                j=0;
                                                while(c_time_string[j]!=10)
                                                {
                                                    fputc(c_time_string[j],fp);
                                                    j++;
                                                }
                                                fputc(10,fp);
                                            }
                                            setNumProducts(numProducts+1);
                                            numProducts++;
                                            setNumProductsUser(curUser.uName,curUser.numOwnedProducts+1);
                                            curUser.numOwnedProducts++;
                                            fflush(fp);
                                            fclose(fp);
                                            return;
                                        }
                                        break;
                                }
                            }
                            while(input!=27&&input!=8&&input!=13);
                        }
                        else
                        {
                            topMessage=1;
                        }
                        selection=7;
                        input=0;
                        break;
                }
                break;
            case 27:
                selection=0;
                do
                {
                    system("cls");
                    cprintf(11,"\n\n\tAre you sure you would like to cancel creating this product?\n\n");
                    if(selection==0)
                    {
                        cprintf(10,"\t > No");
                        cprintf(15,"\n\t   Yes\n\n");
                    }
                    else
                    {
                        cprintf(15,"\t   No");
                        cprintf(12,"\n\t > Yes\n\n");
                    }
                    input=getch();
                    switch(input)
                    {
                        case 72:
                            selection=0;
                            break;
                        case 80:
                            selection=1;
                            break;
                        case 13:
                            if(selection==1)
                            {
                                return;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=0;
                input=0;
                break;
        }
    }
    while(!(input==13&&selection==7));
}

/*
    Function: editProducts
    Does:

    presents the user with a list of all of their items that they have created and leads to other functions where the user does things to those items
    also calls the removeProduct function for removing items

    Local Variables used:

    int i,j,k;
    char input;
    int selection=0;

    int curPageNum=0; keeps track of what page the user is on
    int numToDisplay; keeps track of how many items are on said page for loops

    Receives:

    * nothing

    Returns:

    * nothing
*/

void editProducts(void)
{
    int i,j;
    char input;
    int curPageNum=0;
    int numToDisplay;
    int selection=0;

    loadUserProductPage(1);
    //loadProductPage(1);

    do
    {
        system("cls");
        cprintf(15,"\n\n\tThese are the products you currently own,\n\tplease select a product to continue:\n\n");
        if(curUser.numOwnedProducts-(curPageNum*25)>25)
        {
            numToDisplay=25;
        }
        else
        {
            numToDisplay=curUser.numOwnedProducts;
        }
//        getNumProducts();
//        numToDisplay=numProducts;
        for(i=0;i<numToDisplay;i++)
        {
            if(selection==i)
            {
                cprintf(10,"\t > ");
            }
            else
            {
                cprintf(15,"\t   ");
            }
            printf("%i %s\n",loadedProducts[i/5][i%5].numInFile,loadedProducts[i/5][i%5].name);
        }
        setColor(15);
        printf("\n\tPage %i / %i\n\n",curPageNum+1,((curUser.numOwnedProducts-1)/25)+1);
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<curUser.numOwnedProducts-1)
                {
                    selection++;
                }
                break;
            case 75:
                if(curPageNum>0)
                {
                    curPageNum--;
                    loadUserProductPage(curPageNum+1);
                }
                break;
            case 77:
                if(curPageNum<(curUser.numOwnedProducts-1)/25)
                {
                    curPageNum++;
                    loadUserProductPage(curPageNum+1);
                }
                break;
            case 13:
                tempProduct=loadedProducts[selection/5][selection%5];
                selection=0;
                do
                {
                    system("cls");
                    cprintf(15,"\n\n\tWhat would you like to do with this product?\n\n");
                    if(selection==0)
                    {
                        cprintf(10,"\t > Edit\n");
                        cprintf(15,"\t   Delete\n");
                    }
                    else
                    {
                        cprintf(15,"\t   Edit\n");
                        cprintf(14,"\t > Delete\n");
                    }
                    input=getch();
                    switch(input)
                    {
                    case 72:
                        selection=0;
                        break;
                    case 80:
                        selection=1;
                        break;
                    case 13:
                        switch(selection)
                        {
                            case 0:
                                editOneProduct(0);
                                loadUserProductPage(curPageNum+1);
                                break;
                            case 1:
                                selection=0;
                                do
                                {
                                    system("cls");
                                    cprintf(11,"\n\n\tAre you sure you would like to delete this product?\n\n");
                                    if(selection==0)
                                    {
                                        cprintf(10,"\t > No");
                                        cprintf(15,"\n\t   Yes\n\n");
                                    }
                                    else
                                    {
                                        cprintf(15,"\t   No");
                                        cprintf(12,"\n\t > Yes\n\n");
                                    }
                                    input=getch();
                                    switch(input)
                                    {
                                        case 72:
                                            selection=0;
                                            break;
                                        case 80:
                                            selection=1;
                                            break;
                                        case 13:
                                            if(selection==1)
                                            {
                                                removeProduct(tempProduct.numInFile);
                                                loadUserProductPage(curPageNum+1);
                                            }
                                            break;
                                    }
                                }
                                while(input!=27&&input!=8&&input!=13);
                                selection=0;
                                input=0;
                                break;
                        }
                        break;

                    }
                }
                while(input!=27&&input!=13&&input!=0);
                selection=0;
                input=0;


                break;
        }
    }
    while(input!=27);
}

/*
    Function: editOneProduct
    Does:

    removes a product for allProducts.txt by copying everything from that file into a temp.txt file except for that product and making temp into allProducts

    Local Variables used:

    int i,j,k;
    char input;
    int selection=0;

    char topMessages[2][81] : all top messages which may be displayed
    int topMessageColors[2] : top message colors
    int topMessage=0 : what top message
    FILE*fp and temp : file pointers to allProducts and what will become allProducts eventually

    A lot of int variables keeping track of where the user is at in the string:
    int placeInCurName=0;
    int placeInCurType=0;
    int placeInCurBrand=0;
    int placeInCurDesc=0;
    int placesInStringSpecNames[20]={0};
    int placesInStringSpecs[20]={0};

    Used to tell time, not enitrely sure how they work:
    time_t current_time;
    char* c_time_string;

    Receives:

    * nothing

    Returns:

    * nothing
*/

void editOneProduct(void)
{
    int i,j,k;
    char input;
    int selection=0;
    char topMessages[2][81]={"\n\n\tRevise and save the product information below:\n\n\t","\n\n\tOne or more necessary details are missing\n\n\t"};
    int topMessageColors[2]={15,12};
    int topMessage=0;
    FILE*fp;
    FILE*temp;

    int placeInCurName=0;
    int placeInCurType=0;
    int placeInCurBrand=0;
    int placeInCurDesc=0;
    int placesInStringSpecNames[20]={0};
    int placesInStringSpecs[20]={0};

    time_t current_time;
    char* c_time_string;

    i=0;
    while(tempProduct.name[i]!=0)
    {
        i++;
    }
    placeInCurName=i;
    i=0;
    while(tempProduct.type[i]!=0)
    {
        i++;
    }
    placeInCurType=i;
    i=0;
    while(tempProduct.brand[i]!=0)
    {
        i++;
    }
    placeInCurBrand=i;
    i=0;
    while(tempProduct.desc[i]!=0)
    {
        i++;
    }
    placeInCurDesc=i;
    for(j=0;j<tempProduct.numSpecs;j++)
    {
        i=0;
        while(tempProduct.specNames[j][i]!=0)
        {
            i++;
        }
        placesInStringSpecNames[j]=i;
        i=0;
        while(tempProduct.specs[j][i]!=0)
        {
            i++;
        }
        placesInStringSpecs[j]=i;
    }

    do
    {
        system("cls");
        cprintf(topMessageColors[topMessage],topMessages[topMessage]);
        if(selection==0)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Name: %s\n\t",tempProduct.name);
        if(selection==1)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Type: %s\n\t",tempProduct.type);
        if(selection==2)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Brand: %s\n\t",tempProduct.brand);
        if(selection==3)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Price: $%.2f\n\n\t",((float)tempProduct.priceCents)/100);
        if(selection==4)
        {
            cprintf(15," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Product Description:\n\t%s\n\n\t",tempProduct.desc);
        if(selection==5)
        {
            cprintf(14," > ");
        }
        else
        {
            cprintf(14,"   ");
        }
        printf("Edit Product Specs\n\n\t");
        if(selection==6)
        {
            cprintf(10," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Item Stock: %i\n\n\t",tempProduct.stock);
        if(selection==7)
        {
            cprintf(10," > ");
        }
        else
        {
            cprintf(15,"   ");
        }
        printf("Done\n\n");
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<7)
                {
                    selection++;
                }
                break;
            case 13:
                switch(selection)
                {
                    case 0:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Name:\n\n\t   Product Name: ");
                            printf("%s",tempProduct.name);
                            if(placeInCurName<50)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",tempProduct.type,tempProduct.brand,((float)tempProduct.priceCents)/100,tempProduct.desc,tempProduct.stock);
                            input=getch();
                            if(input==8&&placeInCurName>0)
                            {
                                placeInCurName--;
                                tempProduct.name[placeInCurName]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurName<50)
                            {
                                tempProduct.name[placeInCurName]=input;
                                placeInCurName++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 1:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Type:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s",tempProduct.name);

                            setColor(15);
                            printf("\n\t   Product Type: %s",tempProduct.type);
                            if(placeInCurType<30)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",tempProduct.brand,((float)tempProduct.priceCents)/100,tempProduct.desc,tempProduct.stock);
                            input=getch();
                            if(input==8&&placeInCurType>0)
                            {
                                placeInCurType--;
                                tempProduct.type[placeInCurType]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurType<30)
                            {
                                tempProduct.type[placeInCurType]=input;
                                placeInCurType++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 2:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Brand:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s",tempProduct.name,tempProduct.type);
                            setColor(15);
                            printf("\n\t   Product Brand: %s",tempProduct.brand);
                            if(placeInCurBrand<30)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",((float)tempProduct.priceCents)/100,tempProduct.desc,tempProduct.stock);
                            input=getch();
                            if(input==8&&placeInCurBrand>0)
                            {
                                placeInCurBrand--;
                                tempProduct.brand[placeInCurBrand]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurBrand<30)
                            {
                                tempProduct.brand[placeInCurBrand]=input;
                                placeInCurBrand++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 3:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Price:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s",tempProduct.name,tempProduct.type,tempProduct.brand);
                            setColor(15);
                            printf("\n\t   Product Price: $%.2f",((float)tempProduct.priceCents)/100);
                            setColor(8);
                            printf("\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",tempProduct.desc,tempProduct.stock);
                            input=getch();
                            if(input==8)
                            {
                                tempProduct.priceCents/=10;
                            }
                            else if((input>=48&&input<=57)&&tempProduct.priceCents*10<10000000)
                            {
                                tempProduct.priceCents=(tempProduct.priceCents*10)+input-48;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 4:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease enter the Product Description:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f",tempProduct.name,tempProduct.type,tempProduct.brand,((float)tempProduct.priceCents)/100);
                            setColor(15);
                            printf("\n\n\t   Product Description:\n\t   %s",tempProduct.desc);
                            if(placeInCurDesc<500)
                            {
                                printf("_");
                            }
                            setColor(8);
                            printf("\n\n\t   Edit Product Specs\n\n\t   Item Stock: %i\n\n\t   Done\n\n",tempProduct.stock);
                            input=getch();
                            if(input==8&&placeInCurDesc>0)
                            {
                                placeInCurDesc--;
                                tempProduct.desc[placeInCurDesc]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInCurDesc<500)
                            {
                                tempProduct.desc[placeInCurDesc]=input;
                                placeInCurDesc++;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 5:
                        selection=0;
                        do
                        {
                            system("cls");
                            if(selection==0)
                            {
                                cprintf(14," > ");
                            }
                            else
                            {
                                cprintf(14,"   ");
                            }
                            printf("Create new spec [%i/20]\n",tempProduct.numSpecs);
                            if(selection==1)
                            {
                                cprintf(10," > ");
                            }
                            else
                            {
                                cprintf(15,"   ");
                            }
                            printf("Done\n\n");
                            for(i=1;i<=tempProduct.numSpecs;i++)
                            {
                                if(selection-1==i)
                                {
                                    cprintf(15," > ");
                                }
                                else
                                {
                                    cprintf(15,"   ");
                                }
                                printf("%s : %s\n",tempProduct.specNames[i-1],tempProduct.specs[i-1]);
                            }
                            input=getch();
                            switch(input)
                            {
                                case 72:
                                    if(selection>0)
                                    {
                                        selection--;
                                    }
                                    break;
                                case 80:
                                    if(selection<=tempProduct.numSpecs)
                                    {
                                        selection++;
                                    }
                                    break;
                                case 13:
                                    if(selection==0&&tempProduct.numSpecs<20)
                                    {
                                        do
                                        {
                                            system("cls");
                                            cprintf(15,"\n\n\tPlease Enter the requested specification elements:\n\n\tSpec Name: ");
                                            printf("%s",tempProduct.specNames[tempProduct.numSpecs]);
                                            if(placesInStringSpecNames[tempProduct.numSpecs]<30)
                                            {
                                                printf("_");
                                            }
                                            cprintf(8,"\n\tSpec: ");
                                            printf("%s\n\n",tempProduct.specs[tempProduct.numSpecs]);
                                            input=getch();
                                            if(input==8&&placesInStringSpecNames[tempProduct.numSpecs]>0)
                                            {
                                                placesInStringSpecNames[tempProduct.numSpecs]--;
                                                tempProduct.specNames[tempProduct.numSpecs][placesInStringSpecNames[tempProduct.numSpecs]]=0;
                                            }
                                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placesInStringSpecNames[tempProduct.numSpecs]<30)
                                            {
                                                tempProduct.specNames[tempProduct.numSpecs][placesInStringSpecNames[tempProduct.numSpecs]]=input;
                                                placesInStringSpecNames[tempProduct.numSpecs]++;
                                            }
                                            //acceptChar(input,&placesInStringSpecNames[tempProduct.numSpecs],30,&tempProduct.specNames[tempProduct.numSpecs]);
                                        }
                                        while((input!=13&&input!=27)||tempProduct.specNames[tempProduct.numSpecs][0]==0);
                                        if(input==27)
                                        {
                                            for(i=0;i<30;i++)
                                            {
                                                tempProduct.specNames[tempProduct.numSpecs][i]=0;
                                            }
                                            placesInStringSpecNames[tempProduct.numSpecs]=0;
                                            break;
                                        }
                                        do
                                        {
                                            system("cls");
                                            cprintf(15,"\n\n\tReview the requested specification elements:");
                                            cprintf(8,"\n\n\tSpec Name: ");
                                            printf("%s",tempProduct.specNames[tempProduct.numSpecs]);
                                            if(placesInStringSpecs[tempProduct.numSpecs]<30)
                                            {
                                                printf("_");
                                            }
                                            cprintf(15,"\n\tSpec: ");
                                            printf("%s\n\n",tempProduct.specs[tempProduct.numSpecs]);
                                            input=getch();
                                            if(input==8&&placesInStringSpecs[tempProduct.numSpecs]>0)
                                            {
                                                placesInStringSpecs[tempProduct.numSpecs]--;
                                                tempProduct.specs[tempProduct.numSpecs][placesInStringSpecs[tempProduct.numSpecs]]=0;
                                            }
                                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placesInStringSpecs[tempProduct.numSpecs]<30)
                                            {
                                                tempProduct.specs[tempProduct.numSpecs][placesInStringSpecs[tempProduct.numSpecs]]=input;
                                                placesInStringSpecs[tempProduct.numSpecs]++;
                                            }
                                        }
                                        while((input!=13&&input!=27)||tempProduct.specs[tempProduct.numSpecs][0]==0);
                                        if(input==27)
                                        {
                                            for(i=0;i<30;i++)
                                            {
                                                tempProduct.specNames[tempProduct.numSpecs][i]=0;
                                                tempProduct.specs[tempProduct.numSpecs][i]=0;
                                            }
                                            placesInStringSpecs[tempProduct.numSpecs]=0;
                                            break;
                                        }
                                        tempProduct.numSpecs++;
                                        selection=0;
                                        input=0;
                                    }
                                    else if(selection==1)
                                    {

                                    }
                                    else
                                    {

                                    }
                                    break;
                            }
                        }
                        while(input!=27&&!(input==13&&selection==1));
                        selection=0;
                        input=0;
                        break;
                    case 6:
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tAdjust the number of your product which are for sale:\n\n\t");
                            setColor(8);
                            printf("   Product Name: %s\n\t   Product Type: %s\n\t   Product Brand: %s\n\t   Product Price: $%.2f\n\n\t   Product Description:\n\t   %s\n\n\t   Edit Product Specs",tempProduct.name,tempProduct.type,tempProduct.brand,((float)tempProduct.priceCents)/100,tempProduct.desc);
                            setColor(15);
                            printf("\n\n\t   Item Stock: %i [Arrow Keys to Adjust]",tempProduct.stock);
                            setColor(8);
                            printf("\n\n\t   Done\n\n");
                            input=getch();
                            switch(input)
                            {
                                case 72:case 77:
                                    if(tempProduct.stock<10)
                                    {
                                        tempProduct.stock++;
                                    }
                                    break;
                                case 80:case 75:
                                    if(tempProduct.stock>1)
                                    {
                                        tempProduct.stock--;
                                    }
                                    break;
                            }
                        }
                        while(input!=13&&input!=27);
                        break;
                    case 7:
                        selection=0;
                        if(tempProduct.name[0]!=0&&tempProduct.type[0]!=0&&tempProduct.brand[0]!=0&&tempProduct.priceCents!=0&&tempProduct.desc[0]!=0)
                        {
                            do
                            {
                                system("cls");
                                cprintf(11,"\n\n\tAre you sure you would like to edit this product?\n\n");
                                if(selection==0)
                                {
                                    cprintf(12,"\t > No");
                                    cprintf(15,"\n\t   Yes\n\n");
                                }
                                else
                                {
                                    cprintf(15,"\t   No");
                                    cprintf(10,"\n\t > Yes\n\n");
                                }
                                input=getch();
                                switch(input)
                                {
                                    case 72:
                                        selection=0;
                                        break;
                                    case 80:
                                        selection=1;
                                        break;
                                    case 13:
                                        if(selection==1)
                                        {
                                            fp=fopen("products//allProducts.txt","r");
                                            temp=fopen("products//temp.txt","w");

                                            tempProduct2=tempProduct;
                                            getNumProducts();
                                            for(k=0;k<numProducts;k++)
                                            {
                                                fgetc(fp);
                                                loadNextProduct(fp);
                                                printf("%i,%i",tempProduct2.numInFile-1,k);
                                                getch();
                                                if(tempProduct2.numInFile-1!=k)
                                                {
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct.name[i]!=0)
                                                    {
                                                        fputc(tempProduct.name[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct.type[i]!=0)
                                                    {
                                                        fputc(tempProduct.type[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct.brand[i]!=0)
                                                    {
                                                        fputc(tempProduct.brand[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    for(i=6;i>=0;i--)
                                                    {
                                                        fputc(((tempProduct.priceCents/((int)pow(10,(double)i)))%10)+48,temp);
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct.desc[i]!=0)
                                                    {
                                                        fputc(tempProduct.desc[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    fputc((tempProduct.numSpecs/10)+48,temp);
                                                    fputc((tempProduct.numSpecs%10)+48,temp);
                                                    fputc(10,temp);
                                                    for(i=0;i<tempProduct.numSpecs;i++)
                                                    {
                                                        j=0;
                                                        while(tempProduct.specNames[i][j]!=0)
                                                        {
                                                            fputc(tempProduct.specNames[i][j],temp);
                                                            j++;
                                                        }
                                                        fputc(10,temp);
                                                        j=0;
                                                        while(tempProduct.specs[i][j]!=0)
                                                        {
                                                            fputc(tempProduct.specs[i][j],temp);
                                                            j++;
                                                        }
                                                        fputc(10,temp);
                                                    }
                                                    fputc((tempProduct.stock/10)+48,temp);
                                                    fputc((tempProduct.stock%10)+48,temp);
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct.ownerName[i]!=0)
                                                    {
                                                        fputc(tempProduct.ownerName[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);

                                                    j=0;
                                                    while(tempProduct.dateCreated[j]!=0)
                                                    {
                                                        fputc(tempProduct.dateCreated[j],temp);
                                                        j++;
                                                    }
                                                    j=0;
                                                    fputc(10,temp);
                                                    while(tempProduct.dateEdited[j]!=0)
                                                    {
                                                        fputc(tempProduct.dateEdited[j],temp);
                                                        j++;
                                                    }
                                                    fputc(10,temp);
                                                }
                                                else
                                                {
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct2.name[i]!=0)
                                                    {
                                                        fputc(tempProduct2.name[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct2.type[i]!=0)
                                                    {
                                                        fputc(tempProduct2.type[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct2.brand[i]!=0)
                                                    {
                                                        fputc(tempProduct2.brand[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    for(i=6;i>=0;i--)
                                                    {
                                                        fputc(((tempProduct2.priceCents/((int)pow(10,(double)i)))%10)+48,temp);
                                                    }
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct2.desc[i]!=0)
                                                    {
                                                        fputc(tempProduct2.desc[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);
                                                    fputc((tempProduct2.numSpecs/10)+48,temp);
                                                    fputc((tempProduct2.numSpecs%10)+48,temp);
                                                    fputc(10,temp);
                                                    for(i=0;i<tempProduct2.numSpecs;i++)
                                                    {
                                                        j=0;
                                                        while(tempProduct2.specNames[i][j]!=0)
                                                        {
                                                            fputc(tempProduct2.specNames[i][j],temp);
                                                            j++;
                                                        }
                                                        fputc(10,temp);
                                                        j=0;
                                                        while(tempProduct2.specs[i][j]!=0)
                                                        {
                                                            fputc(tempProduct2.specs[i][j],temp);
                                                            j++;
                                                        }
                                                        fputc(10,temp);
                                                    }
                                                    fputc((tempProduct2.stock/10)+48,temp);
                                                    fputc((tempProduct2.stock%10)+48,temp);
                                                    fputc(10,temp);
                                                    i=0;
                                                    while(tempProduct2.ownerName[i]!=0)
                                                    {
                                                        fputc(tempProduct2.ownerName[i],temp);
                                                        i++;
                                                    }
                                                    fputc(10,temp);

                                                    j=0;
                                                    while(tempProduct2.dateCreated[j]!=0)
                                                    {
                                                        fputc(tempProduct2.dateCreated[j],temp);
                                                        j++;
                                                    }

                                                    current_time = time(NULL);
                                                    c_time_string = ctime(&current_time);

                                                    j=0;
                                                    fputc(10,temp);
                                                    while(c_time_string[j]!=10)
                                                    {
                                                        fputc(c_time_string[j],temp);
                                                        j++;
                                                    }
                                                    fputc(10,temp);
                                                }
                                            }
                                            fflush(temp);
                                            fclose(temp);
                                            fflush(fp);
                                            fclose(fp);
                                            remove("products//allProducts.txt");
                                            rename("products//temp.txt","products//allProducts.txt");
                                            return;
                                        }
                                        break;
                                }
                            }
                            while(input!=27&&input!=8&&input!=13);
                        }
                        else
                        {
                            topMessage=1;
                        }
                        selection=7;
                        input=0;
                        break;
                }
                break;
            case 27:
                selection=0;
                do
                {
                    system("cls");
                    cprintf(11,"\n\n\tAre you sure you would like to cancel editing this product?\n\n");
                    if(selection==0)
                    {
                        cprintf(10,"\t > No");
                        cprintf(15,"\n\t   Yes\n\n");
                    }
                    else
                    {
                        cprintf(15,"\t   No");
                        cprintf(12,"\n\t > Yes\n\n");
                    }
                    input=getch();
                    switch(input)
                    {
                        case 72:
                            selection=0;
                            break;
                        case 80:
                            selection=1;
                            break;
                        case 13:
                            if(selection==1)
                            {
                                return;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=0;
                input=0;
                break;
            }
    }
    while(!(input==13&&selection==7));
}

/*
    Function: removeProduct
    Does:

    removes a product for allProducts.txt by copying everything from that file into a temp.txt file except for that product and making temp into allProducts

    Local Variables used:

    - int i,j,k : counters
    - FILE* fp and temp : file pointers to allProducts and a temp file which will become allProducts by the end

    Receives:

    * an integer representng the number of the product to be removed

    Returns:

    * nothing
*/

void removeProduct(int prodNum)
{
    int i,j,k;
    FILE*fp=fopen("products//allProducts.txt","r");
    FILE*temp=fopen("products//temp.txt","w");

    getNumProducts();
    for(k=0;k<numProducts;k++)
    {
        printf("%i",fgetc(fp));
        getch();
        loadNextProduct(fp);
        printf("%i,%i",prodNum-1,k);
        getch();
        if(prodNum-1!=k)
        {
            fputc(10,temp);
            i=0;
            while(tempProduct.name[i]!=0)
            {
                fputc(tempProduct.name[i],temp);
                i++;
            }
            fputc(10,temp);
            i=0;
            while(tempProduct.type[i]!=0)
            {
                fputc(tempProduct.type[i],temp);
                i++;
            }
            fputc(10,temp);
            i=0;
            while(tempProduct.brand[i]!=0)
            {
                fputc(tempProduct.brand[i],temp);
                i++;
            }
            fputc(10,temp);
            i=0;
            for(i=6;i>=0;i--)
            {
                fputc(((tempProduct.priceCents/((int)pow(10,(double)i)))%10)+48,temp);
            }
            fputc(10,temp);
            i=0;
            while(tempProduct.desc[i]!=0)
            {
                fputc(tempProduct.desc[i],temp);
                i++;
            }
            fputc(10,temp);
            fputc((tempProduct.numSpecs/10)+48,temp);
            fputc((tempProduct.numSpecs%10)+48,temp);
            fputc(10,temp);
            for(i=0;i<tempProduct.numSpecs;i++)
            {
                j=0;
                while(tempProduct.specNames[i][j]!=0)
                {
                    fputc(tempProduct.specNames[i][j],temp);
                    j++;
                }
                fputc(10,temp);
                j=0;
                while(tempProduct.specs[i][j]!=0)
                {
                    fputc(tempProduct.specs[i][j],temp);
                    j++;
                }
                fputc(10,temp);
            }
            fputc((tempProduct.stock/10)+48,temp);
            fputc((tempProduct.stock%10)+48,temp);
            fputc(10,temp);
            i=0;
            while(tempProduct.ownerName[i]!=0)
            {
                fputc(tempProduct.ownerName[i],temp);
                i++;
            }
            fputc(10,temp);

            j=0;
            while(tempProduct.dateCreated[j]!=0)
            {
                fputc(tempProduct.dateCreated[j],temp);
                j++;
            }
            //printf("lol%i ",tempProduct.dateCreated[j]);
            //getch();
            j=0;
            fputc(10,temp);
            while(tempProduct.dateEdited[j]!=0)
            {
                fputc(tempProduct.dateEdited[j],temp);
                j++;
            }
            fputc(10,temp);
        }
    }
    setNumProducts(numProducts-1);
    numProducts--;
    getNumProductsUser(tempProduct.ownerName);
    setNumProductsUser(tempProduct.ownerName,curUser.numOwnedProducts-1);
    getNumProductsUser(curUser.uName);
    fflush(temp);
    fclose(temp);
    fflush(fp);
    fclose(fp);
    remove("products//allProducts.txt");
    rename("products//temp.txt","products//allProducts.txt");
}

/*
    Function: acquireAdmin
    Does:

    presents the user with a menu in which they enter a code and acquire admin ataus if it is correct

    Local Variables used:

    - int i,j
    - char input
    - int selection

    - FILE*fp : file pointer
    - char curCode[] : stores code so far
    - int placeInCode : stores place in typing the code
    - char userDirName[] : full user directory name for fopen

    Receives:

    * nothing

    Returns:

    * nothing
*/

void acquireAdmin(void)
{
    int i=0,j;
    char input;
    int selection=0;
    FILE*fp;
    char curCode[31]={0};
    int placeInCode=0;
    char userDirName[61];
    sprintf(userDirName,"users//%s//userInfo.txt",curUser.uName);

    do
    {
        system("cls");
        cprintf(15,"\n\n\tEnter the admin code to acquire administrator permissions:\n\n\t");
        printf("%s",curCode);
        if(placeInCode<30)
        {
            printf("_");
        }
        printf("\n\n");
        input=getch();
        if(input==8&&placeInCode>0)
        {
            placeInCode--;
            curCode[placeInCode]=0;
        }
        else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))&&placeInCode<30)
        {
            curCode[placeInCode]=input;
            placeInCode++;
        }
    }
    while(input!=13&&input!=27);
    if(input==13)
    {
        if(strcmp(curCode,"ALPHA")==0)
        {
            fp=fopen(userDirName,"r");
            while(fgetc(fp)!=10)
            {
                i++;
            }
            fflush(fp);
            fclose(fp);
            fp=fopen(userDirName,"r+");
            fseek(fp,i+2,SEEK_SET);
            fputc(49,fp);
            fflush(fp);
            fclose(fp);
            curUser.hasAdmin=1;
            system("cls");
            cprintf(10,"\n\n\tPermissions Granted");
            getch();
        }
        else
        {
            system("cls");
            cprintf(12,"\n\n\tCode Invalid, Permission Denied");
            getch();
        }
    }
}

/*
    Function: adminView
    Does:

    presents the user with an admin view with options to change users' passwords and admin statuses

    Local Variables used:

    - int i,j
    - char input
    - int selection

    - char curUsername[] : stores the current user name of the user the admin wants to tamper with
    - int placeInStringUN : stores the place in the username the admin is typing
    - char fullUName[61] : full directory name to be set and used for fopen
    - char newPassword[31] : stores the new password of a user the admin is currently typing
    - int placeInNewPassword : place in said password
    - char oldUserPassword[71] : the old user password
    - char c
    - FILE*fp

    Receives:

    * nothing

    Returns:

    * nothing
*/

void adminView(void)
{
    int i,j;
    char input;
    int selection=0;
    char curUsername[31]={0};
    int placeInStringUN=0;
    char fullUName[61]={0};
    char newPassword[31];
    int placeInNewPassword=0;
    char oldUserPassword[71]={0};
    char c;
    FILE*fp;

    do
    {
        system("cls");
        cprintf(15,"\n\n\tSelect an additional admin privilege to continue:\n\n\t");
        if(selection==0)
        {
            cprintf(10," > Change User Passwords\n\t");
            cprintf(15,"   Change User Admin Status\n\n\t");
        }
        else
        {
            cprintf(15,"   Change User Passwords\n\t");
            cprintf(10," > Change User Admin Status\n\n\t");
        }
        input=getch();
        switch(input)
        {
            case 72:
                selection=0;
                break;
            case 80:
                selection=1l;
                break;
            case 13:
                switch(selection)
                {
                    case 0:
                        selection=0;
                        placeInStringUN=0;
                        for(i=0;i<31;i++)
                        {
                            curUsername[i]=0;
                        }
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease input a username to continue:\n\n\t");
                            printf("%s",curUsername);
                            if(placeInStringUN<30)
                            {
                                cprintf(15,"_");
                            }
                            printf("\n\n");
                            input=getch();
                            if(input==13)
                            {
                                sprintf(fullUName,"users//%s//userInfo.txt",curUsername);
                                fp=fopen(fullUName,"r");
                                if(fp==NULL)
                                {
                                    system("cls");
                                    cprintf(14,"\n\n\tThis user does not exist.");
                                    getch();
                                }
                                else
                                {
                                    placeInNewPassword=0;
                                    for(i=0;i<31;i++)
                                    {
                                        newPassword[i]=0;
                                    }
                                    do
                                    {
                                        system("cls");
                                        cprintf(15,"\n\n\tPlease enter the new password:\n\n\t");
                                        printf("%s",newPassword);
                                        if(placeInNewPassword<30)
                                        {
                                            cprintf(15,"_");
                                        }
                                        printf("\n\n");
                                        input=getch();
                                        if(input==13)
                                        {
                                            if(newPassword[9]==0)
                                            {
                                                system("cls");
                                                cprintf(14,"\n\n\tPassword is too short.");
                                                getch();
                                            }
                                            else
                                            {
                                                c=fgetc(fp);
                                                while(c!=10)
                                                {
                                                    c=fgetc(fp);
                                                }
                                                tempUser.hasAdmin=fgetc(fp)-48;
                                                fflush(fp);
                                                fclose(fp);
                                                fp=fopen(fullUName,"w");
                                                i=0;
                                                while(newPassword[i]!=0)
                                                {
                                                    j=newPassword[i]-((newPassword[i]/65)*43)-((newPassword[i]/97)*58)+4;
                                                    j=8*(j%8)+(j/8);
                                                    printf("%i",j);
                                                    if(j/16>=10)
                                                    {
                                                        fputc((j/16)+55,fp);
                                                    }
                                                    else
                                                    {
                                                        fputc((j/16)+48,fp);
                                                    }
                                                    if(j%16>=10)
                                                    {
                                                        fputc((j%16)+55,fp);
                                                    }
                                                    else
                                                    {
                                                        fputc((j%16)+48,fp);
                                                    }

                                                    i++;
                                                }
                                                fputc(10,fp);
                                                fputc(tempUser.hasAdmin+48,fp);
                                                fflush(fp);
                                                fclose(fp);
                                            }
                                        }
                                        else if(input==8&&placeInNewPassword>0)
                                        {
                                            placeInNewPassword--;
                                            newPassword[placeInNewPassword]=0;
                                        }
                                        else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))&&placeInNewPassword<30)
                                        {
                                            newPassword[placeInNewPassword]=input;
                                            placeInNewPassword++;
                                        }
                                    }
                                    while(input!=27&&input!=13);
                                }
                            }
                            else if(input==8&&placeInStringUN>0)
                            {
                                placeInStringUN--;
                                curUsername[placeInStringUN]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInStringUN<30)
                            {
                                curUsername[placeInStringUN]=input;
                                placeInStringUN++;
                            }
                        }
                        while(input!=27&&input!=13);
                        selection=0;
                        input=0;
                        break;
                    case 1:
                         selection=0;
                        placeInStringUN=0;
                        for(i=0;i<31;i++)
                        {
                            curUsername[i]=0;
                        }
                        do
                        {
                            system("cls");
                            cprintf(15,"\n\n\tPlease input a username to continue:\n\n\t");
                            printf("%s",curUsername);
                            if(placeInStringUN<30)
                            {
                                cprintf(15,"_");
                            }
                            printf("\n\n");
                            input=getch();
                            if(input==13)
                            {
                                sprintf(fullUName,"users//%s//userInfo.txt",curUsername);
                                fp=fopen(fullUName,"r");
                                if(fp==NULL)
                                {
                                    system("cls");
                                    cprintf(14,"\n\n\tThis user does not exist.");
                                    getch();
                                }
                                else
                                {
                                    system("cls");
                                    cprintf(15,"\n\n\tShould this user have admin privileges? [y/n]");
                                    do
                                    {
                                        input=getch();
                                        switch(input)
                                        {
                                        case 'n':case 'N':case 'y':case 'Y':
                                                for(i=0;i<71;i++)
                                                {
                                                    oldUserPassword[i]=0;
                                                }
                                                i=0;
                                                c=fgetc(fp);
                                                while(c!=10)
                                                {
                                                    oldUserPassword[i]=c;
                                                    i++;
                                                    c=fgetc(fp);
                                                }
                                                fflush(fp);
                                                fclose(fp);
                                                fp=fopen(fullUName,"w");
                                                i=0;
                                                while(oldUserPassword[i]!=0)
                                                {
                                                    fputc(oldUserPassword[i],fp);
                                                    i++;
                                                }
                                                fputc(10,fp);
                                                if(input=='n'||input=='N')
                                                {
                                                    fputc(48,fp);
                                                }
                                                else
                                                {
                                                    fputc(49,fp);
                                                }
                                                fflush(fp);
                                                fclose(fp);
                                                if(strcmp(curUser.uName,curUsername)==0)
                                                {
                                                    if(input=='n'||input=='N')
                                                    {
                                                        curUser.hasAdmin=0;
                                                    }
                                                    else
                                                    {
                                                        curUser.hasAdmin=1;
                                                    }
                                                }
                                                input=27;
                                                break;
                                        }
                                    }
                                    while(input!=27&&input!='n'&&input!='N'&&input!='y'&&input!='Y');
                                }
                            }
                            else if(input==8&&placeInStringUN>0)
                            {
                                placeInStringUN--;
                                curUsername[placeInStringUN]=0;
                            }
                            else if(((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==32)&&placeInStringUN<30)
                            {
                                curUsername[placeInStringUN]=input;
                                placeInStringUN++;
                            }
                        }
                        while(input!=27&&input!=13);
                        selection=0;
                        input=0;
                        break;
                }
                break;
        }
    }
    while(input!=27&&input!=0);
}

/*
    Function: getStr
    Does:

    gets a string on one line given a file pointer and stores it in a destination given as a char*

    Local Variables used:

    - char c : last character from file
    - int i : a counter

    Receives:

    - a file pointer
    - a char pointer dest
    - an integer representing the character limit

    Returns:

    * nothing
*/

void getStr(FILE*fp,char*dest,int limit)
{
    int i=0;
    char c;

    for(i=0;i<=limit;i++)//reset dest
    {
        dest[i]=0;
    }

    i=0; //reset
    c=fgetc(fp); //get char
    while(c!=10&&c!=-1) //while not enter or eof
    {
        if(i<limit) // do if the limit has not been reached yet
        {
            dest[i]=c;
            i++;
        }
        c=fgetc(fp); //get char
    }
    dest[i]=0; //set end just in case
}

/*
    Function: getInt
    Does:

    gets an integer on one line given a file pointer and stores it in a destination given as an int*

    Local Variables used:

    - char c : last character from file

    Receives:

    - a file pointer
    - an integer pointer *dest

    Returns:

    * nothing
*/

void getInt(FILE*fp,int*dest)
{
    char c;

    *dest=0; //reset destination
    c=fgetc(fp); //get char
    while(c!=10&&c!=-1) //while neither enter nor eof
    {
        *dest*=10; //multiply existing value by 10 (shift digits it over)
        *dest+=c-48; //add the new digit found
        c=fgetc(fp); //get char
    }
}

//creates a file under a certain name, only ended up being used in the unused initialCheck function

fcreate(char*fName)
{
    FILE*fp=fopen(fName,"w");
    if(fp==NULL)
    {
        fflush(fp);
        fclose(fp);
        return 0;
    }
    else
    {
        fflush(fp);
        fclose(fp);
        return 1;
    }
}

//new seColor function, still don't quite know how it works but it is very useful, can set background color of character as well (logo)

void setColor(int col)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,col);
}

//old setColor function, inferior to my knowledge but is proven to work

void setColor2(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

//simple function prints a string in a color

void cprintf(int c,char*s) //saves a lot of lines of code in display, arguements are color then string
{
    //self-explanatory
    setColor(c);
    printf(s);
}

//simple function prints an integer in a color

void cprinti(int c,int i) //saves some of lines of code in display, arguements are color then integer
{
    //self-explanatory
    setColor(c);
    printf("%i",i);
}

//simple function prints a character in a color

void cprintc(int c,char ch)
{
    //self-explanatory
    setColor(c);
    printf("%c",ch);
}
