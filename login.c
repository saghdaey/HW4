
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DELIM1 ","
#define DELIM2 "\n"
#define DELIM3 "&"
#define DELIM4 "="


//function validPair takes in two char arrays & checks if they make a valid username-password pair
//if valid: append username to loggedIn.csv & return 1. else: return 0

int validPair(char username[], char pwd[])
{
	FILE *members;
	members=fopen("../Members.csv", "rt");

	char currLine[1000]; //represents 1 line of members file
	
	while(fgets(currLine, sizeof(currLine), members))
	{
		char *token;
		token=strtok(currLine, DELIM1); 
		token=strtok(NULL, DELIM1); //token points to 2nd element of currLine (the username)

		//if username is valid, check password
		
		if(strcmp(username,token)==0) 
		{  
			token=strtok(NULL,DELIM1); //make token point to 3rd entry of currLine (the password)
			
			//in csv file (and thus in currLine) the last entry of the line has a \n at the end
			//so password is stored like this: password\n
			//we need to do our string comparison for the password without the \n

			char *pwdMembers;
			pwdMembers=strtok(token, DELIM2); 

			//if passwords match, append username to loggedIn.csv and return 1

			if(strcmp(pwd, pwdMembers)==0) 
			{
				FILE *loggedIn;
				loggedIn=fopen("../LoggedIn.csv", "a");
						
				fprintf(loggedIn, "%s", username);
				fprintf(loggedIn, "%s", DELIM1);
				fclose(loggedIn);
				fclose(members);
				
				return 1; 
			}
		}
	}

	fclose(members);
	return 0; //invalid username password combination entered, return 0
}

//int main(int argc, int *argv[])

int main(void)
{
	int length=atoi(getenv("CONTENT_LENGTH")); 
	char *data=malloc(sizeof(char)*(length+1));//creates space to store whole string
	char *usernameIn=malloc(100*sizeof(char)); //creates space for username inputted
	char *pwdIn=malloc(100*sizeof(char)); //creates space for password inputted
	
	char *tokenU; //for username partition
	char *tokenP; //for password partition
	char *token1;
	char *token2;
	int j=0;
	char c=getchar();
	
	//first we read each char into data array & then add a null char to end
	
	while(c!=EOF && j<length)
	{
		data[j]=c;
		c=getchar();
		j++;
	}
	data[j]='\0'; 
	
	//data is now in this form: "username=usernameIn&password=pwdIn\0"
	//tokenize data to separate username from password
		
	tokenU=strtok(data,DELIM3); //tokenU points to this "username=usernameIn"
	tokenP=strtok(NULL,DELIM3); //tokenP points to this "password=pwdIn/0"
	
	token1=strtok(tokenU,DELIM4); //token1 points to "username"
	usernameIn=strtok(NULL,DELIM4); //usernameIn points to usernameIn
	//printf("%s\n",usernameIn);
	
	token2=strtok(tokenP,DELIM4); //token2 points to "password"
	pwdIn=strtok(NULL,DELIM4); //this gets pwdIn\0
	//printf("%s\n",pwdIn);

	//if user does not fill out both fields - generate an error page
	if(usernameIn==NULL || pwdIn==NULL)
	{
		printf("Content-type:text/html\n\n");
		printf("<html>");
		printf("<head>");
		printf("<title>The CatShop</title>");
		printf("<body>");
		printf("<center>");
		printf("<div style=\"background-color:white;font-size:25;color:#CC6600;padding:10px\">");
		printf("<p> Did you fill out both fields?</p>");
		printf("<br>");
		printf("<img src=\"../pusheenNo.gif\" width=\"400\" height=\"400\">");
		printf("<br>");
		printf("<br>");
		printf("<p>Please try again. </p>");
		printf("<br>");
		printf("<a href=\"http://cgi.cs.mcgill.ca/~aalbar/login.html\">Log In Here, Meow!</a>");
		printf("<br>");
		printf("<a href=\"http://cgi.cs.mcgill.ca/~aalbar/index.html\">Go Home, Meow!</a>");		
		printf("</center>");
		printf("</div>");
		return 0;
	}
	
	//now that username and password inputted variables are set, we call our validPair function
	
	int x=validPair(usernameIn,pwdIn);
	
	//should we make these html prints like this instead?
	//printf("%s\n\n","Content-type:text/html\n\n");
	
	if(x==1) //valid username, password - so display catalogue page
	{
		printf("Content-type:text/html\n\n");
		printf("<html>");
		printf("<head>");
		printf("<body>");
		printf("<p>happy shopping, MEOW!</p>"); 

		//display catalogue

		FILE *catPointer;
		catPointer=fopen("../catalog.html", "rt"); //need adams path to catalogue.html
		char catalogueLine[200]; //will store line from catalogue file
		while(fgets(catalogueLine, sizeof(catalogueLine), catPointer))
		{
			printf("%s", catalogueLine);
		}

		fclose(catPointer);
		printf("<input type=\"hidden\" name=\"usernameIn\" value=%s />", usernameIn);//inserts hidden field, assigns usernameIn to field //FIXXX
	}
	
	else //username and password entered are not correct, generate error page
	{
		printf("Content-type:text/html\n\n");
		printf("<html>");
		printf("<head>");
		printf("<title>The CatShop</title>");
		printf("<body>");
		printf("<div style=\"background-color:white;font-size:25;color:#CC6600;padding:10px\">");
		printf("<center>");
		printf("<p>Is the username & password combination you entered correct? </p>");
		printf("<br>");
		printf("<img src=\"../pusheenNo.gif\" width=\"400\" height=\"400\">");
		printf("<br>");
		printf("<br>");
		printf("<p>Please try again.</p>");
		printf("<a href=\"http://cgi.cs.mcgill.ca/~aalbar/login.html\">Login Again, Meow!</a>");
		printf("<br>");
		printf("<a href=\"http://cgi.cs.mcgill.ca/~aalbar/index.html\">Go Home, Meow!</a>");
		printf("</center>");
		printf("</div>");
	}
	
	printf("</body>");
	printf("</head>");
	printf("</html>");
	return 0;
}



