#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>   
#include <termios.h>



#define TRUE 		1
#define FALSE 		0
#define X 		38
#define Y 		15
#define CLS_CMD 	"clear" 	//replace with cls if you're running on windows



typedef struct cord {
    int x;
    int y;
} cord;


void go_up(cord *pos);
void go_left(cord *pos);
void go_down(cord *pos);
void go_right(cord *pos);
void reprint(cord pos, int map[][X]);
void delay(int number_of_seconds);
void import_map(int map[][X]);
int check_status(cord pos, int map[][X]); //returns TRUE if you're still in game, returns FALSE if you lose, 2 if you won;
void print_menu();
void lose_condition();
char getch(); //unbuffered stdin


int main() {
    int status = 2;
    char cmd;
    cord pos;
    int map[Y][X];

    // don't interrupt for user input
    import_map(map);
    system(CLS_CMD);
    print_menu();	
    while(TRUE){
	cmd = getch();
	if(status == 1){
	switch (cmd){
		case 'a':
			go_left(&pos);
			if(check_status(pos,map)==1) reprint(pos, map);
			else if(check_status(pos,map)==2) {
					system(CLS_CMD);
					printf("WINNER\n");
					delay(1000);
					status = 2;
					system(CLS_CMD);
        			print_menu();
			}
            else status =0;
			break;
		case 's':
			go_down(&pos);
			if(check_status(pos,map)==1) reprint(pos, map);
			else if(check_status(pos,map)==2) {
					system(CLS_CMD);
					printf("WINNER\n");
					delay(1000);
					status = 2;
					system(CLS_CMD);
        			print_menu();
			}
            else status =0;
			break;

		case 'd':
			go_right(&pos);
			if(check_status(pos,map)==1) reprint(pos, map);
		    else if(check_status(pos,map)==2) {
					system(CLS_CMD);
					printf("WINNER\n");
					delay(1000);
					status = 2;
					system(CLS_CMD);
        			print_menu();
			}
            else status =0;
			break;
		case 'w':
			go_up(&pos);
			if(check_status(pos,map)==1) reprint(pos, map);		
            else if(check_status(pos,map)==2) {
					system(CLS_CMD);
					printf("WINNER\n");
					delay(1000);
					status = 2;
					system(CLS_CMD);
        			print_menu();
			}
            else status =0;
			break;
		case 'm':
			status = 2;
			system(CLS_CMD);
                	print_menu();
			break;
		}
		continue;
	}	
	
	if(status == 0){
		lose_condition();
		fflush(stdout);
		delay(1000);
		status = 2;
		system(CLS_CMD);
        	print_menu();
		continue;
	}
	else if(status == 2){
		switch(cmd){
			case '1':
				pos.y = 1;
				pos.x = 0;
				reprint(pos,map);
				status = 1;
				break;
			case '2':
				system(CLS_CMD);
				printf("Calling HElp fun");
				break;
			case '3':
				return 0;
  		}	
	}	

}

}
void go_down(cord *pos){
	pos->y++;
}

void go_left(cord *pos){
	pos->x--;
}

void go_right(cord *pos){
	pos->x++;
}

void go_up(cord *pos){
	pos->y--;
}


void reprint(cord pos, int map[][X]) {
    int i, j, k =0;
	char end[4] = {'e','n','d'};

    system(CLS_CMD);
    for (i = 0; i < Y; i++) {
        for (j = 0; j < X; j++) {
            if (i == pos.y && j == pos.x) printf("X");
            else if(map[i][j] == 1) {
                printf("#");
            }
			else if(map[i][j] == 2){
				printf("%c", end[k]);
				k++;
				if(k==3) k = 0; 

			}
            else printf(" ");

        }
        printf("\n");
    }
printf("Use A S W D to move the cursor - press m to open menu");
}

void delay(int millis){
    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + millis);
}

void import_map(int map[][X]){
    int i,max_x,max_y,j,k = 0;
    FILE *in;
    char c;

    in = fopen("map.labi","r");
    fscanf(in,"%d %d", &max_x, &max_y);
    for(i = 0; i<max_y; i++ ){
        for(j = 0; j<max_x; j++){
            c = (char)getc(in);
            if(c == 'x') map[i][j] =1;
			else if(c == '*') map[i][j] = 2;
            else map[i][j] = 0;
        }
    }
}

int check_status(cord pos, int map[][X]){
	
	if(map[pos.y][pos.x]==2) return 2;
	else if(map[pos.y][pos.x]==1 || pos.y > Y || pos.x > X || pos.y <= 0  || pos.x <=0) return FALSE;
	
	return TRUE;
}


void print_menu(){
	printf("Welcome to LabyrinthC 0.1\n"
		"Choose what would you like to do\n\n\n"
		"1) Play\n"
		"2) Help\n"
	 	"3) Quit\n");
	
}

void lose_condition(){
	system(CLS_CMD);
	int i =0, j =0;
	
	while(i<Y/2){
		while (j<X/2){	
			printf(" ");
			j++;
			}
		printf("\n");
		i++;}
	printf("YOU'RE A LOSER\n\n\n\n");	


}

char getch(){    \\Unix systems dont have an unbuffered getc
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }
