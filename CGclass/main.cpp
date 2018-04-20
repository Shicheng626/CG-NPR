#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other includes
#include "shader.h"
// My GL
#include "myModel.h"
#include "myGameObject.h"

#include "nanogui/nanogui.h"

using namespace std;

using namespace nanogui;



enum cw_mode {
	CW = 0,
	CCW
};

enum polygon_mode {
	FILL = 0,
	LINE,
	POINT
};

// Window dimensions
const GLuint width = 1280, height = 720;

cw_mode cw = cw_mode::CCW;
polygon_mode polymode = polygon_mode::FILL;
NPR npr_mode = NPR::pencil;

Color colval(1.0f, 1.0f, 1.0f, 1.0f);
Color l1a(1.0f, 1.0f, 1.0f, 1.0f), l1d(1.0f, 1.0f, 1.0f, 1.0f), l1s(1.0f, 1.0f, 1.0f, 1.0f), l2a(1.0f, 1.0f, 1.0f, 1.0f), l2d(1.0f, 1.0f, 1.0f, 1.0f), l2s(1.0f, 1.0f, 1.0f, 1.0f);
bool rotate_x = false, rotate_y = false, rotate_z = false, zbuffer = true, outline = true;
bool texture_on = true, normal_on = false;


Screen *screen = nullptr;
std::string model_name = "rock";

GameObject* main_model;
Camera camera;

vector<Light> lights;

Light dirlight(Light::light_type::direct);
Light pointlight(Light::light_type::point);

///////////////////////
//ass1 control
/////////////////////////
void cubeLoadMode() {
	std::string path = "models/"+ model_name +".obj";
	std::string path_diffuse = "texture/" + model_name + "_diffuse.png";
	std::string path_normal = "texture/" + model_name + "_normal.png";
	main_model->LoadModel(path, path_diffuse, path_normal);
	main_model->transform.translation = - main_model->model->meshes[0].get_middle();
}

void move_forward() {
	camera.MoveFoward(0.1f);
}

void move_backward() {
	camera.MoveFoward(-0.1f);
}

void move_right() {
	camera.MoveRight(0.1f);
}

void move_left() {
	camera.MoveRight(-0.1f);
}

void move_up() {
	camera.MoveUp(0.1f);
}

void move_down() {
	camera.MoveUp(-0.1f);
}

void pitch_up() {
	camera.pitch(5.0f);
}

void pitch_down() {
	camera.pitch(-5.0f);
}

void yaw_up() {
	camera.yaw(5.0f);
}

void yaw_down() {
	camera.yaw(-5.0f);
}

void roll_up() {
	camera.roll(5.0f);
}

void roll_down() {
	camera.roll(-5.0f);
}



void reset() {
	lights[1].pos_dir = camera.position;
	camera.reset();
}

glm::vec3 getColor(Color color) {
	glm::vec3 result;
	result[0] = color[0];
	result[1] = color[1];
	result[2] = color[2];

	return result;
}
// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(width, height, "Assignment 0", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Build and compile our shader program
	Shader smoothShader("shader/basic.vs", "shader/basic.frag");
	Shader flatShader("shader/flat.vs", "shader/flat.frag");
	Shader pencilShader("shader/pencil.vs", "shader/pencil.frag", "shader/pencil.gs");
	Shader normalShader("shader/normal.vs", "shader/normal.frag", "shader/normal.gs");
	Shader outliner("shader/outliner.vs", "shader/outliner.frag");





	std::string path = "models/" + model_name + ".obj";
	std::string path_diffuse = "texture/" + model_name + "_diffuse.png";
	std::string path_normal = "texture/" + model_name + "_normal.png";
	Model myModel(path, path_diffuse, path_normal);

	Transform trans;

	trans.translation = -myModel.meshes[0].get_middle();

	main_model = new GameObject(&myModel, trans);

	dirlight.pos_dir = { 0, -1, -1 };
	lights.push_back(dirlight);
	lights.push_back(pointlight);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if defined(NANOGUI_GLAD)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");
	glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
#endif

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create a nanogui screen and pass the glfw pointer to initialize
	screen = new Screen();
	screen->initialize(window, true);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSwapInterval(0);
	glfwSwapBuffers(window);

	// Create nanogui gui
	bool enabled = true;
	FormHelper *gui = new FormHelper(screen);
	nanogui::ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Assignment 1");
	gui->addGroup("Model");
	//gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
	//gui->addVariable("string", strval);
	gui->addVariable("Model Name", model_name);
	gui->addButton("Load Model", cubeLoadMode);
	gui->addVariable("Color", colval);
	gui->addVariable("Shininess", main_model->shininess);

	gui->addGroup("Render Mode");
	gui->addVariable("CW/CCW", cw, enabled)->setItems({ "CW", "CCW" });
	gui->addVariable("Polygon Mode", polymode, enabled)->setItems({ "Fill", "Line","Point" });


	gui->addGroup("Camera Movement");
	gui->addButton("Forward", move_forward);
	gui->addButton("Backward", move_backward);
	gui->addButton("Right", move_right);
	gui->addButton("Left", move_left);
	gui->addButton("Up", move_up);
	gui->addButton("Down", move_down);

	gui->addGroup("Camera Rotation");
	gui->addButton("Pitch+", pitch_up);
	gui->addButton("Pitch-", pitch_down);
	gui->addButton("Yaw+", yaw_up);
	gui->addButton("Yaw-", yaw_down);
	gui->addButton("Roll+", roll_up);
	gui->addButton("Roll-", roll_down);

	gui->addGroup("Camera Position");
	gui->addButton("Reset", reset);
	gui->addVariable("X", camera.position[0])->setSpinnable(true);
	gui->addVariable("Y", camera.position[1])->setSpinnable(true);
	gui->addVariable("Z", camera.position[2])->setSpinnable(true);

	gui->addVariable("Far Plane", camera.far_plane)->setSpinnable(true);
	gui->addVariable("Near Plane", camera.near_plane)->setSpinnable(true);

	FormHelper *gui2 = new FormHelper(screen);
	nanogui::ref<Window> nanoguiWindow2 = gui2->addWindow(Eigen::Vector2i(400, 10), "Assignment 2");

	

	gui2->addGroup("Light 1 Direct light");
	gui2->addVariable("On/Off", lights[0].on);
	gui2->addVariable("ambient", l1a);
	gui2->addVariable("diffuse", l1d);
	gui2->addVariable("specular", l1s);

	gui2->addGroup("Light 2 Point light");
	gui2->addVariable("On/Off", lights[1].on);
	gui2->addVariable("ambient", l2a);
	gui2->addVariable("diffuse", l2d);
	gui2->addVariable("specular", l2s);

	gui2->addVariable("rotate x", rotate_x);
	gui2->addVariable("rotate y", rotate_y);
	gui2->addVariable("rotate z", rotate_z);

	gui2->addGroup("Config");
	gui2->addVariable("NPR", npr_mode, enabled)->setItems({ "Flat", "Smooth","Pencil" });
	gui2->addVariable("Outline", outline);
	
	gui2->addVariable("Z Buffer On", zbuffer);

	gui2->addGroup("Pencil");
	gui2->addVariable("tile factors", main_model->pencilrenderer.tile_factor);
	gui2->addVariable("max", main_model->pencilrenderer.max);
	gui2->addVariable("k", main_model->pencilrenderer.k);
	gui2->addVariable("in-face blending", main_model->pencilrenderer.inface_blending);
	gui2->addVariable("align to normal", main_model->pencilrenderer.align_to_normal);

	gui2->addGroup("Texture");
	gui2->addVariable("Texture On", texture_on);
	gui2->addVariable("Normal On", normal_on);

	screen->setVisible(true);
	screen->performLayout();

	glfwSetCursorPosCallback(window,
		[](GLFWwindow *, double x, double y) {
		screen->cursorPosCallbackEvent(x, y);
	}
	);

	glfwSetMouseButtonCallback(window,
		[](GLFWwindow *, int button, int action, int modifiers) {
		screen->mouseButtonCallbackEvent(button, action, modifiers);
	}
	);

	glfwSetKeyCallback(window,
		[](GLFWwindow *, int key, int scancode, int action, int mods) {
		screen->keyCallbackEvent(key, scancode, action, mods);
	}
	);

	glfwSetCharCallback(window,
		[](GLFWwindow *, unsigned int codepoint) {
		screen->charCallbackEvent(codepoint);
	}
	);

	glfwSetDropCallback(window,
		[](GLFWwindow *, int count, const char **filenames) {
		screen->dropCallbackEvent(count, filenames);
	}
	);

	glfwSetScrollCallback(window,
		[](GLFWwindow *, double x, double y) {
		screen->scrollCallbackEvent(x, y);
	}
	);

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow *, int width, int height) {
		screen->resizeCallbackEvent(width, height);
	}
	);

///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////



	reset();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_CULL_FACE);
		switch (polymode)
		{
		case polygon_mode::FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case polygon_mode::LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case polygon_mode::POINT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}



		if (zbuffer) { glEnable(GL_DEPTH_TEST); 
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		main_model->color = getColor(colval);
		main_model->normal_on = normal_on;
		main_model->texture_on = texture_on;

		lights[0].ambient = getColor(l1a);
		lights[0].diffuse = getColor(l1d);
		lights[0].specular = getColor(l1s);

		lights[1].ambient = getColor(l2a);
		lights[1].diffuse = getColor(l2d);
		lights[1].specular = getColor(l2s);



		////////////////////////////////////////////////////////////////
		//animation
		////////////////////////////////////////////////////////////////
		glm::mat4 rot_mat;
		glm::vec4 temp = { lights[1].pos_dir , 1.0f };
		if (rotate_x) {
			rot_mat = glm::rotate(rot_mat, glm::radians(0.2f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (rotate_z) {
			rot_mat = glm::rotate(rot_mat, glm::radians(0.2f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (rotate_y) {	
			rot_mat = glm::rotate(rot_mat, glm::radians(0.2f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		temp = rot_mat * temp;
		lights[1].pos_dir[0] = temp[0];
		lights[1].pos_dir[1] = temp[1];
		lights[1].pos_dir[2] = temp[2];

		switch (cw)
		{
		case cw_mode::CW:
			glFrontFace(GL_CCW);
			break;
		case cw_mode::CCW:
			glFrontFace(GL_CW);
			break;
		default:
			glFrontFace(GL_CCW);
			break;
		}

		if (outline) {
			outliner.Use();
			main_model->DrawOutline(outliner,camera);
		}

		switch (cw)
		{
		case cw_mode::CW:
			glFrontFace(GL_CW);
			break;
		case cw_mode::CCW:
			glFrontFace(GL_CCW);
			break;
		default:
			glFrontFace(GL_CW);
			break;
		}

		switch (npr_mode){
		case flat:
			flatShader.Use();
			main_model->Draw(flatShader, camera, lights,npr_mode);
			break;
		case smooth:
			smoothShader.Use();
			main_model->Draw(smoothShader, camera, lights, npr_mode);
			break;
		case pencil:
			pencilShader.Use();
			main_model->Draw(pencilShader, camera, lights, npr_mode);
			break;
		}

	//	normalShader.Use();
	//	main_model->Draw(normalShader, camera, lights, npr_mode);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		screen->drawWidgets();
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}