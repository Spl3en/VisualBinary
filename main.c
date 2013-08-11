#include "AppWindow/AppWindow.h"
#include "AppState/Cube3D.h"

int main()
{
	AppWindow *window = AppWindow_new();

	/* Cube Visualisation */
	Cube3D   *cube = cube3d_new();
	Function *cube_drawing_function = function_new(cube3d_draw, cube);

	int draw_state = AppWindow_add_draw_routine(window, cube_drawing_function);
	AppWindow_set_state(window, draw_state);

	AppWindow_main(window);

    return 0;
}
