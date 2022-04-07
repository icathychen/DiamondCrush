//ECE243 Final Project
//Eliza Koshceeva & Cathy Chan

/* This files provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>


/* VGA colors */
//rbg to 16 bit -> http://greekgeeks.net/#maker-tools_convertColor
#define BLUE 0xDF1B
#define WHITE 0xFFDF
#define BACKGROUND 0xE4AF

//CHANGE 
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 20 
#define NUM_BOXES 10 

/* Constants for animation */
#define SIZE_BOARD 10
#define WIDTH 20
#define SIZE_DIAMOND 12


#define SHIFT_X 60
#define SHIFT_Y 20


int position[] = {0,0};
int drawn = 0; 

void plot_pixel(int x, int y, short int line_color);   // plots one pixel
void draw_line_2(int x0, int x1, int y0, int y1, short int colour);
void swap(int *a, int *b);
void clear_screen();				       // clears whole screen
void wait_for_vsync();
void init_screen(); 
void set_A9_IRQ_stack(void);
void config_GIC(void);
void config_KEYs (void);
void config_interrupt (int N, int CPU_target); 
void enable_A9_interrupts (void);
void pushbutton_ISR( void );

//Functions for graphics
void clear_screen();
void clear_screen_char();
void wait_for_vsync();
void plot_pixel(int x, int y, short int line_color);
//void draw_line(int x0, int x1, int y0, int y1, short int colour);
void draw_line(int x0, int y0, int x1, int y1, short int color);
void swap(int *a, int *b);
void move_line();
void clearBlock(int row, int col, int colour);

//Shape
void drawSqaure(int row, int col, short int colour);
void drawVerticalSqaure(int row, int col, short int colour);
void drawTraingle(int row, int col, short int colour);
int convertRow(int row);
int convertCol(int col);


//Function for the game 
bool checkIfValid(int row, int col);
int clearDiamond(int row1, int row2, int col1, int col2);
void printBoard();
void swapValue(int row1, int row2, int col1, int col2);
void initializeBoard();
bool checkDiamondAndClear();

//Rendering
void diamondSelection(int row, int col, int num, int color);
void drawAllDiamonds();
void automaticPlay();

//Move Rendering
void move_triangle(int row1,int row2, int col, int colour);
void move_VerticalSqaure(int row1,int row2, int col, int colour);
void move_sqaure(int row1,int row2, int col, int colour);
void diamondMove(int row1,int row2, int col, int num);
void diamondBlockMove(int row1,int row2, int col1, int col2, int numOfClear);
void move_triangle_pixel(int row, int row1,int row2, int col, int colour);
void move_sqaure_pixel(int row, int row1,int row2, int col, int colour);
void move_VerticalSqaure_pixel(int row, int row1,int row2, int col, int colour);

volatile int pixel_buffer_start; // global variable
volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
volatile int * HEX3_ptr;
volatile int * LEDR_BASE_PTR;
int diamonds[SIZE_BOARD][SIZE_BOARD];
int num;
int score = 0;
char seg7[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
0x7f, 0x67, 0x77};


int main(void) {
    set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
    config_GIC (); // configure the general interrupt controller
    config_KEYs ();  // configure KEYs to generate interrupts
    enable_A9_interrupts (); // enable interrupts in the A9 processor
    
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    
    srand((unsigned) time(NULL));
    
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    init_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = SDRAM_BASE;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    init_screen(); // pixel_buffer_start points to the pixel buffer

    int currentPos[] = {0,0}; 
    while (1)   {
        if ((currentPos[0] != position[0] || currentPos[1] != position[1]) && drawn!=2) {
            /* Erase any previous changes */
            /*
            for (int i=20*currentPos[0] +61; i< 20*currentPos[0] +80; i++) {
                for (int j=20*currentPos[1]+21; j<20*currentPos[1]+40; j++) {
                    plot_pixel(i, j, BACKGROUND); 
                }
            }
	
            //Draw new changes 
            int i, j; 
            for (i=20*position[0] +61; i< 20*position[0] +80; i++) {
                for (j=20*position[1]+21; j<20*position[1]+40; j++) {
                    plot_pixel(i, j, WHITE); 
                }
            } */
            
            drawn +=1; 
            if (drawn==2) {
                currentPos[0] = position[0]; 
                currentPos[1] = position[1]; 
            }
			drawAllDiamonds();
            diamondSelection(position[1],position[0], diamonds[position[0]][position[1]], 2);
            wait_for_vsync(); // swap front and back buffers on VGA vertical sync
            pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
        }
    }
}


void wait_for_vsync() {
    volatile int * s_register = PIXEL_BUF_CTRL_BASE; //pointer to DMA controller
    int status;
    *s_register =1;

    status = *(s_register + 3);
    while((status & 0x01) != 0){
        status = *(s_register + 3);
    }
}

void clear_screen() {
    int x_coord, y_coord; 
	for (x_coord=0; x_coord < RESOLUTION_X; x_coord++) {
        for (y_coord=0; y_coord < RESOLUTION_Y; y_coord++) {
            plot_pixel(x_coord, y_coord, 0); 
        }
    }
}

void init_screen() {
    //set the backround
    int x_coord, y_coord; 
    for (x_coord=0; x_coord < RESOLUTION_X; x_coord++) {
        for (y_coord=0; y_coord < RESOLUTION_Y; y_coord++) {
            plot_pixel(x_coord, y_coord, BLUE); 
        }
    }
    //draw the grid backround 
    for (x_coord=60; x_coord < RESOLUTION_X-60; x_coord++) {
        for (y_coord=20; y_coord < RESOLUTION_Y-20; y_coord++) {
            plot_pixel(x_coord, y_coord, BACKGROUND); 
        }
    }
    //draw the grid lines 
    draw_line(59, 19, 261, 19, WHITE); 
    draw_line(59, 221, 261, 221, WHITE); 
    draw_line(59, 19, 59, 221, WHITE); 
    draw_line(261, 19, 261, 221, WHITE); 

    //horizontal lines -
    y_coord = 20; 
    int line; 
    for (line=0; line<11; line++) {
        draw_line(60, y_coord, 259, y_coord, WHITE); 
        //draw_line(60, y_coord+1, 259, y_coord+1, WHITE); 
        y_coord += 20; 
    }
    //vertical lines |
    x_coord = 60; 
    for (line=0; line<11; line++) {
        draw_line(x_coord, 20, x_coord, 219, WHITE); 
        //draw_line(x_coord+1, 20, x_coord+1, 219, WHITE); 
        x_coord += 20; 
    }
    
    //draw the symbols
    initializeBoard(); 
    drawAllDiamonds(); 
	bool checking = true;
	while(checking == true){
		checking = checkDiamondAndClear();
		int count = score;
		int count_2 = 0;
		while(count >= 10){
			count -= 10;
			count_2++;
		} 
	}
}

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_line(int x0, int y0, int x1, int y1, short int color) {
    bool is_steep = abs(y1-y0) > abs(x1-x0); 
    if (is_steep) {
        swap(&x0, &y0); 
        swap(&x1, &y1); }
    if (x0>x1) {
        swap(&x0, &x1); 
        swap(&y0, &y1); }
    
    int deltax = x1 - x0; 
    int deltay = abs(y1-y0); 
    int error = -(deltax/2); 
    int y = y0; 
    int y_step;
    if (y0<y1) y_step = 1; 
    else y_step = -1; 
    
    for (int x=x0; x<=x1; x++) {
        if (is_steep) plot_pixel(y,x, color); 
        else plot_pixel(x,y, color); 
        
        error = error + deltay; 
        if (error>0) {
            y = y + y_step; 
            error = error - deltax; }
    }
}

void set_A9_IRQ_stack(void) {
    int status = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
    
    int stack, mode;
    stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
    /* change processor to IRQ mode with interrupts disabled */
    mode = 0b11010010;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
    /* set banked stack pointer */
    asm("mov sp, %[ps]" : : [ps] "r" (stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void) {
    config_interrupt (73, 1); // configure the FPGA KEYs interrupt (73)
    // Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities
    *((int *) 0xFFFEC104) = 0xFFFF;
    // Set the enable in the CPU Interface Control Register (ICCICR)
    *((int *) 0xFFFEC100) = 1;
    // Set the enable in the Distributor Control Register (ICDDCR)
    *((int *) 0xFFFED000) = 1;
}

/*
* Configure registers in the GIC for an individual Interrupt ID. We
* configure only the Interrupt Set Enable Registers (ICDISERn) and
* Interrupt Processor Target Registers (ICDIPTRn). The default (reset)
* values are used for other registers in the GIC
*/
void config_interrupt (int N, int CPU_target) {
    int reg_offset, index, value, address;
    /* Configure the Interrupt Set-Enable Registers (ICDISERn).
    * reg_offset = (integer_div(N / 32) * 4; value = 1 << (N mod 32) */
    reg_offset = (N >> 3) & 0xFFFFFFFC;
    index = N & 0x1F;
    value = 0x1 << index;
    address = 0xFFFED100 + reg_offset;
    /* Using the address and value, set the appropriate bit */
    *(int *)address |= value;
    /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
    * reg_offset = integer_div(N / 4) * 4; index = N mod 4 */
    reg_offset = (N & 0xFFFFFFFC);
    index = N & 0x3;
    address = 0xFFFED800 + reg_offset + index;
    /* Using the address and value, write to (only) the appropriate byte */
    *(char *)address = (char) CPU_target;
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs() {
    volatile int * KEY_ptr = (int *) 0xFF200050; // KEY base address
    *(KEY_ptr + 2) = 0xF; // enable interrupts for all four KEYs
}

/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void) {
    int status = 0b01010011;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq (void){
    // Read the ICCIAR from the CPU Interface in the GIC
    int interrupt_ID = *((int *) 0xFFFEC10C);
    if (interrupt_ID == 73) // check if interrupt is from the KEYs
    pushbutton_ISR ();
    else
    while (1); // if unexpected, then stay here
    // Write to the End of Interrupt Register (ICCEOIR)
    *((int *) 0xFFFEC110) = interrupt_ID;
}

/********************************************************************
* Pushbutton - Interrupt Service Routine
*
* This routine checks which KEY has been pressed. It writes to HEX0
*******************************************************************/
void pushbutton_ISR( void ) {
    /* KEY base address */
    volatile int *KEY_ptr = (int *) 0xFF200050;
 
    int press;
    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt
    
    if (press & 0x1) // KEY0 - right 
        position[0] += 1; 
    else if (press & 0x2) // KEY1
        position[0] -= 1;
    else if (press & 0x4) // KEY2
        position[1] += 1;
    else // press & 0x8, which is KEY3
        position[1] -= 1;
    drawn = 0; 
    return;
}


void drawSqaure(int row, int col,short int colour){
	int size = SIZE_DIAMOND, j;
	draw_line_2(col-size/2,col+size/2, row-size/2, row-size/2, colour);//Top
	draw_line_2(col-size/2,col-size/2, row-size/2, row+size/2, colour);//Left
	draw_line_2(col+size/2,col+size/2, row-size/2, row+size/2, colour);//Right
	draw_line_2(col-size/2,col+size/2, row+size/2, row+size/2, colour);//Bottom
	//Fill the colour
	for(j = row-size/2; j < row+size/2; j++){
		draw_line_2(col-size/2,col+size/2, j, j, colour);
	}
}



void drawVerticalSqaure(int row, int col,short int colour){
	int height = SIZE_DIAMOND ;
	draw_line_2(col - height / 2,col, row, row-height/2, colour);//TOP LEFT
	draw_line_2(col,col + height / 2, row-height/2, row, colour);//TOP RIGHT
	draw_line_2(col - height / 2,col, row, row+height/2, colour);//BOTTOM LEFT
	draw_line_2(col,col + height / 2, row+height/2, row, colour);//BOTTOM RIGHT
	//Fill the colour
	int a = col,b = col,c;

	//Fill upper
	for(c = row-height/2; c < row; c++){
		if(a > col-height/2){
			draw_line_2(a,b,c,c,colour);
			a --; 
			b ++;
		}else{
			break;
		}
	}
	//Fill lower
	for(c = row; c < row+height/2; c++){
		if(a < col){
			draw_line_2(a,b,c,c,colour);
			a ++; 
			b --;
		}else{
			break;
		}
	}
}


void drawTraingle(int row, int col, short int colour){
	int height = SIZE_DIAMOND ;
	draw_line_2(col-height/2,col, row+height/2, row-height/2, colour);//LEFT
	draw_line_2(col,col+height/2, row-height/2,row+height/2, colour);//RIGHT
	draw_line_2(col-height/2,col+height/2, row+height/2, row+height/2, colour);//BOTTOM

	int i = col-height/2;
	int j = col+height/2+1;
	int k = row + height/2;
	//Fill in the colour
	while(i != j && k != row-height/2){
		draw_line_2(i,j, k, k, colour);
		i++;
		j--;
		k -= 2;
	}
	i = col-height/2;
	j = col+height/2;
	k = row + height/2 - 1; 
	while(i != j && k != row-height/2 - 1){
		draw_line_2(i,j, k, k, colour);
		i++;
		j--;
		k -= 2;
	}
}


void diamondMove(int row1,int row2, int col, int num){
	row1 = convertRow(row1);
	row2 = convertRow(row2);
	col = convertCol(col);
	if(num == 0){
		move_sqaure(row1,row2,col,RED);
	}else if(num == 1){
		move_sqaure(row1,row2,col,BLUE);
	}else if(num == 2){
		move_VerticalSqaure(row1,row2,col, YELLOW);
	}else if(num == 3){
		move_VerticalSqaure(row1,row2,col, CYAN);
	}else if(num == 4){
		move_triangle(row1,row2,col,GREEN);
	}else if(num == 5){
		move_triangle(row1,row2,col,ORANGE);
	}
}


void diamondBlockMove(int row1,int row2, int col1, int col2, int numOfClear){
	int i = 0;
	int col = 0;
	int totalTravel;
	int count;
	int j,k;
	printf("c\n");

	if(row1 == row2){
		totalTravel = numOfClear * WIDTH;
		count = 0;
		while(count <= totalTravel){
			printf("k\n");
			for(i = 0; i <= row2-numOfClear; i++){
				printf("a\n");
				for(col = col1; col<=col2; col++){
					num = diamonds[i][col];
					j = convertRow(i);
					k = convertCol(col);
					if(num == 0){
						move_sqaure_pixel(j,j+count,row2,k,RED);
					}else if(num == 1){
						move_sqaure_pixel(j,j+count,row2,k,BLUE);
					}else if(num == 2){
						move_VerticalSqaure_pixel(j,j+count,row2,k, YELLOW);
					}else if(num == 3){
						move_VerticalSqaure_pixel(j,j+count,row2,k, CYAN);
					}else if(num == 4){
						move_triangle_pixel(j,j+count,row2,k,GREEN);
					}else if(num == 5){
						move_triangle_pixel(j,j+count,row2,k,ORANGE);
					}
				}
			}
			//clearBlock(j+count-2,k,BACKGROUND);
			wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    		pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
			count ++;
		}
		//clearBlock(j+count-3,k-1,BACKGROUND);
		
	}


	if(col1 == col2){
		totalTravel = numOfClear * WIDTH;
		count = 0;
		
		while(count <= totalTravel){
			printf("kk\n");
			for(i = 0; i <= row2-numOfClear; i++){
				printf("nn\n");
				for(col = (col1); col<=(col2); col++){
					num = diamonds[i][col];
					j = convertRow(i);
					k = convertCol(col);
					if(num == 0){
						move_sqaure_pixel(j,j+count,row2,k,RED);
					}else if(num == 1){
						move_sqaure_pixel(j,j+count,row2,k,BLUE);
					}else if(num == 2){
						move_VerticalSqaure_pixel(j,j+count,row2,k, YELLOW);
					}else if(num == 3){
						move_VerticalSqaure_pixel(j,j+count,row2,k, CYAN);
					}else if(num == 4){
						move_triangle_pixel(j,j+count,row2,k,GREEN);
					}else if(num == 5){
						move_triangle_pixel(j,j+count,row2,k,ORANGE);
					}
				}
			}
			//clearBlock(j+count-2,k,BACKGROUND);
			wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    		pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
			printf("%d",count);
			count ++;
		}
		//clearBlock(j+count-3,k-1,BACKGROUND);
	}


}

void move_triangle_pixel(int row, int row1,int row2, int col, int colour){
	//clearBlock(row2,col,BACKGROUND);
	
	if(row1-2>=row){
	clearBlock(row1-2,col,BACKGROUND);
	}
	drawTraingle(row1,col,colour);
}

void move_sqaure_pixel(int row, int row1,int row2, int col, int colour){
	//clearBlock(row2,col,BACKGROUND);

	
	if(row1-2>=row){
	clearBlock(row1-2,col,BACKGROUND);
	}
	drawSqaure(row1,col,colour);
}

void move_VerticalSqaure_pixel(int row, int row1,int row2, int col, int colour){
	//clearBlock(row2,col,BACKGROUND);
	if(row1-2>=row){
	clearBlock(row1-2,col,BACKGROUND);
	}
	drawVerticalSqaure(row1,col,colour);
}


void move_triangle(int row1,int row2, int col, int colour){
	clearBlock(row2,col,BACKGROUND);
	wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	clearBlock(row2,col,BACKGROUND);
	int i = 0;
	for(i = row1; i <= row2; i+=1){
		if(i-2 >= row1){
			clearBlock(i-2,col,BACKGROUND);
		}
		drawTraingle(i,col,colour);
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	}
}

void move_sqaure(int row1,int row2, int col, int colour){
	clearBlock(row2,col,BACKGROUND);
	wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	clearBlock(row2,col,BACKGROUND);
	int i = 0;
	for(i = row1; i <= row2; i += 1){
		if(i-2 >= row1){
			clearBlock(i-2,col,BACKGROUND);
		}
		drawSqaure(i,col,colour);
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	}
}

void move_VerticalSqaure(int row1,int row2, int col, int colour){
	clearBlock(row2,col,BACKGROUND);
	wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	clearBlock(row2,col,BACKGROUND);
	int i = 0;
	for(i = row1; i <= row2; i+=1){
		if(i-2 >= row1){
			clearBlock(i-2,col,BACKGROUND);
		}
		drawVerticalSqaure(i,col,colour);
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	}
}

void clearBlock(int row, int col,int colour){
	int y, x;
        for (y = row-WIDTH/2;y <= row + WIDTH/2-1; y++)
                for (x = col-WIDTH/2; x <= col + WIDTH/2-1; x++)
                        plot_pixel (x,y,colour);
} 



void initializeBoard(){
    int i = 0, j = 0;
    for( i = 0; i < SIZE_BOARD; i++){
		for(j = 0; j < SIZE_BOARD; j++){
			num = rand() % 6;
			diamonds[i][j] = num;
		}
	}
}

void swapValue(int row1, int row2, int col1, int col2){
    printf("Swap value at row %d, col %d and row %d, col %d\n", row1,col1,row2,col2);
    int value1 = diamonds[row1][col1];
    int value2 = diamonds[row2][col2];
    printf("OK\n");
    diamonds[row1][col1] = value2;
    diamonds[row2][col2] = value1;
}

bool checkIfValid(int row, int col){
	int diamonds_colour = diamonds[row][col];
	//int currentColour = diamonds_colour;
	int count = 0;
	int i = 0;
	int first = 0;
	int row1 = 0,row2 = 0, col1 = 0, col2 = 0;
	//printf("3\n");
	//Check the neighbouring col colour (Horizontal)
	for(i = 0 ; i < SIZE_BOARD; i++){
		if(diamonds[row][i] == diamonds_colour){
			if(first == 0){
				col1 = i;
				first = 1;
			}
			count ++;
		}else{
            //Check if there are 3 or more 
            if(count >= 3){
                col2 = i - 1;
                break;
            }
			count = 0;
            first = 0;
		}
	}
    //printf("4\n");
    //Check if there are 3 or more
	if(count >= 3){
        col2 = i - 1;
		row1 = row;
		row2 = row;
        score += clearDiamond(row1, row2, col1, col2);
		return true;
	}
    //printf("5\n");
    //Check the neighbouring col colour (vertical)
	first = 0;
    count = 0;
	for(i = 0 ; i < SIZE_BOARD; i++){
		if(diamonds[i][col] == diamonds_colour){
			if(first == 0){
				row1 = i;
				first = 1;
			}
			count ++;
		}else{
            if(count >= 3){
                row2 = i - 1;
                break;
            }
			count = 0;
            first = 0;
		}
	}
//printf("6\n");
	if(count >= 3){
		col1 = col;
		col2 = col;
        row2 = i - 1;
		score += clearDiamond(row1, row2, col1, col2);
		return true;
	}

	return false;
}


int clearDiamond(int row1, int row2, int col1, int col2){
	int i = 0;
	int numOfClear = 0;
	int r1 = row1, r2 = row2,c1 = col1, c2 = col2;

    //Clear vertical
	if(col1 == col2){
		numOfClear = row2 - row1 + 1;
		while(row2 >= 0){
			if(row2 - numOfClear >= 0){
				diamonds[row2][col1] = diamonds[row2 - numOfClear][col1];
				//diamondMove(row2 - numOfClear,row2, col1,diamonds[row2 - numOfClear][col1]);
				//diamondBlockMove(row1,row2, col1,col2,numOfClear);
				//diamondSelection(row2, col1, diamonds[row2][col1]);
			}else{
				diamonds[row2][col1] = rand() % 6;
				//diamondMove(0,row2,col1,diamonds[row2][col1]);
				//diamondBlockMove(row1,row2, col1,col2,numOfClear);
				//diamondSelection(row2, col1, diamonds[row2][col1]);
			}
			row2 --;
		}
		//diamondBlockMove(r1,r2, c1,c2,numOfClear);
	}

	//Clear horizontal
	if(row1 == row2){
		numOfClear = col2 - col1 + 1;
		while(row1 >= 0){
			for(i = col1; i <= col2;i++){
				if(row1-1 >= 0){
					diamonds[row1][i] = diamonds[row1-1][i];
					//diamondMove(row1-1,row1,i,diamonds[row1-1][i]);
					//diamondSelection(row1, i, diamonds[row1][i]);
				}else{	
                    diamonds[row1][i] = rand() % 6;
                    //diamondMove(0,row1,i,diamonds[row1][i]);
                    //diamondSelection(row1, i, diamonds[row1][i]);
                }
			}
			row1 --;
		}
		//diamondBlockMove(r1,r2, c1,c2,numOfClear);
	}
    return numOfClear;
}


bool checkDiamondAndClear(){
	int i = 0, j = 0;
	bool win = false;

	for( i = 0; i < SIZE_BOARD; i++){
		for(j = 0; j < SIZE_BOARD; j++){
            win = checkIfValid(i, j);
            if(win == true){
				int count = score;
				int count_2 = 0;
				 while(count >= 10){
					count -= 10;
					 count_2++;
				 }
				 *HEX3_ptr = seg7[count_2]<< 8 | seg7[count];
				*LEDR_BASE_PTR = score;
                return true;
            }
            win = false;
		}
	}
    return false;
}

void drawAllDiamonds(){
	int i = 0, j = 0;
	for( i = 0; i < SIZE_BOARD; i++){
		for(j = 0; j < SIZE_BOARD; j++){
            diamondSelection(i,j, diamonds[i][j], 1);
		}
	}
}

int convertRow(int row){
	row = SHIFT_Y+ WIDTH/2 + row * WIDTH;
	return row;
}

int convertCol(int col){
	col = SHIFT_X + WIDTH/2 + col * WIDTH;
	return col;
}

void diamondSelection(int row, int col, int num, int color){
	row = convertRow(row);
	col = convertCol(col);
    if (color==2) clearBlock(row, col, WHITE);
    else clearBlock(row,col,BACKGROUND); 
	if(num == 0){
		drawSqaure(row,col,RED);
	}else if(num == 1){
		drawSqaure(row,col,BLUE);
	}else if(num == 2){;
		drawVerticalSqaure(row, col, );
	}else if(num == 3){
		drawVerticalSqaure(row,col,);
	}else if(num == 4){
		drawTraingle(row,col, );
	}else if(num == 5){
		drawTraingle(row,col, );
	}
}

void draw_line_2(int x0, int x1, int y0, int y1, short int colour)
{
    bool is_steep = (abs(y1-y0) > abs(x1-x0));
    
    if(is_steep == true){
        swap(&x0,&y0);
        swap(&x1,&y1);
    }
    if(x0 > x1){
        swap(&x0,&x1);
        swap(&y0,&y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = - (deltax / 2);
    int y = y0;
    int y_step;

    if(y0 < y1){
        y_step = 1;
    }else{
        y_step = -1;
    }

    for(int x = x0; x <= x1; ++x){
        if(is_steep == true){
            plot_pixel(y,x, colour);
        }else{
            plot_pixel(x,y, colour);
        }
        error = error + deltay;
        if(error >= 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
    
}







