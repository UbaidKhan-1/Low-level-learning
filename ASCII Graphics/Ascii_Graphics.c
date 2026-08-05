# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <time.h>
# include <stdbool.h>
# define ABS(x) (x >= 0? x : -x)
# define MAX(x, y) ((x)>(y)? (x): (y))

int cols = 100;
int rows = 102;
float fps = 10;


typedef struct{
	int x;
	int y;
	int radius;
	int speed;
	double angle;
	char material;
} ball;


void clearBuffer(char* screen);
void drawBuffer(char* screen);
void drawline(char* screen, int x, int y, int x2, int y2, char symbol);
void drawBall(char* screen, ball* Ball);
void moveBall(ball* Ball);
double getTime();
//__________________________________



int main(){
	char screen[rows*cols];
	float deltaTime = 1/fps;
	// removing cursor
	printf("\033[?25l"); 
	fflush(stdout);
	ball Ball = {0, 0, 4, 5, 45, '*'};
	while (1){
		double startTime = getTime();
		// Clearing everything
		system("clear");
	    clearBuffer(screen);
	    //__________________________
	    
	    moveBall(&Ball);
	    drawBall(screen, &Ball);
		drawBuffer(screen);
		
		// enforcing fps
		double endTime = getTime();
		if (endTime-startTime < deltaTime){
			usleep((unsigned int)((deltaTime - (endTime-startTime))*1000000));
		}
	}
	return 0;
}



void clearBuffer(char* screen){
	int len = rows*cols;
	memset(screen, ' ', len);
}


void drawBuffer(char* screen){
	int len = rows*cols;
	for (int i=0; i<len; i++){
		printf("%c", screen[i]);
		if (i%cols == 0) printf("\n");
	}
}


void drawline(char* screen, int x, int y,int x2, int y2, char symbol){
    int dx = x2 - x;
    int dy = y2 - y;
    int steps = MAX(ABS(dx), ABS(dy));
    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;
    float cx = x;
    float cy = y;

    for (int i = 0; i <= steps; i++) {

        int px = round(cx);
        int py = round(cy);

        if (px >= 0 && px < cols && py >= 0 && py < rows){
            screen[py * cols + px] = symbol;
        }

        cx += x_inc;
        cy += y_inc;
    }
}


void drawBall(char* screen, ball* Ball){
	for (int py=0; py<rows; py++){
		for(int px=0; px<cols; px++){
			float dx = (Ball->x-px)/2;
			float dy = Ball->y-py;
			if (round(sqrt(dx*dx + dy*dy)) == Ball->radius){
				if (0<px && px<cols && py>=0 && py<rows){
					screen[py*cols + px] = Ball->material;
				}
			}
		}
	}
}


void moveBall(ball* Ball){
	double unitdx = cos(Ball->angle*M_PI/180);
	double unitdy = sin(Ball->angle*M_PI/180);
	Ball->x += (int)round(unitdx*Ball->speed);
	Ball->y += (int)round(unitdy*Ball->speed);
}


double getTime(){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	double total_seconds = ts.tv_sec + (ts.tv_nsec / 1000000000.0);
	return total_seconds;
}

