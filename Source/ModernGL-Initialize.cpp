#include "ModernGL-Initialize.hpp"

#include "ModernGL-Types.hpp"
#include "ModernGL-Errors.hpp"
#include "ModernGL-Methods.hpp"
#include "ModernGL.hpp"
#include "OpenGL.hpp"

PyObject * InitializeModernGL(PyObject * self, PyObject * args, PyObject * kwargs) {
	if (initialized) {
		Py_RETURN_NONE;
	}

	if (!PyArg_ParseTuple(args, ":InitializeModernGL")) {
		return 0;
	}

	// Initialize OpenGL
	if (!OpenGL::InitializeOpenGL()) {
		PyErr_SetString(ModuleNotInitialized, "InitializeOpenGL failed.");
		return 0;
	}

	if (OpenGL::isglGetIntegerv()) {
		int major = 0;
		int minor = 0;

		OpenGL::glGetIntegerv(OpenGL::GL_MAJOR_VERSION, &major);
		OpenGL::glGetIntegerv(OpenGL::GL_MINOR_VERSION, &minor);

		versionNumber = major * 100 + minor;
	}

	if (versionNumber < 301) {
		PyErr_SetString(ModuleNotInitialized, "Lack of OpenGL support.");
		return 0;
	}

	LoadMethods();

	// Default blending
	OpenGL::glBlendFunc(OpenGL::GL_SRC_ALPHA, OpenGL::GL_ONE_MINUS_SRC_ALPHA);

	// Default primitive restart index
	OpenGL::glEnable(OpenGL::GL_PRIMITIVE_RESTART);
	OpenGL::glPrimitiveRestartIndex(-1);

	// Default VAO for GL_ELEMENT_ARRAY_BUFFER operations
	OpenGL::glGenVertexArrays(1, (OpenGL::GLuint *)&defaultVertexArray);
	OpenGL::glBindVertexArray(defaultVertexArray);

	// Default FBO and program
	OpenGL::glGetIntegerv(OpenGL::GL_DRAW_FRAMEBUFFER_BINDING, (OpenGL::GLint *)&defaultFramebuffer);
	OpenGL::glGetIntegerv(OpenGL::GL_CURRENT_PROGRAM, (OpenGL::GLint *)&activeProgram);

	// Default texture unit for texture operations
	OpenGL::glGetIntegerv(OpenGL::GL_MAX_TEXTURE_IMAGE_UNITS, (OpenGL::GLint *)&maxTextureUnits);
	defaultTextureUnit = maxTextureUnits - 1;

	activeFramebuffer = defaultFramebuffer;

	int viewport[4] = {};
	OpenGL::glGetIntegerv(OpenGL::GL_VIEWPORT, viewport);
	activeViewportWidth = viewport[2];
	activeViewportHeight = viewport[3];

	initialized = true;
	Py_RETURN_NONE;
}

PythonMethod InitializeMethods[] = {
	{
		0,
		(PyCFunction)InitializeModernGL,
		(PyCFunction)InitializeModernGL,
		METH_VARARGS | METH_KEYWORDS,
		"Init",
		"Initialize the ModernGL module inside a valid OpenGL context.\n"
		"A valid OpenGL context must exists before the function call.\n"
		"\n"

		"Parameters:\n"
		"\tNone\n"
		"\n"

		"Returns:\n"
		"\tNone\n"
		"\n"

		"Errors:\n"
		"\t(ModernGL.NotInitialized) There is no valid OpenGL context or OpenGL version is below 3.1\n"
		"\n"
	},
	{
		0,
		(PyCFunction)InitializeModernGL,
		(PyCFunction)InitializeModernGL,
		METH_VARARGS | METH_KEYWORDS,
		"InitializeModernGL",
		"Initialize the ModernGL module inside a valid OpenGL context.\n"
		"A valid OpenGL context must exists before the function call.\n"
		"\n"

		"Parameters:\n"
		"\tNone\n"
		"\n"

		"Returns:\n"
		"\tNone\n"
		"\n"

		"Errors:\n"
		"\t(ModernGL.NotInitialized) There is no valid OpenGL context or OpenGL version is below 3.1\n"
		"\n"
	},
};

int NumInitializeMethods = sizeof(InitializeMethods) / sizeof(InitializeMethods[0]);
