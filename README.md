# School Gr11 - Amazon Summative (C)

A summative assignment for Gr11 CS which entails creating an amazon-like technology shopping software.

## How to Run

Download the file and / or run the code on your IDE of choice (Codeblocks or DevC++ should both work). The program has only some safety features for deleted files so it is recommended that the downloaded directory is kept as is.

## How to Use

The program's controls should be fairly intuitive as per the criteria of the assignment. The program has many functions including:  
    - Providing a way for users to create accounts with unique usernames  
    - giving additional capabilities to users which acquire administrator status  
        - editing / deleting others' items  
        - changing others' passwords / admin statuses  
    - allowing users to create products which are put on a "market place"  
    - granting owners of said items the ability to edit or delete them at will  
    - allowing other users to permanently buy items in an "add to cart" style  

Information about created accounts is kept in an enclosed folder and passwords are encrypted as per the following cipher:  
           0 1 2 3 4 5 6 7  
        0  a i q y G O W 4  
        8  b j r z H P X 5  
        16 c k s A I Q Y 6  
        24 d l t B J R Z 7  
        32 e m u D K S 0 8  
        40 f n v E L T 1 9  
        48 g o w E M U 2  
        56 h p x F N V 3  
  
The indicated numbers are added up and converted to hexadecimal to form one character.
  
Extensive documentation on the inner workings of the program can be found within its code.
