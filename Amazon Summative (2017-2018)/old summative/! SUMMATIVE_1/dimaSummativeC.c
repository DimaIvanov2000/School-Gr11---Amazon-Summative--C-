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
    - (header files, likely not included)
    - Google Doc with code

    A program of a mock shopping software for electronics

    **************************************
    Admin code is "ALPHA" (case-sensitive)
    **************************************


    Things that this program does:
    -

***************************************************************/

/*

Level 1-2

x Present a ‘Seller’ view and a ‘Buyer’ view (a menu to choose)
x Items for sale are listed.
x Buyers can ‘Buy’ items
    x Users should be able to select at least one item to buy
    x Users should enter how much money they are paying with and be given correct change
    x Quantity of items available should decrease accordingly
x Sellers should be able to:
    x Add new items to the list
o User login screen is attempted, but not functional.
o Use file(s) to store items to disk to preserve data.


Level 3

x Sellers should be able to add new items for sale and delete old, but not "buy"
x Sellers should be able to edit items (change quantity / name / price)
x Buyers should be able to "buy" as above, but not add/edit/delete
x Buyers should be able to "buy" as above, but must be able to buy multiple items and "purchase" them all in one transaction. Remember: quantities must be adjusted accordingly.
*********************************************************

 Use file(s) to read data at only the start of the program, and write at the end of the program

*********************************************************


Level 4

x Provide an initial menu such as: 1. Enter the market / 2. Login
o Provide a login prompt
    x Automatically direct ‘admin’ to an Admin View that will allow the ‘admin’ user to edit sellers accounts (username/password) as well as change the admin password
    x Automatically direct sellers to the ‘Seller’ view as above
x Sellers should only be able to see the items they have listed (and not the items of other sellers)

Level X (Personal Goals)

x Time stamp transactions and posts
o Encrypt Password Storage


*/

/* Header Includes */

#include<stdio.h>
#include<windows.h>
#include<math.h>

/* Define Statements */

//For readability purposes
#define TRUE 1
#define FALSE 0

/* Structure Declarations */

typedef struct
{
    char type[31];
    char name[51];
    char brand[31];
    int priceCents;
    char desc[501];
    int numSpecs;
    char specNames[20][31];
    char specs[20][51];
    int stock;
    char ownerName[30];
}
product;

/* Global Variables */

product productPage[5];
product tempProduct;
int numProducts=0;
char operationalUserName[31]={0};
int userHasAdmin=0;

/* Function Prototypes */

int initialCheck(void);

int displayTitle(void);
int loginScreen(void);
int createAccount(void);

int loadProductPage(char*,int); //NOT WORKING ANY MORE
void printProductPage(int);
void mainAccountView(void);
    void buyerView(void);
    void sellerView(void);
        void createProduct(void);
        void removeProduct(void);
    void acquireAdmin(void);
    void adminView(void);
    void viewHelp(void);
void displayProduct(void);

void acceptChar(char,int*,int,char*);
void getStr(FILE*,char*,int);
void getInt(FILE*,int*);

int fcreate(char*);

//void printOption(int,int,char*);
void setColor(int);
void setColor2(int);
void cprintf(int,char*);
void cprinti(int,int);
void cprintc(int,char);

/* MAIN */
int main(void)
{
    if(initialCheck()==0)
    {
        setColor(15);
        return 0;
    }

    while(displayTitle()==1)
    {
        if(loginScreen()==1)
        {
            mainAccountView();
//            loadProductPage("monitor",1);
//            printProductPage(1);
//            getch();
        }
    }

    setColor(15);
    return 0;
}
/* Function Declarations */


/*
    Function:
    Does:

    Global Variables used:
    -
    -
    -
    Local Variables used:
    -
    -
    -
    Receives:
    Returns:
*/

/*
    Function: initialCheck
    Does:

    Runs a quick check to see if all the basic files needed for the program to operate exist, has options for managing the situation if need be.

    Global Variables used:

    -
    -
    -

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

/*
    Function: displayTitle
    Does:

    Shows the title and awaits a user input, returning 0 if user exits (esc) and 1 otherwise

    Global Variables used:

    -
    -
    -

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
    cprintf(15,"\n\tWelcome to...\n\n\n");
    for(i=0;i<7;i++)
    {
        printf("\t");
        for(j=0;j<31;j++)
        {
            cprintc(title[i][2*j],title[i][2*j+1]);
        }
        printf("\n");
    }

    cprintf(10,"\n\n\tPress any key to continue...");
    if(getch()==27)
    {
        system("cls");
        cprintf(15,"\n\tYou are now leaving...\n\n\n");
        for(i=0;i<7;i++)
        {
            printf("\t");
            for(j=0;j<31;j++)
            {
                cprintc(title[i][2*j],title[i][2*j+1]);
            }
            printf("\n");
        }

        cprintf(15,"\n\n\tThank you for visiting!");
        getch();
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
    Function: loginScreen
    Does:

    Presents user with an interactive login screen with options to enter their credentials,

    Global Variables used:

    -
    -
    -

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
                if(selection<4)
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
                        cprintf(15,"\n\n\tPlease Input Your Account Username:\n\n\t   ");
                        printf("Username: %s",curUsername);
                        if(placeInStringUN<30)
                        {
                            printf("_");
                        }
                        cprintf(8,"\n\t   ");
                        printf("Password: %s\n\n\t   Done\n\n\t   Forgot Password\n\t   Create New Account\n",curPassword);
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
                case 2:
                    if(curUsername[0]==0||curPassword[0]==0)
                    {
                        topMessage=1;
                        break;
                    }
                    sprintf(userInfoDirectoryName,"users//%s//userInfo.txt",curUsername);
                    fp=fopen(userInfoDirectoryName,"r");
                    if(fp==NULL)
                    {
                        fflush(fp);
                        fclose(fp);
                        topMessage=1;
                        break;
                    }
                    i=0;
                    passwordMatches=1;
                    c=fgetc(fp);
                    while(c!=10)
                    {
                        if(c>=65)
                        {
                            curPasswordChar=(c-55)*16;
                        }
                        else
                        {
                            curPasswordChar=(c-48)*16;
                        }
                        c=fgetc(fp);
                        if(c>=65)
                        {
                            curPasswordChar+=c-55;
                        }
                        else
                        {
                            curPasswordChar+=c-48;
                        }
                        curPasswordChar=((curPasswordChar%8)*8)+(curPasswordChar/8);
                        curPasswordChar=curPasswordChar-((curPasswordChar/26)*58)+((curPasswordChar/52)*15)+97;
                        if(curPasswordChar!=curPassword[i])
                        {
                            passwordMatches=0;
                            break;
                        }
                        i++;
                        c=fgetc(fp);
                    }
                    if(c==10)
                    {
                        userHasAdmin=fgetc(fp)-48;
                    }
                    fflush(fp);
                    fclose(fp);
                    if(passwordMatches==0||curPassword[i]!=0)
                    {
                        topMessage=1;
                        break;
                    }
                    else
                    {
                        strcpy(operationalUserName,curUsername);
                        return 1;
                    }
                    break;
                case 3:
                    system("cls");
                    cprintf(15,"\n\n\tLOL THATS A SHAME BYE");
                    getch();
                    break;
                case 4:
                    if(createAccount()==1)
                    {
                        return 1;
                    }
                    break;
                }
                break;
        }
    }
    while(input!=27);
    return 0;
}

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
    int numUsers;
    FILE*fp;
    char tempUN[31]={0};
    int userNameIsSame=0;
    char c;
    char newUserDirectoryName[81]={0};

    do
    {
        system("cls");
        printf("\n\n\t");
        cprintf(topMessageColors[topMessage],topMessages[topMessage]);
        printf("\n\n");
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
                        if(curUsername[0]==0||curPasswords[0][0]==0||curPasswords[1][0]==0)
                        {
                            topMessage=1;
                            break;
                        }
                        if(strcmp(curPasswords[0],curPasswords[1])!=0)
                        {
                            topMessage=2;
                            break;
                        }
                        if(curPasswords[0][9]==0||curPasswords[1][9]==0)
                        {
                            topMessage=3;
                            break;
                        }
                        if(strcmp(curUsername,"generalUserInfo")==0)
                        {
                            topMessage=4;
                            break;
                        }
                        //username in use
                        fp=fopen("users//generalUserInfo.txt","r");
                        c=0;
                        userNameIsSame=0;
                        while(c!=-1)
                        {
                                getStr(fp,tempUN,30);
                                if(strcmp(tempUN,curUsername)==0)
                                {
                                    userNameIsSame=1;
                                    break;
                                }
                                c=fgetc(fp);
                        }
                        fflush(fp);
                        fclose(fp);
                        if(userNameIsSame==1)
                        {
                            topMessage=4;
                            break;
                        }
                        //create account
                        fp=fopen("users//generalUserInfo.txt","a");
                        i=0;
                        while(curUsername[i]!=0)
                        {
                            fputc(curUsername[i],fp);
                            i++;
                        }
                        fputc(10,fp);
                        fputc(10,fp);
                        fflush(fp);
                        fclose(fp);
                        sprintf(newUserDirectoryName,"users//%s",curUsername);
                        mkdir(newUserDirectoryName,0700);
                        strcat(newUserDirectoryName,"//UserInfo.txt");
                        fp=fopen(newUserDirectoryName,"w");
                        i=0;
                        while(curPasswords[0][i]!=0)
                        {
                            j=curPasswords[0][i]-((curPasswords[0][i]/65)*43)-((curPasswords[0][i]/97)*58)+4;
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
                        fputc(48,fp);
                        fflush(fp);
                        fclose(fp);
                        sprintf(newUserDirectoryName,"users//%s//productInfo.txt",curUsername);
                        fp=fopen(newUserDirectoryName,"w");
                        fflush(fp);
                        fclose(fp);
                        userHasAdmin=0;
                        strcpy(operationalUserName,curUsername);
                        system("cls");
                        cprintf(10,"\n\n\tAccount successfully created under the name of ");
                        cprintf(11,operationalUserName);
                        cprintf(15,"\n\tNow entering account interface...");
                        getch();
                        return 1;
                        break;
                }
        }
    }
    while(input!=27);

    return 0;
}

int loadProductPage(char*name,int pageNum)
{
    int i,j;
    char input;
    int selection=0;
    char fName[81];

    sprintf(fName,"products//%s//page%i.txt",name,pageNum);
    FILE*fp=fopen(fName,"r");
    if(fp==NULL)
    {
        do
        {
            system("cls");
            cprintf(12,"\n\tThe requested product page could not be found");
            cprintf(11,"\n\tAttempt to continue anyways?\n\n\t");
            if(selection==0)
            {
                cprintf(10," > No, Exit\n\t");
                cprintf(15,"   Yes, Continue [Not Recommended]\n\n\t");
            }
            else
            {
                cprintf(15,"   No, Exit\n\t");
                cprintf(14," > Yes, Continue [Not Recommended]\n\n\t");
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
                    if(selection==0)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                    break;
            }
        }
        while(input!=27&&input!=8&&input!=13);
        return 0;
    }

    getInt(fp,&numProducts);
    for(i=0;i<numProducts;i++)
    {
        getStr(fp,&productPage[i].type,30);
        getStr(fp,&productPage[i].name,50);
        getStr(fp,&productPage[i].brand,30);
        getStr(fp,&productPage[i].desc,500);
        getInt(fp,&productPage[i].numSpecs);
        for(j=0;j<productPage[i].numSpecs;j++)
        {
            getStr(fp,&productPage[i].specNames[j],30);
            getStr(fp,&productPage[i].specs[j],50);
        }
    }

    fflush(fp);
    fclose(fp);
    return 1;
}

void printProductPage(int highlightNum)
{
    int i,j;

    //setColor(15);
    printf("Showing Results for: %s\n\n",productPage[i].type);

    for(i=0;i<numProducts;i++)
    {
        if(i==highlightNum)
        {
            setColor(10);
        }
        else
        {
            setColor(15);
        }
        printf("%c",201);
        for(j=0;j<59;j++)
        {
            printf("%c",205);
        }
        printf("%c\n",187);
        printf("%cProduct: %-50s%c\n%cBrand:   %-50s%c",186,productPage[i].name,186,186,productPage[i].brand,186);
        printf("\n%c",200);
        for(j=0;j<59;j++)
        {
            printf("%c",205);
        }
        printf("%c\n",188);
    }
}

void mainAccountView(void)
{
    int i,j;
    char input;
    int selection=0;

    do
    {
        system("cls");
        cprintf(15,"\n\n\tWelcome, ");
        cprintf(11,operationalUserName);
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
            if(userHasAdmin==0)
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
            if(userHasAdmin==0)
            {
                cprintf(15,"   Acquire Admin\n\t");
            }
            else
            {
                cprintf(15,"   Admin View\n\t");
            }
        }
        if(selection==3)
        {
            cprintf(10," > Help\n\t");
        }
        else
        {
            cprintf(15,"   Help\n\t");
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
                buyerView();
                break;
            case 1:
                sellerView();
                break;
            case 2:
                if(userHasAdmin==0)
                {
                    acquireAdmin();
                }
                else
                {
                    adminView();
                }
                break;
            case 3:
                viewHelp();
                break;
            }
            break;
        }
    }
    while(input!=27);
}

void buyerView(void)
{
    int i,j;
    char input;
    int selection=0;

    do
    {
        system("cls");

        input=getch();
        switch(input)
        {

        }
    }
    while(input!=27);
}

void sellerView(void)
{
    int i,j;
    char input;
    int selection=0;

//    do
//    {
//        system("cls");
//
//        input=getch();
//        switch(input)
//        {
//
//        }
//    }
//    while(input!=27);
    createProduct();
}

void createProduct(void)
{
    //I COULD HAVE USED A STRUCTURE IM AN IDIOT

    int i,j;
    char input;
    int selection=0;
    char topMessages[2][81]={"\n\n\tPlease provide the information below to create a product:\n\n\t","\n\n\tOne or more necessary details have not been filled in\n\n\t"};
    int topMessageColors[2]={15,12};
    int topMessage=0;

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
                                            return;
                                        }
                                        else
                                        {

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

void removeProduct(void)
{

}

void acquireAdmin(void)
{
    int i=0,j;
    char input;
    int selection=0;
    FILE*fp;
    char curCode[31]={0};
    int placeInCode=0;
    char userDirName[61];
    sprintf(userDirName,"users//%s//userInfo.txt",operationalUserName);

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
            userHasAdmin=1;
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

void adminView(void)
{

}

void viewHelp(void)
{

}

void acceptChar(char c,int*place,int charLimit,char*string)
{
    if(c==8&&place>0)
    {
        place--;
        string[(int)place]=0;
    }
    else if(((c>=48&&c<=57)||(c>=65&&c<=90)||(c>=97&&c<=122)||c==32)&&place<charLimit)
    {
        string[(int)place]=c;
        place++;
    }
}

void getStr(FILE*fp,char*dest,int limit)
{
    int i=0;
    char c;

    for(i=0;i<=limit;i++)
    {
        dest[i]=0;
    }

    i=0;
    c=fgetc(fp);
    while(c!=10&&c!=-1)
    {
        if(i<limit)
        {
            dest[i]=c;
            i++;
        }
        c=fgetc(fp);
    }
}

void getInt(FILE*fp,int*dest)
{
    char c;

    *dest=0;
    c=fgetc(fp);
    while(c!=10)
    {
        *dest*=10;
        *dest+=c-48;
        c=fgetc(fp);
    }
}

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

//void printOption(int selection,int highlightNum,char*text)
//{
//
//}

void setColor(int col)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,col);
}

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

void cprintf(int c,char*s) //saves a lot of lines of code in display, arguements are color then string
{
    //self-explanatory
    setColor(c);
    printf(s);
}

void cprinti(int c,int i) //saves some of lines of code in display, arguements are color then integer
{
    //self-explanatory
    setColor(c);
    printf("%i",i);
}

void cprintc(int c,char ch)
{
    setColor(c);
    printf("%c",ch);
}
