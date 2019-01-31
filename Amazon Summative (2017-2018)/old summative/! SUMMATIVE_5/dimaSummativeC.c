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

o Encrypt Password Storage


*/

/* Header Includes */

#include<stdio.h>
#include<windows.h>
#include<math.h>

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
    char ownerName[31];
    char dateCreated[101];
    char dateEdited[101];
    int numInFile;
}
product;

typedef struct
{
    char uName[31];
    int hasAdmin;
    int numOwnedProducts;
}
user;

/* Global Variables */

product loadedProducts[5][5]; //page, number
product tempProduct;
product tempProduct2;
int numLoadedProducts=0;
int numProducts=0;
user curUser;

/* Function Prototypes */

int initialCheck(void); //NOT WORKING ANY MORE

int displayTitle(void);
int loginScreen(void);
int createAccount(void);

int loadProduct(int);
void loadNextProduct(FILE*);
void skipNextProduct(FILE*);
int loadProductPage(int);
int loadUserProductPage(int);

int getNumProducts(void);
int setNumProducts(int);
int getNumProductsUser(char*);
int setNumProductsUser(char*,int);
//void viewProductPages(void);
//void printUserProductPage(int);
void mainAccountView(void);
    void buyerView(void);
    void sellerView(void);
        void createProduct(void);
        void editProducts(void);
        void removeProduct(int);
    void acquireAdmin(void);
    void adminView(void);
    void viewHelp(void);

void acceptChar(char,int*,int,char*); //NOT WORKING
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
//    if(initialCheck()==0)
//    {
//        setColor(15);
//        return 0;
//    }

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
                        curUser.hasAdmin=fgetc(fp)-48;
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
                        strcpy(curUser.uName,curUsername);
                        getNumProductsUser(curUser.uName);
                        printf("%i",curUser.numOwnedProducts);
                        getch();
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
                        curUser.hasAdmin=0;
                        strcpy(curUser.uName,curUsername);
                        system("cls");
                        cprintf(10,"\n\n\tAccount successfully created under the name of ");
                        cprintf(11,curUser.uName);
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

int loadProduct(int itemNum)
{
    int i,j;
    char c;
    FILE*fp=fopen("products//allProducts.txt","r");
    if(fp==NULL)
    {
        return 0;
    }
    for(i=1;i<itemNum;i++)
    {
        skipNextProduct(fp);
    }
    loadNextProduct(fp);
    fflush(fp);
    fclose(fp);
    return 1;
}

void loadNextProduct(FILE*fp)
{
    int i;
    //getInt(fp,&tempProduct.numInFile);
    getStr(fp,tempProduct.name,50);
    getStr(fp,tempProduct.type,30);
    getStr(fp,tempProduct.brand,30);
    getInt(fp,&tempProduct.priceCents);
    getStr(fp,tempProduct.desc,500);
    getInt(fp,&tempProduct.numSpecs);
    for(i=0;i<tempProduct.numSpecs;i++)
    {
        getStr(fp,tempProduct.specNames[i],30);
        getStr(fp,tempProduct.specs[i],50);
    }
    getInt(fp,&tempProduct.stock);
    getStr(fp,tempProduct.ownerName,30);
    getStr(fp,tempProduct.dateCreated,100);
    getStr(fp,tempProduct.dateEdited,100);
}

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

    for(i=0;i<numLoops;i++)
    {
        c=fgetc(fp);
        if(c==-1)
        {
            break;
        }
        loadNextProduct(fp);
        loadedProducts[i/5][i%5]=tempProduct;
    }
    numLoadedProducts=i;

    fflush(fp);
    fclose(fp);
//    int i,j;
//    char input;
//    int selection=0;
//    char fName[81];
//
//    sprintf(fName,"products//%s//page%i.txt",name,pageNum);
//    FILE*fp=fopen(fName,"r");
//    if(fp==NULL)
//    {
//        do
//        {
//            system("cls");
//            cprintf(12,"\n\tThe requested product page could not be found");
//            cprintf(11,"\n\tAttempt to continue anyways?\n\n\t");
//            if(selection==0)
//            {
//                cprintf(10," > No, Exit\n\t");
//                cprintf(15,"   Yes, Continue [Not Recommended]\n\n\t");
//            }
//            else
//            {
//                cprintf(15,"   No, Exit\n\t");
//                cprintf(14," > Yes, Continue [Not Recommended]\n\n\t");
//            }
//            input=getch();
//            switch(input)
//            {
//                case 72:
//                    selection=0;
//                    break;
//                case 80:
//                    selection=1;
//                    break;
//                case 13:
//                    if(selection==0)
//                    {
//                        return 0;
//                    }
//                    else
//                    {
//                        return 1;
//                    }
//                    break;
//            }
//        }
//        while(input!=27&&input!=8&&input!=13);
//        return 0;
//    }
//
//    getInt(fp,&numProducts);
//    for(i=0;i<numProducts;i++)
//    {
//        getStr(fp,&productPage[i].type,30);
//        getStr(fp,&productPage[i].name,50);
//        getStr(fp,&productPage[i].brand,30);
//        getStr(fp,&productPage[i].desc,500);
//        getInt(fp,&productPage[i].numSpecs);
//        for(j=0;j<productPage[i].numSpecs;j++)
//        {
//            getStr(fp,&productPage[i].specNames[j],30);
//            getStr(fp,&productPage[i].specs[j],50);
//        }
//    }
//
//    fflush(fp);
//    fclose(fp);
    return 1;
}

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

//void viewProductPages(void)
//{
//    int i,j;
//    char input;
//    int selection=0;
//
//    do
//    {
//        input=getch();
//    }
//    while(input!=27);
//}

//void printUserProductPage(int highlightNum)
//{
//    int i,j;
//
//    for(i=0;i<numLoadedProducts;i++)
//    {
//        printf("%s\n",loadedProducts[i/5][i%5].name);
//    }
//    getch();
//}

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
                if(curUser.hasAdmin==0)
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
    int numToLoop;
    int curPageNum=0;

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

            printf("\t%c",192);
            for(j=0;j<80;j++)
            {
                printf("%c",196);
            }
            printf("%c\n",217);
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
            case 13:

                break;
        }
    }
    while(input!=27);
}

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
//                if(selection>0)
//                {
//                    selection--;
//                }
                selection=0;
                break;
            case 80:
//                if(selection<2)
//                {
//                    selection++;
//                }
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

void createProduct(void)
{
    //I COULD HAVE USED A STRUCTURE IM AN IDIOT

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
//                                            while((curPriceCents/((int)pow(10,(double)i)))%10!=0)
//                                            {
//                                                printf("%i",((curPriceCents/((int)pow(10,(double)i)))%10)+48);
//                                                getch();
//                                                fputc(((curPriceCents/((int)pow(10,(double)i)))%10)+48,fp);
//                                                i++;
//                                            }
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

void editProducts(void)
{
    int i,j,k;
    char input;
    int curPageNum=0;
    int numToDisplay;
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
                                                                                //printf("lol%i ",tempProduct2.dateCreated[j]);
                                                                                //getch();
                                                                                j=0;
                                                                                fputc(10,temp);
                                                                                while(tempProduct2.dateEdited[j]!=0)
                                                                                {
                                                                                    fputc(tempProduct2.dateEdited[j],temp);
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
                        break;

                    }
                }
                while(input!=27&&input!=13);
                selection=0;
                input=0;


                break;
        }
    }
    while(input!=27);
}

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
    setNumProductsUser(curUser.uName,curUser.numOwnedProducts-1);
    curUser.numOwnedProducts--;
    fflush(temp);
    fclose(temp);
    fflush(fp);
    fclose(fp);
    remove("products//allProducts.txt");
    rename("products//temp.txt","products//allProducts.txt");
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
    dest[i]=0;
}

void getInt(FILE*fp,int*dest)
{
    int temp;
    char c;
    *dest=0;
    c=fgetc(fp);
    //printf("ayy%i",c);
    //getch();
    while(c!=10&&c!=-1)
    {
        //PROBLEM
        //temp=*dest;
        *dest*=10;
        //printf(" %i %i",*dest,c-48);
        *dest+=c-48;
        //dest-=48;
        //printf(" %i",*dest);
        c=fgetc(fp);
        //printf("ayye%i",c);
        //getch();
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
