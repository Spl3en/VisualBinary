#include "AppWindow/AppWindow.h"
#include "AppState/Cube3D.h"
#include "Analyzer/Analyzer.h"
#include <time.h>

int main(int argc, char **argv)
{
	char *filename = NULL;
	srand(time(NULL));

	if (argc < 2)
		filename = "verdana.ttf";
	else
    {
        printf("Loading %s ...\n", argv[1]);
        filename = argv[1];
    }

	// Get Desktop resolution
	sfVideoMode desktop = sfVideoMode_getDesktopMode();

	AppWindow *window = AppWindow_new("VisualBinary", desktop.width, desktop.height, false);
	float *view = AppWindow_get_view(window);

	Analyzer *analyzer = analyzer_new(filename);

	/* Cube Visualisation */
	Cube3D *cube = cube3d_new(SFML(window), analyzer);
	cube3d_set_rot(cube, 0.0, 0.0);

	// Cube Draw
	DrawFunction *cube3d_draw_function = draw_function_new (cube3d_draw, cube, view);
	int draw_state = AppWindow_add_draw_routine(window, cube3d_draw_function);
	AppWindow_set_state (window, draw_state);

	// Cube Input
	Function *cube3d_input_function = function_new (cube3d_input, cube);
	AppWindow_add_input_routine(window, cube3d_input_function);

	// Cube Update
	Function *cube3d_update_function = function_new(cube3d_update, cube);
	AppWindow_add_update_routine(window, cube3d_update_function);

	AppWindow_main(window);

    return 0;
}
