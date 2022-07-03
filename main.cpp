#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

bool update = true;
double valX = 2;
double valY = 2;


double mapToRange(double min1, double max1, double min2, double max2)
{
    double range = max2 - min2;
    return min1 * (range / max1) + min2;
}

void drawPixels(double r, double b, double g, int x, int y)
{
    glBegin(GL_POINTS);
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
       glVertex2i(x,y);
    glEnd();
}

void generate(int screenWidth, int screenHeight)
{
    int maxiterations = 10000; // Change around and stuff
    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            double a = mapToRange(x, screenWidth, -valX, valX);
            double b = mapToRange(y, screenHeight, -valY, valY);
            
            double ca = a;
            double cb = b;
            
            int n = 0;

            while (n < maxiterations) //Don't iterate too many times
            {
                double aa = a * a - b * b;
                double bb = 2 * a * b;
                a = aa + ca;
                b = bb + cb;
                if (a + b > 16)
                {
                    break;
                }
                n++;
            }
            
            double bright = mapToRange((double)n, (double)maxiterations, 0.0f, 1.0f);
            bright = mapToRange(sqrtf(bright), 1.0f, 0.0f, 255.0f);
                                    
            if (n == maxiterations)
            {
                bright = 0.0;
            }
            
            
            double R = bright * sinf(bright);
            double G = (bright * bright);
            double B = bright;
            
            drawPixels(R, G, B, x, y); // Draw the pixels
        }
    }
    
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods )
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        valX += .5;
        valY += .5;
        update = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        valX -= .5;
        valY -= .5;
        update = true;
    }
}

void branch(float x, float y, float len, float angle, double R, double G, double B)
{
    glColor3f(R, G, B);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + len * cos(angle), y + len * sin(angle));
    glEnd();
}

void fractalTrees(float x, float y, float len, float angle, float len_div, float angle_dif, int depth, int maxDepth)
{
    double R = (mapToRange(depth, maxDepth, 0.1, 1));
    double G = 0;
    double B = 0;
    branch(x, y, len, angle, R, G, B);
    if (depth == 0)
        return;
    
    //Left
    fractalTrees(x + len * cos(angle), y + len * sin(angle), len * len_div, angle - angle_dif, len_div, angle_dif, depth - 1, maxDepth);
    
    //Right
    fractalTrees(x + len * cos(angle), y + len * sin(angle), len * len_div, angle + angle_dif, len_div, angle_dif, depth - 1, maxDepth);
}

int main(int argc, const char * argv[]) {
    std::cout << &update << std::endl;
    int screenHeight = 1000;
    int screenWidth = 1000;
    
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "FRACTALS!!", NULL, NULL);
    
    //glfwSetKeyCallback(window, KeyCallback);
    //glfwSetCursorPosCallback(window, cursorPositionCallback);
    //glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glViewport(0, 0, screenWidth * 2, screenHeight * 2);
    glewExperimental = GL_TRUE;
    glOrtho(0, screenWidth, screenHeight, 0, 1, -1);
    
    // Removes weird line glitching
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
            
        if (update)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //generate(screenWidth, screenHeight);
            fractalTrees(screenWidth / 2, screenHeight - 50, 100.0, -M_PI / 2.0, 0.8, M_PI / 12.0, 20, 20); //Last two are Same
            
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //generate(screenWidth, screenHeight);
            fractalTrees(screenWidth / 2, screenHeight - 50, 100.0, -M_PI / 2.0, 0.8, M_PI / 12.0, 20, 20);
            
            update = false;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
    
    return 0;
}
