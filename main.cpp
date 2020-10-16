#include <GL/glut.h>
#include <iostream>
#include <sstream>

static int WINDOW_WIDTH = 640;
static int WINDOW_HEIGHT = 480;

static unsigned int player_score = 0;
static unsigned int cpu_score = 0;

static unsigned int speed = 5;

struct Point {
    float x;
    float y;
};

static bool left = true;
static bool down = true;

// Player's rectangle bar
Point r1 = {0.0, 5.0};
Point r2 = {100.0, 15.0};
// Note: r1 and r2's x should be changed.

// The bouncing square
Point s1 = {600.0, 400.0};
Point s2 = {620.0, 420.0};

void init(); // Initialize
void display();
std::string score(std::string s, unsigned int score);
void passiveMotion(int x, int y); // This function is activated when mouse moves without any button held.
void moveSquare();

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set the mode to draw in.
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Set the window size in screen pixels.
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2); // Set the window position in screen pixels.
    glutCreateWindow("Pong"); // Create the window with name "Pong"

    // Initialize
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(moveSquare);
    // Loop
    glutMainLoop();
    return 0;
}

std::string score(std::string s, unsigned int score) {
    std::stringstream ss;
    ss << s << score;
    std::string str = ss.str();
    return str;
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set clear color to black
    glMatrixMode(GL_PROJECTION); // Projection matrix
    glLoadIdentity(); // Set current matrix to identity matrix
    glOrtho(0.0, 640.0, 0.0, 480.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); // Model view matrix
    glLoadIdentity(); // Set current matrix to identity matrix
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to clear color (black)

    // Set rectangle bar center
    //float mx = (r1.x + r2.x) / 2.0;
    //float my = (r1.y + r2.y) / 2.0;
    glPushMatrix(); // Remember where you are
        glColor3ub(255, 255, 255); // Set color state to white
        glRectf(r1.x, r1.y, r2.x, r2.y);
    glPopMatrix();
    //TODO: draw the small square
    glPushMatrix(); // Remember where you are
        glColor3ub(255, 255, 255); // Set color state to white
        glRectf(s1.x, s1.y, s2.x, s2.y);
    glPopMatrix();
    // Draw scoreboard
    // Human's score
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(10, 480 - 20);
    std::string human = score("Human's Score: ", player_score);
    for (int i = 0; i < human.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, human[i]);
    }
    // Computer's score
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(10, 480 - 35);
    std::string cpu = score("Computer's Score: ", cpu_score);
    for (int i = 0; i < cpu.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, cpu[i]);
    }

    glutSwapBuffers();
}

void passiveMotion(int x, int y) {
    // Change axis
    int cursor_x = x;
    if (cursor_x >= 50 && cursor_x <= 590) {
        r1.x = cursor_x - 50;
        r2.x = cursor_x + 50;
    }
}

void moveSquare() {
    Sleep((int) (1000 / 60));

    if (left) {
        s1.x -= speed;
        s2.x -= speed;
    } else {
        s1.x += speed;
        s2.x += speed;
    }

    if (down) {
        if (s1.y == r2.y && ((s1.x + s2.x) / 2) < r2.x && ((s1.x + s2.x) / 2) > r1.x) {
            player_score++;
            down = false;
        } else {
            s1.y -= speed;
            s2.y -= speed;
        }
    } else {
        s1.y += speed;
        s2.y += speed;
    }

    if (left && s1.x <= 0) {
        left = false;
    } else if (!left && s2.x >=640) {
        left = true;
    }

    if (down && s1.y <= 0) {
        down = false;
        cpu_score++;
    } else if (!down && s2.y >= 480) {
        down = true;
    }

    glutPostRedisplay(); // Call display
}
