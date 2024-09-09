#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>
int screen_x, screen_y;
int char_x, char_y;
void gen_line (int *, int *, int *, int *, unsigned *);
void gen_barrel ();
void gen_block ();
//void gen_barrel_bottom (int *, int *, int *, int *, unsigned *);
//void gen_barrel_left (int *, int *, int *, int *, unsigned *);
//void gen_barrel_right (int *, int *, int *, int *, unsigned *);
//void print_text (int, int, int, int, unsigned);
volatile sig_atomic_t stop;
void catchSIGINT(int signum){
stop = 1;
}
/* This program draws randomly-generated lines on the VGA display. The code uses
* the character device drivers IntelFPGAUP/video and IntelFPGAUP/KEY. It draws a
* random line each time a pushbutton KEY is pressed. The coordinates of each new
* line are displayed on the bottom right of the screen. Exit by typing ^C. */
int main(int argc, char *argv[]) {
int KEY_data;
int x1, y1, x2, y2;
unsigned color;
//char msg_buffer[80];
time_t t;
// used to seed the rand() function
// catch SIGINT from ^C, instead of having it abruptly close this program
signal(SIGINT, catchSIGINT);
srand ((unsigned) time(&t));
// seed the rand function
// Open the character device drivers
if (!KEY_open ( ) || !video_open ( ))
return -1;
video_read (&screen_x, &screen_y, &char_x, &char_y);
video_erase ( ); // erase any text on the screen
// get screen & text size
// set random initial line coordinates
//gen_line (&x1, &y1, &x2, &y2, &color);


/* There are two VGA buffers. The one we are drawing on is called the Back
* buffer, and the one being displayed is called the Front buffer. The function
* video_show swaps the two buffers, allowing us to display what has been drawn
* on the Back buffer */

video_clear ( );
// clear current VGA Back buffer
video_show ( );
// swap Front/Back to display the cleared buffer
video_clear ( );
// clear the VGA Back buffer, where we will draw lines
//video_line (x1, y1, x2, y2, color);
//gen_barrel_bottom (&x1, &y1, &x2, &y2, &color);
gen_barrel();

//video_show ( );


gen_block();
//video_box (x1, y1, x2, y2, color);
video_show ( );
// swap Front/Back to display the line
while (!stop) {
//sprintf (msg_buffer, "(%03d,%03d) (%03d,%03d) color:%04X", x1, y1, x2, y2,
//color);
//video_text (char_x - strlen(msg_buffer), char_y - 1, msg_buffer);
printf ("Press a pushbutton KEY (^C to exit)\n");
KEY_read (&KEY_data);
while (!KEY_data && !stop)
KEY_read (&KEY_data);
gen_line (&x1, &y1, &x2, &y2, &color);
video_show ( ); // swap Front/Back
video_line (x1, y1, x2, y2, color);
video_show ( ); // swap Front/Back to display lines
}
video_close ( );
KEY_close ( );
printf ("\nExiting sample program\n");
return 0;
}
/* Generate a new random line */
void gen_line (int *x1, int *y1, int *x2, int *y2, unsigned *color) {
unsigned int video_color[] = {video_WHITE, video_YELLOW, video_RED,
video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
video_PINK, video_ORANGE};
// random x position
*x1 = rand()%(screen_x - 1);
// random y position
*y1 = rand()%(screen_y - 1);
// random x position
*x2 = rand()%(screen_x - 1);
// random y position
*y2 = rand()%(screen_y - 1);
*color = video_color[(rand()%10)]; // random out of 10 video colors
}

void gen_barrel () {
    unsigned int barrel_color[] = {video_WHITE};
    int barrel_width = 200;  // Ajuste a largura do barril
    int barrel_height = 370; // Ajuste a altura do barril
    int center_x = screen_x / 2;  // Centraliza na largura da tela
    int base_y = screen_y - 10;   // Define a base do barril na parte inferior da tela
    
    unsigned color = barrel_color[0]; // Define a cor do barril como branco
    
        // Parte inferior do barril
    int x1 = center_x - (barrel_width / 2);
    int y1 = base_y - 10;
    int x2 = center_x + (barrel_width / 2);
    int y2 = base_y;
    
    video_box(x1, y1, x2, y2, color);

        // Parte esquerda do barril
    x1 = center_x - (barrel_width / 2);
    y1 = base_y - (barrel_height / 2);
    x2 = center_x - (barrel_width / 2) + 10;
    y2 = base_y - 10;
    
    video_box(x1, y1, x2, y2, color);
   
        // Parte direita do barril
    x1 = center_x + (barrel_width / 2) - 10;
    y1 = base_y - (barrel_height / 2);
    x2 = center_x + (barrel_width / 2);
    y2 = base_y - 10;
    
    video_box(x1, y1, x2, y2, color);
}

void gen_block () {
    unsigned int block_color[] = {video_BLUE};
    unsigned int border_color = video_GREY;  // Cor da borda
    int block_width = 17;  // Largura do bloco
    int block_height = 17; // Altura do bloco
    int border_thickness = 1; // Espessura da borda
    int center_x = screen_x / 2;  // Centraliza na largura da tela
    int base_y = screen_y - 22;   // Define a base do barril na parte inferior da tela
    
    // Coordenadas da borda (um pouco maior que o bloco)
    int x1 = center_x - (block_width / 2) - border_thickness;
    int y1 = base_y - block_height - border_thickness;
    int x2 = center_x + (block_width / 2) + border_thickness;
    int y2 = base_y + border_thickness;

    // Desenhar a borda cinza
    unsigned color = border_color;
    video_box(x1, y1, x2, y2, color);
    
    // Coordenadas do bloco azul
    x1 = center_x - (block_width / 2);
    y1 = base_y - block_height;
    x2 = center_x + (block_width / 2);
    y2 = base_y;

    // Desenhar o bloco azul
    color = block_color[0];
    video_box(x1, y1, x2, y2, color);
}




