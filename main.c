#include "AppWindow/AppWindow.h"
#include "AppState/Cube3D.h"
#include "Analyzer/Analyzer.h"
#include <time.h>

int main(int argc, char **argv)
{
	srand(time(NULL));

	if (argc < 2)
	{
		printf("usage : %s <binary>", argv[0]);
		exit(EXIT_SUCCESS);
	}

	AppWindow *window = AppWindow_new("VisualBinary");
	float *view = AppWindow_get_view(window);

	Analyzer *analyzer = analyzer_new(argv[1]);

	/* Cube Visualisation */
	Cube3D *cube = cube3d_new(SFML(window), analyzer);

	// Cube Draw
	DrawFunction *cube_drawing_function = draw_function_new (cube3d_draw, cube, view);
	int draw_state = AppWindow_add_draw_routine(window, cube_drawing_function);
	AppWindow_set_state (window, draw_state);

	// Cube Input
	Function *cube_input_function = function_new (cube3d_input, cube);
	AppWindow_add_input_routine(window, cube_input_function);

	AppWindow_main(window);

    return 0;
}
