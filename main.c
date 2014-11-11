#include "AppWindow/AppWindow.h"
#include "AppState/Cube3D.h"
#include "Analyzer/Analyzer.h"
#include "Ztring/Ztring.h"
#include <time.h>

int main(int argc, char **argv)
{
	char *filename = NULL;

	if (argc < 2) {
	    printf ("Usage : %s <filename>\n", argv[0]);
	    exit (0);
	}
	else {
            printf("Loading %s ...\n", argv[1]);
            filename = argv[1];
        }

	// Initialize the seed from the time
	srand (time (NULL));

	// Analyze the file
	Analyzer *analyzer = analyzer_new (filename);

	// Get Desktop resolution and open the window
	sfVideoMode desktop = sfVideoMode_getDesktopMode ();
	AppWindow *window = AppWindow_new ("VisualBinary", desktop.width, desktop.height, false);
	if (!window) {
		return 0;
	}

	float *view = AppWindow_get_view (window);

	// Cube Visualisation
	Cube3D *cube = cube3d_new (SFML(window), analyzer);
	cube3d_set_rot (cube, 0.0, 0.0);

	// Cube Draw
	DrawFunction *cube3d_draw_function = draw_function_new (cube3d_draw, cube, view);
	int draw_state = AppWindow_add_draw_routine (window, cube3d_draw_function);
	AppWindow_set_state (window, draw_state);

	// Cube Input
	Function *cube3d_input_function = function_new (cube3d_input, cube);
	AppWindow_add_input_routine (window, cube3d_input_function);

	// Cube Update
	DrawFunction *cube3d_update_function = draw_function_new (cube3d_update, cube, view);
	AppWindow_add_update_routine (window, cube3d_update_function);

	AppWindow_main (window);

    return 0;
}
