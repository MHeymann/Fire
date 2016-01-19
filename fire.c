#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

/*** Debugger Macros **********************************************************/
#ifdef DEBUGFLAG
	#define DBG_dot()		printf(".")
	#define DBG_fdot()		printf(".\n")
	#define DBG_initing()	printf("Initialising")
	#define DBG_inited()	printf("Initialised!\n")
	#define DBG_mediaing()	printf("Loading Media")
	#define DBG_mediaed()	printf("Media loaded successfully\n")
	#define DBG_close()		printf("Closing...\n")
	#define DBG_ciao()		printf("Ciao!\n")
#else
	#define DBG_dot()
	#define DBG_fdot()
	#define DBG_initing()
	#define DBG_inited()
	#define DBG_mediaing()
	#define DBG_mediaed()
	#define DBG_close()
	#define DBG_ciao()
#endif

/*** General Macros ***********************************************************/
#define TRUE  1
#define FALSE 0
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define FRAME_PERIOD	100
#define LINE_DEPTH		3
#define COAL_SIZE		3

/*** Global Variables *********************************************************/
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
int **colour_buffer = NULL;
int part_rand_max = 0;

/*** Function Headers *********************************************************/
int w_init();
void w_close();
void w_update_buffer();
int f_get_random();

/*** Functions ****************************************************************/
int w_init() 
{
	int i, j;

	DBG_initing();
	/* Initialise SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to initialise DL!  SDL Error: %s\n", 
				SDL_GetError());
		return FALSE;
	}

	DBG_dot();

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		fprintf(stdout, "Warning: Linear texture filtering not enabled!\n");
	}

	DBG_dot();

	/* Create Window */

	g_window = SDL_CreateWindow("Fire!", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN);
	if (g_window == NULL) {
		fprintf(stderr, "Failed to create Window! SDL_Error: %s\n", SDL_GetError());
		return FALSE;
	}

	DBG_dot();

	/* Create a renderer for window */
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);	
	if (g_renderer == NULL) {
		 fprintf(stderr, "Renderer could not be created!  SDL Error: %s\n", 
				 SDL_GetError());
		 return FALSE;
	}

	DBG_dot();

	/* Initialise Renderer Colour */
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	/* Allocate Heap space for the buffer */
	colour_buffer = malloc(sizeof(int *) * SCREEN_WIDTH * (SCREEN_HEIGHT + LINE_DEPTH));
	if (colour_buffer == NULL) {
		fprintf(stderr, "Memory allocation failed for colour buffer!\n");
		return FALSE;
	}
	DBG_dot();

	for (i = 0; i < SCREEN_HEIGHT + LINE_DEPTH; i++) {
		for (j = 0; j < SCREEN_WIDTH; j++) {
			colour_buffer[(i * SCREEN_WIDTH) + j] = malloc(sizeof(int) * 4);
			if (colour_buffer[(i * SCREEN_WIDTH) + j] == NULL) {
				fprintf(stderr, "Memory error in allocation of char pointers");
				return FALSE;
			}
			colour_buffer[(i * SCREEN_WIDTH) + j][0] = 0x00;
			colour_buffer[(i * SCREEN_WIDTH) + j][1] = 0x00;
			colour_buffer[(i * SCREEN_WIDTH) + j][2] = 0x00;
			colour_buffer[(i * SCREEN_WIDTH) + j][3] = 0x00;
		}
	}
	DBG_dot();

	/* 
	i = SCREEN_WIDTH * SCREEN_HEIGHT;
	for (j = 0; j < SCREEN_WIDTH; j++) {
		if (f_get_random()) {
			colour_buffer[i + j][0] = (char)0xFF;
			colour_buffer[i + j][1] = (char)0xA0;
			colour_buffer[i + j][2] = (char)0x0;
			colour_buffer[i + j][3] = (char)0xFF;
		} else {
			colour_buffer[i + j][0] = (char)0x0;
			colour_buffer[i + j][1] = (char)0x0;
			colour_buffer[i + j][2] = (char)0x0;
			colour_buffer[i + j][3] = (char)0x0;
		}
	}
*/
	DBG_fdot();

	part_rand_max = RAND_MAX / 2;

	DBG_inited();
	return TRUE;
}


void w_close()
{
	int i, j;

	DBG_close();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	if (colour_buffer == NULL) {
		goto SKIPFREE;
	}

	for (i = 0; i <= SCREEN_HEIGHT; i++) {
		for (j = 0; j < SCREEN_WIDTH; j++) {
			if (colour_buffer[(i * SCREEN_WIDTH) + j] != NULL) {
				free(colour_buffer[(i * SCREEN_WIDTH) + j]);
			}
		}
	}
	free(colour_buffer);

SKIPFREE:
	SDL_Quit();
}

void w_update_buffer()
{
	int i, j, k, l;
	int index0, index1, index2, index3, index4;
	int total;

	i = SCREEN_WIDTH * SCREEN_HEIGHT;
	for (j = COAL_SIZE; j < SCREEN_WIDTH; j += COAL_SIZE) {
		index1 = i + j;
		if (f_get_random()) {
			for (k = 0; k < COAL_SIZE; k++) {
				for(l = 0; l < LINE_DEPTH; l++) {
					colour_buffer[index1 - k + l * SCREEN_WIDTH][0]  = 0xFF;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][1]  = 0xA0;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][2]  = 0x0;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][3]  = 0xFF;
				}
			}
		} else {
			for (k = 0; k < COAL_SIZE; k++) {
				for(l = 0; l < LINE_DEPTH; l++) {
					colour_buffer[index1 - k + l * SCREEN_WIDTH][0]  = 0x0;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][1]  = 0x0;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][2]  = 0x0;
					colour_buffer[index1 - k + l * SCREEN_WIDTH][3]  = 0x0;
				}
			}
		}
	}

	for (i = SCREEN_HEIGHT / 3 * 2; i < SCREEN_HEIGHT; i++) {
		for (j = 10; j < SCREEN_WIDTH - 10; j++) {
			index0 = (i * SCREEN_WIDTH) + j;
			index1 = ((i + 1) * SCREEN_WIDTH) + j - 2;
			index2 = ((i + 1) * SCREEN_WIDTH) + j - 1;
			index3 = ((i + 1) * SCREEN_WIDTH) + j + 1;
			index4 = ((i + 1) * SCREEN_WIDTH) + j + 2;
			for (k = 0; k < 3; k++) {
				total = 0;
				for (l = 0; l < LINE_DEPTH; l++) {
					total += 
					   (colour_buffer[index1 + SCREEN_WIDTH * l][k] + 
						colour_buffer[index2 + SCREEN_WIDTH * l][k] + 
						colour_buffer[index3 + SCREEN_WIDTH * l][k] + 
						colour_buffer[index4 + SCREEN_WIDTH * l][k]); 
				}
				colour_buffer[index0][k] = total / (LINE_DEPTH * 4) * 0.99;
			}
		}
	}

}

int f_get_random()
{
	int r = rand();
	if (r < part_rand_max) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*** Main Routine *************************************************************/
int main(int argc, char *argv[]) 
{
	int quit;
	char red, green, blue, alpha;
	int i, j;
	long start_frame_millis;
	long total_frame_millis;
	long remaining_millis;
	SDL_Event e;
	

#ifdef DEBUGFLAG
    int num_unmaxed_frames, num_frames;
	long next_print_tick;
	long start_millis;
	long total_update_millis;
	long total_render_millis;
	long cumulative_millis;
#endif

	/* Start up SDL and create window */
	if (!w_init()) {
		fprintf(stderr, "Failed to initialise!\n");
		goto PROG_QUIT;
	}

	/* Clear Screen */
	SDL_SetRenderDrawColor(g_renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(g_renderer);

	quit = FALSE;
	
#ifdef DEBUGFLAG
	next_print_tick = SDL_GetTicks() + 1000;
	num_frames = 0;
	num_unmaxed_frames = 0;
	total_update_millis = 0;
	total_render_millis = 0;
	cumulative_millis = 0;
#endif

	while (!quit) {
		start_frame_millis = SDL_GetTicks();
#ifdef DEBUGFLAG
		if (start_frame_millis >= next_print_tick) {
			printf("Frames: \t\t%d    Unmaxed: %d\n", num_frames, 
					num_unmaxed_frames);
			printf("Total update time: \t%ld \t%.2f%% \n", 
					total_update_millis, (total_update_millis 
						/ (cumulative_millis + 0.0) * 100));
			printf("Total render time: \t%ld \t%.2f%% \n", 
					total_render_millis, (total_render_millis 
						/ (cumulative_millis + 0.0) * 100));
			printf("Total CPU time: \t%ld\n", cumulative_millis);
			printf("\n");
			next_print_tick = start_frame_millis + 1000;
			num_frames = 0;
			num_unmaxed_frames = 0;	
			cumulative_millis = 0;
			total_update_millis = 0;
			total_render_millis = 0;
		}
#endif

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = TRUE;
			}
		}

#ifdef DEBUGFLAG
		start_millis = SDL_GetTicks();
#endif
		w_update_buffer();
#ifdef DEBUGFLAG
		total_update_millis += ((long)SDL_GetTicks() - start_millis);
#endif

#ifdef DEBUGFLAG
		start_millis = SDL_GetTicks();
#endif
		for (i = SCREEN_HEIGHT / 3 * 2; i < SCREEN_HEIGHT; i++) {
			for (j = 0; j < SCREEN_WIDTH; j++) {
				red		= colour_buffer[(i * SCREEN_WIDTH) + j][0];
				green	= colour_buffer[(i * SCREEN_WIDTH) + j][1];
				blue	= colour_buffer[(i * SCREEN_WIDTH) + j][2];
				alpha	= (char)0xFF;
			//	alpha	= colour_buffer[(i * SCREEN_WIDTH) + j][3];
				
				SDL_SetRenderDrawColor(g_renderer, red, green, blue, alpha);
				SDL_RenderDrawPoint(g_renderer, j, i);
			}
		}

#ifdef DEBUGFLAG
		total_render_millis += ((long)SDL_GetTicks() - start_millis);
#endif
			

		/* Update screen */
		SDL_RenderPresent(g_renderer);

		/* Attempt to regulate frame rate */
		total_frame_millis = (long)SDL_GetTicks() - start_frame_millis;
#ifdef DEBUGFLAG
		num_frames++;
		cumulative_millis += total_frame_millis;
#endif
		remaining_millis = FRAME_PERIOD - total_frame_millis;
		if (remaining_millis > 0) {
			SDL_Delay(remaining_millis);
#ifdef DEBUGFLAG
			num_unmaxed_frames++;
#endif
		}

	}

PROG_QUIT:
	w_close();
	DBG_ciao();
	return 0;
}
