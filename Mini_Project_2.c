/* set a single pixel on the screen at x,y
 * x in [0,319], y in [0,239], and colour in [0,65535]
 */
#define JTAG_UART_BASE 0xFF201000
/* This files provides address values that exist in the system */

#define BOARD "DE1-SoC"

/* Memory */
#define DDR_BASE 0x00000000
#define DDR_END 0x3FFFFFFF
#define A9_ONCHIP_BASE 0xFFFF0000
#define A9_ONCHIP_END 0xFFFFFFFF
#define SDRAM_BASE 0xC0000000
#define SDRAM_END 0xC3FFFFFF
#define FPGA_ONCHIP_BASE 0xC8000000
#define FPGA_ONCHIP_END 0xC803FFFF
#define FPGA_CHAR_BASE 0xC9000000
#define FPGA_CHAR_END 0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
#define JP1_BASE 0xFF200060
#define JP2_BASE 0xFF200070
#define PS2_BASE 0xFF200100
#define PS2_DUAL_BASE 0xFF200108
#define JTAG_UART_BASE 0xFF201000
#define JTAG_UART_2_BASE 0xFF201008
#define IrDA_BASE 0xFF201020
#define TIMER_BASE 0xFF202000
#define AV_CONFIG_BASE 0xFF203000
#define PIXEL_BUF_CTRL_BASE 0xFF203020
#define CHAR_BUF_CTRL_BASE 0xFF203030
#define AUDIO_BASE 0xFF203040
#define VIDEO_IN_BASE 0xFF203060
#define ADC_BASE 0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE 0xFF709000
#define HPS_TIMER0_BASE 0xFFC08000
#define HPS_TIMER1_BASE 0xFFC09000
#define HPS_TIMER2_BASE 0xFFD00000
#define HPS_TIMER3_BASE 0xFFD01000
#define FPGA_BRIDGE 0xFFD0501C

/* ARM A9 MPCORE devices */
#define PERIPH_BASE 0xFFFEC000		 // base address of peripheral devices
#define MPCORE_PRIV_TIMER 0xFFFEC600 // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF 0xFFFEC100 // PERIPH_BASE + 0x100
#define ICCICR 0x00					// offset to CPU interface control reg
#define ICCPMR 0x04					// offset to interrupt priority mask reg
#define ICCIAR 0x0C					// offset to interrupt acknowledge reg
#define ICCEOIR 0x10				// offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST 0xFFFED000		   // PERIPH_BASE + 0x1000
#define ICDDCR 0x00						   // offset to distributor control reg
#define ICDISER 0x100					   // offset to interrupt set-enable regs
#define ICDICER 0x180					   // offset to interrupt clear-enable regs
#define ICDIPTR 0x800					   // offset to interrupt processor targets regs
#define ICDICFR 0xC00					   // offset to interrupt configuration regs
volatile int *KEY_PTR = (int *)0xFF200050; // address of the input register

int colors[320][240];
int app_launch[320][240];
int note_no=0;
char notes[10][300];

void ball_pos(int x, int y, int r, short c);
char get_jtag(volatile int *JTAG_UART_ptr);
void put_jtag(volatile int *JTAG_UART_ptr, char c);
char getc();
void write_pixel(int x, int y, short colour);
void write_pixel_mouse(int x, int y, short colour);
void write_char(int x, int y, char c);
void clear_screen();
void clear_char();
void write_string(int x,int y, char s[]);
void clear_cursor(int x, int y);
void win_background();
void cursor(int x, int y);
void game1_logo(int x0, int y0, int radius, int color);
void text_editor_logo(int x,int y, int color);
void folder_logo(int x,int y, int color);
void over_game1(int ascore, int bscore);
void game1();
void folder_ui();
void folder ();
void text_editor_s(int note_save);
void text_editor_top_s();
void menu_opt1();
void menu_game1();
void text_editor_top_w();
void text_editor_w();





int main()
{

	windows();

	return 0;
}

void windows()
{
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;

	clear_screen();
	clear_char();
	int cur_x = 90, cur_y = 70;
	cursor(90, 70);
	win_background();
	game1_logo(20, 20, 8, 0xf800);
    text_editor_logo(15,55,0);
    folder_logo(15,80,0x0400);

	while (1)
	{

		char c = get_jtag(JTAG_UART_ptr);
		
		if (c == 87 || c == 119)
		{
			clear_cursor(cur_x, cur_y);
			cur_y -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 83 || c == 115)
		{
			clear_cursor(cur_x, cur_y);
			cur_y += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 65 || c == 97)
		{
			clear_cursor(cur_x, cur_y);
			cur_x -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 68 || c == 100)
		{
			clear_cursor(cur_x, cur_y);
			cur_x += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 0x0a && app_launch[cur_x][cur_y] == 1)
		{
			clear_screen();
			clear_char();
			menu_game1();
		}
        if (c == 0x0a && app_launch[cur_x][cur_y] == 2)
		{
			clear_screen();
			clear_char();
			text_editor_w();
		}
          if (c == 0x0a && app_launch[cur_x][cur_y] == 3)
		{
			clear_screen();
			clear_char();
			folder();
		}
		for (int i = 0; i < 10000; i++)
			;
	}
}

void ball_pos(int x, int y, int r, short c)
{
	int x1, y1;

	for (x1 = x - r; x1 <= x + r; x1++)
	{
		for (y1 = y - r; y1 <= y + r; y1++)
		{
			if ((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) <= r * r)
			{
				write_pixel(x1, y1, c);
			}
		}
	}
}
char get_jtag(volatile int *JTAG_UART_ptr)
{
	int data;
	data = *(JTAG_UART_ptr);
	if (data & 0x00008000) // check RVALID
		return ((char)data & 0xFF);
	else
		return ('\0');
}

// print user text to console
void put_jtag(volatile int *JTAG_UART_ptr, char c)
{
	int control;
	control = *(JTAG_UART_ptr + 1); // read control reg
	if (control & 0xFFFF0000)
	{
		*(JTAG_UART_ptr) = c;
	}
}
char getc()
{
	char c;

	while ((*KEY_PTR & 0x1) == 0)
		;				   // wait for a key to be pressed
	c = (*KEY_PTR & 0xFF); // read the ASCII code of the key pressed
	*KEY_PTR = 0;		   // clear the input register

	return c;
}

void write_pixel(int x, int y, short colour)
{
	volatile short *vga_addr = (volatile short *)(0x08000000 + (y << 10) + (x << 1));
	*vga_addr = colour;
	colors[x][y] = colour;
}

void write_pixel_mouse(int x, int y, short colour)
{
	volatile short *vga_addr = (volatile short *)(0x08000000 + (y << 10) + (x << 1));
	*vga_addr = colour;
}

void write_char(int x, int y, char c)
{
	// VGA character buffer
	volatile char *character_buffer = (char *)(0x09000000 + (y << 7) + x);
	*character_buffer = c;
}

void clear_screen()
{
	int x, y;
	for (x = 0; x < 320; x++)
	{
		for (y = 0; y < 240; y++)
		{
			write_pixel(x, y, 0);
			colors[x][y] = 0;
		}
	}
}
void clear_char()
{
	int x, y;
	for (x = 0; x < 80; x++)
	{
		for (y = 0; y < 60; y++)
		{
			write_char(x, y, 0);
		}
	}
}

void write_string(int x,int y, char s[])
{
    int a=x; int b=y;
    for(int i=0;i<strlen(s);i++)
    {
        write_char(a,b,s[i]);
        a++;
    }
}

void clear_cursor(int x, int y)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++)
		{
			int colo = colors[x - i][y + j];
			write_pixel_mouse(x - i, y + j, colo);
		}
}

void win_background()
{
	int x, y;

	// Draw blue sky
	for (x = 0; x < 320; x++)
	{
		for (y = 0; y < 120; y++)
		{
			write_pixel(x, y, 0x001F);
		}
	}

	// Draw green grass
	for (x = 0; x < 320; x++)
	{
		for (y = 120; y < 240; y++)
		{
			write_pixel(x, y, 0x0400);
		}
	}

	// Draw Windows logo
	int logo_x = 120;
	int logo_y = 80;
	int logo_size = 40;
	for (x = logo_x; x < logo_x + logo_size; x++)
	{
		for (y = logo_y; y < logo_y + logo_size; y++)
		{
			{
				write_pixel(x, y, 0xF800); // green
			}
		}
	}

	for (x = logo_x + logo_size; x < logo_x + logo_size * 2; x++)
	{
		for (y = logo_y; y < logo_y + logo_size; y++)
		{
			write_pixel(x, y, 0x3E0); // blue
		}
	}
	for (x = logo_x + logo_size; x < logo_x + logo_size * 2; x++)
	{
		for (y = logo_y + logo_size; y < logo_y + logo_size * 2; y++)
		{
			write_pixel(x, y, 0xFFE0); // blue
		}
	}
	for (x = logo_x; x < logo_x + logo_size; x++)
	{
		for (y = logo_y + logo_size; y < logo_y + logo_size * 2; y++)
		{
			write_pixel(x, y, 0x001F); // blue
		}
	}

	// Draw green tree
	/*for (x = 150; x < 170; x++) {
	  for (y = 110; y < 130; y++) {
		if ((x - 160) * (x - 160) + (y - 120) * (y - 120) < 100) {
		  write_pixel(x, y, 0x0400);
		}
	  }
	}*/
}

void cursor(int x, int y)
{
	for (int i = 0; i < 10; i++) {
		write_pixel_mouse(x + i, y, 0xffff);
		write_pixel_mouse(x + i, y + 1, 0xffff);
		write_pixel_mouse(x + i, y + 2, 0xffff);
		write_pixel_mouse(x + i, y + 3, 0xffff);
		write_pixel_mouse(x + i, y + 4, 0xffff);
		write_pixel_mouse(x + i, y + 5, 0xffff);
	}
	
	// Nose
	write_pixel_mouse(x + 4, y - 1, 0xffff);
	write_pixel_mouse(x + 5, y - 1, 0xffff);
	write_pixel_mouse(x + 3, y - 2, 0xffff);
	write_pixel_mouse(x + 6, y - 2, 0xffff);
	write_pixel_mouse(x + 2, y - 3, 0xffff);
	write_pixel_mouse(x + 7, y - 3, 0xffff);
	write_pixel_mouse(x + 3, y - 4, 0xffff);
	write_pixel_mouse(x + 6, y - 4, 0xffff);
	write_pixel_mouse(x + 4, y - 5, 0xffff);
	write_pixel_mouse(x + 5, y - 5, 0xffff);
	
	// Fins
	write_pixel_mouse(x + 1, y + 1, 0xffff);
	write_pixel_mouse(x + 2, y + 2, 0xffff);
	write_pixel_mouse(x + 1, y + 3, 0xffff);
	write_pixel_mouse(x + 0, y + 2, 0xffff);
	
	write_pixel_mouse(x + 8, y + 1, 0xffff);
	write_pixel_mouse(x + 7, y + 2, 0xffff);
	write_pixel_mouse(x + 8, y + 3, 0xffff);
	write_pixel_mouse(x + 9, y + 2, 0xffff);
}



void game1_logo(int x0, int y0, int radius, int color)
{
	int x, y;
	for (y = -radius; y <= radius; y++)
	{
		for (x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				write_pixel(x0 + x, y0 + y, color);
				app_launch[x0 + x][y0 + y] = 1;
			}
		}
	}
write_string(3,8,"PING!");
write_string(3,10,"PONG!");
write_string(30,15,"WELCOME TO WINDOWS!");
}

void text_editor_logo(int x,int y, int color)
{
    for(int i=x;i<x+16;i++)
    for(int j=y;j<y+20;j++)
    {
        
        write_pixel(i, j, color);
        if(j%4==2&&i>x+4&&i<x+12) write_pixel(i, j, 0xffff);
	    app_launch[i][j] = 2;

    }

    write_string(4,20,"Text");
write_string(3,22,"Editor");


}
void folder_logo(int x,int y, int color)
{
    for(int i=x;i<x+16;i++)
    for(int j=y;j<y+20;j++)
    {
        
        write_pixel(i, j, color);
        if(j%4==2&&i>x+4&&i<x+12) write_pixel(i, j, 0xffff);
	    app_launch[i][j] = 3;

    }

    write_string(4,30,"Folder");



}



char seg7[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
				 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

void over_game1(int ascore, int bscore)
{

	volatile int *KEYS_ptr = (int *)KEY_BASE;
	int a = 0;
	int edge_capture;
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;
	if (ascore > bscore)
	{
        write_string(25,10,"PLAYER-1 WIN!");
		
	}
	else
	{
		write_string(25,10,"PLAYER-2 WIN!");
	}

    write_string(25,20,"Press P to play again!");

    write_string(25,24,"Press Q to Quit!");

	
   
	while (1)
	{
       
		char c = get_jtag(JTAG_UART_ptr);
		if (c != '\0')
		{
			if (c == 'q' || c == 'Q')
				windows();
			if (c == 'p' || c == 'P')
				game1();
		}
        

		// edge_capture = *(KEYS_ptr + 3); // read the KEYs edge capture register
		// if (edge_capture)
		// {
		// 	*(KEYS_ptr + 3) = edge_capture; // clear edge capture register
		// 	windows();
		// 	// toggle flag for stopping
		// }
	}
}
void game1()
{
	int xdir = 1;
	int ydir = 1;
	int x;
	volatile int *LEDR_ptr = (int *)LEDR_BASE;
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;
	*HEX3_HEX0_ptr = 0; // clear the display
	*HEX5_HEX4_ptr = 0; // clear the display
	int x_center = 320 / 2;
	int y_center = 240 / 2;
	int ypada = 120;
	int xpada = 10;
	int ypadb = 120;
	int xpadb = 309;
	int dpada = 0;
	int dpadb = 0;
	int ascore = 0;
	int bscore = 0;
	*LEDR_ptr = ascore;
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;
	clear_screen();
	clear_char();
	int kk;
	for (int kk = 0; kk < 10000; kk++)
		;
	while (1)
	{

		int i;

		*HEX3_HEX0_ptr = seg7[ascore / 10] << 8;
		*HEX3_HEX0_ptr |= seg7[ascore % 10];

		*HEX5_HEX4_ptr = seg7[bscore / 10] << 8;
		*HEX5_HEX4_ptr |= seg7[bscore % 10];

		int side = 2;
		if (x_center - side <= 0)
		{
			bscore += 1;
			x_center = 320 / 2;
			y_center = 240 / 2;

			for (i = 0; i < 100; i++)
				;
			continue;
		}
		if (x_center + side >= 320)
		{
			ascore += 1;
			x_center = 320 / 2;
			y_center = 240 / 2;
			for (i = 0; i < 100; i++)
				;
			continue;
		}

		if (ascore > 5)
		{
			over_game1(ascore, bscore);
		}
		if (bscore > 5)
		{
			over_game1(ascore, bscore);
		}
		int y;

		int speed = 3;

		if (x_center - side <= xpada + 3 && x_center - side >= xpada - 3 && y_center <= ypada + 15 && y_center >= ypada - 15)
		{
			xdir *= -1;
			ydir *= -1;
		}

		if (x_center + side <= xpadb + 3 && x_center + side >= xpadb - 3 && y_center <= ypadb + 15 && y_center >= ypadb - 15)
		{
			xdir *= -1;
			ydir *= -1;
		}

		if (x_center + side >= 320)
		{

			xdir *= -1;
		}

		if (x_center - side <= 0)
		{
			xdir *= -1;
		}

		if (y_center - side <= 0)
		{
			ydir *= -1;
		}

		if (y_center + side >= 240)
		{

			ydir *= -1;
		}

		char c = get_jtag(JTAG_UART_ptr);
		if (c != '\0')
		{
			if (c == 'w')
				dpada = -1;
			if (c == 's')
				dpada = 1;
			if (c == 'k')
				dpadb = -1;
			if (c == 'm')
				dpadb = 1;
		}
		ypada += dpada * 10;
		ypadb += dpadb * 10;

		if (ypada <= 0)
		{
			ypada = 0;
		}
		if (ypada >= 240)
		{
			ypada = 239;
		}

		if (ypadb <= 0)
		{
			ypadb = 0;
		}
		if (ypadb >= 240)
		{
			ypadb = 239;
		}
		dpada = 0;
		dpadb = 0;
		x_center = x_center + speed * xdir;
		y_center = y_center + speed * ydir;

		// Write Hello, world!

		for (x = x_center - side; x <= x_center + side; x++)
		{
			for (y = y_center - side; y <= y_center + side; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					if ((x_center - x) * (x_center - x) + (y_center - y) * (y_center - y) <= side * side)
						write_pixel_mouse(x, y, 0xffff);
				}
			}
		}

		// ball_pos(x_center, y_center, side, 0xffff);

		for (x = xpada - 3; x <= xpada + 3; x++)
		{
			for (y = ypada - 15; y <= ypada + 15; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					write_pixel(x, y, 0xffff);
				}
			}
		}

		for (x = xpadb - 3; x <= xpadb + 3; x++)
		{
			for (y = ypadb - 15; y <= ypadb + 15; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					write_pixel(x, y, 0xffff);
				}
			}
		}

		for (i = 0; i < 100000; i++)
			;

		for (x = x_center - side; x <= x_center + side; x++)
		{
			for (y = y_center - side; y <= y_center + side; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					if ((x_center - x) * (x_center - x) + (y_center - y) * (y_center - y) <= side * side)
						write_pixel_mouse(x, y, 0x0);
				}
			}
		}
		// //	ball_pos(x_center, y_center, side, 0x0000);

		for (x = xpada - 3; x <= xpada + 3; x++)
		{
			for (y = ypada - 15; y <= ypada + 15; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					write_pixel(x, y, 0x0);
				}
			}
		}

		for (x = xpadb - 3; x <= xpadb + 3; x++)
		{
			for (y = ypadb - 15; y <= ypadb + 15; y++)
			{

				if (x >= 0 && x < 320 && y >= 0 && y < 240)
				{
					write_pixel(x, y, 0x0);
				}
			}
		}
	}
}
void folder_ui()
{
    for(int i=0;i<320;i++)
    for(int j=0;j<240;j++)
    write_pixel(i,j,0);

    for(int i=310;i<320;i++)
    for(int j=0;j<10;j++)
    {
        write_pixel(i,j,0xF800);
        app_launch[i][j]=20;
    }
    for(int i=312;i<318;i++)
	for(int j=0;j<10;j++)
    write_pixel(i,j,0xFFFF);

    for(int i=0;i<note_no;i++)
    {
        for(int j=10;j<26;j++)
        {
            for(int k=i*40+20;k<i*40+40;k++)
            {
                write_pixel(j,k,0x0400);
                app_launch[j][k]=i+1;
            }
        }
        write_string(10,i*10+6,"Folder");
        write_char(16,i*10+6,i+48);
    }
    
}
void folder ()
{

    int a=0,b=0;
    for(a=0;a<320;a++)
    for(b=0;b<240;b++)
    app_launch[a][b]=0;

volatile int *JTAG_UART_ptr = (int *)0xFF201000;
    clear_screen();
    clear_char();
    folder_ui();
int cur_x = 90, cur_y = 70;
	cursor(90, 70);
	while (1)
	{

		char c = get_jtag(JTAG_UART_ptr);
		
		if (c == 87 || c == 119)
		{
			clear_cursor(cur_x, cur_y);
			cur_y -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 83 || c == 115)
		{
			clear_cursor(cur_x, cur_y);
			cur_y += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 65 || c == 97)
		{
			clear_cursor(cur_x, cur_y);
			cur_x -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 68 || c == 100)
		{
			clear_cursor(cur_x, cur_y);
			cur_x += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 0x0a && app_launch[cur_x][cur_y] == 20)
		{
			clear_screen();
			clear_char();
			windows();
		}
        int a=0;
        for( a=1;a<11;a++)
        {
       if (c == 0x0a && app_launch[cur_x][cur_y] == a)
		{
			clear_screen();
			clear_char();
			text_editor_s(a-1);
		}
        }
       
		for (int i = 0; i < 10000; i++)
			;
	}

}
void text_editor_s(int note_save)
{
    int text=0;
	
	clear_screen();
	clear_char();
	text_editor_top_w();

	int x_cursor = 0;
	int y_cursor = 27;
	int inr = 4;
	int xx = 0;
	int yy = 6;
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;

	int x, y;
	while (1)
	{
        

		char cs = notes[note_save][text];
		char c = get_jtag(JTAG_UART_ptr);

       

		if (cs != 0)
		{

			if (cs == 0x0a)
			{
				y_cursor += inr;
				x_cursor = 0;
				xx = 0;
				yy = yy + 1;

                text++;
			}

			else
			{
                
                {

                write_char(xx, yy, cs);
				 x_cursor += 4;
				xx++;
				if (xx >= 80)
				{
					y_cursor += inr;
					x_cursor = 0;
					xx = 0;
					yy++;
				}
                
                text++;

                }
				
			}
		}

		if (c != 0)
		{

			if (c == 0x0a)
			{
				y_cursor += inr;
				x_cursor = 0;
				xx = 0;
				yy = yy + 1;

                notes[note_save][text]=c;
                text++;
			}

			else
			{
                if(c==0x09)
                {
                    
                    windows();
                    
                }
                else{

                    if(c==0x1b)
                {
                    
                  windows();
                    // break();
                }
                else{
                     write_char(xx, yy, c);
				x_cursor += 4;
				xx++;
				if (xx >= 80)
				{
					y_cursor += inr;
					x_cursor = 0;
					xx = 0;
					yy++;
				}
                notes[note_save][text]=c;
                text++;

                }

               

                }
				
			}
		}

		for (x = x_cursor; x < x_cursor + 4; x++)
		{

			if (x >= 0 && x < 320)
			{
				write_pixel(x, y_cursor, 0xffff);
			}
		}

		for (x = 0; x < 10000; x++)
			;

		for (x = x_cursor; x < x_cursor + 4; x++)
		{

			if (x >= 0 && x < 320)
			{
				write_pixel(x, y_cursor, 0x0000);
			}
		}

		// for (x = x_cursor; x < x_cursor + 4; x++)
		// {

		// 	if (x >= 0 && x < 320)
		// 	{
		// 		write_pixel(x, y_cursor, 0xffff);
		// 	}
		// }

		// for (x = 0; x < 10000; x++)
		// 	;

		// for (x = x_cursor; x < x_cursor + 4; x++)
		// {

		// 	if (x >= 0 && x < 320)
		// 	{
		// 		write_pixel(x, y_cursor, 0x0000);
		// 	}
		// }
	}
}
void text_editor_top_s()
{
	int i=0;int j=0;
    for( i=0;i<320;i++)
    for( j=0;j<240;j++)
    app_launch[i][j]=0;

	for( i=0;i<320;i++)
		for( j=0;j<20;j++)
		write_pixel(i,j,0x001F);

	write_string(31,2,"Notepad Plus Ultra");

    for( i=140;i<180;i++)
    for( j=220;j<230;j++)
    write_pixel(i,j,0x8410);
	 write_string(38,56,"EXIT");
	write_string(32,52,"Press ESC to EXIT");

   
}

void menu_opt1()
{
	int x=0;int y=0;
	for ( x = 80; x < 240; x++)
		for ( y = 50; y < 95; y++)
		{
			write_pixel(x, y, 0x3E0);
			app_launch[x][y] = 1;
		}

	write_string(20,50,"EXIT");
    write_string(20,25,"NEW GAME");
	

	for ( x = 80; x < 240; x++)
		for ( y = 145; y < 195; y++)
		{
			write_pixel(x, y, 0xF800);
			app_launch[x][y] = 2;
		}
}

void menu_game1()
{
	int i=0;int j=0;
     for( i=0;i<320;i++)
    for( j=0;j<240;j++)
    app_launch[i][j]=0;
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;

	int cur_x = 90, cur_y = 70;
	cursor(90, 70);

	menu_opt1();
	while (1)
	{

		char c = get_jtag(JTAG_UART_ptr);

		if (c == 87 || c == 119)
		{
			clear_cursor(cur_x, cur_y);
			cur_y -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 83 || c == 115)
		{
			clear_cursor(cur_x, cur_y);
			cur_y += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 65 || c == 97)
		{
			clear_cursor(cur_x, cur_y);
			cur_x -= 1;
			cursor(cur_x, cur_y);
		}
		if (c == 68 || c == 100)
		{
			clear_cursor(cur_x, cur_y);
			cur_x += 1;
			cursor(cur_x, cur_y);
		}
		if (c == 0x0a && app_launch[cur_x][cur_y] == 1)
		{
			clear_screen();
			clear_char();
			game1();
		}
		if (c == 0x0a && app_launch[cur_x][cur_y] == 2)
		{
			clear_screen();
			clear_char();
			windows();
		}
		int a=0; int b=0;
		for ( a = 0; i < 10000; i++)
			;
	}
}
void text_editor_top_w()
{
    for(int i=0;i<320;i++)
    for(int j=0;j<240;j++)
    app_launch[i][j]=0;

	for(int i=0;i<320;i++)
		for(int j=0;j<20;j++)
		write_pixel(i,j,0x001F);

	write_string(31,2,"Notepad Plus Ultra");

    for(int i=80;i<120;i++)
    for(int j=220;j<230;j++)
    write_pixel(i,j,0x8410);

    write_string(23,56,"SAVE");
	write_string(17,52,"Press TAB to SAVE");
	
	 write_string(53,56,"EXIT");
	write_string(47,52,"Press ESC to EXIT");

    for(int i=200;i<240;i++)
    for(int j=220;j<230;j++)
    write_pixel(i,j,0x8410);
}
void text_editor_w()
{

    int text=0;
	clear_screen();
	clear_char();
	text_editor_top_w();

	int x_cursor = 0;
	int y_cursor = 27;
	int inr = 4;
	int xx = 0;
	int yy = 6;
	volatile int *JTAG_UART_ptr = (int *)0xFF201000;

	int x, y;
	while (1)
	{

		char c = get_jtag(JTAG_UART_ptr);

		if (c != '\0')
		{

			if (c == 0x0a)
			{
				y_cursor += inr;
				x_cursor = 0;
				xx = 0;
				yy = yy + 1;

                notes[note_no][text]=c;
                text++;
			}

			else
			{
                if(c==0x09)
                {
                    note_no++;
                    text_editor_s(note_no-1);
                    
                }
                else{

                    if(c==0x1b)
                {
                    
                  windows();
                    // break();
                }
                else{
                     write_char(xx, yy, c);
				x_cursor += 4;
				xx++;
				if (xx >= 80)
				{
					y_cursor += inr;
					x_cursor = 0;
					xx = 0;
					yy++;
				}
                notes[note_no][text]=c;
                text++;

                }

               

                }
				
			}
		}

		for (x = x_cursor; x < x_cursor + 4; x++)
		{

			if (x >= 0 && x < 320)
			{
				write_pixel(x, y_cursor, 0xffff);
			}
		}

		for (x = 0; x < 10000; x++)
			;

		for (x = x_cursor; x < x_cursor + 4; x++)
		{

			if (x >= 0 && x < 320)
			{
				write_pixel(x, y_cursor, 0x0000);
			}
		}
	}
}
