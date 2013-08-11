#include "AppWindow/AppWindow.h"
#include "AppState/Cube3D.h"
#include "Analyzer/Analyzer.h"
#include <time.h>

int main()
{
	srand(time(NULL));

	AppWindow *window = AppWindow_new();
	float *view = AppWindow_get_view(window);

	Analyzer *analyzer = analyzer_new("VisualBinary.exe");

	/* Cube Visualisation */
	Cube3D       *cube = cube3d_new(analyzer->frames);
	DrawFunction *cube_drawing_function = draw_function_new (cube3d_draw, cube, view);
	int draw_state = AppWindow_add_draw_routine(window, cube_drawing_function);
	AppWindow_set_state (window, draw_state);

	AppWindow_main(window);

    return 0;
}
