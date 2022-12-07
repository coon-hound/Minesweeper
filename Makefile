all:
	gcc main.c restartbutton.c homebutton.c main_background.c beginner_background.c intermediate_background.c expert_background.c marker.c mine.c 13e_draw.c 13e_ascii.c -I/opt/homebrew/Cellar/glfw/3.3.6/include -lglfw -L/opt/homebrew/Cellar/glfw/3.3.6/lib -framework opengl
